
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

#include <chrono>
#include <ratio>
#include <string>
#include <fstream>
#include <vector>
#include <optional>

#include "HandyBase.hpp"


using namespace std::literals;

/// Include <filesystem> in all the myriad ways, and make sure it is in std::filesystem.
#if !defined IS_ANDROID /// ANDROID NDK has ZERO support for std::filesystem right now. Maybe in NDK 19...?
	#if defined(__GNUC__)
		//In principal, this check should work on VS also, but it doesn't because __cplusplus is reported incorrectly on VS without special compiler flags
		#if (__cplusplus >= 201703L)
			#if __has_include(<filesystem>)
				#include <filesystem>
			#else
				#include <experimental/filesystem>
				namespace std { namespace filesystem { using namespace std::experimental::filesystem; } }
			#endif
		#else
			#include <experimental/filesystem>
			namespace std { namespace filesystem { using namespace std::experimental::filesystem; } }
		#endif
	#elif defined(_MSC_VER)
		//Some versions of VS have <filesystem> also, but that gives you the "Microsoft-specific implementation" instead of the C++ standard version
		#if defined _MSVC_LANG && _MSVC_LANG >= 201700L
			#include <filesystem>
		#else
			#include <experimental/filesystem>
			namespace std { namespace filesystem { using namespace std::experimental::filesystem; } }
		#endif
	#endif
	
	namespace std
	{
		inline filesystem::path operator+(filesystem::path lhs, filesystem::path const & rhs)
		{
		    lhs += rhs;
		    return lhs;
		}
		/*filesystem::path operator+(filesystem::path lhs, char const * rhs)
		{
		    filesystem::path rhsPath(rhs);
		    lhs += rhsPath;
		    return lhs;
		}*/
	}
#endif


/// Everything below here is just to make sure <span> (or a substitute) is included, and that it works in unordered collections.


#if __has_include(<span>)
	#include <span>	
#else
	#include "Extended/GSL/include/gsl/span"
	namespace std { using namespace gsl; }
#endif

namespace std
{
	template<class ElementType, ptrdiff_t Extent>
	struct less<span<ElementType, Extent>>
	{
		bool operator()(span<ElementType, Extent> const & lhs, span<ElementType, Extent> const & rhs) const
		{
			if (lhs.size_bytes() < rhs.size_bytes()) return true;
			if (lhs.size_bytes() > rhs.size_bytes()) return false;

			return std::lexicographical_compare(lhs.data(), lhs.data() + lhs.size_bytes(), rhs.data(), rhs.data() + rhs.size_bytes());
		}
	};

	template<class ElementType, ptrdiff_t Extent>
	struct equal_to<span<ElementType, Extent>>
	{
		bool operator()(span<ElementType, Extent> const & lhs, span<ElementType, Extent> const & rhs) const
		{
			if (lhs.size_bytes() != rhs.size_bytes()) return false;

			return std::equal(lhs.data(), lhs.data() + lhs.size_bytes(), rhs.data(), rhs.data() + rhs.size_bytes());
		}
	};
}

namespace HANDY_NS::detail
{
	FORCEINLINE size_t FNV1aAppendBytes(size_t h, const std::byte * const p, const size_t size) noexcept
	{
		for (size_t s = 0; s < size; ++s)
		{
			h ^= static_cast<size_t>(p[s]);

			if constexpr (sizeof(size_t) == 8)
				h *= 1099511628211ULL;
			else
				h *= 16777619U;
		}

		return h;
	}

	FORCEINLINE size_t FNV1aAppendBytes(const std::byte * const p, const size_t size) noexcept
	{
		if constexpr (sizeof(size_t) == 8)
			return FNV1aAppendBytes(14695981039346656037ULL, p, size);
		else
			return FNV1aAppendBytes(2166136261U, p, size);
	}
}

namespace std
{
	template <class ElementType, std::ptrdiff_t Extent>
	struct hash<span<ElementType, Extent>>
	{
		typedef span<ElementType, Extent> argument_type;
		typedef size_t result_type;

		result_type operator()(const argument_type& a) const { return HANDY_NS::detail::FNV1aAppendBytes(reinterpret_cast<std::byte const *>(a.data()), a.size_bytes()); }
	};

}
