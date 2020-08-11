#pragma once

#include <cstdint>

#define HANDY_NO_VECTOR_MATH
	#include "../../../Handy.hpp"
#undef HANDY_NO_VECTOR_MATH

#ifndef HANDYMATH_NS
	#define HANDYMATH_NS Math
#endif

// TODO
//#define USE_AVX512F
//#define USE_AVX512CDI
//#define USE_AVX512PFI
//#define USE_AVX512ERI
//#define USE_AVX512VLI
//#define USE_AVX512BW
//#define USE_AVX512DQ
//#define USE_AVX512FMA52
//#define USE_AVX512BMI
//#define USE_AVX512BMI2

#if   defined USE_AVX2 
#elif defined USE_FMA3 
#elif defined USE_AVX 
#elif defined USE_SSE42 
#elif defined USE_SSE41
#elif defined USE_SSSE3 
#elif defined USE_SSE3
#elif defined USE_SSE2
#elif defined USE_SCALAR
#elif defined USE_NEON
#elif defined IS_ARM
	#define USE_NEON
#elif defined IS_EMSCRIPTEN
	#define USE_SCALAR
#elif defined __SSE2__ || defined IS_X64 || (defined _M_IX86_FP && _M_IX86_FP >= 2)
	#define USE_SSE2
#else
	#define USE_SCALAR
#endif



#ifdef  USE_AVX2
	#define USE_FMA3
	#define USE_LZCNT
	#define USE_BMI2
#endif
#ifdef  USE_FMA3
	#define USE_AVX
#endif
#ifdef  USE_AVX
	#define USE_SSE42
	#define USE_POPCNT
	#define USE_BMI1
#endif
#ifdef  USE_SSE42
	#define USE_SSE41
#endif
#ifdef  USE_SSE41
	#define USE_SSSE3
#endif
#ifdef  USE_SSSE3
	#define USE_SSE3
#endif
#ifdef  USE_SSE3
	#define USE_SSE2
#endif

// Note: Older versions of G++ and possibly Clang do not nest x86intrin.h inclusions properly.
#if defined IS_GNU || defined IS_CLANG
	#if !defined USE_SCALAR
		#include <x86intrin.h>
	#endif
#else
	#  if defined USE_AVX || defined USE_AVX2 || defined USE_FMA3 || defined USE_POPCNT || defined USE_LZCNT
		#include <immintrin.h> // ...also includes AVX2 and FMA3 in clang, gcc, and msvc
	#elif defined USE_SSE42
		#include <nmmintrin.h>
	#elif defined USE_SSE41
		#include <smmintrin.h>
	#elif defined USE_SSSE3
		#include <tmmintrin.h>
	#elif defined USE_SSE3
		#include <pmmintrin.h>
	#elif defined USE_SSE2
		#include <emmintrin.h>
	#endif
#endif

#ifdef USE_SSE2
	#include "../../ext/sse_mathfun.h"
#endif

//struct Block4x4f_t { float FBlock[16]; };
//#define ARCHFT4X4 Block4x4f_t
//#define ARCHFT4X4 __m512

#ifdef USE_AVX 
	#define HANDYMATH_ALIGN32 alignas(32) 
	#define ARCHDT4 __m256d
#else
	#define HANDYMATH_ALIGN32
	struct Block4d_t { double  X, Y, Z, W; };
	#define ARCHDT4   Block4d_t
#endif

#ifdef USE_SSE2
	#define HANDYMATH_ALIGN16 alignas(16) 
	#define ARCHFT4 __m128
	#define ARCHIT4 __m128i
	#define ARCHDT2 __m128d
	#define RESHUFFLE(A1, A2, A3, A4) _MM_SHUFFLE(A4, A3, A2, A1)
#else
	#define HANDYMATH_ALIGN16
	struct Block4f_t { float   X, Y, Z, W; };
	struct Block4i_t { int32_t X, Y, Z, W; };
	struct Block2d_t { double  X, Y; };
	#define ARCHFT4   Block4f_t
	#define ARCHIT4   Block4i_t
	#define ARCHDT2   Block2d_t
#endif

#ifdef USE_NEON
	#include <arm_neon.h>
	struct Block4d_t { double  X, Y, Z, W; };
	#define ARCHDT4   Block4d_t
	#define ARCHFT4 float32x4_t
	#define ARCHFT2 float32x2_t
	#define ARCHDT2 float64x2_t
	#define ARCHIT4 int32x4_t
#endif

#if !defined USE_SCALAR && !defined USE_SSE2 && !defined USE_NEON
	#error No implementation defined
#endif

#if !defined USE_SCALAR && !defined USE_NEON
	#define USE_SIMD
#endif

/// ---

