
/// See ../../License.txt for license info.

#pragma once

#include <ostream>
#include <cmath>
#include <cstdint>
#include <limits>
#include <array>
#include <cstring>  // memcpy

#include "Base.hpp"

//#include "Vector3d.hpp"
//#include "Vector4d.hpp"

// TODO, Exp, and Log

namespace HANDYMATH_NS {

	class Vector4d;
	class Vector3d;
	class Vector2;

	class HANDYMATH_ALIGN16 Vector2d 
	{
		friend class Vector4d;
		friend class Vector3d;
		//friend class Vector2i64;

	public:
		static constexpr int32_t NumElements  =  2_i32;
		static constexpr int32_t ElementBytes =  8_i32;
		static constexpr int32_t StrideBytes  = 16_i32;
		static constexpr int32_t UsefulBytes  = NumElements * ElementBytes;

		union
		{
			ARCHDT2               Native;
			struct { double       X, Y; };
			struct { double       x, y; };
			std::array<double, 2> Array;
		};

		// implicit
		Vector2d(ARCHDT2 const & native);
		operator ARCHDT2() const;

		Vector2d();
		Vector2d(Vector2d const & v);

		explicit 
		Vector2d(double xy);
		Vector2d(double xIn, double yIn);

		explicit 
		Vector2d(double const * ary);

		Vector2d & operator=(Vector2d const &) = default;

		// Conversions:
		Vector3d  XYZ(double z) const;
		Vector4d  XYZW(double z, double w) const;
		Vector4d  XYZW(Vector2d const & zw) const;
		Vector2   ToFloat() const;
		//Vector2i64 ToInteger() const;
		Vector4d X000() const;
		Vector4d XY00() const;
		Vector4d XY01() const;
		Vector4d XY10() const;
		Vector4d XY11() const;

		Vector3d X00()  const;
		Vector3d XY0()  const;
		Vector3d XY1()  const;
		// ---

		static Vector2d   FromArray(double const * ary);
		void           CopyToArray(double       * ary) const;

		template<int i> Vector2d Insert(double f) const;
		template<int i> Vector2d InsertZero()    const;

		Vector2d InsertX(double xIn) const;
		Vector2d InsertY(double yIn) const;


		// ---

		Vector2d XX() const;
		Vector2d YY() const;
		Vector2d YX() const;

		Vector2d Min(Vector2d const & rhs) const;
		Vector2d Max(Vector2d const & rhs) const;

		Vector2d Min(double rhs) const;
		Vector2d Max(double rhs) const;

		double   Min()     const;
		double   Max()     const;
		double   Sum()     const;
		double   Product() const;
		double   Mean()    const;

		Vector2d Abs()              const;
		Vector2d Sign()             const; //Returns, per component, the sign of the input vector: input >= 0.0 ? 1.0 : -1.0
		Vector2d Half()             const;
		Vector2d Reciprocal()       const;
		Vector2d ReciprocalApprox() const;
		Vector2d Sqrt()             const;
		Vector2d RSqrt()            const;
		Vector2d RSqrtApprox()      const;
		Vector2d Log2()             const;

		Vector2d To(Vector2d const & rhs) const;

		double   Distance          (Vector2d const & rhs) const;
		double   DistanceSquared   (Vector2d const & rhs) const;

		double   Length()           const;
		double   LengthSquared()    const;

		Vector2d Normalized() const;

		double   Angle(Vector2d const & rhs) const;
		double   Dot  (Vector2d const & rhs) const;

		Vector2d PerpCW() const;
		Vector2d PerpCCW() const;

		Vector2d Power      (double n) const;
		Vector2d PowerApprox(double n) const;

		bool    IsFacing(Vector2d const & rhs) const;

		bool    HasNaN()              const;
		bool    HasPositiveInfinity() const;
		bool    HasNegativeInfinity() const;

		bool    IsNaN()               const;
		bool    IsPositiveInfinity()  const;
		bool    IsNegativeInfinity()  const;

		bool    IsUnbounded()         const;

		template <CMP op> bool Compare(double           rhs) const;
		template <CMP op> bool Compare(Vector2d const & rhs) const;

		Vector2d Clamp(Vector2d const & min, Vector2d const & max) const;
		Vector2d Clamp(double   const   min, double   const   max) const;

		Vector2d MultiplyAdd   (Vector2d const & m2, Vector2d const &  a) const; // ((*this) * rhs) + a
		Vector2d MultiplySub   (Vector2d const & m2, Vector2d const &  a) const; // ((*this) * rhs) - a

		Vector2d Lerp      (Vector2d const & rhs, double           t) const;
		Vector2d Lerp      (Vector2d const & rhs, Vector2d const & t) const;
		Vector2d SmoothStep(Vector2d const & rhs, double           t) const;
		Vector2d SmoothStep(Vector2d const & rhs, Vector2d const & t) const;

