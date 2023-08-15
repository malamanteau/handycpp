
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

	//class Vector4i64;
	//class Vector3i64;
	//class Vector2i64;

	class Vector4;
	class Vector3d;
	class Vector2d;

	class HANDYMATH_ALIGN32 Vector4d 
	{
		//friend class Vector4i64;
		friend class Vector3d;
		friend class Vector2d;

	public:
		static constexpr int32_t NumElements  =  4_i32;
		static constexpr int32_t ElementBytes =  8_i32;
		static constexpr int32_t StrideBytes  = 32_i32;
		static constexpr int32_t UsefulBytes  = NumElements * ElementBytes;

		union
		{
			ARCHDT4               Native;
			struct { ARCHDT2      NativeXY, NativeZW; };
			struct { double       X, Y, Z, W; };
			struct { double       x, y, z, w; };
			std::array<double, 4> Array;
		};

		// implicit
		Vector4d(ARCHDT4 const & native);
		operator ARCHDT4() const;

		Vector4d();
		Vector4d(Vector4d const & v);

		explicit 
		Vector4d(double xyzw);
		Vector4d(double xIn, double yIn, double zIn, double wIn);

		explicit 
		Vector4d(double const * ary);

		Vector4d & operator=(Vector4d const &) = default;

		// Conversions:
		Vector2d XY () const;
		Vector2d ZW () const;
		Vector3d XYZ() const;
		Vector4  ToFloat() const;
		//Vector4i64 ToInteger() const;
		// ---

		static Vector4d FromArray  (double const * ary);
		void            CopyToArray(double       * ary) const;

		//Vector4i64 SI256() const;

		template<int i0, int i1, int i2, int i3> Vector4d Shuffle() const;
		template<int i0, int i1, int i2, int i3> Vector4d ShuffleAABB(Vector4d const & b) const;

		template<int i> Vector4d Insert(double f) const;
		template<int i> Vector4d InsertZero()    const;

		Vector4d InsertX(double xIn) const;
		Vector4d InsertY(double yIn) const;
		Vector4d InsertZ(double zIn) const;
		Vector4d InsertW(double wIn) const;
		
		Vector4d X000() const;
		Vector4d XY00() const;
		Vector4d XY01() const;
		Vector4d XY10() const;
		Vector4d XY11() const;
		Vector4d XYZ0() const;
		Vector4d XYZ1() const;

		Vector4d XXZZ() const;
		Vector4d YYWW() const;

		Vector4d XXYY() const;
		Vector4d XYXY() const;

		Vector4d ZZWW() const;
		Vector4d ZWZW() const;

		Vector4d YBWD(Vector4d const & abcd) const;
		Vector4d XAZC(Vector4d const & abcd) const;

		Vector4d ZCWD(Vector4d const & abcd) const;
		Vector4d XYAB(Vector4d const & abcd) const;
		Vector4d ZWCD(Vector4d const & abcd) const;
		Vector4d XAYB(Vector4d const & abcd) const;
		// ---

		Vector4d XXXX() const;
		Vector4d YYYY() const;
		Vector4d ZZZZ() const;
		Vector4d WWWW() const;

		Vector4d WZYX() const;
		Vector4d ZWXY() const;

		Vector4d Min(Vector4d const & rhs) const;
		Vector4d Max(Vector4d const & rhs) const;

		Vector4d Min(double rhs) const;
		Vector4d Max(double rhs) const;

		double   Min()     const;
		double   Max()     const;
		double   Sum()     const;
		double   Product() const;
		double   Mean()    const;

		Vector4d Abs()              const;
		Vector4d Sign()             const; //Returns, per component, the sign of the input vector: input >= 0.0 ? 1.0 : -1.0
		Vector4d Half()             const;
		Vector4d Reciprocal()       const;
		Vector4d ReciprocalApprox() const;
		Vector4d Sqrt()             const;
		Vector4d RSqrt()            const;
		Vector4d RSqrtApprox()      const;

		Vector4d To(Vector4d const & rhs) const;

		double   Distance       (Vector4d const & rhs) const;
		double   DistanceSquared(Vector4d const & rhs) const;

		double   Length()        const;
		double   LengthSquared() const;

		Vector4d Normalized() const;

		double   Angle(Vector4d const & rhs) const;
		double   Dot  (Vector4d const & rhs) const;
		//Vector4 Cross(Vector4 const & rhs) const;

		Vector4d Power      (double n) const;
		Vector4d PowerApprox(double n) const;

		bool    IsFacing(Vector4d const & rhs) const;

		bool    HasNaN()              const;
		bool    HasPositiveInfinity() const;
		bool    HasNegativeInfinity() const;

		bool    IsNaN()               const;
		bool    IsPositiveInfinity()  const;
		bool    IsNegativeInfinity()  const;

		bool    IsUnbounded()         const;

		template <CMP op> bool Compare(double           rhs) const;
		template <CMP op> bool Compare(Vector4d const & rhs) const;

		Vector4d Clamp(Vector4d const & min, Vector4d const & max) const;
		Vector4d Clamp(double   const   min, double   const   max) const;

		Vector4d MultiplyAdd   (Vector4d const & m2, Vector4d const &  a) const; // ((*this) * rhs) + a
		Vector4d MultiplySub   (Vector4d const & m2, Vector4d const &  a) const; // ((*this) * rhs) - a

		Vector4d Lerp      (Vector4d const & rhs, double           t) const;
		Vector4d Lerp      (Vector4d const & rhs, Vector4d const & t) const;
		Vector4d SmoothStep(Vector4d const & rhs, double           t) const;
		Vector4d SmoothStep(Vector4d const & rhs, Vector4d const & t) const;

		Vector4d Round_Near () const;
		Vector4d Round_Floor() const;
		Vector4d Round_Ceil () const;
		Vector4d Round_Trunc() const;

		Vector4d FMod(Vector4d const & div) const;
		Vector4d FMod(double div) const;

		double & operator[](int idx);
		double   operator[](int idx) const;

		Vector4d Sin ();
		Vector4d Cos ();
		Vector4d Tan ();

		Vector4d ASin();
		Vector4d ACos();
		Vector4d ATan();

		Vector4d SinH();
		Vector4d CosH();
		Vector4d TanH();

		static Vector4d Zero();
		static Vector4d One();
		static Vector4d NegOne();
		static Vector4d UnitX();
		static Vector4d UnitY();
		static Vector4d UnitZ();
		static Vector4d UnitW();
		static Vector4d UnitXW();
		static Vector4d UnitYW();
		static Vector4d UnitZW();
		static Vector4d NegUnitX();
		static Vector4d NegUnitY();
		static Vector4d NegUnitZ();
		static Vector4d NegUnitW();
		static Vector4d PositiveInfinity();
		static Vector4d NegativeInfinity();
		static Vector4d NaN();
		static Vector4d FFFFFFFFFFFFFFFF();

		template <class SerialOp> void serial(SerialOp & ser)
		{
			ser
				(X)
				(Y)
				(Z)
				(W);
		}
	};

	Vector4d operator-(Vector4d const & lhs);
	Vector4d operator~(Vector4d const & lhs);

	Vector4d operator+(Vector4d const & lhs, Vector4d const & rhs);
	Vector4d operator-(Vector4d const & lhs, Vector4d const & rhs);
	Vector4d operator*(Vector4d const & lhs, Vector4d const & rhs);
	Vector4d operator/(Vector4d const & lhs, Vector4d const & rhs);
	Vector4d operator&(Vector4d const & lhs, Vector4d const & rhs);
	Vector4d operator|(Vector4d const & lhs, Vector4d const & rhs);
	Vector4d operator^(Vector4d const & lhs, Vector4d const & rhs);

	Vector4d operator+(Vector4d const & lhs, double rhs);
	Vector4d operator-(Vector4d const & lhs, double rhs);
	Vector4d operator*(Vector4d const & lhs, double rhs);
	Vector4d operator/(Vector4d const & lhs, double rhs);
	Vector4d operator&(Vector4d const & lhs, double rhs);
	Vector4d operator|(Vector4d const & lhs, double rhs);
	Vector4d operator^(Vector4d const & lhs, double rhs);

	Vector4d operator+(double lhs, Vector4d const & rhs);
	Vector4d operator-(double lhs, Vector4d const & rhs);
	Vector4d operator*(double lhs, Vector4d const & rhs);
	Vector4d operator/(double lhs, Vector4d const & rhs);
	Vector4d operator&(double lhs, Vector4d const & rhs);
	Vector4d operator|(double lhs, Vector4d const & rhs);
	Vector4d operator^(double lhs, Vector4d const & rhs);

	Vector4d & operator+=(Vector4d & lhs, Vector4d const & rhs);
	Vector4d & operator-=(Vector4d & lhs, Vector4d const & rhs);
	Vector4d & operator*=(Vector4d & lhs, Vector4d const & rhs);
	Vector4d & operator/=(Vector4d & lhs, Vector4d const & rhs);
	Vector4d & operator&=(Vector4d & lhs, Vector4d const & rhs);
	Vector4d & operator|=(Vector4d & lhs, Vector4d const & rhs);
	Vector4d & operator^=(Vector4d & lhs, Vector4d const & rhs);

	Vector4d & operator+=(Vector4d & lhs, double rhs);
	Vector4d & operator-=(Vector4d & lhs, double rhs);
	Vector4d & operator*=(Vector4d & lhs, double rhs);
	Vector4d & operator/=(Vector4d & lhs, double rhs);
	Vector4d & operator&=(Vector4d & lhs, double rhs);
	Vector4d & operator|=(Vector4d & lhs, double rhs);
	Vector4d & operator^=(Vector4d & lhs, double rhs);

	bool operator< (Vector4d const & lhs, Vector4d const & rhs);

	bool operator==(Vector4d const & lhs, Vector4d const & rhs);
	bool operator!=(Vector4d const & lhs, Vector4d const & rhs);

	bool operator==(Vector4d const & lhs, double rhs);
	bool operator!=(Vector4d const & lhs, double rhs);

	bool operator==(double lhs, Vector4d const & rhs);
	bool operator!=(double lhs, Vector4d const & rhs);
}

