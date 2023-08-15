
/// See ../../License.txt for license info.

#pragma once

#include <ostream>
#include <cmath>
#include <cstdint>
#include <limits>
#include <array>
#include <cstring>  // memcpy

#include "Base.hpp"

// TODO, Exp, and Log

namespace HANDYMATH_NS {
	
	class Vector4i;
	class Vector3i;
	class Vector2i;

	class Vector3;
	class Vector2;

	class HANDYMATH_ALIGN16 Vector4 
	{
		friend class Vector4i;
		friend class Vector3;
		friend class Vector2;

	public:
		static constexpr int32_t NumElements  =  4_i32;
		static constexpr int32_t ElementBytes =  4_i32;
		static constexpr int32_t StrideBytes  = 16_i32;
		static constexpr int32_t UsefulBytes  = NumElements * ElementBytes;

		union
		{
			ARCHFT4              Native;
			struct { float       X, Y, Z, W; };
			struct { float       x, y, z, w; };
			std::array<float, 4> Array;
		};

		// implicit
		Vector4(ARCHFT4 const & native);
		operator ARCHFT4() const;

		Vector4();
		Vector4(Vector4 const & v);
		
		explicit 
		Vector4(float xyzw);
		Vector4(float xIn, float yIn, float zIn, float wIn);
		
		explicit 
		Vector4(float const * ary);

		Vector4 & operator=(Vector4 const &) = default;

		// Conversions:
		Vector2 XY () const;
		Vector2 ZW () const;
		Vector3 XYZ() const;
		Vector4i ToInteger() const;
		// ---

		static Vector4 FromArray  (float const * ary);
		void           CopyToArray(float       * ary) const;

		Vector4i SI128() const;

		template<int i0, int i1, int i2, int i3> Vector4 Shuffle() const;
		template<int i0, int i1, int i2, int i3> Vector4 ShuffleAABB(Vector4 const & b) const;

		template<int i> Vector4 Insert(float f) const;
		template<int i> Vector4 InsertZero()    const;
		
		Vector4 InsertX(float xIn) const;
		Vector4 InsertY(float yIn) const;
		Vector4 InsertZ(float zIn) const;
		Vector4 InsertW(float wIn) const;

		// These have all been optimized to be faster than using Shuffle<>()!
		Vector4 X000() const;
		Vector4 XY00() const;
		Vector4 XY01() const;
		Vector4 XY10() const;
		Vector4 XY11() const;
		Vector4 XYZ0() const;
		Vector4 XYZ1() const;

		Vector4 XXZZ() const;
		Vector4 YYWW() const;

		Vector4 XXYY() const;
		Vector4 XYXY() const;

		Vector4 ZZWW() const;
		Vector4 ZWZW() const;

		Vector4 ZCWD(Vector4 const & abcd) const;
		Vector4 XYAB(Vector4 const & abcd) const;
		Vector4 ZWCD(Vector4 const & abcd) const;
		Vector4 XAYB(Vector4 const & abcd) const;
		// ---
		
		Vector4 XXXX() const;
		Vector4 YYYY() const;
		Vector4 ZZZZ() const;
		Vector4 WWWW() const;

		Vector4 WZYX() const;
		Vector4 ZWXY() const;

		Vector4 Min(Vector4 const & rhs) const;
		Vector4 Max(Vector4 const & rhs) const;

		Vector4 Min(float rhs) const;
		Vector4 Max(float rhs) const;

		float   Min()     const;
		float   Max()     const;
		float   Sum()     const;
		float   Product() const;
		float   Mean()    const;

		Vector4 Abs()              const;
		Vector4 Sign()             const; //Returns, per component, the sign of the input vector: input >= 0.0 ? 1.0 : -1.0
		Vector4 Half()             const;
		Vector4 Reciprocal()       const;
		Vector4 ReciprocalApprox() const;
		Vector4 Sqrt()             const;
		Vector4 RSqrt()            const;
		Vector4 RSqrtApprox()      const;

		Vector4 To(Vector4 const & rhs) const;

		float   Distance       (Vector4 const & rhs) const;
		float   DistanceSquared(Vector4 const & rhs) const;

		float   Length()        const;
		float   LengthSquared() const;

		Vector4 Normalized() const;

		float   Angle(Vector4 const & rhs) const;
		float   Dot  (Vector4 const & rhs) const;
		//Vector4 Cross(Vector4 const & rhs) const;
		
		Vector4 Power      (float n) const;
		Vector4 PowerApprox(float n) const;

		bool    IsFacing(Vector4 const & rhs) const;

		bool    HasNaN()              const;
		bool    HasPositiveInfinity() const;
		bool    HasNegativeInfinity() const;

		bool    IsNaN()               const;
		bool    IsPositiveInfinity()  const;
		bool    IsNegativeInfinity()  const;

		bool    IsUnbounded()         const;

		template <CMP op> bool Compare(float           rhs) const;
		template <CMP op> bool Compare(Vector4 const & rhs) const;

		Vector4 Clamp(Vector4 const & min, Vector4 const & max) const;
		Vector4 Clamp(float   const   min, float   const   max) const;
				
		Vector4 MultiplyAdd   (Vector4 const & m2, Vector4 const &  a) const; // ((*this) * rhs) + a
		Vector4 MultiplySub   (Vector4 const & m2, Vector4 const &  a) const; // ((*this) * rhs) - a

		Vector4 Lerp      (Vector4 const & rhs, float           t) const;
		Vector4 Lerp      (Vector4 const & rhs, Vector4 const & t) const;
		Vector4 SmoothStep(Vector4 const & rhs, float           t) const;
		Vector4 SmoothStep(Vector4 const & rhs, Vector4 const & t) const;

		Vector4 Round_Near () const;
		Vector4 Round_Floor() const;
		Vector4 Round_Ceil () const;
		Vector4 Round_Trunc() const;

		Vector4 FMod(Vector4 const & div) const;
		Vector4 FMod(float div) const;
		
		float & operator[](int idx);
		float   operator[](int idx) const;

		Vector4 Sin ();
		Vector4 Cos ();
		Vector4 Tan ();

		Vector4 ASin();
		Vector4 ACos();
		Vector4 ATan();

		Vector4 SinH();
		Vector4 CosH();
		Vector4 TanH();

		static Vector4 Zero();
		static Vector4 One();
		static Vector4 NegOne();
		static Vector4 UnitX();
		static Vector4 UnitY();
		static Vector4 UnitZ();
		static Vector4 UnitW();
		static Vector4 UnitXW();
		static Vector4 UnitYW();
		static Vector4 UnitZW();
		static Vector4 NegUnitX();
		static Vector4 NegUnitY();
		static Vector4 NegUnitZ();
		static Vector4 NegUnitW();
		static Vector4 PositiveInfinity();
		static Vector4 NegativeInfinity();
		static Vector4 NaN();
		static Vector4 FFFFFFFF();

		template <class SerialOp> void serial(SerialOp & ser)
		{
			ser
				(X)
				(Y)
				(Z)
				(W);
		}
	};

	Vector4 operator-(Vector4 const & lhs);
	Vector4 operator~(Vector4 const & lhs);

	Vector4 operator+(Vector4 const & lhs, Vector4 const & rhs);
	Vector4 operator-(Vector4 const & lhs, Vector4 const & rhs);
	Vector4 operator*(Vector4 const & lhs, Vector4 const & rhs);
	Vector4 operator/(Vector4 const & lhs, Vector4 const & rhs);
	Vector4 operator&(Vector4 const & lhs, Vector4 const & rhs);
	Vector4 operator|(Vector4 const & lhs, Vector4 const & rhs);
	Vector4 operator^(Vector4 const & lhs, Vector4 const & rhs);

	Vector4 operator+(Vector4 const & lhs, float rhs);
	Vector4 operator-(Vector4 const & lhs, float rhs);
	Vector4 operator*(Vector4 const & lhs, float rhs);
	Vector4 operator/(Vector4 const & lhs, float rhs);
	Vector4 operator&(Vector4 const & lhs, float rhs);
	Vector4 operator|(Vector4 const & lhs, float rhs);
	Vector4 operator^(Vector4 const & lhs, float rhs);