		Vector2d Round_Near () const;
		Vector2d Round_Floor() const;
		Vector2d Round_Ceil () const;
		Vector2d Round_Trunc() const;

		Vector2d FMod(Vector2d const & div) const;
		Vector2d FMod(double           div) const;

		double & operator[](int idx);
		double   operator[](int idx) const;

		Vector2d Sin ();
		Vector2d Cos ();
		Vector2d Tan ();

		Vector2d ASin();
		Vector2d ACos();
		Vector2d ATan();

		Vector2d SinH();
		Vector2d CosH();
		Vector2d TanH();

		static Vector2d Zero();
		static Vector2d One();
		static Vector2d NegOne();
		static Vector2d UnitX();
		static Vector2d UnitY();
		static Vector2d NegUnitX();
		static Vector2d NegUnitY();
		static Vector2d PositiveInfinity();
		static Vector2d NegativeInfinity();
		static Vector2d NaN();
		static Vector2d FFFFFFFFFFFFFFFF();

		template <class SerialOp> void serial(SerialOp & ser)
		{
			ser
				(X)
				(Y);
		}
	};

	Vector2d operator-(Vector2d const & lhs);
	Vector2d operator~(Vector2d const & lhs);

	Vector2d operator+(Vector2d const & lhs, Vector2d const & rhs);
	Vector2d operator-(Vector2d const & lhs, Vector2d const & rhs);
	Vector2d operator*(Vector2d const & lhs, Vector2d const & rhs);
	Vector2d operator/(Vector2d const & lhs, Vector2d const & rhs);
	Vector2d operator&(Vector2d const & lhs, Vector2d const & rhs);
	Vector2d operator|(Vector2d const & lhs, Vector2d const & rhs);
	Vector2d operator^(Vector2d const & lhs, Vector2d const & rhs);

	Vector2d operator+(Vector2d const & lhs, double rhs);
	Vector2d operator-(Vector2d const & lhs, double rhs);
	Vector2d operator*(Vector2d const & lhs, double rhs);
	Vector2d operator/(Vector2d const & lhs, double rhs);
	Vector2d operator&(Vector2d const & lhs, double rhs);
	Vector2d operator|(Vector2d const & lhs, double rhs);
	Vector2d operator^(Vector2d const & lhs, double rhs);

	Vector2d operator+(double lhs, Vector2d const & rhs);
	Vector2d operator-(double lhs, Vector2d const & rhs);
	Vector2d operator*(double lhs, Vector2d const & rhs);
	Vector2d operator/(double lhs, Vector2d const & rhs);
	Vector2d operator&(double lhs, Vector2d const & rhs);
	Vector2d operator|(double lhs, Vector2d const & rhs);
	Vector2d operator^(double lhs, Vector2d const & rhs);

	Vector2d & operator+=(Vector2d & lhs, Vector2d const & rhs);
	Vector2d & operator-=(Vector2d & lhs, Vector2d const & rhs);
	Vector2d & operator*=(Vector2d & lhs, Vector2d const & rhs);
	Vector2d & operator/=(Vector2d & lhs, Vector2d const & rhs);
	Vector2d & operator&=(Vector2d & lhs, Vector2d const & rhs);
	Vector2d & operator|=(Vector2d & lhs, Vector2d const & rhs);
	Vector2d & operator^=(Vector2d & lhs, Vector2d const & rhs);

	Vector2d & operator+=(Vector2d & lhs, double rhs);
	Vector2d & operator-=(Vector2d & lhs, double rhs);
	Vector2d & operator*=(Vector2d & lhs, double rhs);
	Vector2d & operator/=(Vector2d & lhs, double rhs);
	Vector2d & operator&=(Vector2d & lhs, double rhs);
	Vector2d & operator|=(Vector2d & lhs, double rhs);
	Vector2d & operator^=(Vector2d & lhs, double rhs);

	bool operator< (Vector2d const & lhs, Vector2d const & rhs);

	bool operator==(Vector2d const & lhs, Vector2d const & rhs);
	bool operator!=(Vector2d const & lhs, Vector2d const & rhs);

	bool operator==(Vector2d const & lhs, double rhs);
	bool operator!=(Vector2d const & lhs, double rhs);

	bool operator==(double lhs, Vector2d const & rhs);
	bool operator!=(double lhs, Vector2d const & rhs);
}

// Not supposed to add overloads to std technically... but I want my templates that use std::to_string, min, max, etc to work, so oh well then.
namespace std {
	FORCEINLINE ::HANDYMATH_NS::Vector2d min(::HANDYMATH_NS::Vector2d const & a, ::HANDYMATH_NS::Vector2d const & b);
	FORCEINLINE ::HANDYMATH_NS::Vector2d max(::HANDYMATH_NS::Vector2d const & a, ::HANDYMATH_NS::Vector2d const & b);