namespace std {
	::HANDYMATH_NS::Vector4d min(::HANDYMATH_NS::Vector4d const & a, ::HANDYMATH_NS::Vector4d const & b);
	::HANDYMATH_NS::Vector4d max(::HANDYMATH_NS::Vector4d const & a, ::HANDYMATH_NS::Vector4d const & b);
}

static inline std::ostream & operator<<(std::ostream & os, const ::HANDYMATH_NS::Vector4d & v)  
{  
	os << "(" << v.X << ", " << v.Y << ", " << v.Z << ", " << v.W << ")";  
	return os;  
}  

namespace HANDYMATH_NS {

	FORCEINLINE Vector4d::Vector4d() : Native(Vector4d::Zero()) { }

	FORCEINLINE Vector4d::Vector4d(ARCHDT4 const & native)                 : Native(native)             { }
	FORCEINLINE Vector4d::Vector4d(Vector4d const & v)                     : Native(v.Native)           { }

	#if defined USE_SIMD && defined USE_AVX
	FORCEINLINE Vector4d::Vector4d(double xyzw)                            : Native(_mm256_set1_pd(xyzw)) { }
	FORCEINLINE Vector4d::Vector4d(double xIn, double yIn, double zIn, double wIn) : Native(_mm256_setr_pd(xIn, yIn, zIn, wIn)) { }
	FORCEINLINE Vector4d::Vector4d(double const * ary)                     : Native(_mm256_loadu_pd(ary)) { }
	#else
	FORCEINLINE Vector4d::Vector4d(double xyzw)                            : Native { xyzw, xyzw, xyzw, xyzw } { }
	FORCEINLINE Vector4d::Vector4d(double xIn, double yIn, double zIn, double wIn) : Native { xIn, yIn, zIn, wIn } { }
	FORCEINLINE Vector4d::Vector4d(double const * ary)                     : Native { ary[0], ary[1], ary[2], ary[3] } { }
	#endif

	FORCEINLINE Vector4d::operator ARCHDT4() const { return Native; }

	FORCEINLINE Vector4d Vector4d::FromArray(double const * ary) { return Vector4d(ary); }

	FORCEINLINE void Vector4d::CopyToArray(double * ary) const
	{
		SIMD_IMP_AVX   (_mm256_storeu_pd(ary, Native);)
		SIMD_IMP_NO_AVX(memcpy(ary, &Array[0], sizeof(double) * 4);)
		NON_SIMD_IMP   (memcpy(ary, &Array[0], sizeof(double) * 4);)
	}

	template<int i0, int i1, int i2, int i3> FORCEINLINE Vector4d Vector4d::Shuffle() const
	{
		return Vector4d(Array[i0], Array[i1], Array[i2], Array[i3]);
	}
	
	template<int i0, int i1, int i2, int i3> FORCEINLINE Vector4d Vector4d::ShuffleAABB(Vector4d const & b) const
	{
		return Vector4d(Array[i0], Array[i1], b.Array[i2], b.Array[i3]);
	}

	template<int i> FORCEINLINE Vector4d Vector4d::Insert(double f) const
	{
		auto n = *this;
		n.Array[i] = f;
		return n;
	}

	template<int i> FORCEINLINE Vector4d Vector4d::InsertZero() const
	{
		auto n = *this;
		n.Array[i] = 0.0f;
		return n;
	}

	FORCEINLINE Vector4d Vector4d::InsertX(double xIn) const { return Insert<0>(xIn); }
	FORCEINLINE Vector4d Vector4d::InsertY(double yIn) const { return Insert<1>(yIn); }
	FORCEINLINE Vector4d Vector4d::InsertZ(double zIn) const { return Insert<2>(zIn); }
	FORCEINLINE Vector4d Vector4d::InsertW(double wIn) const { return Insert<3>(wIn); }