	Vector4 operator+(float lhs, Vector4 const & rhs);
	Vector4 operator-(float lhs, Vector4 const & rhs);
	Vector4 operator*(float lhs, Vector4 const & rhs);
	Vector4 operator/(float lhs, Vector4 const & rhs);
	Vector4 operator&(Vector4 const & lhs, float rhs);
	Vector4 operator|(Vector4 const & lhs, float rhs);
	Vector4 operator^(Vector4 const & lhs, float rhs);

	Vector4 & operator+=(Vector4 & lhs, Vector4 const & rhs);
	Vector4 & operator-=(Vector4 & lhs, Vector4 const & rhs);
	Vector4 & operator*=(Vector4 & lhs, Vector4 const & rhs);
	Vector4 & operator/=(Vector4 & lhs, Vector4 const & rhs);
	Vector4 & operator&=(Vector4 & lhs, Vector4 const & rhs);
	Vector4 & operator|=(Vector4 & lhs, Vector4 const & rhs);
	Vector4 & operator^=(Vector4 & lhs, Vector4 const & rhs);

	Vector4 & operator+=(Vector4 & lhs, float rhs);
	Vector4 & operator-=(Vector4 & lhs, float rhs);
	Vector4 & operator*=(Vector4 & lhs, float rhs);
	Vector4 & operator/=(Vector4 & lhs, float rhs);
	Vector4 & operator&=(Vector4 & lhs, float rhs);
	Vector4 & operator|=(Vector4 & lhs, float rhs);
	Vector4 & operator^=(Vector4 & lhs, float rhs);

	bool operator< (Vector4 const & lhs, Vector4 const & rhs);

	bool operator==(Vector4 const & lhs, Vector4 const & rhs);
	bool operator!=(Vector4 const & lhs, Vector4 const & rhs);

	bool operator==(Vector4 const & lhs, float rhs);
	bool operator!=(Vector4 const & lhs, float rhs);

	bool operator==(float lhs, Vector4 const & rhs);
	bool operator!=(float lhs, Vector4 const & rhs);
}

// Not supposed to add overloads to std technically... but I want my templates that use std::to_string, min, max, etc to work, so oh well then.
namespace std {
	FORCEINLINE ::HANDYMATH_NS::Vector4 min(::HANDYMATH_NS::Vector4 const & a, ::HANDYMATH_NS::Vector4 const & b);
	FORCEINLINE ::HANDYMATH_NS::Vector4 max(::HANDYMATH_NS::Vector4 const & a, ::HANDYMATH_NS::Vector4 const & b);

	FORCEINLINE std::string to_string(::HANDYMATH_NS::Vector4 const & v)
	{
		return "("s + std::to_string(v.X) + ", "s + std::to_string(v.Y) + ", "s + std::to_string(v.Z) + ", "s + std::to_string(v.W) + ")"s;
	}
}

static inline std::ostream & operator<<(std::ostream & os, const ::HANDYMATH_NS::Vector4 & v)  
{  
	os << "(" << v.X << ", " << v.Y << ", " << v.Z << ", " << v.W << ")";  
	return os;  
}  

#if defined USE_SIMD
FORCEINLINE __m128 _mm_floor_ps2(const __m128& x)
{
	__m128i v0  = _mm_setzero_si128();
	__m128i v1  = _mm_cmpeq_epi32(v0,v0);
	__m128i ji  = _mm_srli_epi32( v1, 25);
	auto temp = _mm_slli_epi32( ji, 23); //create vector 1.0f
	__m128  j = *(__m128*)&temp;
	__m128i i   = _mm_cvttps_epi32(x);
	__m128  fi  = _mm_cvtepi32_ps(i);
	__m128  igx = _mm_cmpgt_ps(fi, x);
	j           = _mm_and_ps(igx, j);
	return _mm_sub_ps(fi, j);
}
FORCEINLINE __m128 _mm_ceil_ps2(const __m128 & x)
{
	__m128i v0  = _mm_setzero_si128();
	__m128i v1  = _mm_cmpeq_epi32(v0,v0);
	__m128i ji  = _mm_srli_epi32( v1, 25);
	auto temp =_mm_slli_epi32( ji, 23); //create vector 1.0f
	__m128  j = *(__m128*)&temp;
	__m128i i   = _mm_cvttps_epi32(x);
	__m128  fi  = _mm_cvtepi32_ps(i);
	__m128  igx = _mm_cmplt_ps(fi, x);
	j           = _mm_and_ps(igx, j);
	return _mm_add_ps(fi, j);
}

FORCEINLINE __m128 _mm_round_ps2(const __m128 & a)
{
	__m128  v0        = _mm_setzero_ps();             //generate the highest value &lt; 2
	__m128  v1        = _mm_cmpeq_ps(v0,v0);
	auto temp = _mm_srli_epi32(*(__m128i*)&v1, 2);
	__m128  vNearest2 = *(__m128*)&temp;
	__m128i i         = _mm_cvttps_epi32(a);
	__m128  aTrunc    = _mm_cvtepi32_ps(i);        // truncate a
	__m128  rmd       = _mm_sub_ps(a, aTrunc);        // get remainder
	__m128  rmd2      = _mm_mul_ps( rmd, vNearest2); // mul remainder by near 2 will yield the needed offset
	__m128i rmd2i     = _mm_cvttps_epi32(rmd2);    // after being truncated of course
	__m128  rmd2Trunc = _mm_cvtepi32_ps(rmd2i);
	__m128  r         = _mm_add_ps(aTrunc, rmd2Trunc);
	return  r;
}

FORCEINLINE __m128 _mm_mod_ps2(const __m128& a, const __m128& aDiv)
{
	__m128  c      = _mm_div_ps(a,aDiv);
	__m128i i      = _mm_cvttps_epi32(c);
	__m128  cTrunc = _mm_cvtepi32_ps(i);
	__m128  base   = _mm_mul_ps(cTrunc, aDiv);
	__m128  r      = _mm_sub_ps(a, base);
	return r;
}

#endif

namespace HANDYMATH_NS {

	FORCEINLINE Vector4::Vector4()                       : Native(Vector4::Zero()) { }
	FORCEINLINE Vector4::Vector4(ARCHFT4 const & native) : Native(native) { }
	FORCEINLINE Vector4::Vector4(Vector4 const & v)      : Native(v.Native) { }

	#if defined USE_SIMD
	FORCEINLINE Vector4::Vector4(float xyzw) : Native(_mm_set1_ps(xyzw)) { }
	FORCEINLINE Vector4::Vector4(float xIn, float yIn, float zIn, float wIn) : Native(_mm_setr_ps(xIn, yIn, zIn, wIn)) { }
	FORCEINLINE Vector4::Vector4(float const * ary) : Native (_mm_loadu_ps(ary)) { }
	#elif defined USE_NEON
	FORCEINLINE Vector4::Vector4(float xyzw) : Native(vdupq_n_f32(xyzw)) { }
	FORCEINLINE Vector4::Vector4(float xIn, float yIn, float zIn, float wIn) { float t[4] = { xIn,yIn,zIn,wIn }; Native = vld1q_f32(t); }
	FORCEINLINE Vector4::Vector4(float const * ary) : { float * ary32 = (float const *)ary; Native = vld1q_f32(ary32); }
	#elif defined USE_SCALAR
	FORCEINLINE Vector4::Vector4(float xyzw) : Native{xyzw, xyzw, xyzw, xyzw} { }
	FORCEINLINE Vector4::Vector4(float xIn, float yIn, float zIn, float wIn) : Native{ xIn, yIn, zIn, wIn } { }
	FORCEINLINE Vector4::Vector4(float const * ary) : Native { ary[0], ary[1], ary[2], ary[3] } { }
	#endif

	FORCEINLINE Vector4::operator ARCHFT4() const { return Native; }

	FORCEINLINE Vector4 Vector4::FromArray(float const * ary) { return Vector4(ary); }

	FORCEINLINE void    Vector4::CopyToArray(float       * ary) const 
	{
		SIMD_IMP  (_mm_storeu_ps(ary, Native);)
		NEON_IMP  (vst1q_f32(ary, Native);)
		SCALAR_IMP(memcpy(ary, &Array[0], sizeof(float) * 4);)
	}