	FORCEINLINE std::string to_string(::HANDYMATH_NS::Vector2d const & v);
}

static inline std::ostream & operator<<(std::ostream & os, const ::HANDYMATH_NS::Vector2d & v)  
{  
	os << "(" << v.X << ", " << v.Y << ")";  
	return os;  
}  

namespace HANDYMATH_NS {

	FORCEINLINE Vector2d::Vector2d() : Native(Vector2d::Zero()) { }

	FORCEINLINE Vector2d::Vector2d(ARCHDT2 const & native) : Native(native)             { }
	FORCEINLINE Vector2d::Vector2d(Vector2d const & v)     : Native(v.Native)           { }

	#if defined USE_SIMD
	FORCEINLINE Vector2d::Vector2d(double xy)              : Native(_mm_set1_pd(xy)) { }
	FORCEINLINE Vector2d::Vector2d(double xIn, double yIn) : Native(_mm_setr_pd(xIn, yIn)) { }
	FORCEINLINE Vector2d::Vector2d(double const * ary)     : Native(_mm_loadu_pd(ary)) { }
	#else
	FORCEINLINE Vector2d::Vector2d(double xy)              : Native { xy, xy } { }
	FORCEINLINE Vector2d::Vector2d(double xIn, double yIn) : Native { xIn, yIn } { }
	FORCEINLINE Vector2d::Vector2d(double const * ary) : Native { ary[0], ary[1] } { }
	#endif

	FORCEINLINE Vector2d::operator ARCHDT2() const { return Native; }

	FORCEINLINE Vector2d Vector2d::FromArray(double const * ary) { return Vector2d(ary); }

	FORCEINLINE void Vector2d::CopyToArray(double * ary) const
	{
		SIMD_IMP    (_mm_storeu_pd(ary, Native);)
		NON_SIMD_IMP(memcpy(ary, &Array[0], sizeof(double) * 2);)
	}

	template<int i> FORCEINLINE Vector2d Vector2d::Insert(double f) const
	{
		auto n = *this;
		n.Array[i] = f;
		return n;
	}

	template<int i> FORCEINLINE Vector2d Vector2d::InsertZero() const
	{
		auto n = *this;
		n.Array[i] = 0.0f;
		return n;
	}

	FORCEINLINE Vector2d Vector2d::InsertX(double xIn) const { return Insert<0>(xIn); }
	FORCEINLINE Vector2d Vector2d::InsertY(double yIn) const { return Insert<1>(yIn); }

	FORCEINLINE Vector2d Vector2d::XX()   const { return Vector2d(X, X); }
	FORCEINLINE Vector2d Vector2d::YY()   const { return Vector2d(Y, Y); }
	FORCEINLINE Vector2d Vector2d::YX()   const { return Vector2d(Y, X); }

	FORCEINLINE Vector2d Vector2d::Min(Vector2d const & rhs) const
	{
		SIMD_IMP    (return _mm_min_pd(Native, rhs);)
		NON_SIMD_IMP(return Vector2d(Handy::FastMin(X, rhs.X), Handy::FastMin(Y, rhs.Y));)
	}

	FORCEINLINE Vector2d Vector2d::Max(Vector2d const & rhs) const
	{
		SIMD_IMP    (return _mm_max_pd(Native, rhs);)
		NON_SIMD_IMP(return Vector2d(Handy::FastMax(X, rhs.X), Handy::FastMax(Y, rhs.Y));)
	}

	FORCEINLINE Vector2d Vector2d::Min(double rhs) const
	{
		SIMD_IMP    (return _mm_min_pd(Native, Vector2d(rhs));)
		NON_SIMD_IMP(return Vector2d(Handy::FastMin(X, rhs), Handy::FastMin(Y, rhs));)
	}

	FORCEINLINE Vector2d Vector2d::Max(double rhs) const
	{
		SIMD_IMP    (return _mm_max_pd(Native, Vector2d(rhs));)
		NON_SIMD_IMP(return Vector2d(Handy::FastMax(X, rhs), Handy::FastMax(Y, rhs));)
	}

	FORCEINLINE double Vector2d::Min()     const 
	{
		return Handy::FastMin(X, Y);
	}

	FORCEINLINE double Vector2d::Max()     const 
	{
		return Handy::FastMax(X, Y);
	}

	FORCEINLINE double Vector2d::Sum()     const 
	{
		return X + Y;
	}
		
	FORCEINLINE double Vector2d::Product() const 
	{
		return X * Y;
	}

	FORCEINLINE double Vector2d::Mean()    const { return Sum()   * 0.5_d; }