	FORCEINLINE Vector4d Vector4d::X000() const { return Vector4d(X, 0.0_d, 0.0_d, 0.0_d); }
	FORCEINLINE Vector4d Vector4d::XY00() const { return Vector4d(X,    Y, 0.0_d, 0.0_d); }
	FORCEINLINE Vector4d Vector4d::XY01() const { return Vector4d(X,    Y, 0.0_d, 1.0_d); }
	FORCEINLINE Vector4d Vector4d::XY10() const { return Vector4d(X,    Y, 1.0_d, 0.0_d); }
	FORCEINLINE Vector4d Vector4d::XY11() const { return Vector4d(X,    Y, 1.0_d, 1.0_d); }

	FORCEINLINE Vector4d Vector4d::XYZ0() const 
	{
		SIMD_IMP    (return InsertZero<3>();)
		NON_SIMD_IMP(return Vector4d(X,    Y,    Z, 0.0f);)
	}

	FORCEINLINE Vector4d Vector4d::XYZ1() const 
	{
		SIMD_IMP    (return Insert<3>(1.0f);)
		NON_SIMD_IMP(return Vector4d(X,    Y,    Z, 1.0f);)
	}

	FORCEINLINE Vector4d Vector4d::XXZZ() const 
	{
		SIMD_IMP_AVX   (return _mm256_movedup_pd(Native);)
		SIMD_IMP_NO_AVX(return Vector4d(X, X, Z, Z);)
		NON_SIMD_IMP   (return Vector4d(X, X, Z, Z);)
	}

	FORCEINLINE Vector4d Vector4d::YYWW() const 
	{
		//SIMD_IMP_AVX   (return _mm256_unpackhi_pd(Native, Native);) // This also works but is likely slower.
		SIMD_IMP_AVX   (return _mm256_permute_pd(Native, 15);)
		SIMD_IMP_NO_AVX(return Vector4d(Y, Y, W, W);)
		NON_SIMD_IMP   (return Vector4d(Y, Y, W, W);)
	}

	FORCEINLINE Vector4d Vector4d::XXYY() const { return Vector4d(X, X, Y, Y); }
	FORCEINLINE Vector4d Vector4d::XYXY() const { return Vector4d(X, Y, X, Y); }
	FORCEINLINE Vector4d Vector4d::ZZWW() const { return Vector4d(Z, Z, W, W); }
	FORCEINLINE Vector4d Vector4d::ZWZW() const { return Vector4d(Z, W, Z, W); }

	/// _mm256_unpacklo_pd(Native, Native) = XXZZ


	FORCEINLINE Vector4d Vector4d::YBWD(Vector4d const & abcd) const 
	{
		SIMD_IMP_AVX   (return _mm256_unpackhi_pd(Native, abcd);)
		SIMD_IMP_NO_AVX(return Vector4d(Y, abcd.Y, W, abcd.W);)
		NON_SIMD_IMP   (return Vector4d(Y, abcd.Y, W, abcd.W);)
	}
	
	FORCEINLINE Vector4d Vector4d::XAZC(Vector4d const & abcd) const 
	{
		SIMD_IMP_AVX   (return _mm256_unpacklo_pd(Native, abcd);)
		SIMD_IMP_NO_AVX(return Vector4d(X, abcd.X, Z, abcd.Z);)
		NON_SIMD_IMP   (return Vector4d(X, abcd.X, Z, abcd.Z);)
	}

	FORCEINLINE Vector4d Vector4d::ZCWD(Vector4d const & abcd) const { return Vector4d(Z, abcd.Z,      W, abcd.W); }
	FORCEINLINE Vector4d Vector4d::XYAB(Vector4d const & abcd) const { return Vector4d(X,      Y, abcd.X, abcd.Y); }
	FORCEINLINE Vector4d Vector4d::ZWCD(Vector4d const & abcd) const { return Vector4d(Z,      W, abcd.Z, abcd.W); }
	FORCEINLINE Vector4d Vector4d::XAYB(Vector4d const & abcd) const { return Vector4d(X, abcd.X,      Y, abcd.Y); }

	FORCEINLINE Vector4d Vector4d::XXXX() const { return Shuffle<0, 0, 0, 0>(); }
	FORCEINLINE Vector4d Vector4d::YYYY() const { return Shuffle<1, 1, 1, 1>(); }
	FORCEINLINE Vector4d Vector4d::ZZZZ() const { return Shuffle<2, 2, 2, 2>(); }
	FORCEINLINE Vector4d Vector4d::WWWW() const { return Shuffle<3, 3, 3, 3>(); }

	FORCEINLINE Vector4d Vector4d::WZYX() const { return Shuffle<3, 2, 1, 0>(); }
	FORCEINLINE Vector4d Vector4d::ZWXY() const { return Shuffle<2, 3, 0, 1>(); }

	FORCEINLINE Vector4d Vector4d::Min(Vector4d const & rhs) const
	{
		SIMD_IMP_AVX   (return _mm256_min_pd(Native, rhs);)
		SIMD_IMP_NO_AVX(return Vector4d(Handy::FastMin(X, rhs.X), Handy::FastMin(Y, rhs.Y), Handy::FastMin(Z, rhs.Z), Handy::FastMin(W, rhs.W));)
		NON_SIMD_IMP   (return Vector4d(Handy::FastMin(X, rhs.X), Handy::FastMin(Y, rhs.Y), Handy::FastMin(Z, rhs.Z), Handy::FastMin(W, rhs.W));)
	}

	FORCEINLINE Vector4d Vector4d::Max(Vector4d const & rhs) const
	{
		SIMD_IMP_AVX   (return _mm256_max_pd(Native, rhs);)
		SIMD_IMP_NO_AVX(return Vector4d(Handy::FastMax(X, rhs.X), Handy::FastMax(Y, rhs.Y), Handy::FastMax(Z, rhs.Z), Handy::FastMax(W, rhs.W));)
		NON_SIMD_IMP   (return Vector4d(Handy::FastMax(X, rhs.X), Handy::FastMax(Y, rhs.Y), Handy::FastMax(Z, rhs.Z), Handy::FastMax(W, rhs.W));)
	}

	FORCEINLINE Vector4d Vector4d::Min(double rhs) const
	{
		SIMD_IMP_AVX   (return _mm256_min_pd(Native, Vector4d(rhs));)
		SIMD_IMP_NO_AVX(return Vector4d(Handy::FastMin(X, rhs), Handy::FastMin(Y, rhs), Handy::FastMin(Z, rhs), Handy::FastMin(W, rhs));)
		NON_SIMD_IMP   (return Vector4d(Handy::FastMin(X, rhs), Handy::FastMin(Y, rhs), Handy::FastMin(Z, rhs), Handy::FastMin(W, rhs));)
	}

	FORCEINLINE Vector4d Vector4d::Max(double rhs) const
	{
		SIMD_IMP_AVX   (return _mm256_max_pd(Native, Vector4d(rhs));)
		SIMD_IMP_NO_AVX(return Vector4d(Handy::FastMax(X, rhs), Handy::FastMax(Y, rhs), std::max(Z, rhs), std::max(W, rhs));)
		NON_SIMD_IMP   (return Vector4d(std::max(X, rhs), std::max(Y, rhs), std::max(Z, rhs), std::max(W, rhs));)
	}

