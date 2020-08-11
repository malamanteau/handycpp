
/// See ../../License.txt for license info.

#pragma once

#include <ostream>
#include <cmath>
#include <cstdint>
#include <limits>
#include <array>
#include <cstring>  // memcpy

#include "Base.hpp"

#include "Vector3.hpp"
#include "Vector4.hpp"

// TODO, Exp, and Log

namespace HANDYMATH_NS {

	class Vector4;
	class Vector3;

	class CVector2 /// Compact version of Vector2, for storage, rather than computation.
	{
	public:
		float X = 0.0f, Y = 0.0f;
		
		CVector2() = default;
		CVector2(float xIn, float yIn) : X(xIn), Y(yIn) { }

		CVector2 & operator=(CVector2 const &) = default;
	};

	class HANDYMATH_ALIGN16 Vector2 
	{
		friend class Vector4;
		friend class Vector3;
		friend class Vector2i;
		friend class Vector2d;

		// IMPLICIT, PRIVATE. 
		// Make these public and suffer my wrath. This cast is unsafe, and doesn't sanitize Native.ZW to zero!
		Vector2(Vector4 const & native);
		operator Vector4() const;

	public:
		static constexpr int32_t NumElements  =  2_i32;
		static constexpr int32_t ElementBytes =  4_i32;
		static constexpr int32_t StrideBytes  = 16_i32;
		static constexpr int32_t UsefulBytes  = NumElements * ElementBytes;

		union
		{
			Vector4              Native;
			struct { float       X, Y, _zeroA, _zeroB; };
			std::array<float, 4> Array;
			CVector2             Compact;
			struct { float       x, y; };
		};
		
		Vector2(CVector2 const & v2c);
		operator CVector2() const;

		Vector2();
		Vector2(Vector2 const & v);

		explicit 
		Vector2(float xy);
		Vector2(float xIn, float yIn);

		explicit 
		Vector2(float const * ary);

		Vector2 & operator=(Vector2 const &) = default;

		// Conversions:
		Vector3  XYZ(float z) const;
		Vector4  XYZW(float z, float w) const;
		Vector4  XYZW(Vector2 const & zw) const;
		Vector2i ToInteger() const;
		// ---

		static Vector2   FromArray(float const * ary);
		void           CopyToArray(float       * ary) const;

		template<int i0, int i1>                 Vector2 Shuffle    () const;
		template<int i0, int i1, int i2>         Vector3 ShuffleXY0 () const;
		template<int i0, int i1, int i2, int i3> Vector4 ShuffleXY00() const;

		template<int i> Vector2 Insert(float f) const;
		template<int i> Vector2 InsertZero()    const;

		Vector2 InsertX(float xIn) const;
		Vector2 InsertY(float yIn) const;

		// These have all been optimized to be faster than using Shuffle<>()!
		Vector4 X000() const;
		Vector4 XY00() const; // THIS OPERATION IS 100% FREE, IT WILL BE COMPLETELY ERASED WHEN INLINED!
		Vector4 XY01() const;
		Vector4 XY10() const;
		Vector4 XY11() const;

		Vector3 X00()  const;
		Vector3 XY0()  const; // THIS OPERATION IS 100% FREE, IT WILL BE COMPLETELY ERASED WHEN INLINED!
		Vector3 XY1()  const;
		// ---

		Vector2 XX() const;
		Vector2 YY() const;
		Vector2 YX() const;

		Vector2 Min(Vector2 const & rhs) const;
		Vector2 Max(Vector2 const & rhs) const;

		Vector2 Min(float rhs) const;
		Vector2 Max(float rhs) const;

		float   Min()     const;
		float   Max()     const;
		float   Sum()     const;
		float   Product() const;
		float   Mean()    const;

		Vector2 Abs()              const;
		Vector2 Sign()             const; //Returns, per component, the sign of the input vector: input >= 0.0 ? 1.0 : -1.0
		Vector2 Half()             const;
		Vector2 Reciprocal()       const;
		Vector2 ReciprocalApprox() const;
		Vector2 Sqrt()             const;
		Vector2 RSqrt()            const;
		Vector2 RSqrtApprox()      const;

		Vector2 To(Vector2 const & rhs) const;