	FORCEINLINE Vector2d Vector2d::Abs() const 
	{
		SIMD_IMP    (return _mm_and_pd(Native, _mm_castsi128_pd(_mm_set1_epi64x(0x7FFFFFFFFFFFFFFFLL)));)
		NON_SIMD_IMP(return Vector4d(std::abs(X), std::abs(Y), std::abs(Z), std::abs(W));)
	}

	FORCEINLINE Vector2d Vector2d::Sign() const
	{
		SIMD_IMP_AVX   (Vector2d mask = _mm_cmp_pd(Native, Zero(), _CMP_GE_OQ); return _mm_andnot_pd(mask, NegOne()) | (One() & mask);)
		SIMD_IMP_NO_AVX(return Vector2d(X >= 0.0 ? 1.0 : -1.0, Y >= 0.0 ? 1.0 : -1.0);)
		NON_SIMD_IMP   (return Vector2d(X >= 0.0 ? 1.0 : -1.0, Y >= 0.0 ? 1.0 : -1.0);)
		
	}

	FORCEINLINE Vector2d Vector2d::Half()             const { return (*this) * 0.5_d; }
	FORCEINLINE Vector2d Vector2d::Reciprocal()       const { return 1.0_d / (*this); }
	FORCEINLINE Vector2d Vector2d::ReciprocalApprox() const { return 1.0_d / (*this); }
	FORCEINLINE Vector2d Vector2d::Sqrt()             const 
	{
		SIMD_IMP    (return _mm_sqrt_pd(Native);)
		NON_SIMD_IMP(return Vector2d(std::sqrt(X), std::sqrt(Y));)
	}

	FORCEINLINE Vector2d Vector2d::RSqrt()            const { return this->Sqrt().Reciprocal(); }
	FORCEINLINE Vector2d Vector2d::RSqrtApprox()      const { return this->Sqrt().Reciprocal(); }

	FORCEINLINE Vector2d Vector2d::Log2()             const { return { log2(X), log2(Y) }; }

	FORCEINLINE Vector2d Vector2d::To(Vector2d const & rhs) const { return rhs - (*this); }

	FORCEINLINE double   Vector2d::Distance       (Vector2d const & rhs) const { return (rhs - (*this)).Length(); }
	FORCEINLINE double   Vector2d::DistanceSquared(Vector2d const & rhs) const { return (rhs - (*this)).LengthSquared(); }

	FORCEINLINE double   Vector2d::Length()        const { return std::sqrt(Dot(*this)); }
	FORCEINLINE double   Vector2d::LengthSquared() const { return Dot(*this); }

	FORCEINLINE Vector2d Vector2d::Normalized() const { return (*this) * (1.0_d / Length()); }

	FORCEINLINE double   Vector2d::Angle(Vector2d const & rhs) const
	{
		double vdot = Normalized().Dot(rhs.Normalized());

		if (vdot < -1.0_d)
			vdot = -1.0_d;

		if (vdot > 1.0_d)
			vdot = 1.0_d;

		return std::acos(vdot);
	}
	
	FORCEINLINE double   Vector2d::Dot  (Vector2d const & rhs) const { return ((*this) * rhs).Sum(); }

	FORCEINLINE Vector2d Vector2d::PerpCW()  const { return {  Y, -X }; } // Vector2d( Y, -X);
	FORCEINLINE Vector2d Vector2d::PerpCCW() const { return { -Y,  X }; } // Vector2d(-Y,  X);

	FORCEINLINE Vector2d Vector2d::Power      (double n) const { return Vector2d(std::pow(X, n), std::pow(Y, n)); }
	FORCEINLINE Vector2d Vector2d::PowerApprox(double n) const { return Vector2d(std::pow(X, n), std::pow(Y, n)); }

	FORCEINLINE bool Vector2d::IsFacing(Vector2d const & rhs) const { return Dot(rhs) > 0.0_d; }

	FORCEINLINE bool Vector2d::HasNaN()              const { return std::isnan(X) || std::isnan(Y); }
	FORCEINLINE bool Vector2d::HasPositiveInfinity() const { return Compare<CMP::AnyEQ>( std::numeric_limits<double>::infinity()); } 
	FORCEINLINE bool Vector2d::HasNegativeInfinity() const { return Compare<CMP::AnyEQ>(-std::numeric_limits<double>::infinity()); }

	FORCEINLINE bool Vector2d::IsNaN()              const { return std::isnan(X) && std::isnan(Y); }
	FORCEINLINE bool Vector2d::IsPositiveInfinity() const { return Compare<CMP::AllEQ>( std::numeric_limits<double>::infinity()); }
	FORCEINLINE bool Vector2d::IsNegativeInfinity() const { return Compare<CMP::AllEQ>(-std::numeric_limits<double>::infinity()); }

	FORCEINLINE bool Vector2d::IsUnbounded() const { return HasPositiveInfinity() || HasNegativeInfinity(); }

	template <CMP op>
	FORCEINLINE bool Vector2d::Compare(double rhs) const
	{
		return Compare<op>(Vector2d(rhs));
	}