	FORCEINLINE double Vector4d::Min()     const 
	{
		SIMD_IMP_AVX   
		(
			__m128d vlow  = _mm256_castpd256_pd128(Native);
			__m128d vhigh = _mm256_extractf128_pd(Native, 1); // high 128
			vlow  = _mm_min_pd(vlow, vhigh);     // reduce down to 128

			__m128d high64 = _mm_unpackhi_pd(vlow, vlow);
			return  _mm_cvtsd_f64(_mm_min_sd(vlow, high64));  // reduce to scalar
		)
		SIMD_IMP_NO_AVX(return Handy::FastMin(Handy::FastMin(X, Y), Handy::FastMin(Z, W)); )
		NON_SIMD_IMP   (return Handy::FastMin(Handy::FastMin(X, Y), Handy::FastMin(Z, W)); )
	}

	FORCEINLINE double Vector4d::Max()     const 
	{
		SIMD_IMP_AVX   
		(
			__m128d vlow  = _mm256_castpd256_pd128(Native);
			__m128d vhigh = _mm256_extractf128_pd(Native, 1); // high 128
			vlow  = _mm_max_pd(vlow, vhigh);     // reduce down to 128

			__m128d high64 = _mm_unpackhi_pd(vlow, vlow);
			return  _mm_cvtsd_f64(_mm_max_sd(vlow, high64));  // reduce to scalar
		)
		SIMD_IMP_NO_AVX(return Handy::FastMax(Handy::FastMax(X, Y), Handy::FastMax(Z, W)); )
		NON_SIMD_IMP   (return Handy::FastMax(Handy::FastMax(X, Y), Handy::FastMax(Z, W)); )
	}
	
	FORCEINLINE double Vector4d::Sum()     const 
	{
		SIMD_IMP_AVX   
		(
			__m128d vlow  = _mm256_castpd256_pd128(Native);
			__m128d vhigh = _mm256_extractf128_pd(Native, 1); // high 128
			vlow  = _mm_add_pd(vlow, vhigh);     // reduce down to 128

			__m128d high64 = _mm_unpackhi_pd(vlow, vlow);
			return  _mm_cvtsd_f64(_mm_add_sd(vlow, high64));  // reduce to scalar
		)
		SIMD_IMP_NO_AVX(return X + Y + Z + W;)
		NON_SIMD_IMP   (return X + Y + Z + W;)
	}
		
	FORCEINLINE double Vector4d::Product() const 
	{
		SIMD_IMP_AVX   
		(
			__m128d vlow  = _mm256_castpd256_pd128(Native);
			__m128d vhigh = _mm256_extractf128_pd(Native, 1); // high 128
			vlow  = _mm_mul_pd(vlow, vhigh);     // reduce down to 128

			__m128d high64 = _mm_unpackhi_pd(vlow, vlow);
			return  _mm_cvtsd_f64(_mm_mul_sd(vlow, high64));  // reduce to scalar
		)
		SIMD_IMP_NO_AVX(return X * Y * Z * W;)
		NON_SIMD_IMP   (return X * Y * Z * W;)
	}


	FORCEINLINE double Vector4d::Mean()    const { return Sum()   * 0.25_d; }

	FORCEINLINE Vector4d Vector4d::Abs() const 
	{
		SIMD_IMP_AVX   (return _mm256_and_pd(Native, _mm256_castsi256_pd(_mm256_set1_epi64x(0x7FFFFFFFFFFFFFFFLL)));)
		SIMD_IMP_NO_AVX(return Vector4d(std::abs(X), std::abs(Y), std::abs(Z), std::abs(W));)
		NON_SIMD_IMP   (return Vector4d(std::abs(X), std::abs(Y), std::abs(Z), std::abs(W));)
	}

	FORCEINLINE Vector4d Vector4d::Sign() const
	{
		SIMD_IMP_AVX   (Vector4d mask = _mm256_cmp_pd(Native, Zero(), _CMP_GE_OQ); return _mm256_andnot_pd(mask, NegOne()) | (One() & mask);)
		SIMD_IMP_NO_AVX(return Vector4d(X >= 0.0 ? 1.0 : -1.0, Y >= 0.0 ? 1.0 : -1.0, Z >= 0.0 ? 1.0 : -1.0, W >= 0.0 ? 1.0 : -1.0);)
		NON_SIMD_IMP   (return Vector4d(X >= 0.0 ? 1.0 : -1.0, Y >= 0.0 ? 1.0 : -1.0, Z >= 0.0 ? 1.0 : -1.0, W >= 0.0 ? 1.0 : -1.0);)
	}

	FORCEINLINE Vector4d Vector4d::Half()             const { return (*this) * 0.5_d; }
	FORCEINLINE Vector4d Vector4d::Reciprocal()       const { return 1.0_d / (*this); }
	FORCEINLINE Vector4d Vector4d::ReciprocalApprox() const { return 1.0_d / (*this); }
	FORCEINLINE Vector4d Vector4d::Sqrt()             const 
	{
		SIMD_IMP_AVX   (return _mm256_sqrt_pd(Native);)
		SIMD_IMP_NO_AVX(return Vector4d(std::sqrt(X), std::sqrt(Y), std::sqrt(Z), std::sqrt(W));)
		NON_SIMD_IMP   (return Vector4d(std::sqrt(X), std::sqrt(Y), std::sqrt(Z), std::sqrt(W));)
	}

	FORCEINLINE Vector4d Vector4d::RSqrt()            const { return this->Sqrt().Reciprocal(); }
	FORCEINLINE Vector4d Vector4d::RSqrtApprox()      const { return this->Sqrt().Reciprocal(); }

	FORCEINLINE Vector4d Vector4d::To(Vector4d const & rhs) const { return rhs - (*this); }

	FORCEINLINE double   Vector4d::Distance       (Vector4d const & rhs) const { return (rhs - (*this)).Length(); }
	FORCEINLINE double   Vector4d::DistanceSquared(Vector4d const & rhs) const { return (rhs - (*this)).LengthSquared(); }

	FORCEINLINE double   Vector4d::Length()        const { return std::sqrt(Dot(*this)); }
	FORCEINLINE double   Vector4d::LengthSquared() const { return Dot(*this); }

	FORCEINLINE Vector4d Vector4d::Normalized()    const { return (*this) * (1.0_d / Length()); }

	FORCEINLINE double   Vector4d::Angle(Vector4d const & rhs) const
	{
		double vdot = Normalized().Dot(rhs.Normalized());

		if (vdot < -1.0_d)
			vdot = -1.0_d;

		if (vdot > 1.0_d)
			vdot = 1.0_d;

		return std::acos(vdot);
	}

	FORCEINLINE double   Vector4d::Dot  (Vector4d const & rhs) const { return ((*this) * rhs).Sum(); }

	FORCEINLINE Vector4d Vector4d::Power(double n) const { return Vector4d(std::pow(X, n), std::pow(Y, n), std::pow(Z, n), std::pow(W, n)); }

	FORCEINLINE Vector4d Vector4d::PowerApprox(double n) const { return Vector4d(std::pow(X, n), std::pow(Y, n), std::pow(Z, n), std::pow(W, n)); }