		float   Distance          (Vector2 const & rhs) const;
		float   DistanceSquared   (Vector2 const & rhs) const;

		float   Length()           const;
		float   LengthSquared()    const;

		Vector2 Normalized() const;

		float   Angle(Vector2 const & rhs) const;
		float   Dot  (Vector2 const & rhs) const;
		
		Vector2 OrthoCW() const;
		Vector2 OrthoCCW() const;

		Vector2 Power      (float n) const;
		Vector2 PowerApprox(float n) const;

		bool    IsFacing(Vector2 const & rhs) const;

		bool    HasNaN()              const;
		bool    HasPositiveInfinity() const;
		bool    HasNegativeInfinity() const;

		bool    IsNaN()               const;
		bool    IsPositiveInfinity()  const;
		bool    IsNegativeInfinity()  const;

		bool    IsUnbounded()         const;

		template <CMP op> bool Compare(float           rhs) const;
		template <CMP op> bool Compare(Vector2 const & rhs) const;

		Vector2 Clamp(Vector2 const & min, Vector2 const & max) const;
		Vector2 Clamp(float   const   min, float   const   max) const;

		Vector2 MultiplyAdd   (Vector2 const & m2, Vector2 const &  a) const; // ((*this) * rhs) + a
		Vector2 MultiplySub   (Vector2 const & m2, Vector2 const &  a) const; // ((*this) * rhs) - a

		Vector2 Lerp      (Vector2 const & rhs, float           t) const;
		Vector2 Lerp      (Vector2 const & rhs, Vector2 const & t) const;
		Vector2 SmoothStep(Vector2 const & rhs, float           t) const;
		Vector2 SmoothStep(Vector2 const & rhs, Vector2 const & t) const;

		Vector2 Round_Near () const;
		Vector2 Round_Floor() const;
		Vector2 Round_Ceil () const;
		Vector2 Round_Trunc() const;

		Vector2 FMod(Vector2 const & div) const;
		Vector2 FMod(float           div) const;

		float & operator[](int idx);
		float   operator[](int idx) const;

		Vector2 Sin ();
		Vector2 Cos ();
		Vector2 Tan ();

		Vector2 ASin();
		Vector2 ACos();
		Vector2 ATan();

		Vector2 SinH();
		Vector2 CosH();
		Vector2 TanH();

		static Vector2 Zero();
		static Vector2 One();
		static Vector2 NegOne();
		static Vector2 UnitX();
		static Vector2 UnitY();
		static Vector2 NegUnitX();
		static Vector2 NegUnitY();
		static Vector2 PositiveInfinity();
		static Vector2 NegativeInfinity();
		static Vector2 NaN();

		static Vector2 UnsafeCast(Vector4 const & v); // Certain operators need to cast from Vector4 to Vector2 without any checking. File this under "Stupid Hacks".
	};

	/*static*/ FORCEINLINE Vector2 Vector2::UnsafeCast(Vector4 const & v) { return Vector2(v); }

	Vector2 operator-(Vector2 const & lhs);

	Vector2 operator+(Vector2 const & lhs, Vector2 const & rhs);
	Vector2 operator-(Vector2 const & lhs, Vector2 const & rhs);
	Vector2 operator*(Vector2 const & lhs, Vector2 const & rhs);
	Vector2 operator/(Vector2 const & lhs, Vector2 const & rhs);

	Vector2 operator+(Vector2 const & lhs, float rhs);
	Vector2 operator-(Vector2 const & lhs, float rhs);
	Vector2 operator*(Vector2 const & lhs, float rhs);
	Vector2 operator/(Vector2 const & lhs, float rhs);

	Vector2 operator+(float lhs, Vector2 const & rhs);
	Vector2 operator-(float lhs, Vector2 const & rhs);
	Vector2 operator*(float lhs, Vector2 const & rhs);
	Vector2 operator/(float lhs, Vector2 const & rhs);
	
	Vector2 & operator+=(Vector2 & lhs, Vector2 const & rhs);
	Vector2 & operator-=(Vector2 & lhs, Vector2 const & rhs);
	Vector2 & operator*=(Vector2 & lhs, Vector2 const & rhs);
	Vector2 & operator/=(Vector2 & lhs, Vector2 const & rhs);
	