	template <CMP op>
	FORCEINLINE bool Vector2d::Compare(Vector2d const & rhs) const
	{
		/// IMPORTANT: If any of these are upgraded, the implementation in the Vector2-3* versions need to be changed too.
		if (op == CMP::AnyLT)  { return X <  rhs.X || Y <  rhs.Y; }
		if (op == CMP::AllLT)  { return X <  rhs.X && Y <  rhs.Y; }
		if (op == CMP::AnyLEQ) { return X <= rhs.X || Y <= rhs.Y; }
		if (op == CMP::AllLEQ) { return X <= rhs.X && Y <= rhs.Y; }
		if (op == CMP::AnyGT)  { return X >  rhs.X || Y >  rhs.Y; }
		if (op == CMP::AllGT)  { return X >  rhs.X && Y >  rhs.Y; }
		if (op == CMP::AnyGEQ) { return X >= rhs.X || Y >= rhs.Y; }
		if (op == CMP::AllGEQ) { return X >= rhs.X && Y >= rhs.Y; }
		if (op == CMP::AnyEQ)  { return X == rhs.X || Y == rhs.Y; }
		if (op == CMP::AllEQ)  { return X == rhs.X && Y == rhs.Y; }
		if (op == CMP::AnyNEQ) { return X != rhs.X || Y != rhs.Y; }
		if (op == CMP::AllNEQ) { return X != rhs.X && Y != rhs.Y; }

		if (op == CMP::AnyWE)
		{
			return (HANDY_NS::WithinEpsilon(X, rhs.X)
				||  HANDY_NS::WithinEpsilon(Y, rhs.Y));
		}

		if (op == CMP::AllWE)
		{
			return (HANDY_NS::WithinEpsilon(X, rhs.X)
				&&  HANDY_NS::WithinEpsilon(Y, rhs.Y));
		}

		if (op == CMP::AnyNWE)
		{
			return !(HANDY_NS::WithinEpsilon(X, rhs.X)
				 &&  HANDY_NS::WithinEpsilon(Y, rhs.Y));
		}

		if (op == CMP::AllNWE)
		{
			return !(HANDY_NS::WithinEpsilon(X, rhs.X)
				 ||  HANDY_NS::WithinEpsilon(Y, rhs.Y));
		}

		// Used for storing Vectors in collections. Compare NWE=>LT, X, then Y, then.... Default op for operator< on vector types.
		if (op == CMP::Ordinal)
		{
			if (!HANDY_NS::WithinEpsilon(X, rhs.X)) return X < rhs.X;
			if (!HANDY_NS::WithinEpsilon(Y, rhs.Y)) return Y < rhs.Y;
			return false;
		} 
	}

	FORCEINLINE Vector2d Vector2d::Clamp(Vector2d const & min, Vector2d const & max) const { return Max(min).Min(max); }
	FORCEINLINE Vector2d Vector2d::Clamp(  double const   min,   double const   max) const { return Max(min).Min(max); }

	FORCEINLINE Vector2d Vector2d::MultiplyAdd(Vector2d const & m2, Vector2d const &  a) const { return ((*this) * m2) + a; } // ((*this) * rhs) + a
	FORCEINLINE Vector2d Vector2d::MultiplySub(Vector2d const & m2, Vector2d const &  a) const { return ((*this) * m2) - a; } // ((*this) * rhs) + a

	FORCEINLINE Vector2d Vector2d::Lerp(Vector2d const & rhs, double           t) const { return (*this) * t + rhs * (1.0_d - t); }
	FORCEINLINE Vector2d Vector2d::Lerp(Vector2d const & rhs, Vector2d const & t) const { return (*this) * t + rhs * (1.0_d - t); }

	FORCEINLINE Vector2d Vector2d::SmoothStep(Vector2d const & rhs, double           t) const { return Lerp(rhs, t * t * (3.0_d - 2.0_d * t)); }
	FORCEINLINE Vector2d Vector2d::SmoothStep(Vector2d const & rhs, Vector2d const & t) const { return Lerp(rhs, t * t * (3.0_d - 2.0_d * t)); }

	FORCEINLINE Vector2d Vector2d::Round_Near () const
	{
		SIMD_IMP_SSE41   (return _mm_round_pd(Native, (_MM_FROUND_TO_NEAREST_INT |_MM_FROUND_NO_EXC));)
		SIMD_IMP_NO_SSE41(return Vector2d(std::round(X), std::round(Y));)
		NON_SIMD_IMP     (return Vector2d(std::round(X), std::round(Y));)
	}