	FORCEINLINE bool Vector4d::IsFacing(Vector4d const & rhs) const { return Dot(rhs) > 0.0_d; }

	FORCEINLINE bool Vector4d::HasNaN()              const { return std::isnan(X) || std::isnan(Y) || std::isnan(Z) || std::isnan(W); }
	FORCEINLINE bool Vector4d::HasPositiveInfinity() const { return Compare<CMP::AnyEQ>( std::numeric_limits<double>::infinity()); } 
	FORCEINLINE bool Vector4d::HasNegativeInfinity() const { return Compare<CMP::AnyEQ>(-std::numeric_limits<double>::infinity()); }

	FORCEINLINE bool Vector4d::IsNaN()              const { return std::isnan(X) && std::isnan(Y) && std::isnan(Z) && std::isnan(W); }
	FORCEINLINE bool Vector4d::IsPositiveInfinity() const { return Compare<CMP::AllEQ>( std::numeric_limits<double>::infinity()); }
	FORCEINLINE bool Vector4d::IsNegativeInfinity() const { return Compare<CMP::AllEQ>(-std::numeric_limits<double>::infinity()); }

	FORCEINLINE bool Vector4d::IsUnbounded() const { return HasPositiveInfinity() || HasNegativeInfinity(); }

	template <CMP op>
	FORCEINLINE bool Vector4d::Compare(double rhs) const
	{
		return Compare<op>(Vector4d(rhs));
	}