	Vector2 & operator+=(Vector2 & lhs, float rhs);
	Vector2 & operator-=(Vector2 & lhs, float rhs);
	Vector2 & operator*=(Vector2 & lhs, float rhs);
	Vector2 & operator/=(Vector2 & lhs, float rhs);

	bool operator< (Vector2 const & lhs, Vector2 const & rhs);

	bool operator==(Vector2 const & lhs, Vector2 const & rhs);
	bool operator!=(Vector2 const & lhs, Vector2 const & rhs);

	bool operator==(Vector2 const & lhs, float rhs);
	bool operator!=(Vector2 const & lhs, float rhs);

	bool operator==(float lhs, Vector2 const & rhs);
	bool operator!=(float lhs, Vector2 const & rhs);
}

// Not supposed to add overloads to std technically... but I want my templates that use std::to_string, min, max, etc to work, so oh well then.
namespace std {
	FORCEINLINE ::HANDYMATH_NS::Vector2 min(::HANDYMATH_NS::Vector2 const & a, ::HANDYMATH_NS::Vector2 const & b);
	FORCEINLINE ::HANDYMATH_NS::Vector2 max(::HANDYMATH_NS::Vector2 const & a, ::HANDYMATH_NS::Vector2 const & b);

	FORCEINLINE std::string to_string(::HANDYMATH_NS::Vector2 const & v)
	{
		return "("s + std::to_string(v.X) + ", "s + std::to_string(v.Y) + ")"s;
	}
}

static inline std::ostream & operator<<(std::ostream & os, const ::HANDYMATH_NS::Vector2 & v)  
{  
	os << "(" << v.X << ", " << v.Y << ")";  
	return os;  
}  



namespace HANDYMATH_NS {

	// implicit
	FORCEINLINE Vector2::Vector2(Vector4 const & native) : Native(native) { }
	FORCEINLINE Vector2::operator Vector4() const { return Native; }
	
	// implicit
	FORCEINLINE Vector2::Vector2(CVector2 const & v2c) : Native(v2c.X, v2c.Y, 0.0f, 0.0f) { }
	FORCEINLINE Vector2::operator CVector2() const { return Compact; }

	FORCEINLINE Vector2::Vector2() : Native() { }
	FORCEINLINE Vector2::Vector2(Vector2 const & v) : Native(v.Native) { }

	//explicit
	FORCEINLINE Vector2::Vector2(float xy)             : Native( xy,  xy, 0.0f, 0.0f) { }
	FORCEINLINE Vector2::Vector2(float xIn, float yIn) : Native(xIn, yIn, 0.0f, 0.0f) { }

	//explicit
	FORCEINLINE Vector2::Vector2(float const * ary) : Native(ary[0], ary[1], 0.0f, 0.0f) { }

	/*static*/ FORCEINLINE Vector2 Vector2::  FromArray(float const * ary) { return Vector2(ary); }
	FORCEINLINE void               Vector2::CopyToArray(float       * ary) const { memcpy(ary, &Array[0], sizeof(float) * 2); } // TODO: Would memcpy be faster?

	template<int i0, int i1>                 Vector2 Vector2::Shuffle    () const { return Native.Shuffle<i0, i1,  2, 3>(); }
	template<int i0, int i1, int i2>         Vector3 Vector2::ShuffleXY0 () const { return Native.Shuffle<i0, i1, i2, 3>(); }
	template<int i0, int i1, int i2, int i3> Vector4 Vector2::ShuffleXY00() const { return Native.Shuffle<i0, i1, i2, i3>(); }

	template<int i> FORCEINLINE Vector2 Vector2::Insert(float f) const { return Native.Insert<i>(f); }
	template<int i> FORCEINLINE Vector2 Vector2::InsertZero()    const { return Native.InsertZero<i>(); }

	FORCEINLINE Vector2 Vector2::InsertX(float xIn) const { return Native.InsertX(xIn); }
	FORCEINLINE Vector2 Vector2::InsertY(float yIn) const { return Native.InsertY(yIn); }

	FORCEINLINE Vector4 Vector2::X000() const { return Native.X000(); }
	FORCEINLINE Vector4 Vector2::XY00() const { return Native; }
	FORCEINLINE Vector4 Vector2::XY01() const { return Native.XY01(); }
	FORCEINLINE Vector4 Vector2::XY10() const { return Native.XY10(); }
	FORCEINLINE Vector4 Vector2::XY11() const { return Native.XY11(); }