#if defined USE_SIMD
	#define SIMD_IMP(...) __VA_ARGS__
	#define NON_SIMD_IMP(...)

	#if defined USE_FMA3
		#define SIMD_IMP_FMA3(...) __VA_ARGS__
		#define SIMD_IMP_NO_FMA3(...)
	#else
		#define SIMD_IMP_FMA3(...)
		#define SIMD_IMP_NO_FMA3(...) __VA_ARGS__
	#endif
	
	#if defined USE_SSE3
		#define SIMD_IMP_SSE3(...) __VA_ARGS__
		#define SIMD_IMP_NO_SSE3(...)
	#else
		#define SIMD_IMP_SSE3(...)
		#define SIMD_IMP_NO_SSE3(...) __VA_ARGS__
	#endif

	#if defined USE_SSSE3
		#define SIMD_IMP_SSSE3(...) __VA_ARGS__
		#define SIMD_IMP_NO_SSSE3(...)
	#else
		#define SIMD_IMP_SSSE3(...)
		#define SIMD_IMP_NO_SSSE3(...) __VA_ARGS__
	#endif

	
	#if defined USE_SSE41
		#define SIMD_IMP_SSE41(...) __VA_ARGS__
		#define SIMD_IMP_NO_SSE41(...)
	#else
		#define SIMD_IMP_SSE41(...)
		#define SIMD_IMP_NO_SSE41(...) __VA_ARGS__
	#endif

	#if defined USE_AVX
		#define SIMD_IMP_AVX(...) __VA_ARGS__
		#define SIMD_IMP_NO_AVX(...)
	#else
		#define SIMD_IMP_AVX(...)
		#define SIMD_IMP_NO_AVX(...) __VA_ARGS__
	#endif

	#if defined USE_AVX2
		#define SIMD_IMP_AVX2(...) __VA_ARGS__
		#define SIMD_IMP_NO_AVX2(...)
	#else
		#define SIMD_IMP_AVX2(...)
		#define SIMD_IMP_NO_AVX2(...) __VA_ARGS__
	#endif

#else
	#define SIMD_IMP(...)
	#define NON_SIMD_IMP(...) __VA_ARGS__

	#define SIMD_IMP_FMA3(...)
	#define SIMD_IMP_NO_FMA3(...)

	#define SIMD_IMP_SSE3(...)
	#define SIMD_IMP_NO_SSE3(...)

	#define SIMD_IMP_SSSE3(...)
	#define SIMD_IMP_NO_SSSE3(...)
	
	#define SIMD_IMP_SSE41(...)
	#define SIMD_IMP_NO_SSE41(...)

	#define SIMD_IMP_AVX(...)
	#define SIMD_IMP_NO_AVX(...)

	#define SIMD_IMP_AVX2(...)
	#define SIMD_IMP_NO_AVX2(...)


#endif

#if defined USE_SCALAR
	#define SCALAR_IMP(...) __VA_ARGS__
	#define NON_SCALAR_IMP(...)
#else
	#define SCALAR_IMP(...)
	#define NON_SCALAR_IMP(...) __VA_ARGS__
#endif

#if defined USE_NEON
	#define NEON_IMP(...) __VA_ARGS__
	#define NON_NEON_IMP(...)
#else
	#define NEON_IMP(...)
	#define NON_NEON_IMP(...) __VA_ARGS__
#endif

/// ---

#if !defined USE_SCALAR && !defined USE_SIMD && !defined USE_NEON
	#error NO API DEFINED
#endif

/// See ../../License.txt for license info.

namespace HANDYMATH_NS
{
	inline char const * BuildDescription()
	{
		return "Math SIMD ("
			#if defined USE_NEON
			" NEON"
			#endif
			#if defined USE_SCALAR
			" SCALAR"
			#endif
			#if defined USE_SSE2
			" SSE2"
			#endif
			#if defined USE_SSE3
			" SSE3"
			#endif
			#if defined USE_SSSE3 
			" SSSE3"
			#endif
			#if defined USE_SSE41
			" SSE4.1"
			#endif
			#if defined USE_SSE42 
			" SSE4.2"
			#endif
			#if defined USE_AVX 
			" AVX"
			#endif
			#if defined USE_FMA3 
			" FMA3"
			#endif
			#if defined USE_AVX2
			" AVX2"
			#endif
			" )";
	}

	enum class CMP : uint_fast8_t
	{
		AnyLT, AllLT, AnyLEQ, AllLEQ,
		AnyGT, AllGT, AnyGEQ, AllGEQ,
		AnyEQ, AllEQ, AnyNEQ, AllNEQ,
		AnyWE, AllWE, AnyNWE, AllNWE,

		Ordinal // Used for storing Vectors in collections. Compare NWE=>LT, X, then Y, then.... Default op for operator< on vector types.
	};

	enum class CMPI : uint_fast8_t
	{
		AnyLT, AllLT, AnyLEQ, AllLEQ,
		AnyGT, AllGT, AnyGEQ, AllGEQ,
		AnyEQ, AllEQ, AnyNEQ, AllNEQ,

		Ordinal // Used for storing Vectors in collections. Compare NEQ=>LT, X, then Y, then.... Default op for operator< on vector types.
	};

	/// Definitely faster than "1/std::sqrt(x);", but is an approximation.
	FORCEINLINE float RSqrtFast     (float x) 
	{
		SIMD_IMP  (return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(x)));)
		NEON_IMP  (return 1.0f / std::sqrt(x);)
		SCALAR_IMP(return 1.0f / std::sqrt(x);)
	}

	/// Not sure if this is faster than a plain "1/x", but being an approximation, it should be.
	FORCEINLINE float ReciprocalFast(float x) 
	{
		SIMD_IMP  (return _mm_cvtss_f32(_mm_rcp_ss(_mm_set_ss(x)));)
		NEON_IMP  (return 1.0f / x;)
		SCALAR_IMP(return 1.0f / x;)
	}

	/// Not sure if this is faster than a plain "std::sqrt", but being an approximation, it should be.
	FORCEINLINE float SqrtFast(float x)
	{
		SIMD_IMP  (return _mm_cvtss_f32(_mm_rcp_ss(_mm_rsqrt_ss(_mm_set_ss(x))));)
		NEON_IMP  (return std::sqrt(x);)
		SCALAR_IMP(return std::sqrt(x);)
	}
}