	template<int i0, int i1, int i2, int i3> FORCEINLINE Vector4 Vector4::Shuffle() const
	{
		SIMD_IMP  (return _mm_shuffle_ps(Native, Native, RESHUFFLE(i0, i1, i2, i3));) // In theory this might be faster on certain compilers. I saw ZERO DIFFERENCE: return _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(Native), RESHUFFLE(i0, i1, i2, i3)));
		NEON_IMP  (return Vector4(Array[i0], Array[i1], Array[i2], Array[i3]);)
		SCALAR_IMP(return Vector4(Array[i0], Array[i1], Array[i2], Array[i3]);)
	}

	template<int i0, int i1, int i2, int i3> FORCEINLINE Vector4 Vector4::ShuffleAABB(Vector4 const & b) const
	{
		SIMD_IMP  (return _mm_shuffle_ps(Native, b, RESHUFFLE(i0, i1, i2, i3));)
		NEON_IMP  (return Vector4(Array[i0], Array[i1], b.Array[i2], b.Array[i3]);)
		SCALAR_IMP(return Vector4(Array[i0], Array[i1], b.Array[i2], b.Array[i3]);)
	}

	template<int i> FORCEINLINE Vector4 Vector4::Insert(float f) const
	{
		SIMD_IMP_SSE41   (return _mm_insert_ps(Native, _mm_set_ss(f), i<<4);)
		SIMD_IMP_NO_SSE41(auto n = *this; n.Array[i] = f; return n;)
		NEON_IMP         (auto n = *this; n.Array[i] = f; return n;)
		SCALAR_IMP       (auto n = *this; n.Array[i] = f; return n;)
	}

	template<int i> FORCEINLINE Vector4 Vector4::InsertZero()    const
	{
		SIMD_IMP_SSE41   (return _mm_insert_ps(Native, _mm_setzero_ps(), i<<4);)
		SIMD_IMP_NO_SSE41(auto n = *this; n.Array[i] = 0.0f; return n;)
		NEON_IMP         (auto n = *this; n.Array[i] = 0.0f; return n;)
		SCALAR_IMP       (auto n = *this; n.Array[i] = 0.0f; return n;)
	}

	FORCEINLINE Vector4 Vector4::InsertX(float xIn) const { return Insert<0>(xIn); }
	FORCEINLINE Vector4 Vector4::InsertY(float yIn) const { return Insert<1>(yIn); }
	FORCEINLINE Vector4 Vector4::InsertZ(float zIn) const { return Insert<2>(zIn); }
	FORCEINLINE Vector4 Vector4::InsertW(float wIn) const { return Insert<3>(wIn); }

	FORCEINLINE Vector4 Vector4::X000() const 
	{
		SIMD_IMP  (return _mm_set_ss(X);)
		NEON_IMP  (return Vector4(X, 0.0f, 0.0f, 0.0f);)
		SCALAR_IMP(return Vector4(X, 0.0f, 0.0f, 0.0f);)
	}

	FORCEINLINE Vector4 Vector4::XY00() const 
	{
		SIMD_IMP  (return _mm_movelh_ps(Native, _mm_setzero_ps());)
		NEON_IMP  (return Vector4(X,    Y, 0.0f, 0.0f);)
		SCALAR_IMP(return Vector4(X,    Y, 0.0f, 0.0f);)
	}

	FORCEINLINE Vector4 Vector4::XY01() const 
	{
		SIMD_IMP  (return _mm_movelh_ps(Native, UnitY());)
		NEON_IMP  (return Vector4(X,    Y, 0.0f, 1.0f);)
		SCALAR_IMP(return Vector4(X,    Y, 0.0f, 1.0f);)
	}

	FORCEINLINE Vector4 Vector4::XY11() const 
	{
		SIMD_IMP  (return _mm_movelh_ps(Native,   One());)
		NEON_IMP  (return Vector4(X,    Y, 1.0f, 1.0f);)
		SCALAR_IMP(return Vector4(X,    Y, 1.0f, 1.0f);)
	}

	FORCEINLINE Vector4 Vector4::XYZ0() const 
	{
		SIMD_IMP  (return InsertZero<3>();)
		NEON_IMP  (return Vector4(X,    Y,    Z, 0.0f);)
		SCALAR_IMP(return Vector4(X,    Y,    Z, 0.0f);)
	}

	FORCEINLINE Vector4 Vector4::XYZ1() const 
	{
		SIMD_IMP  (return Insert<3>(1.0f);)
		NEON_IMP  (return Vector4(X,    Y,    Z, 1.0f);)
		SCALAR_IMP(return Vector4(X,    Y,    Z, 1.0f);)
	}

	FORCEINLINE Vector4 Vector4::XY10() const 
	{
		SIMD_IMP  (return _mm_movelh_ps(Native, _mm_set_ss(1.0f));)
		NEON_IMP  (return Vector4(X,    Y, 1.0f, 0.0f);)
		SCALAR_IMP(return Vector4(X,    Y, 1.0f, 0.0f);)
	}

	FORCEINLINE Vector4 Vector4::XXZZ() const 
	{
		SIMD_IMP_SSE3   (return _mm_moveldup_ps(Native);) // SLIGHTLY faster
		SIMD_IMP_NO_SSE3(return _mm_shuffle_ps(Native, Native, RESHUFFLE(0, 0, 2, 2) );)
		NEON_IMP        (return Vector4(X, X, Z, Z);)
		SCALAR_IMP      (return Vector4(X, X, Z, Z);)
	}

	FORCEINLINE Vector4 Vector4::YYWW() const 
	{
		SIMD_IMP_SSE3   (return _mm_movehdup_ps(Native);) // SLIGHTLY faster
		SIMD_IMP_NO_SSE3(return _mm_shuffle_ps(Native, Native, RESHUFFLE(1, 1, 3, 3) );)
		NEON_IMP        (return Vector4(Y, Y, W, W);)
		SCALAR_IMP      (return Vector4(Y, Y, W, W);)
	}

	FORCEINLINE Vector4 Vector4::XXYY() const 
	{
		SIMD_IMP  (return _mm_unpacklo_ps(Native, Native);)
		NEON_IMP  (return Vector4(X, X, Y, Y);)
		SCALAR_IMP(return Vector4(X, X, Y, Y);)
	}

	FORCEINLINE Vector4 Vector4::XYXY() const 
	{
		SIMD_IMP  (return _mm_movelh_ps  (Native, Native);)
		NEON_IMP  (return Vector4(X, Y, X, Y);)
		SCALAR_IMP(return Vector4(X, Y, X, Y);)
	}

	FORCEINLINE Vector4 Vector4::ZZWW() const 
	{
		SIMD_IMP  (return _mm_unpackhi_ps(Native, Native);)
		NEON_IMP  (return Vector4(Z, Z, W, W);)
		SCALAR_IMP(return Vector4(Z, Z, W, W);)
	}

	FORCEINLINE Vector4 Vector4::ZWZW() const 
	{
		SIMD_IMP  (return _mm_movehl_ps(Native, Native);)
		NEON_IMP  (return Vector4(Z, W, Z, W);)
		SCALAR_IMP(return Vector4(Z, W, Z, W);)
	}

	FORCEINLINE Vector4 Vector4::ZCWD(Vector4 const & abcd) const 
	{
		SIMD_IMP  (return _mm_unpackhi_ps(Native, abcd);)
		NEON_IMP  (return Vector4(Z, abcd.Z, W, abcd.W);)
		SCALAR_IMP(return Vector4(Z, abcd.Z, W, abcd.W);)
	}

	FORCEINLINE Vector4 Vector4::XYAB(Vector4 const & abcd) const 
	{
		SIMD_IMP  (return _mm_movelh_ps(Native, abcd);)
		NEON_IMP  (return Vector4(X, Y, abcd.X, abcd.Y);)
		SCALAR_IMP(return Vector4(X, Y, abcd.X, abcd.Y);)
	}

	FORCEINLINE Vector4 Vector4::ZWCD(Vector4 const & abcd) const 
	{
		SIMD_IMP  (return _mm_movehl_ps(abcd, Native);)
		NEON_IMP  (return Vector4(Z, W, abcd.Z, abcd.W);)
		SCALAR_IMP(return Vector4(Z, W, abcd.Z, abcd.W);)
	}

