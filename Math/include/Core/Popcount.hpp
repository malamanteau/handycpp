
/// See ../../License.txt for license info.

#pragma once

#include <cstdint>
#include <limits>
#include <array>

#include "Base.hpp"

#ifdef USE_POPCNT

	namespace HANDYMATH_NS {

		FORCEINLINE int64_t Popcount(uint64_t num) { return _mm_popcnt_u64(num); }
		FORCEINLINE int32_t Popcount(uint32_t num) { return _mm_popcnt_u32(num); }

	}

#else
	namespace HANDYMATH_NS {

		FORCEINLINE int64_t Popcount(uint64_t num)
		{
			int64_t c = 0; // c accumulates the total bits set in v

			for (; num; c++)
				num &= num - 1; // clear the least significant bit set

			return c;
		}

		FORCEINLINE int32_t Popcount(uint32_t num)
		{
			int32_t c = 0; // c accumulates the total bits set in v

			for (; num; c++)
				num &= num - 1; // clear the least significant bit set

			return c;
		}

	}
#endif