	FORCEINLINE Vector2d Vector2d::Round_Floor() const
	{
		SIMD_IMP_SSE41   (return _mm_round_pd(Native, (_MM_FROUND_TO_NEG_INF |_MM_FROUND_NO_EXC));)
		SIMD_IMP_NO_SSE41(return Vector2d(std::floor(X), std::floor(Y));)
		NON_SIMD_IMP     (return Vector2d(std::floor(X), std::floor(Y));)
	}

	FORCEINLINE Vector2d Vector2d::Round_Ceil() const
	{
		SIMD_IMP_SSE41   (return _mm_round_pd(Native, (_MM_FROUND_TO_POS_INF |_MM_FROUND_NO_EXC));)
		SIMD_IMP_NO_SSE41(return Vector2d(std::ceil (X), std::ceil (Y));)
		NON_SIMD_IMP     (return Vector2d(std::ceil (X), std::ceil (Y));)
	}

	FORCEINLINE Vector2d Vector2d::Round_Trunc() const
	{
		SIMD_IMP_SSE41   (return _mm_round_pd(Native, (_MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC));)
		SIMD_IMP_NO_SSE41(return Vector2d(std::trunc(X), std::trunc(Y));)
		NON_SIMD_IMP     (return Vector2d(std::trunc(X), std::trunc(Y));)
	}
	
	FORCEINLINE Vector2d Vector2d::FMod(Vector2d const & div) const { return Vector2d(std::fmod(X, div.X), std::fmod(Y, div.Y)); }
	FORCEINLINE Vector2d Vector2d::FMod(double div)           const { return Vector2d(std::fmod(X, div),   std::fmod(Y, div)); }

	FORCEINLINE double & Vector2d::operator[](int idx)       { return Array[idx]; }
	FORCEINLINE double   Vector2d::operator[](int idx) const { return Array[idx]; }

	FORCEINLINE Vector2d Vector2d::Sin () { return Vector2d(std::sin (X), std::sin (Y)); }
	FORCEINLINE Vector2d Vector2d::Cos () { return Vector2d(std::cos (X), std::cos (Y)); }
	FORCEINLINE Vector2d Vector2d::Tan () { return Vector2d(std::tan (X), std::tan (Y)); }

	FORCEINLINE Vector2d Vector2d::ASin() { return Vector2d(std::asin(X), std::asin(Y)); }
	FORCEINLINE Vector2d Vector2d::ACos() { return Vector2d(std::acos(X), std::acos(Y)); }
	FORCEINLINE Vector2d Vector2d::ATan() { return Vector2d(std::atan(X), std::atan(Y)); }

	FORCEINLINE Vector2d Vector2d::SinH() { return Vector2d(std::sinh(X), std::sinh(Y)); }
	FORCEINLINE Vector2d Vector2d::CosH() { return Vector2d(std::cosh(X), std::cosh(Y)); }
	FORCEINLINE Vector2d Vector2d::TanH() { return Vector2d(std::tanh(X), std::tanh(Y)); }


	/*static*/ FORCEINLINE Vector2d Vector2d::Zero()    
	{
		SIMD_IMP    (return _mm_setzero_pd();)
		NON_SIMD_IMP(return Vector2d(0.0_d);)
	}
	/*static*/ FORCEINLINE Vector2d Vector2d::One()      { return Vector2d( 1.0_d); }
	/*static*/ FORCEINLINE Vector2d Vector2d::NegOne()   { return Vector2d(-1.0_d); }
	/*static*/ FORCEINLINE Vector2d Vector2d::UnitX()    { return Vector2d( 1.0_d,  0.0_d); }
	/*static*/ FORCEINLINE Vector2d Vector2d::UnitY()    { return Vector2d( 0.0_d,  1.0_d); }
	/*static*/ FORCEINLINE Vector2d Vector2d::NegUnitX() { return Vector2d(-1.0_d,  0.0_d); }
	/*static*/ FORCEINLINE Vector2d Vector2d::NegUnitY() { return Vector2d( 0.0_d, -1.0_d); }

	/*static*/ FORCEINLINE Vector2d Vector2d::PositiveInfinity() { return Vector2d(-std::numeric_limits<double>::infinity()); }
	/*static*/ FORCEINLINE Vector2d Vector2d::NegativeInfinity() { return Vector2d( std::numeric_limits<double>::infinity()); }
	/*static*/ FORCEINLINE Vector2d Vector2d::NaN()              { return Vector2d( std::numeric_limits<double>::quiet_NaN()); }
	/*static*/ FORCEINLINE Vector2d Vector2d::FFFFFFFFFFFFFFFF() 
	{ 
		constexpr uint64_t luf = 0xFFFFFFFFFFFFFFFF_u64;
		double res = *(double*)&(luf);
		return Vector2d(res);
	}

	FORCEINLINE Vector2d operator-(Vector2d const & lhs)
	{
		SIMD_IMP    (return _mm_xor_pd(lhs, _mm_set1_pd(-0.0f));)
		NON_SIMD_IMP(return 0.0_d - lhs;)
	}