	FORCEINLINE Vector4 Vector4::XAYB(Vector4 const & abcd) const 
	{
		SIMD_IMP  (return _mm_unpacklo_ps(Native, abcd);)
		NEON_IMP  (return Vector4(X, abcd.X,      Y, abcd.Y);)
		SCALAR_IMP(return Vector4(X, abcd.X,      Y, abcd.Y);)
	}

	FORCEINLINE Vector4 Vector4::XXXX() const
	{
		NON_NEON_IMP(return Shuffle<0, 0, 0, 0>();)
		    NEON_IMP(return vdupq_lane_f32( vget_low_f32(Native), 0);)
	}

	FORCEINLINE Vector4 Vector4::YYYY() const 
	{
		NON_NEON_IMP(return Shuffle<1, 1, 1, 1>();)
		    NEON_IMP(return vdupq_lane_f32( vget_low_f32(Native), 1);)
	}

	FORCEINLINE Vector4 Vector4::ZZZZ() const
	{
		NON_NEON_IMP(return Shuffle<2, 2, 2, 2>();)
		    NEON_IMP(return vdupq_lane_f32(vget_high_f32(Native), 0);)
	}

	FORCEINLINE Vector4 Vector4::WWWW() const
	{
		NON_NEON_IMP(return Shuffle<3, 3, 3, 3>();)
		    NEON_IMP(return vdupq_lane_f32(vget_high_f32(Native), 1);)
	}

	FORCEINLINE Vector4 Vector4::ZWXY() const { return Shuffle<2, 3, 0, 1>(); }
	FORCEINLINE Vector4 Vector4::WZYX() const { return Shuffle<3, 2, 1, 0>(); }

	FORCEINLINE Vector4 Vector4::Min(Vector4 const & rhs) const 
	{
		SIMD_IMP  (return _mm_min_ps(Native, rhs);)
		NEON_IMP  (return vminq_f32(Native, rhs);)
		SCALAR_IMP(return Vector4(Handy::FastMin(X, rhs.X), Handy::FastMin(Y, rhs.Y), Handy::FastMin(Z, rhs.Z), Handy::FastMin(W, rhs.W));)
	}
		 
	FORCEINLINE Vector4 Vector4::Max(Vector4 const & rhs) const
	{
		SIMD_IMP  (return _mm_max_ps(Native, rhs);)
		NEON_IMP  (return vmaxq_f32(Native, rhs);)
		SCALAR_IMP(return Vector4(Handy::FastMax(X, rhs.X), Handy::FastMax(Y, rhs.Y), Handy::FastMax(Z, rhs.Z), Handy::FastMax(W, rhs.W));)
	}

	FORCEINLINE Vector4 Vector4::Min(float rhs) const
	{
		SIMD_IMP  (return _mm_min_ps(Native, Vector4(rhs));)
		NEON_IMP  (return vminq_f32(Native, Vector4(rhs));)
		SCALAR_IMP(return Vector4(Handy::FastMin(X, rhs), Handy::FastMin(Y, rhs), Handy::FastMin(Z, rhs), Handy::FastMin(W, rhs));)
	}

	FORCEINLINE Vector4 Vector4::Max(float rhs) const
	{
		SIMD_IMP  (return _mm_max_ps(Native, Vector4(rhs));)
		NEON_IMP  (return vmaxq_f32(Native, Vector4(rhs));)
		SCALAR_IMP(return Vector4(Handy::FastMax(X, rhs), Handy::FastMax(Y, rhs), Handy::FastMax(Z, rhs), Handy::FastMax(W, rhs));)
	}

	FORCEINLINE float Vector4::Min() const 
	{
		SIMD_IMP  (auto abba = WZYX().Min(Native); return _mm_cvtss_f32(abba.Min(abba.ZWXY()));)
		NEON_IMP  (return Handy::FastMin(Handy::FastMin(X, Y), Handy::FastMin(Z, W));)
		SCALAR_IMP(return Handy::FastMin(Handy::FastMin(X, Y), Handy::FastMin(Z, W));)
	}

	FORCEINLINE float Vector4::Max() const 
	{
		SIMD_IMP  (auto abba = WZYX().Max(Native); return _mm_cvtss_f32(abba.Max(abba.ZWXY()));)
		NEON_IMP  (return Handy::FastMax(Handy::FastMax(X, Y), Handy::FastMax(Z, W));)
		SCALAR_IMP(return Handy::FastMax(Handy::FastMax(X, Y), Handy::FastMax(Z, W));)
	}

	FORCEINLINE float Vector4::Sum() const 
	{
		#if defined USE_SIMD
			#if defined(USE_SSE41)
				return _mm_cvtss_f32(_mm_dp_ps(Native, Vector4(1.0f), 0xff));
			#elif defined(USE_SSE3)
				auto temp = _mm_hadd_ps(Native, Native);
				return ((Vector4)_mm_hadd_ps(temp, temp)).X;
			//#elif defined(USE_SSE3)
			//	__m128 shuf = _mm_movehdup_ps(Native);   // broadcast elements 3,1 to 2,0
			//	__m128 sums = _mm_add_ps(Native, shuf);
			//	shuf        = _mm_movehl_ps(shuf, sums); // high half -> low half
			//	sums        = _mm_add_ss(sums, shuf);
			//	return        _mm_cvtss_f32(sums);
			#else
				__m128 shuf = _mm_shuffle_ps(Native, Native, _MM_SHUFFLE(2, 3, 0, 1));  // [ C D | A B ]
				__m128 sums = _mm_add_ps(Native, shuf);  // sums = [ D+C C+D | B+A A+B ]
				shuf        = _mm_movehl_ps(shuf, sums); //        [   C   D | D+C C+D ]  // let the compiler avoid a mov by reusing shuf
				sums        = _mm_add_ss(sums, shuf);
				return        _mm_cvtss_f32(sums);
			#endif
		#endif
		NEON_IMP  (return X + Y + Z + W;)
		SCALAR_IMP(return X + Y + Z + W;)
	}

	FORCEINLINE float Vector4::Product() const 
	{
		SIMD_IMP  (auto abba = WZYX() * Vector4(Native); return _mm_cvtss_f32(abba * abba.ZWXY());)
		NEON_IMP  (return X * Y * Z * W;)
		SCALAR_IMP(return X * Y * Z * W;)
	}

	FORCEINLINE float   Vector4::Mean()       const { return Sum()   * 0.25f; }
	
