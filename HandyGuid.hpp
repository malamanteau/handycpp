
/// =============================================================================
/// crossguid (https://github.com/graeme-hill/crossguid)
/// 
/// LICENSE: MIT
/// 
/// Copyright (c) 2014 Graeme Hill (http://graemehill.ca)
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///
/// =============================================================================

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

namespace HANDY_NS {

	// Class to represent a GUID/UUID. Each instance acts as a wrapper around a
	// 16 byte value that can be passed around by value. It also supports
	// conversion to string (via the stream operator <<) and conversion from a
	// string via constructor.
	class Guid
	{
	public:
		static Guid New();
		static Guid Zero();

		explicit Guid(std::array<uint8_t, 16> const  &bytes);
		explicit Guid(std::array<uint8_t, 16>       &&bytes);
		explicit Guid(std::span<std::byte>            bytes);

		template <EncodingScheme SCHEME> std::string ToString() const;
		template <EncodingScheme SCHEME> static std::optional<Guid> From            (std::string_view const & fromString);
		template <EncodingScheme SCHEME> static               Guid  From_FastNoCheck(std::string_view const & fromString);

		//template <EncodingScheme SCHEME> static std::optional<Guid> From(std::string_view const & fromString);

		Guid();
		Guid(bool) { } // NO INIT CTOR, WILL HAVE JUNK DATA!

		Guid(Guid const  &other) = default;
		Guid(Guid       &&other) = default;

		Guid & operator=(Guid const  & other) = default;
		Guid & operator=(Guid       && other) = default;

		bool operator==(Guid const & other) const;
		bool operator!=(Guid const & other) const;

		//std::string str() const;
		void swap(Guid & other);
		bool IsValid() const;

		std::array<uint8_t, 16> Bytes;

		std::string toString() const
		{
			return ToString<Handy::EncodingScheme::Hex>();
		}

		void fromString(std::string_view fromString)
		{
			Encoding<Handy::EncodingScheme::Hex>::Decode(Bytes, fromString);
		}

	private:
		void zeroify();
	};

	namespace details
	{
		template <typename...> struct hash;

		template<typename T>
		struct hash<T> : public std::hash<T>
		{
			using std::hash<T>::hash;
		};


		template <typename T, typename... Rest>
		struct hash<T, Rest...>
		{
			inline std::size_t operator()(const T& v, const Rest&... rest) {
				std::size_t seed = hash<Rest...>{}(rest...);
				seed ^= hash<T>{}(v)+0x9e3779b9 + (seed << 6) + (seed >> 2);
				return seed;
			}
		};
	}

	FORCEINLINE Guid::Guid(std::array<uint8_t, 16> const & bytes) : Bytes(bytes) { } // create a guid from vector of bytes
	FORCEINLINE Guid::Guid(std::array<uint8_t, 16> &&bytes) : Bytes(std::move(bytes)) { } // create a guid from vector of bytes
	FORCEINLINE Guid::Guid(std::span<std::byte> bytes) { memcpy(&Bytes[0], bytes.data(), 16); } // create a guid from a span of bytes, DOES NOT SIZE CHECK!

	template<EncodingScheme SCHEME>
	FORCEINLINE std::string Guid::ToString() const
	{
		std::string ret;
		Encoding<SCHEME>::Encode(ret, std::span<std::byte const>(reinterpret_cast<std::byte const *>(&Bytes[0]), 16));
		return ret;
	}

	// static
	template <EncodingScheme SCHEME> 
	FORCEINLINE /*static*/ std::optional<Guid> Guid::From(std::string_view const & fromString)
	{
		Guid ret;
		if (Encoding<SCHEME>::Decode(ret.Bytes, fromString))
			return ret;

		return std::optional<Guid>();
	}

	// static
	template <EncodingScheme SCHEME> 
	FORCEINLINE /*static*/ Guid Guid::From_FastNoCheck(std::string_view const & fromString)
	{
		Guid ret;
		Encoding<SCHEME>::Decode(ret.Bytes, fromString);
		return ret;
	}

	FORCEINLINE Guid::Guid() : Bytes{ {0} } { } // create empty guid

	FORCEINLINE void Guid::zeroify() { std::fill(Bytes.begin(), Bytes.end(), static_cast<uint8_t>(0)); } // set all bytes to zero
	FORCEINLINE bool Guid::operator==(const Guid &other) const { return ((*(uint64_t*)&Bytes[0]) == (*(uint64_t*)&other.Bytes[0])) && ((*(uint64_t*)&Bytes[8]) == (*(uint64_t*)&other.Bytes[8])); } // overload equality operator
	FORCEINLINE bool Guid::operator!=(const Guid &other) const { return !((*this) == other); } // overload inequality operator
	FORCEINLINE void Guid::swap(Guid &other) { Bytes.swap(other.Bytes); } // member swap function
	FORCEINLINE bool operator<(const Guid &lhs, const Guid &rhs) { return lhs.Bytes < rhs.Bytes; }
	FORCEINLINE bool Guid::IsValid() const { return *this != Zero(); }

	FORCEINLINE Guid Guid::Zero() { return Guid(); }

	/// Implement the Handy::Serializer for Handy::Guid:
	template <>
	FORCEINLINE std::span<std::byte const> Serializer<Guid>::get_span(Guid const & inItem)
	{
		return std::span<std::byte const>((std::byte *)(void *)&inItem.Bytes[0], inItem.Bytes.size());
	}

	/// Implement the Handy::Deserializer for Handy::Guid:
	template <>
	FORCEINLINE std::byte * Deserializer<Guid>::prepare_span(size_t sizeBytes, Guid & outItem)
	{
		if (sizeBytes != outItem.Bytes.size())
		{
			#if defined HANDY_SERDE_NOISY_NULLRET
			std::cerr << "Handy::Deserializer wrong size to assign a Guid.";
			#endif
			return nullptr;
		}

		return (std::byte *)(void *)&outItem.Bytes[0];
	}

}

namespace std
{
	template<>
	FORCEINLINE void swap<HANDY_NS::Guid>(HANDY_NS::Guid & lhs, HANDY_NS::Guid & rhs) noexcept
	{
		std::swap(lhs.Bytes, rhs.Bytes);
	}

	// Specialization for std::hash<Guid> -- this implementation uses std::hash<std::string> on the 
	// stringification of the guid to calculate the hash
	template <>
	struct hash<HANDY_NS::Guid>
	{
		std::size_t operator()(HANDY_NS::Guid const &guid) const
		{
			const uint64_t* p = reinterpret_cast<const uint64_t*>(guid.Bytes.data());
			return HANDY_NS::details::hash<uint64_t, uint64_t>{}(p[0], p[1]);
		}
	};

	template<>
	struct less<HANDY_NS::Guid>
	{
		bool operator()(HANDY_NS::Guid const & lhs, HANDY_NS::Guid const & rhs) const
		{
			return lhs.Bytes < rhs.Bytes;
		}
	};

	template<>
	struct equal_to<HANDY_NS::Guid>
	{
		bool operator()(HANDY_NS::Guid const & lhs, HANDY_NS::Guid const & rhs) const
		{
			return lhs.Bytes == rhs.Bytes;
		}
	};
}

inline bool operator==(HANDY_NS::Guid const & lhs, HANDY_NS::Guid const & rhs) { return lhs.Bytes == rhs.Bytes; }
inline bool operator< (HANDY_NS::Guid const & lhs, HANDY_NS::Guid const & rhs) { return lhs.Bytes <  rhs.Bytes; }