	FORCEINLINE Vector2d operator~(Vector2d const & lhs)
	{
		SIMD_IMP    (return _mm_andnot_pd(lhs, Vector2d::FFFFFFFFFFFFFFFF());)
		NON_SIMD_IMP(
			uint64_t lufX = *(uint64_t*)&lhs.X;
			uint64_t lufY = *(uint64_t*)&lhs.Y;

			double resX = *(double*)&(lufX = ~lufX);
			double resY = *(double*)&(lufY = ~lufY);

			return Vector2d(resX, resY);)
	}

	FORCEINLINE Vector2d operator+(Vector2d const & lhs, Vector2d const & rhs) 
	{
		SIMD_IMP    (return _mm_add_pd(lhs, rhs);)
		NON_SIMD_IMP(return Vector2d(lhs.X + rhs.X, lhs.Y + rhs.Y);)
	}

	FORCEINLINE Vector2d operator-(Vector2d const & lhs, Vector2d const & rhs)
	{
		SIMD_IMP    (return _mm_sub_pd(lhs, rhs);)
		NON_SIMD_IMP(return Vector2d(lhs.X - rhs.X, lhs.Y - rhs.Y);)
	}

	FORCEINLINE Vector2d operator*(Vector2d const & lhs, Vector2d const & rhs)
	{
		SIMD_IMP    (return _mm_mul_pd(lhs, rhs);)
		NON_SIMD_IMP(return Vector2d(lhs.X * rhs.X, lhs.Y * rhs.Y);)
	}

	FORCEINLINE Vector2d operator/(Vector2d const & lhs, Vector2d const & rhs)
	{
		SIMD_IMP    (return _mm_div_pd(lhs, rhs);)
		NON_SIMD_IMP(return Vector2d(lhs.X / rhs.X, lhs.Y / rhs.Y);)
	}

	FORCEINLINE Vector2d operator&(Vector2d const & lhs, Vector2d const & rhs)
	{
		SIMD_IMP    (return _mm_and_pd(lhs, rhs);)
		NON_SIMD_IMP(
			uint64_t lufX = *(uint64_t*)&lhs.X;
			uint64_t lufY = *(uint64_t*)&lhs.Y;

			uint64_t rufX = *(uint64_t*)&rhs.X;
			uint64_t rufY = *(uint64_t*)&rhs.Y;

			double resX = *(double*)&(lufX &= rufX);
			double resY = *(double*)&(lufY &= rufY);

			return Vector2d(resX, resY);)
	}

	FORCEINLINE Vector2d operator|(Vector2d const & lhs, Vector2d const & rhs)
	{
		SIMD_IMP    (return _mm_or_pd (lhs, rhs);)
		NON_SIMD_IMP(
			uint64_t lufX = *(uint64_t*)&lhs.X;
			uint64_t lufY = *(uint64_t*)&lhs.Y;

			uint64_t rufX = *(uint64_t*)&rhs.X;
			uint64_t rufY = *(uint64_t*)&rhs.Y;

			double resX = *(double*)&(lufX |= rufX);
			double resY = *(double*)&(lufY |= rufY);

			return Vector2d(resX, resY);)
	}

	FORCEINLINE Vector2d operator^(Vector2d const & lhs, Vector2d const & rhs)
	{
		SIMD_IMP    (return _mm_xor_pd(lhs, rhs);)
		NON_SIMD_IMP(
			uint64_t lufX = *(uint64_t*)&lhs.X;
			uint64_t lufY = *(uint64_t*)&lhs.Y;

			uint64_t rufX = *(uint64_t*)&rhs.X;
			uint64_t rufY = *(uint64_t*)&rhs.Y;

			double resX = *(double*)&(lufX ^= rufX);
			double resY = *(double*)&(lufY ^= rufY);

			return Vector2d(resX, resY);)
	}
	
	FORCEINLINE Vector2d operator+(Vector2d const & lhs, double rhs) { return lhs + Vector2d(rhs); }
	FORCEINLINE Vector2d operator-(Vector2d const & lhs, double rhs) { return lhs - Vector2d(rhs); }
	FORCEINLINE Vector2d operator*(Vector2d const & lhs, double rhs) { return lhs * Vector2d(rhs); }
	FORCEINLINE Vector2d operator/(Vector2d const & lhs, double rhs) { return lhs / Vector2d(rhs); }
	FORCEINLINE Vector2d operator&(Vector2d const & lhs, double rhs) { return lhs & Vector2d(rhs); }
	FORCEINLINE Vector2d operator|(Vector2d const & lhs, double rhs) { return lhs | Vector2d(rhs); }
	FORCEINLINE Vector2d operator^(Vector2d const & lhs, double rhs) { return lhs ^ Vector2d(rhs); }

