
/// ========================================================================
/// UNLICENSE
/// 
/// This is free and unencumbered software released into the public domain.
/// Anyone is free to copy, modify, publish, use, compile, sell, or
/// distribute this software, either in source code form or as a compiled
/// binary, for any purpose, commercial or non-commercial, and by any
/// means.
///
/// In jurisdictions that recognize copyright laws, the author or authors
/// of this software dedicate any and all copyright interest in the
/// software to the public domain. We make this dedication for the benefit
/// of the public at large and to the detriment of our heirs and
/// successors. We intend this dedication to be an overt act of
/// relinquishment in perpetuity of all present and future rights to this
/// software under copyright law.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
/// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
/// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
/// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
/// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
/// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
/// OTHER DEALINGS IN THE SOFTWARE.
///
/// For more information, please refer to <http://unlicense.org/>
/// ========================================================================

#pragma once

#include <functional>
#include <chrono>
#include <string>
#include <iostream>
#include <sstream>
#include <atomic>
#include <random>
#ifdef __GNUG__
	#include <cxxabi.h> // for TypeInfo
#endif

#if defined IS_WINDOWS
	#include <intrin.h>
	#pragma intrinsic(_BitScanForward)
	#pragma intrinsic(_BitScanReverse)
	#pragma intrinsic(_BitScanForward64)
	#pragma intrinsic(_BitScanReverse64)
#endif

#include "HandyBase.hpp"
#include "HandyCompat.hpp"
#include "HandyEncoding.hpp"
#include "HandySerDe.hpp"

namespace HANDY_NS {

	inline void PrintSystemInfo()
	{
		std::cout << BuildInfoString();
	}

	/// Escape will allow you to prevent compiler value pre-evaluation optimization for 
	/// a specific (presumably a result) value/variable. Great for benchmarking intrinsics.
	#ifdef _MSC_VER
		#pragma optimize("", off)
		inline void Escape(void * p) 
		{
			*reinterpret_cast<char       volatile*>(p) =
			*reinterpret_cast<char const volatile*>(p);
		}
		#pragma optimize("", on)
	#else
		// Only works on GGC/Clang: Boo.
		inline void Clobber()       { asm volatile("" : :        : "memory"); }
		inline void Escape(void* p) { asm volatile("" : : "g"(p) : "memory"); }
	#endif


	/// Errno checker for Windows and *nix, returns empty string if no error. 
	/// Does not clear errors, just reports them. Typically, you'll call both in sequence.
	/// Convenient to use as part of a render loop, etc...
	std::string GetAnyError();
	void        ClearErrors();

	class OnScopeExit final
	{
	public:
		inline explicit OnScopeExit(std::function<void()> onExit) : m_onExit(onExit) {}
		inline ~OnScopeExit() { m_onExit(); }

	private:
		std::function<void()> m_onExit;

		OnScopeExit(const OnScopeExit &) = delete;
		OnScopeExit & operator=(OnScopeExit const &) = delete;
	};

	class StopWatch
	{
		typedef std::chrono::steady_clock clock;
		typedef std::chrono::microseconds microseconds;
		typedef std::chrono::milliseconds milliseconds;
		typedef std::chrono::seconds seconds;

		clock::time_point mStart;

	public:

		FORCEINLINE StopWatch() : mStart(clock::now()) { static_assert(std::chrono::steady_clock::is_steady, "Serious OS/C++ library issues. std::chrono::steady_clock is not steady."); }
		FORCEINLINE StopWatch(StopWatch const & other) : mStart(other.mStart) { }
		FORCEINLINE StopWatch & operator=(StopWatch const & rhs) { mStart = rhs.mStart; return *this; }

		FORCEINLINE uint64_t Microseconds()  const { return std::chrono::duration_cast<microseconds>(clock::now() - mStart).count(); }
		FORCEINLINE uint64_t Milliseconds()  const { return std::chrono::duration_cast<milliseconds>(clock::now() - mStart).count(); }
		FORCEINLINE uint64_t Seconds()       const { return std::chrono::duration_cast<seconds>     (clock::now() - mStart).count(); }

		FORCEINLINE float    SecondsF()      const { return std::chrono::duration_cast<std::chrono::duration<float >>(clock::now() - mStart).count(); }
		FORCEINLINE double   SecondsD()      const { return std::chrono::duration_cast<std::chrono::duration<double>>(clock::now() - mStart).count(); }

		FORCEINLINE auto     Duration()      const { return clock::now() - mStart; }

		FORCEINLINE void     Restart() { mStart = clock::now();}
	};

	/// Timer that prints how long the object existed, from ctor to dtor.
	class DisposeTimer final
	{
		StopWatch m_timer;

		std::string m_message;
		bool        m_logged = false;

	public:
		FORCEINLINE explicit DisposeTimer(std::string const & message = "")
			: m_timer(),
			m_message(message)
		{ }

		FORCEINLINE ~DisposeTimer()
		{
			if (m_logged)
				return;
			m_logged = true;

			if (m_message.size() == 0)
				std::cout << "Time : " << m_timer.SecondsF() << std::endl;
			else
				std::cout << m_message << " : " << m_timer.SecondsF() << std::endl;
		}
	};

	class UIDMixin
	{
		static std::atomic_int64_t & s_lastUID() { static std::atomic_int64_t instance = 0; return instance; }