	template <CMP op>
	FORCEINLINE bool Vector4d::Compare(Vector4d const & rhs) const
	{
		/// IMPORTANT: If any of these are upgraded, the implementation in the Vector2-3* versions need to be changed too.
		if (op == CMP::AnyLT)  { return X <  rhs.X || Y <  rhs.Y || Z <  rhs.Z || W <  rhs.W; }
		if (op == CMP::AllLT)  { return X <  rhs.X && Y <  rhs.Y && Z <  rhs.Z && W <  rhs.W; }
		if (op == CMP::AnyLEQ) { return X <= rhs.X || Y <= rhs.Y || Z <= rhs.Z || W <= rhs.W; }
		if (op == CMP::AllLEQ) { return X <= rhs.X && Y <= rhs.Y && Z <= rhs.Z && W <= rhs.W; }
		if (op == CMP::AnyGT)  { return X >  rhs.X || Y >  rhs.Y || Z >  rhs.Z || W >  rhs.W; }
		if (op == CMP::AllGT)  { return X >  rhs.X && Y >  rhs.Y && Z >  rhs.Z && W >  rhs.W; }
		if (op == CMP::AnyGEQ) { return X >= rhs.X || Y >= rhs.Y || Z >= rhs.Z || W >= rhs.W; }
		if (op == CMP::AllGEQ) { return X >= rhs.X && Y >= rhs.Y && Z >= rhs.Z && W >= rhs.W; }
		if (op == CMP::AnyEQ)  { return X == rhs.X || Y == rhs.Y || Z == rhs.Z || W == rhs.W; }
		if (op == CMP::AllEQ)  { return X == rhs.X && Y == rhs.Y && Z == rhs.Z && W == rhs.W; }
		if (op == CMP::AnyNEQ) { return X != rhs.X || Y != rhs.Y || Z != rhs.Z || W != rhs.W; }
		if (op == CMP::AllNEQ) { return X != rhs.X && Y != rhs.Y && Z != rhs.Z && W != rhs.W; }

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
				 &&  HANDY_NS::WithinEpsilon(Y, rhs.Y)
				 &&  HANDY_NS::WithinEpsilon(Z, rhs.Z)
				 &&  HANDY_NS::WithinEpsilon(W, rhs.W));
		}

		if (op == CMP::AllNWE)
		{
			return !(HANDY_NS::WithinEpsilon(X, rhs.X)
				 ||  HANDY_NS::WithinEpsilon(Y, rhs.Y)
				 ||  HANDY_NS::WithinEpsilon(Z, rhs.Z)
				 ||  HANDY_NS::WithinEpsilon(W, rhs.W));
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


	FORCEINLINE Vector4d Vector4d::Clamp(Vector4d const & min, Vector4d const & max) const { return Max(min).Min(max); }
	FORCEINLINE Vector4d Vector4d::Clamp(  double const   min,   double const   max) const { return Max(min).Min(max); }

	FORCEINLINE Vector4d Vector4d::MultiplyAdd   (Vector4d const & m2, Vector4d const &  a) const { return ((*this) * m2) + a; } // ((*this) * rhs) + a
	FORCEINLINE Vector4d Vector4d::MultiplySub   (Vector4d const & m2, Vector4d const &  a) const { return ((*this) * m2) - a; } // ((*this) * rhs) + a

	FORCEINLINE Vector4d Vector4d::Lerp(Vector4d const & rhs, double           t) const { return (*this) * t + rhs * (1.0_d - t); }
	FORCEINLINE Vector4d Vector4d::Lerp(Vector4d const & rhs, Vector4d const & t) const { return (*this) * t + rhs * (1.0_d - t); }

	FORCEINLINE Vector4d Vector4d::SmoothStep(Vector4d const & rhs, double           t) const { return Lerp(rhs, t * t * (3.0_d - 2.0_d * t)); }
	FORCEINLINE Vector4d Vector4d::SmoothStep(Vector4d const & rhs, Vector4d const & t) const { return Lerp(rhs, t * t * (3.0_d - 2.0_d * t)); }

	FORCEINLINE Vector4d Vector4d::Round_Near () const
	{
		SIMD_IMP_AVX   (return _mm256_round_pd(Native, (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC));)
		SIMD_IMP_NO_AVX(return Vector4d(std::round(X), std::round(Y), std::round(Z), std::round(W));)
		NON_SIMD_IMP   (return Vector4d(std::round(X), std::round(Y), std::round(Z), std::round(W));)
	}

	FORCEINLINE Vector4d Vector4d::Round_Floor() const
	{
		SIMD_IMP_AVX   (return _mm256_round_pd(Native, (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC));)
		SIMD_IMP_NO_AVX(return Vector4d(std::floor(X), std::floor(Y), std::floor(Z), std::floor(W));)
		NON_SIMD_IMP   (return Vector4d(std::floor(X), std::floor(Y), std::floor(Z), std::floor(W));)
	}

	FORCEINLINE Vector4d Vector4d::Round_Ceil() const
	{
		SIMD_IMP_AVX   (return _mm256_round_pd(Native, (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC));)
		SIMD_IMP_NO_AVX(return Vector4d(std::ceil (X), std::ceil (Y), std::ceil (Z), std::ceil (W));)
		NON_SIMD_IMP   (return Vector4d(std::ceil (X), std::ceil (Y), std::ceil (Z), std::ceil (W));)
	}
	
	FORCEINLINE Vector4d Vector4d::Round_Trunc() const
	{
		SIMD_IMP_AVX   (return _mm256_round_pd(Native, (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC));)
		SIMD_IMP_NO_AVX(return Vector4d(std::trunc(X), std::trunc(Y), std::trunc(Z), std::trunc(W));)
		NON_SIMD_IMP   (return Vector4d(std::trunc(X), std::trunc(Y), std::trunc(Z), std::trunc(W));)
	}

	FORCEINLINE Vector4d Vector4d::FMod(Vector4d const & div) const { return Vector4d(std::fmod(X, div.X), std::fmod(Y, div.Y), std::fmod(Z, div.Z), std::fmod(W, div.W)); }
	FORCEINLINE Vector4d Vector4d::FMod(double div)           const { return Vector4d(std::fmod(X, div),   std::fmod(Y, div),   std::fmod(Z, div),   std::fmod(W, div)); }

	FORCEINLINE double & Vector4d::operator[](int idx)       { return Array[idx]; }
	FORCEINLINE double   Vector4d::operator[](int idx) const { return Array[idx]; }
	
	FORCEINLINE Vector4d Vector4d::Sin () { return Vector4d(std::sin(X), std::sin(Y), std::sin(Z), std::sin(W)); }
	FORCEINLINE Vector4d Vector4d::Cos () { return Vector4d(std::cos(X), std::cos(Y), std::cos(Z), std::cos(W)); }
	FORCEINLINE Vector4d Vector4d::Tan () { return Vector4d(std::tan(X), std::tan(Y), std::tan(Z), std::tan(W)); }

	FORCEINLINE Vector4d Vector4d::ASin() { return Vector4d(std::asin(X), std::asin(Y), std::asin(Z), std::asin(W)); }
	FORCEINLINE Vector4d Vector4d::ACos() { return Vector4d(std::acos(X), std::acos(Y), std::acos(Z), std::acos(W)); }
	FORCEINLINE Vector4d Vector4d::ATan() { return Vector4d(std::atan(X), std::atan(Y), std::atan(Z), std::atan(W)); }

	FORCEINLINE Vector4d Vector4d::SinH() { return Vector4d(std::sinh(X), std::sinh(Y), std::sinh(Z), std::sinh(W)); }
	FORCEINLINE Vector4d Vector4d::CosH() { return Vector4d(std::cosh(X), std::cosh(Y), std::cosh(Z), std::cosh(W)); }
	FORCEINLINE Vector4d Vector4d::TanH() { return Vector4d(std::tanh(X), std::tanh(Y), std::tanh(Z), std::tanh(W)); }

	/*static*/ FORCEINLINE Vector4d Vector4d::Zero()    
	{
		SIMD_IMP_AVX   (return _mm256_setzero_pd();)
		SIMD_IMP_NO_AVX(return Vector4d(0.0_d);)
		NON_SIMD_IMP   (return Vector4d(0.0_d);)
	}
	/*static*/ FORCEINLINE Vector4d Vector4d::One()      { return Vector4d( 1.0_d); }
	/*static*/ FORCEINLINE Vector4d Vector4d::NegOne()   { return Vector4d(-1.0_d); }
	/*static*/ FORCEINLINE Vector4d Vector4d::UnitX()    { return Vector4d( 1.0_d,  0.0_d,  0.0_d,  0.0_d); }
	/*static*/ FORCEINLINE Vector4d Vector4d::UnitY()    { return Vector4d( 0.0_d,  1.0_d,  0.0_d,  0.0_d); }
	/*static*/ FORCEINLINE Vector4d Vector4d::UnitZ()    { return Vector4d( 0.0_d,  0.0_d,  1.0_d,  0.0_d); }
	/*static*/ FORCEINLINE Vector4d Vector4d::UnitW()    { return Vector4d( 0.0_d,  0.0_d,  0.0_d,  1.0_d); }
	/*static*/ FORCEINLINE Vector4d Vector4d::UnitXW()   { return Vector4d( 1.0_d,  0.0_d,  0.0_d,  1.0_d); }
	/*static*/ FORCEINLINE Vector4d Vector4d::UnitYW()   { return Vector4d( 0.0_d,  1.0_d,  0.0_d,  1.0_d); }
	/*static*/ FORCEINLINE Vector4d Vector4d::UnitZW()   { return Vector4d( 0.0_d,  0.0_d,  1.0_d,  1.0_d); }
	/*static*/ FORCEINLINE Vector4d Vector4d::NegUnitX() { return Vector4d(-1.0_d,  0.0_d,  0.0_d,  0.0_d); }
	/*static*/ FORCEINLINE Vector4d Vector4d::NegUnitY() { return Vector4d( 0.0_d, -1.0_d,  0.0_d,  0.0_d); }
	/*static*/ FORCEINLINE Vector4d Vector4d::NegUnitZ() { return Vector4d( 0.0_d,  0.0_d, -1.0_d,  0.0_d); }
	/*static*/ FORCEINLINE Vector4d Vector4d::NegUnitW() { return Vector4d( 0.0_d,  0.0_d,  0.0_d, -1.0_d); }

	/*static*/ FORCEINLINE Vector4d Vector4d::PositiveInfinity() { return Vector4d( std::numeric_limits<double>::infinity()); }
	/*static*/ FORCEINLINE Vector4d Vector4d::NegativeInfinity() { return Vector4d(-std::numeric_limits<double>::infinity()); }
	/*static*/ FORCEINLINE Vector4d Vector4d::NaN()              { return Vector4d( std::numeric_limits<double>::quiet_NaN()); }
	/*static*/ FORCEINLINE Vector4d Vector4d::FFFFFFFFFFFFFFFF() 
	{ 
		constexpr uint64_t luf = 0xFFFFFFFFFFFFFFFF_u64;
		double res = *(double*)&(luf);
		return Vector4d(res);
	}

	FORCEINLINE Vector4d operator-(Vector4d const & lhs)
	{
		SIMD_IMP_AVX   (return _mm256_xor_pd(lhs, _mm256_set1_pd(-0.0f));)
		SIMD_IMP_NO_AVX(return 0.0_d - lhs;)
		NON_SIMD_IMP   (return 0.0_d - lhs;)
	}

	FORCEINLINE Vector4d operator~(Vector4d const & lhs)
	{
		SIMD_IMP_AVX   (return _mm256_andnot_pd(lhs, Vector4d::FFFFFFFFFFFFFFFF());)
		SIMD_IMP_NO_AVX(
			uint64_t lufX = *(uint64_t*)&lhs.X;
			uint64_t lufY = *(uint64_t*)&lhs.Y;
			uint64_t lufZ = *(uint64_t*)&lhs.Z;
			uint64_t lufW = *(uint64_t*)&lhs.W;

			double resX = *(double*)&(lufX = ~lufX);
			double resY = *(double*)&(lufY = ~lufY);
			double resZ = *(double*)&(lufZ = ~lufZ);
			double resW = *(double*)&(lufW = ~lufW);

			return Vector4d(resX, resY, resZ, resW);)
		NON_SIMD_IMP   (
			uint64_t lufX = *(uint64_t*)&lhs.X;
			uint64_t lufY = *(uint64_t*)&lhs.Y;
			uint64_t lufZ = *(uint64_t*)&lhs.Z;
			uint64_t lufW = *(uint64_t*)&lhs.W;

			double resX = *(double*)&(lufX = ~lufX);
			double resY = *(double*)&(lufY = ~lufY);
			double resZ = *(double*)&(lufZ = ~lufZ);
			double resW = *(double*)&(lufW = ~lufW);

			return Vector4d(resX, resY, resZ, resW);)
	}

	FORCEINLINE Vector4d operator+(Vector4d const & lhs, Vector4d const & rhs) 
	{
		SIMD_IMP_AVX   (return _mm256_add_pd(lhs, rhs);)
		SIMD_IMP_NO_AVX(return Vector4d(lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z, lhs.W + rhs.W);)
		NON_SIMD_IMP   (return Vector4d(lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z, lhs.W + rhs.W);)
	}

	FORCEINLINE Vector4d operator-(Vector4d const & lhs, Vector4d const & rhs)
	{
		SIMD_IMP_AVX   (return _mm256_sub_pd(lhs, rhs);)
		SIMD_IMP_NO_AVX(return Vector4d(lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z, lhs.W - rhs.W);)
		NON_SIMD_IMP   (return Vector4d(lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z, lhs.W - rhs.W);)
	}

	FORCEINLINE Vector4d operator*(Vector4d const & lhs, Vector4d const & rhs)
	{
		SIMD_IMP_AVX   (return _mm256_mul_pd(lhs, rhs);)
		SIMD_IMP_NO_AVX(return Vector4d(lhs.X * rhs.X, lhs.Y * rhs.Y, lhs.Z * rhs.Z, lhs.W * rhs.W);)
		NON_SIMD_IMP   (return Vector4d(lhs.X * rhs.X, lhs.Y * rhs.Y, lhs.Z * rhs.Z, lhs.W * rhs.W);)
	}

	FORCEINLINE Vector4d operator/(Vector4d const & lhs, Vector4d const & rhs)
	{
		SIMD_IMP_AVX   (return _mm256_div_pd(lhs, rhs);)
		SIMD_IMP_NO_AVX(return Vector4d(lhs.X / rhs.X, lhs.Y / rhs.Y, lhs.Z / rhs.Z, lhs.W / rhs.W);)
		NON_SIMD_IMP   (return Vector4d(lhs.X / rhs.X, lhs.Y / rhs.Y, lhs.Z / rhs.Z, lhs.W / rhs.W);)
	}

	FORCEINLINE Vector4d operator&(Vector4d const & lhs, Vector4d const & rhs)
	{
		SIMD_IMP_AVX   (return _mm256_and_pd(lhs, rhs);)
		SIMD_IMP_NO_AVX(
			uint64_t lufX = *(uint64_t*)&lhs.X;
			uint64_t lufY = *(uint64_t*)&lhs.Y;
			uint64_t lufZ = *(uint64_t*)&lhs.Z;
			uint64_t lufW = *(uint64_t*)&lhs.W;

			uint64_t rufX = *(uint64_t*)&rhs.X;
			uint64_t rufY = *(uint64_t*)&rhs.Y;
			uint64_t rufZ = *(uint64_t*)&rhs.Z;
			uint64_t rufW = *(uint64_t*)&rhs.W;

			double resX = *(double*)&(lufX &= rufX);
			double resY = *(double*)&(lufY &= rufY);
			double resZ = *(double*)&(lufZ &= rufZ);
			double resW = *(double*)&(lufW &= rufW);

			return Vector4d(resX, resY, resZ, resW);)
		NON_SIMD_IMP   (
			uint64_t lufX = *(uint64_t*)&lhs.X;
			uint64_t lufY = *(uint64_t*)&lhs.Y;
			uint64_t lufZ = *(uint64_t*)&lhs.Z;
			uint64_t lufW = *(uint64_t*)&lhs.W;

			uint64_t rufX = *(uint64_t*)&rhs.X;
			uint64_t rufY = *(uint64_t*)&rhs.Y;
			uint64_t rufZ = *(uint64_t*)&rhs.Z;
			uint64_t rufW = *(uint64_t*)&rhs.W;

			double resX = *(double*)&(lufX &= rufX);
			double resY = *(double*)&(lufY &= rufY);
			double resZ = *(double*)&(lufZ &= rufZ);
			double resW = *(double*)&(lufW &= rufW);

			return Vector4d(resX, resY, resZ, resW);)
	}

	FORCEINLINE Vector4d operator|(Vector4d const & lhs, Vector4d const & rhs)
	{
		SIMD_IMP_AVX   (return _mm256_or_pd (lhs, rhs);)
		SIMD_IMP_NO_AVX(
			uint64_t lufX = *(uint64_t*)&lhs.X;
			uint64_t lufY = *(uint64_t*)&lhs.Y;
			uint64_t lufZ = *(uint64_t*)&lhs.Z;
			uint64_t lufW = *(uint64_t*)&lhs.W;

			uint64_t rufX = *(uint64_t*)&rhs.X;
			uint64_t rufY = *(uint64_t*)&rhs.Y;
			uint64_t rufZ = *(uint64_t*)&rhs.Z;
			uint64_t rufW = *(uint64_t*)&rhs.W;

			double resX = *(double*)&(lufX |= rufX);
			double resY = *(double*)&(lufY |= rufY);
			double resZ = *(double*)&(lufZ |= rufZ);
			double resW = *(double*)&(lufW |= rufW);

			return Vector4d(resX, resY, resZ, resW);)
		NON_SIMD_IMP(
			uint64_t lufX = *(uint64_t*)&lhs.X;
			uint64_t lufY = *(uint64_t*)&lhs.Y;
			uint64_t lufZ = *(uint64_t*)&lhs.Z;
			uint64_t lufW = *(uint64_t*)&lhs.W;

			uint64_t rufX = *(uint64_t*)&rhs.X;
			uint64_t rufY = *(uint64_t*)&rhs.Y;
			uint64_t rufZ = *(uint64_t*)&rhs.Z;
			uint64_t rufW = *(uint64_t*)&rhs.W;

			double resX = *(double*)&(lufX |= rufX);
			double resY = *(double*)&(lufY |= rufY);
			double resZ = *(double*)&(lufZ |= rufZ);
			double resW = *(double*)&(lufW |= rufW);

			return Vector4d(resX, resY, resZ, resW);)
	}

	FORCEINLINE Vector4d operator^(Vector4d const & lhs, Vector4d const & rhs)
	{
		SIMD_IMP_AVX   (return _mm256_xor_pd(lhs, rhs);)
		SIMD_IMP_NO_AVX(
			uint64_t lufX = *(uint64_t*)&lhs.X;
			uint64_t lufY = *(uint64_t*)&lhs.Y;
			uint64_t lufZ = *(uint64_t*)&lhs.Z;
			uint64_t lufW = *(uint64_t*)&lhs.W;

			uint64_t rufX = *(uint64_t*)&rhs.X;
			uint64_t rufY = *(uint64_t*)&rhs.Y;
			uint64_t rufZ = *(uint64_t*)&rhs.Z;
			uint64_t rufW = *(uint64_t*)&rhs.W;

			double resX = *(double*)&(lufX ^= rufX);
			double resY = *(double*)&(lufY ^= rufY);
			double resZ = *(double*)&(lufZ ^= rufZ);
			double resW = *(double*)&(lufW ^= rufW);

			return Vector4d(resX, resY, resZ, resW);)
		NON_SIMD_IMP(
			uint64_t lufX = *(uint64_t*)&lhs.X;
			uint64_t lufY = *(uint64_t*)&lhs.Y;
			uint64_t lufZ = *(uint64_t*)&lhs.Z;
			uint64_t lufW = *(uint64_t*)&lhs.W;

			uint64_t rufX = *(uint64_t*)&rhs.X;
			uint64_t rufY = *(uint64_t*)&rhs.Y;
			uint64_t rufZ = *(uint64_t*)&rhs.Z;
			uint64_t rufW = *(uint64_t*)&rhs.W;

			double resX = *(double*)&(lufX ^= rufX);
			double resY = *(double*)&(lufY ^= rufY);
			double resZ = *(double*)&(lufZ ^= rufZ);
			double resW = *(double*)&(lufW ^= rufW);

			return Vector4d(resX, resY, resZ, resW);)
	}

	FORCEINLINE Vector4d operator+(Vector4d const & lhs, double rhs) { return lhs + Vector4d(rhs); }
	FORCEINLINE Vector4d operator-(Vector4d const & lhs, double rhs) { return lhs - Vector4d(rhs); }
	FORCEINLINE Vector4d operator*(Vector4d const & lhs, double rhs) { return lhs * Vector4d(rhs); }
	FORCEINLINE Vector4d operator/(Vector4d const & lhs, double rhs) { return lhs / Vector4d(rhs); }
	FORCEINLINE Vector4d operator&(Vector4d const & lhs, double rhs) { return lhs & Vector4d(rhs); }
	FORCEINLINE Vector4d operator|(Vector4d const & lhs, double rhs) { return lhs | Vector4d(rhs); }
	FORCEINLINE Vector4d operator^(Vector4d const & lhs, double rhs) { return lhs ^ Vector4d(rhs); }

	FORCEINLINE Vector4d operator+(double lhs, Vector4d const & rhs) { return Vector4d(lhs) + rhs; }
	FORCEINLINE Vector4d operator-(double lhs, Vector4d const & rhs) { return Vector4d(lhs) - rhs; }
	FORCEINLINE Vector4d operator*(double lhs, Vector4d const & rhs) { return Vector4d(lhs) * rhs; }
	FORCEINLINE Vector4d operator/(double lhs, Vector4d const & rhs) { return Vector4d(lhs) / rhs; }
	FORCEINLINE Vector4d operator&(double lhs, Vector4d const & rhs) { return Vector4d(lhs) & rhs; }
	FORCEINLINE Vector4d operator|(double lhs, Vector4d const & rhs) { return Vector4d(lhs) | rhs; }
	FORCEINLINE Vector4d operator^(double lhs, Vector4d const & rhs) { return Vector4d(lhs) ^ rhs; }

	FORCEINLINE Vector4d & operator+=(Vector4d & lhs, Vector4d const & rhs) { lhs = lhs + rhs; return lhs; }
	FORCEINLINE Vector4d & operator-=(Vector4d & lhs, Vector4d const & rhs) { lhs = lhs - rhs; return lhs; }
	FORCEINLINE Vector4d & operator*=(Vector4d & lhs, Vector4d const & rhs) { lhs = lhs * rhs; return lhs; }
	FORCEINLINE Vector4d & operator/=(Vector4d & lhs, Vector4d const & rhs) { lhs = lhs / rhs; return lhs; }
	FORCEINLINE Vector4d & operator&=(Vector4d & lhs, Vector4d const & rhs) { lhs = lhs & rhs; return lhs; }
	FORCEINLINE Vector4d & operator|=(Vector4d & lhs, Vector4d const & rhs) { lhs = lhs | rhs; return lhs; }
	FORCEINLINE Vector4d & operator^=(Vector4d & lhs, Vector4d const & rhs) { lhs = lhs / rhs; return lhs; }

	FORCEINLINE Vector4d & operator+=(Vector4d & lhs, double rhs) { lhs = lhs + rhs; return lhs; }
	FORCEINLINE Vector4d & operator-=(Vector4d & lhs, double rhs) { lhs = lhs - rhs; return lhs; }
	FORCEINLINE Vector4d & operator*=(Vector4d & lhs, double rhs) { lhs = lhs * rhs; return lhs; }
	FORCEINLINE Vector4d & operator/=(Vector4d & lhs, double rhs) { lhs = lhs / rhs; return lhs; }
	FORCEINLINE Vector4d & operator&=(Vector4d & lhs, double rhs) { lhs = lhs & rhs; return lhs; }
	FORCEINLINE Vector4d & operator|=(Vector4d & lhs, double rhs) { lhs = lhs | rhs; return lhs; }
	FORCEINLINE Vector4d & operator^=(Vector4d & lhs, double rhs) { lhs = lhs / rhs; return lhs; }

	FORCEINLINE bool operator<(Vector4d const & lhs, Vector4d const & rhs) { return lhs.Compare<CMP::Ordinal>(rhs); }

	// There are ways to do this with SIMD in SSE, but it will be a total PITA, optimize if this ever becomes an issue.
	FORCEINLINE bool operator==(Vector4d const & lhs, Vector4d const & rhs) { return lhs.Compare<CMP::AllWE> (rhs); }
	FORCEINLINE bool operator!=(Vector4d const & lhs, Vector4d const & rhs) { return lhs.Compare<CMP::AnyNWE>(rhs); }

	FORCEINLINE bool operator==(Vector4d const & lhs, double rhs) { return lhs == Vector4d(rhs); }
	FORCEINLINE bool operator!=(Vector4d const & lhs, double rhs) { return lhs != Vector4d(rhs); }

	FORCEINLINE bool operator==(double lhs, Vector4d const & rhs) { return Vector4d(lhs) == rhs; }
	FORCEINLINE bool operator!=(double lhs, Vector4d const & rhs) { return Vector4d(lhs) != rhs; }
}

// Not supposed to add overloads to std technically... but I want my templates that use std::to_string, min, max, etc to work, so oh well then.
namespace std {
	FORCEINLINE ::HANDYMATH_NS::Vector4d min(::HANDYMATH_NS::Vector4d const & a, ::HANDYMATH_NS::Vector4d const & b) { return a.Min(b); }
	FORCEINLINE ::HANDYMATH_NS::Vector4d max(::HANDYMATH_NS::Vector4d const & a, ::HANDYMATH_NS::Vector4d const & b) { return a.Max(b); }

	FORCEINLINE std::string to_string(::HANDYMATH_NS::Vector4d const & v)
	{
		return "("s + std::to_string(v.X) + ", "s + std::to_string(v.Y) + ", "s + std::to_string(v.Z) + ", "s + std::to_string(v.W) + ")"s;
	}
}

MAKE_HASHABLE(HANDYMATH_NS::Vector4d, t.X, t.Y, t.Z, t.W)