	FORCEINLINE Vector2d operator+(double lhs, Vector2d const & rhs) { return Vector2d(lhs) + rhs; }
	FORCEINLINE Vector2d operator-(double lhs, Vector2d const & rhs) { return Vector2d(lhs) - rhs; }
	FORCEINLINE Vector2d operator*(double lhs, Vector2d const & rhs) { return Vector2d(lhs) * rhs; }
	FORCEINLINE Vector2d operator/(double lhs, Vector2d const & rhs) { return Vector2d(lhs) / rhs; }
	FORCEINLINE Vector2d operator&(double lhs, Vector2d const & rhs) { return Vector2d(lhs) & rhs; }
	FORCEINLINE Vector2d operator|(double lhs, Vector2d const & rhs) { return Vector2d(lhs) | rhs; }
	FORCEINLINE Vector2d operator^(double lhs, Vector2d const & rhs) { return Vector2d(lhs) ^ rhs; }

	FORCEINLINE Vector2d & operator+=(Vector2d & lhs, Vector2d const & rhs) { lhs = lhs + rhs; return lhs; }
	FORCEINLINE Vector2d & operator-=(Vector2d & lhs, Vector2d const & rhs) { lhs = lhs - rhs; return lhs; }
	FORCEINLINE Vector2d & operator*=(Vector2d & lhs, Vector2d const & rhs) { lhs = lhs * rhs; return lhs; }
	FORCEINLINE Vector2d & operator/=(Vector2d & lhs, Vector2d const & rhs) { lhs = lhs / rhs; return lhs; }
	FORCEINLINE Vector2d & operator&=(Vector2d & lhs, Vector2d const & rhs) { lhs = lhs & rhs; return lhs; }
	FORCEINLINE Vector2d & operator|=(Vector2d & lhs, Vector2d const & rhs) { lhs = lhs | rhs; return lhs; }
	FORCEINLINE Vector2d & operator^=(Vector2d & lhs, Vector2d const & rhs) { lhs = lhs / rhs; return lhs; }

	FORCEINLINE Vector2d & operator+=(Vector2d & lhs, double rhs) { lhs = lhs + rhs; return lhs; }
	FORCEINLINE Vector2d & operator-=(Vector2d & lhs, double rhs) { lhs = lhs - rhs; return lhs; }
	FORCEINLINE Vector2d & operator*=(Vector2d & lhs, double rhs) { lhs = lhs * rhs; return lhs; }
	FORCEINLINE Vector2d & operator/=(Vector2d & lhs, double rhs) { lhs = lhs / rhs; return lhs; }
	FORCEINLINE Vector2d & operator&=(Vector2d & lhs, double rhs) { lhs = lhs & rhs; return lhs; }
	FORCEINLINE Vector2d & operator|=(Vector2d & lhs, double rhs) { lhs = lhs | rhs; return lhs; }
	FORCEINLINE Vector2d & operator^=(Vector2d & lhs, double rhs) { lhs = lhs / rhs; return lhs; }
	
	FORCEINLINE bool operator< (Vector2d const & lhs, Vector2d const & rhs) { return lhs.Compare<CMP::Ordinal>(rhs); }

	FORCEINLINE bool operator==(Vector2d const & lhs, Vector2d const & rhs) { return lhs.Compare<CMP::AllWE> (rhs); }
	FORCEINLINE bool operator!=(Vector2d const & lhs, Vector2d const & rhs) { return lhs.Compare<CMP::AnyNWE>(rhs); }
	
	FORCEINLINE bool operator==(Vector2d const & lhs, double rhs) { return lhs == Vector2d(rhs); }
	FORCEINLINE bool operator!=(Vector2d const & lhs, double rhs) { return lhs != Vector2d(rhs); }

	FORCEINLINE bool operator==(double lhs, Vector2d const & rhs) { return Vector2d(lhs) == rhs; }
	FORCEINLINE bool operator!=(double lhs, Vector2d const & rhs) { return Vector2d(lhs) != rhs; }
}

// Not supposed to add overloads to std technically... but I want my templates that use std::to_string, min, max, etc to work, so oh well then.
namespace std {
	FORCEINLINE ::HANDYMATH_NS::Vector2d min(::HANDYMATH_NS::Vector2d const & a, ::HANDYMATH_NS::Vector2d const & b) { return a.Min(b); }
	FORCEINLINE ::HANDYMATH_NS::Vector2d max(::HANDYMATH_NS::Vector2d const & a, ::HANDYMATH_NS::Vector2d const & b) { return a.Max(b); }
	FORCEINLINE std::string to_string(::HANDYMATH_NS::Vector2d const & v)
	{
		return "("s + std::to_string(v.X) + ", "s + std::to_string(v.Y) + ")"s;
	}
}

MAKE_HASHABLE(HANDYMATH_NS::Vector2d, t.X, t.Y)