	public:
		static void ResetCounter() { s_lastUID() = 0; }

		int64_t UID;

		UIDMixin() : UID(s_lastUID()++) { }

		bool operator<(UIDMixin const & rhs) const { return UID < rhs.UID; }
	};


	/// Can be used as a mixin or standalone
	class UniqueName
	{
		static uint64_t getInc() { static std::atomic_uint64_t instance = 0; return instance.operator++(); };

	public:
		uint64_t    const UniqueValue; /// DO NOT CHANGE DECLARATION ORDER
		std::string const UniqueSuffix;

		UniqueName()
			: UniqueValue(getInc())
			, UniqueSuffix(std::to_string(UniqueValue))
		{ }

		virtual ~UniqueName() = default;
	};


	template <typename T>
	T RandomInRange(T min, T max)
	{
		static std::random_device rd;     // only used once to initialise (seed) engine
		static std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
		static std::mutex mtx;

		static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value, "Unsupported Type: RandomInRange<T>");

		Locked (mtx)
		{
			if constexpr (std::is_integral<T>::value)
			{
				std::uniform_int_distribution<T> uni(min,max); // guaranteed unbiased
				return uni(rng);
			}
			else /// Then it must be floating point, due to the assert above.
			{
				std::uniform_real_distribution<T> uni(min,max); // guaranteed unbiased
				return uni(rng);
			}
		}
	}

	template <typename T>
	class OnlyOne
	{
	public:
		static std::shared_ptr<T> Get()
		{
			static std::weak_ptr<T> Existing;

			auto instance = Existing.lock();

			if (!instance)
			{
				instance.reset(new T());
				Existing = instance;
			}
			return instance;

		}
	};


	/// Search the mask data from least significant bit (LSB) to the most significant bit (MSB) for a set bit (1).
	/// Returns 64 if there are no set bits, otherwise returns the index of the first set bit.
	FORCEINLINE uint8_t BitscanLSB(uint64_t n)
	{
		#if defined IS_WINDOWS
			unsigned long result = 0;
			if (!_BitScanForward64(&result, n))
				return 64_u8;
			return (uint8_t)result;
		#else
			if (!n)
				return 64;
			return (uint8_t)__builtin_ctzll(n);
		#endif

	}

	/// Search the mask data from most significant bit (MSB) to least significant bit (LSB) for a set bit (1).
	/// Returns 64 if there are no set bits, otherwise returns the index of the first set bit.
	FORCEINLINE uint8_t BitscanMSB(uint64_t n)
	{
		#if defined IS_WINDOWS
			unsigned long result = 0;
			if (!_BitScanReverse64(&result, n))
				return 64_u8;
			return (uint8_t)result;
		#else
			if (!n)
				return 64;
			return (uint8_t)__builtin_clzll(n);
		#endif
	}

	#if !defined IS_ANDROID
		///-----------------------------------------------
		/// Type Info
		///
		class TypeDescriptor;
		typedef const TypeDescriptor * TypeInfo;

		template <class TYPE> TypeInfo NOINLINE FCONST typeInfo();

		class TypeDescriptor
		{
		public:
			const std::string name;
			const size_t size;

		private:
			TypeDescriptor(const std::type_info & info, size_t sz);

			template <class TYPE> friend TypeInfo typeInfo();
		};

		template <class TYPE> TypeInfo NOINLINE FCONST typeInfo()
		{
			static const TypeDescriptor descriptor(typeid(TYPE), sizeof(TYPE));
			return &descriptor;
		}

		template <class TYPE> inline FCONST TypeInfo typeInfo(TYPE)
		{
			return typeInfo<TYPE>();
		}

		namespace detail {

			#ifdef IS_GPP
			namespace
			{
				struct nameptr
				{
					char * p;
					nameptr(char * ptr) : p(ptr) {}
					~nameptr() { free(p); }
				};
			}

			static std::string UnmangleName(const char * name)
			{
				int status = -1;
				nameptr ptr(abi::__cxa_demangle(name, nullptr, nullptr, &status));
				return (status == 0 ? ptr.p : name);
			}
			#else
			static std::string UnmangleName(const char * name)
			{
				return name;
			}
			#endif
		}

		inline
			TypeDescriptor::TypeDescriptor(const std::type_info & info, size_t sz)
			: name(detail::UnmangleName(info.name()))
			, size(sz)
		{ }
		///-----------------------------------------------
	#endif

	#if !defined IS_ANDROID
		//Get the paths of all sub-directories of a given directory
		inline std::vector<std::filesystem::path> SubDirectories(std::filesystem::path DirPath) 
		{
			std::vector<std::filesystem::path> subdirs;
			if ((! std::filesystem::exists(DirPath)) || (! std::filesystem::is_directory(DirPath)))
				return subdirs;

			std::filesystem::directory_iterator dirIter{DirPath};
			while (dirIter != std::filesystem::directory_iterator{}) 
			{
				std::filesystem::path myPath = dirIter->path();
				if (std::filesystem::is_directory(myPath) && (! std::filesystem::equivalent(myPath, DirPath)))
					subdirs.push_back(myPath);

				dirIter++;
			}
			return subdirs;
		}
	#endif

} // HANDY_NS