	FORCEINLINE Vector3 Vector2::X00() const { return Vector3(Native.X000()); }
	FORCEINLINE Vector3 Vector2::XY0() const { return Vector3(Native); }
	FORCEINLINE Vector3 Vector2::XY1() const { return Vector3(Native.XY10()); }

	FORCEINLINE Vector2 Vector2::XX() const { return Native.XXZZ(); }
	FORCEINLINE Vector2 Vector2::YY() const { return Native.YYWW(); }
	FORCEINLINE Vector2 Vector2::YX() const { return Native.Shuffle<1, 0, 3, 2>(); }
	
	FORCEINLINE Vector2 Vector2::Min(Vector2 const & rhs) const { return Native.Min(rhs.Native); }
	FORCEINLINE Vector2 Vector2::Max(Vector2 const & rhs) const { return Native.Max(rhs.Native); }

	FORCEINLINE Vector2 Vector2::Min(float rhs) const { return Min(Vector2(rhs)); }
	FORCEINLINE Vector2 Vector2::Max(float rhs) const { return Max(Vector2(rhs)); }

	FORCEINLINE float   Vector2::Min()     const { return Native.XXYY().Min(); }
	FORCEINLINE float   Vector2::Max()     const { return Native.XXYY().Max(); }
	FORCEINLINE float   Vector2::Sum()     const { return Native.Sum(); }
	FORCEINLINE float   Vector2::Product() const { return Native.XY11().Product(); }
	FORCEINLINE float   Vector2::Mean()    const { return Sum() * 0.5f; }

	FORCEINLINE Vector2 Vector2::Abs()              const { return Native.Abs(); }
	FORCEINLINE Vector2 Vector2::Sign()             const { return Native.Sign().XY00(); }
	FORCEINLINE Vector2 Vector2::Half()             const { return Native.Half(); }
	FORCEINLINE Vector2 Vector2::Reciprocal()       const { return Native.Reciprocal()      .XY00(); } // Erase the Divide By Zero result!
	FORCEINLINE Vector2 Vector2::ReciprocalApprox() const { return Native.ReciprocalApprox().XY00(); } // Erase the Divide By Zero result!
	FORCEINLINE Vector2 Vector2::Sqrt()             const { return Native.Sqrt(); }
	FORCEINLINE Vector2 Vector2::RSqrt()            const { return Native.RSqrt()      .XY00(); } // Erase the Divide By Zero result!
	FORCEINLINE Vector2 Vector2::RSqrtApprox()      const { return Native.RSqrtApprox().XY00(); } // Erase the Divide By Zero result!

	FORCEINLINE Vector2 Vector2::To(Vector2 const & rhs) const { return Native.To(rhs); }

	FORCEINLINE float   Vector2::Distance       (Vector2 const & rhs) const { return Native.Distance(rhs.Native); }
	FORCEINLINE float   Vector2::DistanceSquared(Vector2 const & rhs) const { return Native.DistanceSquared(rhs.Native); }

	FORCEINLINE float   Vector2::Length()        const { return Native.Length(); }
	FORCEINLINE float   Vector2::LengthSquared() const { return Native.LengthSquared(); }

	FORCEINLINE Vector2 Vector2::Normalized() const { return Native.Normalized(); }

	FORCEINLINE float   Vector2::Angle(Vector2 const & rhs) const { return Native.Angle(rhs.Native); }
	FORCEINLINE float   Vector2::Dot  (Vector2 const & rhs) const { return Native.Dot  (rhs.Native); }

	FORCEINLINE Vector2 Vector2::OrthoCW()  const { return YX() * NegUnitY(); } // Vector2(Y, -X);
	FORCEINLINE Vector2 Vector2::OrthoCCW() const { return YX() * NegUnitX(); } // Vector2(Y, -X);

	FORCEINLINE Vector2 Vector2::Power      (float n) const { return Native.Power(n); }
	FORCEINLINE Vector2 Vector2::PowerApprox(float n) const { return Native.PowerApprox(n); }

	FORCEINLINE bool Vector2::IsFacing(Vector2 const & rhs) const { return Native.IsFacing(rhs.Native); }