	FORCEINLINE Vector4 Vector4::Abs() const 
	{
		SIMD_IMP  (return _mm_and_ps(Native, _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF)));)
		NEON_IMP  (return Vector4(std::abs(X), std::abs(Y), std::abs(Z), std::abs(W));)
		SCALAR_IMP(return Vector4(std::abs(X), std::abs(Y), std::abs(Z), std::abs(W));)
	}

	FORCEINLINE Vector4 Vector4::Sign() const
	{
		    SIMD_IMP(Vector4 mask = _mm_cmpge_ps(Native, Zero()); return _mm_andnot_ps(mask, NegOne()) | (One() & mask);)
		NON_SIMD_IMP(return Vector4(X >= 0.0f ? 1.0f : -1.0f, Y >= 0.0f ? 1.0f : -1.0f, Z >= 0.0f ? 1.0f : -1.0f, W >= 0.0f ? 1.0f : -1.0f);)
	}

	FORCEINLINE Vector4 Vector4::Half()       const { return (*this) * 0.5f; }
	
	FORCEINLINE Vector4 Vector4::Reciprocal() const { return 1.0f / (*this); }

	FORCEINLINE Vector4 Vector4::ReciprocalApprox() const 
	{
		SIMD_IMP  (return _mm_rcp_ps(Native);)
		NEON_IMP  (Vector4 estimate = vrecpeq_f32(Native);
							 estimate = vmulq_f32(vrecpsq_f32(estimate, Native), estimate);
							 estimate = vmulq_f32(vrecpsq_f32(estimate, Native), estimate);
					 return estimate;)
		SCALAR_IMP(return 1.0f / (*this);)
	}

	FORCEINLINE Vector4 Vector4::Sqrt() const 
	{
		SIMD_IMP  (return _mm_sqrt_ps(Native);)
		NEON_IMP  (return Vector4(std::sqrt(X), std::sqrt(Y), std::sqrt(Z), std::sqrt(W));)
		SCALAR_IMP(return Vector4(std::sqrt(X), std::sqrt(Y), std::sqrt(Z), std::sqrt(W));)
	}

	FORCEINLINE Vector4 Vector4::RSqrt()      const { return this->Sqrt().Reciprocal(); }

	FORCEINLINE Vector4 Vector4::RSqrtApprox() const
	{
		SIMD_IMP  (return _mm_rsqrt_ps(Native);)
		NEON_IMP  (return this->Sqrt().Reciprocal();)
		SCALAR_IMP(return this->Sqrt().Reciprocal();)
	}

	FORCEINLINE Vector4 Vector4::To(Vector4 const & rhs) const { return rhs - (*this); }

	FORCEINLINE float   Vector4::Distance       (Vector4 const & rhs) const { return (rhs - (*this)).Length(); }
	FORCEINLINE float   Vector4::DistanceSquared(Vector4 const & rhs) const { return (rhs - (*this)).LengthSquared(); }

	FORCEINLINE float Vector4::Length() const 
	{
		SIMD_IMP_SSE41   (return _mm_cvtss_f32(Vector4(_mm_dp_ps(Native, Native, 0xff)).Sqrt());)
		SIMD_IMP_NO_SSE41(return std::sqrt(Dot(*this));)
		NEON_IMP         (return std::sqrt(Dot(*this));)
		SCALAR_IMP       (return std::sqrt(Dot(*this));)
	}

	FORCEINLINE float   Vector4::LengthSquared() const { return Dot(*this); }

	FORCEINLINE Vector4 Vector4::Normalized()  const { return (*this) * (1.0f / Length()); }

	FORCEINLINE float   Vector4::Angle(Vector4 const & rhs) const
	{
		float vdot = Normalized().Dot(rhs.Normalized());

		if (vdot < -1.0f)
			vdot = -1.0f;

		if (vdot > 1.0f)
			vdot = 1.0f;

		return std::acos(vdot);
	}

	FORCEINLINE float Vector4::Dot  (Vector4 const & rhs) const 
	{
		SIMD_IMP_SSE41   (return _mm_cvtss_f32(_mm_dp_ps(Native, rhs, 0xff));)
		SIMD_IMP_NO_SSE41(return ((*this) * rhs).Sum();)
		NEON_IMP         (return ((*this) * rhs).Sum();)
		SCALAR_IMP       (return ((*this) * rhs).Sum();)
	}

	FORCEINLINE Vector4 Vector4::Power(float n) const { return Vector4(std::pow(X, n), std::pow(Y, n), std::pow(Z, n), std::pow(W, n)); } // SVML: // return _mm_pow_ps(Native, Vector4(n));

	FORCEINLINE Vector4 Vector4::PowerApprox(float n) const 
	{
		SIMD_IMP  (return exp_ps(n * Vector4(log_ps(Native)));) // Note: This does NOT generate NaN, or any other denormals AFAICT.
		NEON_IMP  (return Vector4(std::pow(X, n), std::pow(Y, n), std::pow(Z, n), std::pow(W, n));)
		SCALAR_IMP(return Vector4(std::pow(X, n), std::pow(Y, n), std::pow(Z, n), std::pow(W, n));)
	}
	
	FORCEINLINE bool Vector4::IsFacing(Vector4 const & rhs) const { return Dot(rhs) > 0.0f; }

	FORCEINLINE bool Vector4::HasNaN()              const { return std::isnan(X) || std::isnan(Y) || std::isnan(Z) || std::isnan(W); }
	FORCEINLINE bool Vector4::HasPositiveInfinity() const { return Compare<CMP::AnyEQ>( std::numeric_limits<float>::infinity()); } 
	FORCEINLINE bool Vector4::HasNegativeInfinity() const { return Compare<CMP::AnyEQ>(-std::numeric_limits<float>::infinity()); }

	FORCEINLINE bool Vector4::IsNaN()               const { return std::isnan(X) && std::isnan(Y) && std::isnan(Z) && std::isnan(W); }
	FORCEINLINE bool Vector4::IsPositiveInfinity()  const { return Compare<CMP::AllEQ>( std::numeric_limits<float>::infinity()); }
	FORCEINLINE bool Vector4::IsNegativeInfinity()  const { return Compare<CMP::AllEQ>(-std::numeric_limits<float>::infinity()); }

	FORCEINLINE bool Vector4::IsUnbounded() const { return HasPositiveInfinity() || HasNegativeInfinity(); }

	template <CMP op>
	FORCEINLINE bool Vector4::Compare(float rhs) const
	{
		return Compare<op>(Vector4(rhs));
	}

	template <CMP op>
	FORCEINLINE bool Vector4::Compare(Vector4 const & rhs) const
	{
		/// IMPORTANT: If any of these are upgraded, the implementation in the Vector2-3* versions need to be changed too.
		if (op == CMP::AnyLT)  { SIMD_IMP(return _mm_movemask_ps(_mm_cmplt_ps (Native, rhs)) != 0;)   NON_SIMD_IMP(return X <  rhs.X || Y <  rhs.Y || Z <  rhs.Z || W <  rhs.W;) }
		if (op == CMP::AllLT)  { SIMD_IMP(return _mm_movemask_ps(_mm_cmplt_ps (Native, rhs)) == 0xF;) NON_SIMD_IMP(return X <  rhs.X && Y <  rhs.Y && Z <  rhs.Z && W <  rhs.W;) }
		if (op == CMP::AnyLEQ) { SIMD_IMP(return _mm_movemask_ps(_mm_cmple_ps (Native, rhs)) != 0;)   NON_SIMD_IMP(return X <= rhs.X || Y <= rhs.Y || Z <= rhs.Z || W <= rhs.W;) }
		if (op == CMP::AllLEQ) { SIMD_IMP(return _mm_movemask_ps(_mm_cmple_ps (Native, rhs)) == 0xF;) NON_SIMD_IMP(return X <= rhs.X && Y <= rhs.Y && Z <= rhs.Z && W <= rhs.W;) }
		if (op == CMP::AnyGT)  { SIMD_IMP(return _mm_movemask_ps(_mm_cmpgt_ps (Native, rhs)) != 0;)   NON_SIMD_IMP(return X >  rhs.X || Y >  rhs.Y || Z >  rhs.Z || W >  rhs.W;) }
		if (op == CMP::AllGT)  { SIMD_IMP(return _mm_movemask_ps(_mm_cmpgt_ps (Native, rhs)) == 0xF;) NON_SIMD_IMP(return X >  rhs.X && Y >  rhs.Y && Z >  rhs.Z && W >  rhs.W;) }
		if (op == CMP::AnyGEQ) { SIMD_IMP(return _mm_movemask_ps(_mm_cmpge_ps (Native, rhs)) != 0;)   NON_SIMD_IMP(return X >= rhs.X || Y >= rhs.Y || Z >= rhs.Z || W >= rhs.W;) }
		if (op == CMP::AllGEQ) { SIMD_IMP(return _mm_movemask_ps(_mm_cmpge_ps (Native, rhs)) == 0xF;) NON_SIMD_IMP(return X >= rhs.X && Y >= rhs.Y && Z >= rhs.Z && W >= rhs.W;) }
		if (op == CMP::AnyEQ)  { SIMD_IMP(return _mm_movemask_ps(_mm_cmpeq_ps (Native, rhs)) != 0;)   NON_SIMD_IMP(return X == rhs.X || Y == rhs.Y || Z == rhs.Z || W == rhs.W;) }
		if (op == CMP::AllEQ)  { SIMD_IMP(return _mm_movemask_ps(_mm_cmpeq_ps (Native, rhs)) == 0xF;) NON_SIMD_IMP(return X == rhs.X && Y == rhs.Y && Z == rhs.Z && W == rhs.W;) }
		if (op == CMP::AnyNEQ) { SIMD_IMP(return _mm_movemask_ps(_mm_cmpneq_ps(Native, rhs)) != 0;)   NON_SIMD_IMP(return X != rhs.X || Y != rhs.Y || Z != rhs.Z || W != rhs.W;) }
		if (op == CMP::AllNEQ) { SIMD_IMP(return _mm_movemask_ps(_mm_cmpneq_ps(Native, rhs)) == 0xF;) NON_SIMD_IMP(return X != rhs.X && Y != rhs.Y && Z != rhs.Z && W != rhs.W;) }
	
		if (op == CMP::AnyWE)
		{
			return (HANDY_NS::WithinEpsilon(X, rhs.X)
				||  HANDY_NS::WithinEpsilon(Y, rhs.Y)
				||  HANDY_NS::WithinEpsilon(Z, rhs.Z)
				||  HANDY_NS::WithinEpsilon(W, rhs.W));
		}

		if (op == CMP::AllWE)
		{
			return (HANDY_NS::WithinEpsilon(X, rhs.X)
				&&  HANDY_NS::WithinEpsilon(Y, rhs.Y)
				&&  HANDY_NS::WithinEpsilon(Z, rhs.Z)
				&&  HANDY_NS::WithinEpsilon(W, rhs.W));
		}

		if (op == CMP::AnyNWE)
		{
			return !(HANDY_NS::WithinEpsilon(X, rhs.X)
				&&   HANDY_NS::WithinEpsilon(Y, rhs.Y)
				&&   HANDY_NS::WithinEpsilon(Z, rhs.Z)
				&&   HANDY_NS::WithinEpsilon(W, rhs.W));
		}

		if (op == CMP::AllNWE)
		{
			return !(HANDY_NS::WithinEpsilon(X, rhs.X)
				||   HANDY_NS::WithinEpsilon(Y, rhs.Y)
				||   HANDY_NS::WithinEpsilon(Z, rhs.Z)
				||   HANDY_NS::WithinEpsilon(W, rhs.W));
		}

		// Used for storing Vectors in collections. Compare NWE=>LT, X, then Y, then.... Default op for operator< on vector types.
		if (op == CMP::Ordinal)
		{
			if (!HANDY_NS::WithinEpsilon(X, rhs.X)) return X < rhs.X;
			if (!HANDY_NS::WithinEpsilon(Y, rhs.Y)) return Y < rhs.Y;
			if (!HANDY_NS::WithinEpsilon(Z, rhs.Z)) return Z < rhs.Z;
			if (!HANDY_NS::WithinEpsilon(W, rhs.W)) return W < rhs.W;
			return false;
		} 
	}

	FORCEINLINE Vector4 Vector4::Clamp(Vector4 const & min, Vector4 const & max) const { return Max(min).Min(max); }
	FORCEINLINE Vector4 Vector4::Clamp(  float const   min,   float const   max) const { return Max(min).Min(max); }

	/// ((*this) * m2) + a
	FORCEINLINE Vector4 Vector4::MultiplyAdd(Vector4 const & m2, Vector4 const & a) const
	{
		SIMD_IMP_FMA3   (return _mm_fmadd_ps(Native, m2, a);)
		SIMD_IMP_NO_FMA3(return ((*this) * m2) + a;)
		SCALAR_IMP      (return ((*this) * m2) + a;)
		NEON_IMP        (return vmlaq_f32(a, Native, m2);)
	}

	/// ((*this) * m2) - a
	FORCEINLINE Vector4 Vector4::MultiplySub   (Vector4 const & m2, Vector4 const &  a) const 
	{
		SIMD_IMP_FMA3   (return _mm_fmsub_ps(Native, m2, a);)
		SIMD_IMP_NO_FMA3(return ((*this) * m2) - a;)
		SCALAR_IMP      (return ((*this) * m2) - a;)
		NEON_IMP        (return ((*this) * m2) - a;) /// This MIGHT be faster, maybe: -Vector4(vmlsq_f32(a, (*this), m2));
	}

	FORCEINLINE Vector4 Vector4::Lerp(Vector4 const & rhs, float           t) const { return (*this) * t + rhs * (1.0f - t); }
	FORCEINLINE Vector4 Vector4::Lerp(Vector4 const & rhs, Vector4 const & t) const { return (*this) * t + rhs * (1.0f - t); }

	FORCEINLINE Vector4 Vector4::SmoothStep(Vector4 const & rhs, float           t) const { return Lerp(rhs, t * t * (3.0f - 2.0f * t)); }
	FORCEINLINE Vector4 Vector4::SmoothStep(Vector4 const & rhs, Vector4 const & t) const { return Lerp(rhs, t * t * (3.0f - 2.0f * t)); }

	FORCEINLINE Vector4 Vector4::Round_Near() const
	{
		SIMD_IMP_SSE41   (return _mm_round_ps(Native, (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC));)
		SIMD_IMP_NO_SSE41(return _mm_round_ps2(Native);)
		NEON_IMP         (return Vector4(std::roundf(X), std::roundf(Y), std::roundf(Z), std::roundf(W));)
		SCALAR_IMP       (return Vector4(std::roundf(X), std::roundf(Y), std::roundf(Z), std::roundf(W));)
	}

	FORCEINLINE Vector4 Vector4::Round_Floor() const
	{
		SIMD_IMP_SSE41   (return _mm_round_ps(Native, (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC));)
		SIMD_IMP_NO_SSE41(return _mm_floor_ps2(Native);)
		NEON_IMP         (return Vector4(std::floorf(X), std::floorf(Y), std::floorf(Z), std::floorf(W));)
		SCALAR_IMP       (return Vector4(std::floorf(X), std::floorf(Y), std::floorf(Z), std::floorf(W));)
	}

	FORCEINLINE Vector4 Vector4::Round_Ceil() const
	{
		SIMD_IMP_SSE41   (return _mm_round_ps(Native, (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC));)
		SIMD_IMP_NO_SSE41(return _mm_ceil_ps2(Native);)
		NEON_IMP         (return Vector4(std::ceilf (X), std::ceilf (Y), std::ceilf (Z), std::ceilf (W));)
		SCALAR_IMP       (return Vector4(std::ceilf (X), std::ceilf (Y), std::ceilf (Z), std::ceilf (W));)
	}
	
	FORCEINLINE Vector4 Vector4::Round_Trunc() const
	{
		SIMD_IMP_SSE41   (return _mm_round_ps(Native, (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC));)
		SIMD_IMP_NO_SSE41(return _mm_cvtepi32_ps(_mm_cvttps_epi32(Native));)
		NEON_IMP         (return Vector4(std::truncf(X), std::truncf(Y), std::truncf(Z), std::truncf(W));)
		SCALAR_IMP       (return Vector4(std::truncf(X), std::truncf(Y), std::truncf(Z), std::truncf(W));)
	}

	FORCEINLINE Vector4 Vector4::FMod(Vector4 const & div) const
	{
		SIMD_IMP  (return _mm_mod_ps2(Native, div);)
		NEON_IMP  (return Vector4(std::fmodf(X, div.X), std::fmodf(Y, div.Y), std::fmodf(Z, div.Z), std::fmodf(W, div.W));)
		SCALAR_IMP(return Vector4(std::fmodf(X, div.X), std::fmodf(Y, div.Y), std::fmodf(Z, div.Z), std::fmodf(W, div.W));)
	}
	
	FORCEINLINE Vector4 Vector4::FMod(float div) const
	{
		SIMD_IMP  (return _mm_mod_ps2(Native, Vector4(div));)
		NEON_IMP  (return Vector4(std::fmodf(X, div), std::fmodf(Y, div), std::fmodf(Z, div), std::fmodf(W, div));)
		SCALAR_IMP(return Vector4(std::fmodf(X, div), std::fmodf(Y, div), std::fmodf(Z, div), std::fmodf(W, div));)
	}

	FORCEINLINE float & Vector4::operator[](int idx)       { return Array[idx]; }
	FORCEINLINE float   Vector4::operator[](int idx) const { return Array[idx]; }

	FORCEINLINE Vector4 Vector4::Sin () { return Vector4(std::sin(X), std::sin(Y), std::sin(Z), std::sin(W)); }
	FORCEINLINE Vector4 Vector4::Cos () { return Vector4(std::cos(X), std::cos(Y), std::cos(Z), std::cos(W)); }
	FORCEINLINE Vector4 Vector4::Tan () { return Vector4(std::tan(X), std::tan(Y), std::tan(Z), std::tan(W)); }

	FORCEINLINE Vector4 Vector4::ASin() { return Vector4(std::asin(X), std::asin(Y), std::asin(Z), std::asin(W)); }
	FORCEINLINE Vector4 Vector4::ACos() { return Vector4(std::acos(X), std::acos(Y), std::acos(Z), std::acos(W)); }
	FORCEINLINE Vector4 Vector4::ATan() { return Vector4(std::atan(X), std::atan(Y), std::atan(Z), std::atan(W)); }

	FORCEINLINE Vector4 Vector4::SinH() { return Vector4(std::sinh(X), std::sinh(Y), std::sinh(Z), std::sinh(W)); }
	FORCEINLINE Vector4 Vector4::CosH() { return Vector4(std::cosh(X), std::cosh(Y), std::cosh(Z), std::cosh(W)); }
	FORCEINLINE Vector4 Vector4::TanH() { return Vector4(std::tanh(X), std::tanh(Y), std::tanh(Z), std::tanh(W)); }

	#if defined NEON_NON_INTEGRATED
		/// In theory these should be faster on NEON, and there is an equivalent for SIMD, but I like having public .X, Y, etc fields for direct access. 
		FORCEINLINE float Vector4::GetXFast() { return vgetq_lane_f32(Native, 0); }
		FORCEINLINE float Vector4::GetYFast() { return vgetq_lane_f32(Native, 1); }
		FORCEINLINE float Vector4::GetZFast() { return vgetq_lane_f32(Native, 2); }
		FORCEINLINE float Vector4::GetWFast() { return vgetq_lane_f32(Native, 3); }
	#endif   

	/*static*/ FORCEINLINE Vector4 Vector4::Zero()
	{
		SIMD_IMP  (return _mm_setzero_ps();)
		NEON_IMP  (return vdupq_n_f32(0.0f);)
		SCALAR_IMP(return Vector4(0.0f,  0.0f,  0.0f,  0.0f);)
	}
	/*static*/ FORCEINLINE Vector4 Vector4::One()      { return Vector4( 1.0f); }
	/*static*/ FORCEINLINE Vector4 Vector4::NegOne()   { return Vector4(-1.0f); }
	/*static*/ FORCEINLINE Vector4 Vector4::UnitX()
	{
		SIMD_IMP  (return _mm_set_ss(1.0f);)
		NEON_IMP  (return Vector4( 1.0f,  0.0f,  0.0f,  0.0f);)
		SCALAR_IMP(return Vector4( 1.0f,  0.0f,  0.0f,  0.0f);)
	}
	/*static*/ FORCEINLINE Vector4 Vector4::UnitY()    { return Vector4( 0.0f,  1.0f,  0.0f,  0.0f); }
	/*static*/ FORCEINLINE Vector4 Vector4::UnitZ()    { return Vector4( 0.0f,  0.0f,  1.0f,  0.0f); }
	/*static*/ FORCEINLINE Vector4 Vector4::UnitW()    { return Vector4( 0.0f,  0.0f,  0.0f,  1.0f); }
	/*static*/ FORCEINLINE Vector4 Vector4::UnitXW()   { return Vector4( 1.0f,  0.0f,  0.0f,  1.0f); }
	/*static*/ FORCEINLINE Vector4 Vector4::UnitYW()   { return Vector4( 0.0f,  1.0f,  0.0f,  1.0f); }
	/*static*/ FORCEINLINE Vector4 Vector4::UnitZW()   { return Vector4( 0.0f,  0.0f,  1.0f,  1.0f); }
	/*static*/ FORCEINLINE Vector4 Vector4::NegUnitX() { return Vector4(-1.0f,  0.0f,  0.0f,  0.0f); }
	/*static*/ FORCEINLINE Vector4 Vector4::NegUnitY() { return Vector4( 0.0f, -1.0f,  0.0f,  0.0f); }
	/*static*/ FORCEINLINE Vector4 Vector4::NegUnitZ() { return Vector4( 0.0f,  0.0f, -1.0f,  0.0f); }
	/*static*/ FORCEINLINE Vector4 Vector4::NegUnitW() { return Vector4( 0.0f,  0.0f,  0.0f, -1.0f); }

	/*static*/ FORCEINLINE Vector4 Vector4::PositiveInfinity() { return Vector4( std::numeric_limits<float>::infinity()); }
	/*static*/ FORCEINLINE Vector4 Vector4::NegativeInfinity() { return Vector4(-std::numeric_limits<float>::infinity()); }
	/*static*/ FORCEINLINE Vector4 Vector4::NaN()              { return Vector4( std::numeric_limits<float>::quiet_NaN()); }
	/*static*/ FORCEINLINE Vector4 Vector4::FFFFFFFF()
	{
		SIMD_IMP  (return _mm_cmpeq_ps(Zero(), Zero());)
		NEON_IMP  (constexpr uint32_t luf = 0xFFFFFFFF_u32; float res = *(float*)&(luf); return Vector4(res);)
		SCALAR_IMP(constexpr uint32_t luf = 0xFFFFFFFF_u32; float res = *(float*)&(luf); return Vector4(res);)
	}

	FORCEINLINE Vector4 operator-(Vector4 const & lhs)
	{
		SIMD_IMP  (return _mm_xor_ps(lhs, _mm_set1_ps(-0.0f));) /* { return _mm_setzero_ps() - lhs; } */
		NEON_IMP  (return vnegq_f32(lhs);)
		SCALAR_IMP(return 0.0f - lhs;)
	}

	FORCEINLINE Vector4 operator~(Vector4 const & lhs)
	{
		#if defined USE_SIMD
		return _mm_andnot_ps(lhs, Vector4::FFFFFFFF());
		#else
		uint32_t lufX = *(uint32_t*)&lhs.X;
		uint32_t lufY = *(uint32_t*)&lhs.Y;
		uint32_t lufZ = *(uint32_t*)&lhs.Z;
		uint32_t lufW = *(uint32_t*)&lhs.W;

		float resX = *(float*)&(lufX = ~lufX);
		float resY = *(float*)&(lufY = ~lufY);
		float resZ = *(float*)&(lufZ = ~lufZ);
		float resW = *(float*)&(lufW = ~lufW);

		return Vector4(resX, resY, resZ, resW);
		#endif
	}

	FORCEINLINE Vector4 operator+(Vector4 const & lhs, Vector4 const & rhs) 
	{
		SIMD_IMP  (return _mm_add_ps(lhs, rhs);)
		NEON_IMP  (return vaddq_f32(lhs, rhs);)
		SCALAR_IMP(return Vector4(lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z, lhs.W + rhs.W);)
	}

	FORCEINLINE Vector4 operator-(Vector4 const & lhs, Vector4 const & rhs)
	{
		SIMD_IMP  (return _mm_sub_ps(lhs, rhs);)
		NEON_IMP  (return vsubq_f32(lhs, rhs);)
		SCALAR_IMP(return Vector4(lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z, lhs.W - rhs.W);)
	}

	FORCEINLINE Vector4 operator*(Vector4 const & lhs, Vector4 const & rhs)
	{
		SIMD_IMP  (return _mm_mul_ps(lhs, rhs);)
		NEON_IMP  (return vmulq_f32(lhs, rhs);)
		SCALAR_IMP(return Vector4(lhs.X * rhs.X, lhs.Y * rhs.Y, lhs.Z * rhs.Z, lhs.W * rhs.W);)
	}

	FORCEINLINE Vector4 operator/(Vector4 const & lhs, Vector4 const & rhs)
	{
		SIMD_IMP(return _mm_div_ps(lhs, rhs);)
		#if defined USE_NEON
			#if defined(USE_V8)
				return vdivq_f32(lhs, rhs);
			#else
				return vmulq_f32(lhs, rhs.Reciprocal());
			#endif
		#endif	
		SCALAR_IMP(return Vector4(lhs.X / rhs.X, lhs.Y / rhs.Y, lhs.Z / rhs.Z, lhs.W / rhs.W);)
	}

	FORCEINLINE Vector4 operator&(Vector4 const & lhs, Vector4 const & rhs)
	{
		SIMD_IMP(return _mm_and_ps(lhs, rhs);)
		NEON_IMP(return vandq_u32(lhs, rhs);)
		#if defined USE_SCALAR
			uint32_t lufX = *(uint32_t*)&lhs.X;
			uint32_t lufY = *(uint32_t*)&lhs.Y;
			uint32_t lufZ = *(uint32_t*)&lhs.Z;
			uint32_t lufW = *(uint32_t*)&lhs.W;

			uint32_t rufX = *(uint32_t*)&rhs.X;
			uint32_t rufY = *(uint32_t*)&rhs.Y;
			uint32_t rufZ = *(uint32_t*)&rhs.Z;
			uint32_t rufW = *(uint32_t*)&rhs.W;

			float resX = *(float*)&(lufX &= rufX);
			float resY = *(float*)&(lufY &= rufY);
			float resZ = *(float*)&(lufZ &= rufZ);
			float resW = *(float*)&(lufW &= rufW);

			return Vector4(resX, resY, resZ, resW);
		#endif
	}

	FORCEINLINE Vector4 operator|(Vector4 const & lhs, Vector4 const & rhs)
	{
		SIMD_IMP(return _mm_or_ps (lhs, rhs);)
		NEON_IMP(return vorrq_u32 (lhs, rhs);)
		#if defined USE_SCALAR
			uint32_t lufX = *(uint32_t*)&lhs.X;
			uint32_t lufY = *(uint32_t*)&lhs.Y;
			uint32_t lufZ = *(uint32_t*)&lhs.Z;
			uint32_t lufW = *(uint32_t*)&lhs.W;

			uint32_t rufX = *(uint32_t*)&rhs.X;
			uint32_t rufY = *(uint32_t*)&rhs.Y;
			uint32_t rufZ = *(uint32_t*)&rhs.Z;
			uint32_t rufW = *(uint32_t*)&rhs.W;

			float resX = *(float*)&(lufX |= rufX);
			float resY = *(float*)&(lufY |= rufY);
			float resZ = *(float*)&(lufZ |= rufZ);
			float resW = *(float*)&(lufW |= rufW);

			return Vector4(resX, resY, resZ, resW);
		#endif
	}

	FORCEINLINE Vector4 operator^(Vector4 const & lhs, Vector4 const & rhs)
	{
		SIMD_IMP(return _mm_xor_ps(lhs, rhs);)
		NEON_IMP(return veorq_u32(lhs, rhs);)
		#if defined USE_SCALAR
			uint32_t lufX = *(uint32_t*)&lhs.X;
			uint32_t lufY = *(uint32_t*)&lhs.Y;
			uint32_t lufZ = *(uint32_t*)&lhs.Z;
			uint32_t lufW = *(uint32_t*)&lhs.W;

			uint32_t rufX = *(uint32_t*)&rhs.X;
			uint32_t rufY = *(uint32_t*)&rhs.Y;
			uint32_t rufZ = *(uint32_t*)&rhs.Z;
			uint32_t rufW = *(uint32_t*)&rhs.W;

			float resX = *(float*)&(lufX ^= rufX);
			float resY = *(float*)&(lufY ^= rufY);
			float resZ = *(float*)&(lufZ ^= rufZ);
			float resW = *(float*)&(lufW ^= rufW);

			return Vector4(resX, resY, resZ, resW);
		#endif
	}

	FORCEINLINE Vector4 operator+(Vector4 const & lhs, float rhs) { return lhs + Vector4(rhs); }
	FORCEINLINE Vector4 operator-(Vector4 const & lhs, float rhs) { return lhs - Vector4(rhs); }
	FORCEINLINE Vector4 operator*(Vector4 const & lhs, float rhs) { return lhs * Vector4(rhs); }
	FORCEINLINE Vector4 operator/(Vector4 const & lhs, float rhs) { return lhs / Vector4(rhs); }
	FORCEINLINE Vector4 operator&(Vector4 const & lhs, float rhs) { return lhs & Vector4(rhs); }
	FORCEINLINE Vector4 operator|(Vector4 const & lhs, float rhs) { return lhs | Vector4(rhs); }
	FORCEINLINE Vector4 operator^(Vector4 const & lhs, float rhs) { return lhs ^ Vector4(rhs); }

	FORCEINLINE Vector4 operator+(float lhs, Vector4 const & rhs) { return Vector4(lhs) + rhs; }
	FORCEINLINE Vector4 operator-(float lhs, Vector4 const & rhs) { return Vector4(lhs) - rhs; }
	FORCEINLINE Vector4 operator*(float lhs, Vector4 const & rhs) { return Vector4(lhs) * rhs; }
	FORCEINLINE Vector4 operator/(float lhs, Vector4 const & rhs) { return Vector4(lhs) / rhs; }
	FORCEINLINE Vector4 operator&(float lhs, Vector4 const & rhs) { return Vector4(lhs) & rhs; }
	FORCEINLINE Vector4 operator|(float lhs, Vector4 const & rhs) { return Vector4(lhs) | rhs; }
	FORCEINLINE Vector4 operator^(float lhs, Vector4 const & rhs) { return Vector4(lhs) ^ rhs; }

	FORCEINLINE Vector4 & operator+=(Vector4 & lhs, Vector4 const & rhs) { lhs = lhs + rhs; return lhs; }
	FORCEINLINE Vector4 & operator-=(Vector4 & lhs, Vector4 const & rhs) { lhs = lhs - rhs; return lhs; }
	FORCEINLINE Vector4 & operator*=(Vector4 & lhs, Vector4 const & rhs) { lhs = lhs * rhs; return lhs; }
	FORCEINLINE Vector4 & operator/=(Vector4 & lhs, Vector4 const & rhs) { lhs = lhs / rhs; return lhs; }
	FORCEINLINE Vector4 & operator&=(Vector4 & lhs, Vector4 const & rhs) { lhs = lhs & rhs; return lhs; }
	FORCEINLINE Vector4 & operator|=(Vector4 & lhs, Vector4 const & rhs) { lhs = lhs | rhs; return lhs; }
	FORCEINLINE Vector4 & operator^=(Vector4 & lhs, Vector4 const & rhs) { lhs = lhs / rhs; return lhs; }

	FORCEINLINE Vector4 & operator+=(Vector4 & lhs, float rhs) { lhs = lhs + rhs; return lhs; }
	FORCEINLINE Vector4 & operator-=(Vector4 & lhs, float rhs) { lhs = lhs - rhs; return lhs; }
	FORCEINLINE Vector4 & operator*=(Vector4 & lhs, float rhs) { lhs = lhs * rhs; return lhs; }
	FORCEINLINE Vector4 & operator/=(Vector4 & lhs, float rhs) { lhs = lhs / rhs; return lhs; }
	FORCEINLINE Vector4 & operator&=(Vector4 & lhs, float rhs) { lhs = lhs & rhs; return lhs; }
	FORCEINLINE Vector4 & operator|=(Vector4 & lhs, float rhs) { lhs = lhs | rhs; return lhs; }
	FORCEINLINE Vector4 & operator^=(Vector4 & lhs, float rhs) { lhs = lhs / rhs; return lhs; }

	FORCEINLINE bool operator< (Vector4 const & lhs, Vector4 const & rhs) { return lhs.Compare<CMP::Ordinal>(rhs); }

	FORCEINLINE bool operator==(Vector4 const & lhs, Vector4 const & rhs) { return lhs.Compare<CMP::AllWE>(rhs); }
	FORCEINLINE bool operator!=(Vector4 const & lhs, Vector4 const & rhs) { return lhs.Compare<CMP::AnyNWE>(rhs); }

	FORCEINLINE bool operator==(Vector4 const & lhs, float rhs) { return lhs == Vector4(rhs); }
	FORCEINLINE bool operator!=(Vector4 const & lhs, float rhs) { return lhs != Vector4(rhs); }

	FORCEINLINE bool operator==(float lhs, Vector4 const & rhs) { return Vector4(lhs) == rhs; }
	FORCEINLINE bool operator!=(float lhs, Vector4 const & rhs) { return Vector4(lhs) != rhs; }
}

namespace std {
	FORCEINLINE ::HANDYMATH_NS::Vector4 min(::HANDYMATH_NS::Vector4 const & a, ::HANDYMATH_NS::Vector4 const & b) { return a.Min(b); }
	FORCEINLINE ::HANDYMATH_NS::Vector4 max(::HANDYMATH_NS::Vector4 const & a, ::HANDYMATH_NS::Vector4 const & b) { return a.Max(b); }
}

MAKE_HASHABLE(HANDYMATH_NS::Vector4, t.X, t.Y, t.Z, t.W)