	FORCEINLINE bool Vector2::HasNaN()              const { return std::isnan(X) || std::isnan(Y); }
	FORCEINLINE bool Vector2::HasPositiveInfinity() const { return Native.HasPositiveInfinity(); }
	FORCEINLINE bool Vector2::HasNegativeInfinity() const { return Native.HasNegativeInfinity(); }

	FORCEINLINE bool Vector2::IsNaN()               const { return std::isnan(X) && std::isnan(Y); }
	FORCEINLINE bool Vector2::IsPositiveInfinity()  const { return Compare<CMP::AllEQ>( std::numeric_limits<float>::infinity()); }
	FORCEINLINE bool Vector2::IsNegativeInfinity()  const { return Compare<CMP::AllEQ>(-std::numeric_limits<float>::infinity()); }

	FORCEINLINE bool Vector2::IsUnbounded()         const { return Native.IsUnbounded(); }

	template <CMP op>
	FORCEINLINE bool Vector2::Compare(float rhs) const
	{
		return Compare<op>(Vector2(rhs));
	}

	template <CMP op>
	FORCEINLINE bool Vector2::Compare(Vector2 const & rhs) const
	{
		/// These ops require no masking, since lhs.zw is already equal to rhs.zw (both zero), and therefore will not affect the result for the XYZ components.
		if (op == CMP::AnyLT)   { return Native.Compare<op>(rhs.Native); }
		if (op == CMP::AnyGT)   { return Native.Compare<op>(rhs.Native); }
		if (op == CMP::AllLEQ)  { return Native.Compare<op>(rhs.Native); }
		if (op == CMP::AllGEQ)  { return Native.Compare<op>(rhs.Native); }
		if (op == CMP::AllEQ)   { return Native.Compare<op>(rhs.Native); }
		if (op == CMP::AnyNEQ)  { return Native.Compare<op>(rhs.Native); }
		if (op == CMP::AllWE)   { return Native.Compare<op>(rhs.Native); }
		if (op == CMP::AnyNWE)  { return Native.Compare<op>(rhs.Native); }
		if (op == CMP::Ordinal) { return Native.Compare<op>(rhs.Native); }

		/// These ops require lhs.zw to be greater than rhs.zw to avoid affecting the result for xyz
		if (op == CMP::AllGT)   { return Native.XY11().Compare<op>(rhs); }
		if (op == CMP::AnyLEQ)  { return Native.XY11().Compare<op>(rhs); }

		/// These ops require lhs.zw to be less than rhs.zw to avoid affecting the result for xyz
		if (op == CMP::AllLT)   { return Native.Compare<op>(rhs.Native.XY11()); }
		if (op == CMP::AnyGEQ)  { return Native.Compare<op>(rhs.Native.XY11()); }

		/// These ops require lhs.zw to not be equal to rhs.zw to avoid affecting the result for xyz
		if (op == CMP::AnyEQ)   { return Native.Compare<op>(rhs.Native.XY11()); }
		if (op == CMP::AllNEQ)  { return Native.Compare<op>(rhs.Native.XY11()); }
		if (op == CMP::AnyWE)   { return Native.Compare<op>(rhs.Native.XY11()); }
		if (op == CMP::AllNWE)  { return Native.Compare<op>(rhs.Native.XY11()); }
	}

	FORCEINLINE Vector2 Vector2::Clamp(Vector2 const & min, Vector2 const & max) const { return Native.Clamp(min.Native, max.Native).InsertZero<3>(); }
	FORCEINLINE Vector2 Vector2::Clamp(float   const   min, float   const   max) const { return Native.Clamp(min,        max)       .InsertZero<3>(); }

	FORCEINLINE Vector2 Vector2::MultiplyAdd   (Vector2 const & m2, Vector2 const &  a) const { return Native.MultiplyAdd(m2.Native, a.Native); } // ((*this) * rhs) + a
	FORCEINLINE Vector2 Vector2::MultiplySub   (Vector2 const & m2, Vector2 const &  a) const { return Native.MultiplySub(m2.Native, a.Native); } // ((*this) * rhs) - a

	FORCEINLINE Vector2 Vector2::Lerp      (Vector2 const & rhs, float           t) const { return Native.Lerp(rhs.Native, t); }
	FORCEINLINE Vector2 Vector2::Lerp      (Vector2 const & rhs, Vector2 const & t) const { return Native.Lerp(rhs.Native, t.Native); }
	FORCEINLINE Vector2 Vector2::SmoothStep(Vector2 const & rhs, float           t) const { return Native.SmoothStep(rhs.Native, t); }
	FORCEINLINE Vector2 Vector2::SmoothStep(Vector2 const & rhs, Vector2 const & t) const { return Native.SmoothStep(rhs.Native, t.Native); }

	FORCEINLINE Vector2 Vector2::Round_Near () const { return Native.Round_Near (); }
	FORCEINLINE Vector2 Vector2::Round_Floor() const { return Native.Round_Floor(); }
	FORCEINLINE Vector2 Vector2::Round_Ceil () const { return Native.Round_Ceil (); }
	FORCEINLINE Vector2 Vector2::Round_Trunc() const { return Native.Round_Trunc(); }

	FORCEINLINE Vector2 Vector2::FMod(Vector2 const & div) const { return Native.FMod(div.Native).XY00(); }
	FORCEINLINE Vector2 Vector2::FMod(float           div) const { return Native.FMod(div)       .XY00(); }

	FORCEINLINE float & Vector2::operator[](int idx)       { return Native[idx]; }
	FORCEINLINE float   Vector2::operator[](int idx) const { return Native[idx]; }

	FORCEINLINE Vector2 Vector2::Sin () { return Vector2(std::sin (X), std::sin (Y)); }
	FORCEINLINE Vector2 Vector2::Cos () { return Vector2(std::cos (X), std::cos (Y)); }
	FORCEINLINE Vector2 Vector2::Tan () { return Vector2(std::tan (X), std::tan (Y)); }

	FORCEINLINE Vector2 Vector2::ASin() { return Vector2(std::asin(X), std::asin(Y)); }
	FORCEINLINE Vector2 Vector2::ACos() { return Vector2(std::acos(X), std::acos(Y)); }
	FORCEINLINE Vector2 Vector2::ATan() { return Vector2(std::atan(X), std::atan(Y)); }

	FORCEINLINE Vector2 Vector2::SinH() { return Vector2(std::sinh(X), std::sinh(Y)); }
	FORCEINLINE Vector2 Vector2::CosH() { return Vector2(std::cosh(X), std::cosh(Y)); }
	FORCEINLINE Vector2 Vector2::TanH() { return Vector2(std::tanh(X), std::tanh(Y)); }

	/*static*/ FORCEINLINE Vector2 Vector2::Zero()     { return Vector4::Zero(); }
	/*static*/ FORCEINLINE Vector2 Vector2::One()      { return Vector2( 1.0f); }
	/*static*/ FORCEINLINE Vector2 Vector2::NegOne()   { return Vector2(-1.0f); }
	/*static*/ FORCEINLINE Vector2 Vector2::UnitX()    { return Vector4::UnitX(); }
	/*static*/ FORCEINLINE Vector2 Vector2::UnitY()    { return Vector4::UnitY(); }
	/*static*/ FORCEINLINE Vector2 Vector2::NegUnitX() { return Vector4::NegUnitX(); }
	/*static*/ FORCEINLINE Vector2 Vector2::NegUnitY() { return Vector4::NegUnitY(); }

	/*static*/ FORCEINLINE Vector2 Vector2::PositiveInfinity() { return Vector2(-std::numeric_limits<float>::infinity()); }
	/*static*/ FORCEINLINE Vector2 Vector2::NegativeInfinity() { return Vector2( std::numeric_limits<float>::infinity()); }
	/*static*/ FORCEINLINE Vector2 Vector2::NaN()              { return Vector2( std::numeric_limits<float>::quiet_NaN()); }

	FORCEINLINE Vector2 operator-(Vector2 const & lhs) { return Vector2::UnsafeCast((-lhs.Native).XY00()); } // Eliminate -0.0 in third/fourth element

	FORCEINLINE Vector2 operator+(Vector2 const & lhs, Vector2 const & rhs) { return Vector2::UnsafeCast((lhs.Native + rhs.Native)); }
	FORCEINLINE Vector2 operator-(Vector2 const & lhs, Vector2 const & rhs) { return Vector2::UnsafeCast((lhs.Native - rhs.Native).XY00()); } // Eliminate -0.0 in third/fourth element
	FORCEINLINE Vector2 operator*(Vector2 const & lhs, Vector2 const & rhs) { return Vector2::UnsafeCast((lhs.Native * rhs.Native)); }
	FORCEINLINE Vector2 operator/(Vector2 const & lhs, Vector2 const & rhs) { return Vector2::UnsafeCast((lhs.Native / rhs.Native.XY11()).XY00()); } // Eliminate Divide By Zero in third/fourth element

	FORCEINLINE Vector2 operator+(Vector2 const & lhs, float rhs) { return lhs + Vector2(rhs); }
	FORCEINLINE Vector2 operator-(Vector2 const & lhs, float rhs) { return lhs - Vector2(rhs); }
	FORCEINLINE Vector2 operator*(Vector2 const & lhs, float rhs) { return lhs * Vector2(rhs); }
	FORCEINLINE Vector2 operator/(Vector2 const & lhs, float rhs) { return lhs / Vector2(rhs); }

	FORCEINLINE Vector2 operator+(float lhs, Vector2 const & rhs) { return Vector2(lhs) + rhs; }
	FORCEINLINE Vector2 operator-(float lhs, Vector2 const & rhs) { return Vector2(lhs) - rhs; }
	FORCEINLINE Vector2 operator*(float lhs, Vector2 const & rhs) { return Vector2(lhs) * rhs; }
	FORCEINLINE Vector2 operator/(float lhs, Vector2 const & rhs) { return Vector2(lhs) / rhs; }

	FORCEINLINE Vector2 & operator+=(Vector2 & lhs, Vector2 const & rhs) { lhs = lhs + rhs; return lhs; }
	FORCEINLINE Vector2 & operator-=(Vector2 & lhs, Vector2 const & rhs) { lhs = lhs - rhs; return lhs; }
	FORCEINLINE Vector2 & operator*=(Vector2 & lhs, Vector2 const & rhs) { lhs = lhs * rhs; return lhs; }
	FORCEINLINE Vector2 & operator/=(Vector2 & lhs, Vector2 const & rhs) { lhs = lhs / rhs; return lhs; }

	FORCEINLINE Vector2 & operator+=(Vector2 & lhs, float rhs) { lhs = lhs + rhs; return lhs; }
	FORCEINLINE Vector2 & operator-=(Vector2 & lhs, float rhs) { lhs = lhs - rhs; return lhs; }
	FORCEINLINE Vector2 & operator*=(Vector2 & lhs, float rhs) { lhs = lhs * rhs; return lhs; }
	FORCEINLINE Vector2 & operator/=(Vector2 & lhs, float rhs) { lhs = lhs / rhs; return lhs; }

	FORCEINLINE bool operator< (Vector2 const & lhs, Vector2 const & rhs) { return lhs.Native  < rhs.Native; }

	FORCEINLINE bool operator==(Vector2 const & lhs, Vector2 const & rhs) { return lhs.Native == rhs.Native; }
	FORCEINLINE bool operator!=(Vector2 const & lhs, Vector2 const & rhs) { return lhs.Native != rhs.Native; }

	FORCEINLINE bool operator==(Vector2 const & lhs, float rhs) { return lhs == Vector2(rhs); }
	FORCEINLINE bool operator!=(Vector2 const & lhs, float rhs) { return lhs != Vector2(rhs); }

	FORCEINLINE bool operator==(float lhs, Vector2 const & rhs) { return Vector2(lhs) == rhs; }
	FORCEINLINE bool operator!=(float lhs, Vector2 const & rhs) { return Vector2(lhs) != rhs; }
}

namespace std {
	FORCEINLINE ::HANDYMATH_NS::Vector2 min(::HANDYMATH_NS::Vector2 const & a, ::HANDYMATH_NS::Vector2 const & b) { return a.Min(b); }
	FORCEINLINE ::HANDYMATH_NS::Vector2 max(::HANDYMATH_NS::Vector2 const & a, ::HANDYMATH_NS::Vector2 const & b) { return a.Max(b); }
}

MAKE_HASHABLE(HANDYMATH_NS::Vector2, t.X, t.Y)