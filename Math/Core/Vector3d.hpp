
/// See ../../License.txt for license info.

#pragma once

#include <ostream>
#include <cmath>
#include <cstdint>
#include <limits>
#include <array>
#include <cstring>  // memcpy

#include "Base.hpp"

#include "Vector4d.hpp"

// TODO, Exp, and Log

namespace HANDYMATH_NS {

	class Vector4d;
	class Vector2d;

	class CVector3d /// Compact version of Vector3, for storage, rather than computation.
	{
	public:
		double X = 0.0_d, Y = 0.0_d, Z = 0.0_d;

		CVector3d() = default;
		CVector3d(double xIn, double yIn, double zIn) : X(xIn), Y(yIn), Z(zIn) { }

		CVector3d & operator=(CVector3d const &) = default;
	};

	class HANDYMATH_ALIGN32 Vector3d 
	{
		friend class Vector4d;
		//friend class Vector3i64;
		friend class Vector2d;

		// IMPLICIT, PRIVATE. 
		// Make these public and suffer my wrath. This cast is unsafe, and doesn't sanitize Native.W to zero!
		Vector3d(Vector4d const & native);
		operator Vector4d() const;

	public:
		static constexpr int32_t NumElements  =  3_i32;
		static constexpr int32_t ElementBytes =  8_i32;
		static constexpr int32_t StrideBytes  = 32_i32;
		static constexpr int32_t UsefulBytes  = NumElements * ElementBytes;

		union
		{
			Vector4d              Native;
			struct { double       X, Y, Z, _zero; };
			std::array<double, 4> Array;
			CVector3d             Compact;
			struct { double       x, y, z; };
		};

		Vector3d(CVector3d const & v3c);
		operator CVector3d() const;

		Vector3d();
		Vector3d(Vector3d const & v);

		explicit 
		Vector3d(double xyz);
		Vector3d(double xIn, double yIn, double zIn);

		explicit 
		Vector3d(double const * ary);

		Vector3d & operator=(Vector3d const &) = default;

		// Conversions:
		Vector2d  XY()           const;
		Vector4d  XYZW(double w) const;
		Vector3   ToFloat() const;
		//Vector3i64 ToInteger()  const;
		// ---

		static Vector3d FromArray  (double const * ary);
		void            CopyToArray(double       * ary) const;

		template<int i0, int i1, int i2>         Vector3d Shuffle()     const;
		template<int i0, int i1, int i2, int i3> Vector4d ShuffleXYZ0() const;

		template<int i> Vector3d Insert(double f) const;
		template<int i> Vector3d InsertZero()    const;

		Vector3d InsertX(double xIn) const;
		Vector3d InsertY(double yIn) const;
		Vector3d InsertZ(double zIn) const;

		// These have all been optimized to be faster than using Shuffle<>()!
		Vector4d X000() const;
		Vector4d XY00() const;
		Vector4d XY01() const;
		Vector4d XY10() const;
		Vector4d XY11() const;
		Vector4d XYZ0() const; // THIS OPERATION IS 100% FREE, IT WILL BE COMPLETELY ERASED WHEN INLINED!
		Vector4d XYZ1() const;

		Vector3d X00() const;
		Vector3d XY0() const;
		Vector3d XY1() const;

		Vector3d YY0() const;
		Vector3d ZZ0() const;
		Vector3d Z0Z() const;

		Vector3d ZC0(Vector3d const & abcd) const;
		Vector3d Z0C(Vector3d const & abcd) const;
		// ---

		Vector3d XXX() const;
		Vector3d YYY() const;
		Vector3d ZZZ() const;

		Vector3d ZYX() const;

		Vector3d Min(Vector3d const & rhs) const;
		Vector3d Max(Vector3d const & rhs) const;

		Vector3d Min(double rhs) const;
		Vector3d Max(double rhs) const;

		double   Min()     const;
		double   Max()     const;
		double   Sum()     const;
		double   Product() const;
		double   Mean()    const;

		Vector3d Abs()              const;
		Vector3d Sign()             const; //Returns, per component, the sign of the input vector: input >= 0.0 ? 1.0 : -1.0
		Vector3d Half()             const;
		Vector3d Reciprocal()       const;
		Vector3d ReciprocalApprox() const;
		Vector3d Sqrt()             const;
		Vector3d RSqrt()            const;
		Vector3d RSqrtApprox()      const;

		Vector3d To(Vector3d const & rhs) const;

		double   Distance          (Vector3d const & rhs) const;
		double   DistanceSquared   (Vector3d const & rhs) const;

		double   Length()           const;
		double   LengthSquared()    const;

		Vector3d Normalized() const;

		double   Angle(Vector3d const & rhs) const;
		double   Dot  (Vector3d const & rhs) const;
		Vector3d Cross(Vector3d const & rhs) const;

		Vector3d Power      (double n) const;
		Vector3d PowerApprox(double n) const;

		bool    IsFacing(Vector3d const & rhs) const;

		bool    HasNaN()              const;
		bool    HasPositiveInfinity() const;
		bool    HasNegativeInfinity() const;

		bool    IsNaN()               const;
		bool    IsPositiveInfinity()  const;
		bool    IsNegativeInfinity()  const;

		bool    IsUnbounded()         const;

		template <CMP op> bool Compare(double           rhs) const;
		template <CMP op> bool Compare(Vector3d const & rhs) const;

		Vector3d Clamp(Vector3d const & min, Vector3d const & max) const;
		Vector3d Clamp(double   const   min, double   const   max) const;

		Vector3d MultiplyAdd   (Vector3d const & m2, Vector3d const &  a) const; // ((*this) * rhs) + a
		Vector3d MultiplySub   (Vector3d const & m2, Vector3d const &  a) const; // ((*this) * rhs) - a

		Vector3d Lerp      (Vector3d const & rhs, double           t) const;
		Vector3d Lerp      (Vector3d const & rhs, Vector3d const & t) const;
		Vector3d SmoothStep(Vector3d const & rhs, double           t) const;
		Vector3d SmoothStep(Vector3d const & rhs, Vector3d const & t) const;

		Vector3d Round_Near () const;
		Vector3d Round_Floor() const;
		Vector3d Round_Ceil () const;
		Vector3d Round_Trunc() const;

		Vector3d FMod(Vector3d const & div) const;
		Vector3d FMod(double           div) const;

		Vector3d RotatedAbout(Vector3d const & axis, double radians) const;

		double & operator[](int idx);
		double   operator[](int idx) const;

		Vector3d Sin();
		Vector3d Cos();
		Vector3d Tan();

		Vector3d ASin();
		Vector3d ACos();
		Vector3d ATan();

		Vector3d SinH();
		Vector3d CosH();
		Vector3d TanH();

		static Vector3d Zero();
		static Vector3d One();
		static Vector3d NegOne();
		static Vector3d UnitX();
		static Vector3d UnitY();
		static Vector3d UnitZ();
		static Vector3d NegUnitX();
		static Vector3d NegUnitY();
		static Vector3d NegUnitZ();
		static Vector3d PositiveInfinity();
		static Vector3d NegativeInfinity();
		static Vector3d NaN();

		static Vector3d UnsafeCast(Vector4d const & v); // Certain operators need to cast from Vector4d to Vector3d without any checking. File this under "Stupid Hacks".

		template <class SerialOp> void serial(SerialOp & ser)
		{
			ser
				(X)
				(Y)
				(Z);
		}
	};

	/*static*/ FORCEINLINE Vector3d Vector3d::UnsafeCast(Vector4d const & v) { return Vector3d(v); }

	Vector3d operator+(Vector3d const & lhs, double rhs);
	Vector3d operator-(Vector3d const & lhs, double rhs);
	Vector3d operator*(Vector3d const & lhs, double rhs);
	Vector3d operator/(Vector3d const & lhs, double rhs);

	Vector3d operator+(double lhs, Vector3d const & rhs);
	Vector3d operator-(double lhs, Vector3d const & rhs);
	Vector3d operator*(double lhs, Vector3d const & rhs);
	Vector3d operator/(double lhs, Vector3d const & rhs);
	
	Vector3d & operator+=(Vector3d & lhs, Vector3d const & rhs);
	Vector3d & operator-=(Vector3d & lhs, Vector3d const & rhs);
	Vector3d & operator*=(Vector3d & lhs, Vector3d const & rhs);
	Vector3d & operator/=(Vector3d & lhs, Vector3d const & rhs);
	
	Vector3d & operator+=(Vector3d & lhs, double rhs);
	Vector3d & operator-=(Vector3d & lhs, double rhs);
	Vector3d & operator*=(Vector3d & lhs, double rhs);
	Vector3d & operator/=(Vector3d & lhs, double rhs);

	bool operator< (Vector3d const & lhs, Vector3d const & rhs);

	bool operator==(Vector3d const & lhs, Vector3d const & rhs);
	bool operator!=(Vector3d const & lhs, Vector3d const & rhs);

	bool operator==(Vector3d const & lhs, double rhs);
	bool operator!=(Vector3d const & lhs, double rhs);

	bool operator==(double lhs, Vector3d const & rhs);
	bool operator!=(double lhs, Vector3d const & rhs);
}

// Not supposed to add overloads to std technically... but I want my templates that use std::to_string, min, max, etc to work, so oh well then.
namespace std {
	FORCEINLINE ::HANDYMATH_NS::Vector3d min(::HANDYMATH_NS::Vector3d const & a, ::HANDYMATH_NS::Vector3d const & b);
	FORCEINLINE ::HANDYMATH_NS::Vector3d max(::HANDYMATH_NS::Vector3d const & a, ::HANDYMATH_NS::Vector3d const & b);

	FORCEINLINE std::string to_string(::HANDYMATH_NS::Vector3d const & v)
	{
		return "("s + std::to_string(v.X) + ", "s + std::to_string(v.Y) + ", "s + std::to_string(v.Z) + ")"s;
	}
}

static inline std::ostream & operator<<(std::ostream & os, const ::HANDYMATH_NS::Vector3d & v)  
{  
	os << "(" << v.X << ", " << v.Y << ", " << v.Z << ")";  
	return os;  
}  

namespace HANDYMATH_NS {

	// implicit
	FORCEINLINE Vector3d::Vector3d(Vector4d const & native) : Native(native) { }
	FORCEINLINE Vector3d::operator Vector4d() const { return Native; }
	
	// implicit
	FORCEINLINE Vector3d::Vector3d(CVector3d const & v3c) : Native(v3c.X, v3c.Y, v3c.Z, 0.0_d) { }
	FORCEINLINE Vector3d::operator CVector3d() const { return Compact; }

	FORCEINLINE Vector3d::Vector3d() : Native() { }
	FORCEINLINE Vector3d::Vector3d(Vector3d const & v) : Native(v.Native) { }

	//explicit
	FORCEINLINE Vector3d::Vector3d(double xyz)                         : Native(xyz, xyz, xyz, 0.0_d) { }
	FORCEINLINE Vector3d::Vector3d(double xIn, double yIn, double zIn) : Native(xIn, yIn, zIn, 0.0_d) { }

	//explicit
	FORCEINLINE Vector3d::Vector3d(double const * ary)           : Native(ary[0], ary[1], ary[2], 0.0_d) { }

	/*static*/ FORCEINLINE Vector3d Vector3d::  FromArray(double const * ary) { return Vector3d(ary); }
	FORCEINLINE void                Vector3d::CopyToArray(double       * ary) const { memcpy(ary, &Array[0], sizeof(double) * 3); } // TODO: Would memcpy be faster?

	template<int i0, int i1, int i2>         
	FORCEINLINE Vector3d Vector3d::Shuffle()     const { return Native.Shuffle<i0, i1, i2,  3>(); }
	
	template<int i0, int i1, int i2, int i3> 
	FORCEINLINE Vector4d Vector3d::ShuffleXYZ0() const { return Native.Shuffle<i0, i1, i2, i3>(); }

	template<int i> FORCEINLINE Vector3d Vector3d::Insert(double f) const { return Native.Insert<i>(f); }
	template<int i> FORCEINLINE Vector3d Vector3d::InsertZero()     const { return Native.InsertZero<i>(); }

	FORCEINLINE Vector3d Vector3d::InsertX(double xIn) const { return Native.InsertX(xIn); }
	FORCEINLINE Vector3d Vector3d::InsertY(double yIn) const { return Native.InsertY(yIn); }
	FORCEINLINE Vector3d Vector3d::InsertZ(double zIn) const { return Native.InsertZ(zIn); }

	FORCEINLINE Vector4d Vector3d::X000() const { return Native.X000(); }
	FORCEINLINE Vector4d Vector3d::XY00() const { return Native.XY00(); }
	FORCEINLINE Vector4d Vector3d::XY01() const { return Native.XY01(); }
	FORCEINLINE Vector4d Vector3d::XY10() const { return Native.XY10(); }
	FORCEINLINE Vector4d Vector3d::XY11() const { return Native.XY11(); }
	FORCEINLINE Vector4d Vector3d::XYZ0() const { return Native;        }
	FORCEINLINE Vector4d Vector3d::XYZ1() const { return Native.XYZ1(); }

	FORCEINLINE Vector3d Vector3d::X00() const { return Native.X000(); }
	FORCEINLINE Vector3d Vector3d::XY0() const { return Native.XY00(); }
	FORCEINLINE Vector3d Vector3d::XY1() const { return Native.InsertZ(1.0_d); }
	
	FORCEINLINE Vector3d Vector3d::YY0() const { return Native.YYWW(); }

	FORCEINLINE Vector3d Vector3d::ZZ0() const { return Native.ZZWW(); }
	FORCEINLINE Vector3d Vector3d::Z0Z() const { return Native.ZWZW(); }

	FORCEINLINE Vector3d Vector3d::ZC0(Vector3d const & abc) const { return Native.ZCWD(abc); }
	FORCEINLINE Vector3d Vector3d::Z0C(Vector3d const & abc) const { return Native.ZWCD(abc); }

	FORCEINLINE Vector3d Vector3d::XXX() const { return Native.Shuffle<0, 0, 0, 3>(); }
	FORCEINLINE Vector3d Vector3d::YYY() const { return Native.Shuffle<1, 1, 1, 3>(); }
	FORCEINLINE Vector3d Vector3d::ZZZ() const { return Native.Shuffle<2, 2, 2, 3>(); }
	
	FORCEINLINE Vector3d Vector3d::ZYX() const { return Native.Shuffle<2, 1, 0, 3>(); }//.ZYXW(); }

	FORCEINLINE Vector3d Vector3d::Min(Vector3d const & rhs) const { return Native.Min(rhs.Native); }
	FORCEINLINE Vector3d Vector3d::Max(Vector3d const & rhs) const { return Native.Max(rhs.Native); }

	FORCEINLINE Vector3d Vector3d::Min(double rhs) const { return Min(Vector3d(rhs)); }
	FORCEINLINE Vector3d Vector3d::Max(double rhs) const { return Max(Vector3d(rhs)); }

	FORCEINLINE double   Vector3d::Min()     const { return Native.Shuffle<0, 1, 2, 2>().Min(); }
	FORCEINLINE double   Vector3d::Max()     const { return Native.Shuffle<0, 1, 2, 2>().Max(); }
	FORCEINLINE double   Vector3d::Sum()     const { return Native.Sum(); }
	FORCEINLINE double   Vector3d::Product() const { return Native.XYZ1().Product(); }
	FORCEINLINE double   Vector3d::Mean()    const { return Sum() * 0.333333333333333333333_d; }

	FORCEINLINE Vector3d Vector3d::Abs()              const { return Native.Abs(); }
	FORCEINLINE Vector3d Vector3d::Sign()             const { return Native.Sign().XY00(); }
	FORCEINLINE Vector3d Vector3d::Half()             const { return Native.Half(); }
	FORCEINLINE Vector3d Vector3d::Reciprocal()       const { return Native.Reciprocal()      .XYZ0(); } // Erase the Divide By Zero result!
	FORCEINLINE Vector3d Vector3d::ReciprocalApprox() const { return Native.ReciprocalApprox().XYZ0(); } // Erase the Divide By Zero result!
	FORCEINLINE Vector3d Vector3d::Sqrt()             const { return Native.Sqrt(); }
	FORCEINLINE Vector3d Vector3d::RSqrt()            const { return Native.RSqrt()      .XYZ0(); } // Erase the Divide By Zero result!
	FORCEINLINE Vector3d Vector3d::RSqrtApprox()      const { return Native.RSqrtApprox().XYZ0(); } // Erase the Divide By Zero result!

	FORCEINLINE Vector3d Vector3d::To(Vector3d const & rhs) const { return Native.To(rhs); }

	FORCEINLINE double   Vector3d::Distance       (Vector3d const & rhs) const { return Native.Distance(rhs.Native); }
	FORCEINLINE double   Vector3d::DistanceSquared(Vector3d const & rhs) const { return Native.DistanceSquared(rhs.Native); }

	FORCEINLINE double   Vector3d::Length()        const { return Native.Length(); }
	FORCEINLINE double   Vector3d::LengthSquared() const { return Native.LengthSquared(); }

	FORCEINLINE Vector3d Vector3d::Normalized() const { return Native.Normalized(); }

	FORCEINLINE double   Vector3d::Angle(Vector3d const & rhs) const { return Native.Angle(rhs.Native); }
	FORCEINLINE double   Vector3d::Dot  (Vector3d const & rhs) const { return Native.Dot  (rhs.Native); }
	FORCEINLINE Vector3d Vector3d::Cross(Vector3d const & rhs) const
	{
		return Vector3d(
			(Y * rhs.Z) - (Z * rhs.Y), 
			(Z * rhs.X) - (X * rhs.Z),
			(X * rhs.Y) - (Y * rhs.X));
	}

	FORCEINLINE Vector3d Vector3d::Power      (double n) const { return Native.Power(n); }
	FORCEINLINE Vector3d Vector3d::PowerApprox(double n) const { return Native.PowerApprox(n); }

	FORCEINLINE bool Vector3d::IsFacing(Vector3d const & rhs) const { return Native.IsFacing(rhs.Native); }

	FORCEINLINE bool Vector3d::HasNaN()              const { return std::isnan(X) || std::isnan(Y) || std::isnan(Z); }
	FORCEINLINE bool Vector3d::HasPositiveInfinity() const { return Native.HasPositiveInfinity(); }
	FORCEINLINE bool Vector3d::HasNegativeInfinity() const { return Native.HasNegativeInfinity(); }

	FORCEINLINE bool Vector3d::IsNaN()               const { return std::isnan(X) && std::isnan(Y) && std::isnan(Z); }
	FORCEINLINE bool Vector3d::IsPositiveInfinity()  const { return Compare<CMP::AllEQ>( std::numeric_limits<double>::infinity()); }
	FORCEINLINE bool Vector3d::IsNegativeInfinity()  const { return Compare<CMP::AllEQ>(-std::numeric_limits<double>::infinity()); }

	FORCEINLINE bool Vector3d::IsUnbounded()         const { return Native.IsUnbounded(); }
	
	template <CMP op>
	FORCEINLINE bool Vector3d::Compare(double rhs) const
	{
		return Compare<op>(Vector3d(rhs));
	}

	template <CMP op>
	FORCEINLINE bool Vector3d::Compare(Vector3d const & rhs) const
	{
		/// These ops require no masking, since lhs.w is already equal to rhs.w (both zero), and therefore will not affect the result for the XYZ components.
		if (op == CMP::AnyLT)   { return Native.Compare<op>(rhs.Native); }
		if (op == CMP::AnyGT)   { return Native.Compare<op>(rhs.Native); }
		if (op == CMP::AllLEQ)  { return Native.Compare<op>(rhs.Native); }
		if (op == CMP::AllGEQ)  { return Native.Compare<op>(rhs.Native); }
		if (op == CMP::AllEQ)   { return Native.Compare<op>(rhs.Native); }
		if (op == CMP::AnyNEQ)  { return Native.Compare<op>(rhs.Native); }
		if (op == CMP::AllWE)   { return Native.Compare<op>(rhs.Native); }
		if (op == CMP::AnyNWE)  { return Native.Compare<op>(rhs.Native); }
		if (op == CMP::Ordinal) { return Native.Compare<op>(rhs.Native); }

		/// These ops require lhs.w to be greater than rhs.w to avoid affecting the result for xyz
		if (op == CMP::AllGT)   { return Native.XYZ1().Compare<op>(rhs); }
		if (op == CMP::AnyLEQ)  { return Native.XYZ1().Compare<op>(rhs); }

		/// These ops require lhs.w to be less than rhs.w to avoid affecting the result for xyz
		if (op == CMP::AllLT)   { return Native.Compare<op>(rhs.Native.XYZ1()); }
		if (op == CMP::AnyGEQ)  { return Native.Compare<op>(rhs.Native.XYZ1()); }

		/// These ops require lhs.w to not be equal to rhs.w to avoid affecting the result for xyz
		if (op == CMP::AnyEQ)   { return Native.Compare<op>(rhs.Native.XYZ1()); }
		if (op == CMP::AllNEQ)  { return Native.Compare<op>(rhs.Native.XYZ1()); }
		if (op == CMP::AnyWE)   { return Native.Compare<op>(rhs.Native.XYZ1()); }
		if (op == CMP::AllNWE)  { return Native.Compare<op>(rhs.Native.XYZ1()); }
	}
	
	FORCEINLINE Vector3d Vector3d::Clamp(Vector3d const & min, Vector3d const & max) const { return Native.Clamp(min.Native, max.Native).InsertZero<3>(); }
	FORCEINLINE Vector3d Vector3d::Clamp(double   const   min, double   const   max) const { return Native.Clamp(min,        max)       .InsertZero<3>(); }

	FORCEINLINE Vector3d Vector3d::MultiplyAdd   (Vector3d const & m2, Vector3d const &  a) const { return Native.MultiplyAdd(m2.Native, a.Native); } // ((*this) * rhs) + a
	FORCEINLINE Vector3d Vector3d::MultiplySub   (Vector3d const & m2, Vector3d const &  a) const { return Native.MultiplySub(m2.Native, a.Native); } // ((*this) * rhs) - a

	FORCEINLINE Vector3d Vector3d::Lerp      (Vector3d const & rhs, double           t) const { return Native.Lerp(rhs.Native, t); }
	FORCEINLINE Vector3d Vector3d::Lerp      (Vector3d const & rhs, Vector3d const & t) const { return Native.Lerp(rhs.Native, t.Native); }
	FORCEINLINE Vector3d Vector3d::SmoothStep(Vector3d const & rhs, double           t) const { return Native.SmoothStep(rhs.Native, t); }
	FORCEINLINE Vector3d Vector3d::SmoothStep(Vector3d const & rhs, Vector3d const & t) const { return Native.SmoothStep(rhs.Native, t.Native); }

	FORCEINLINE Vector3d Vector3d::Round_Near () const { return Native.Round_Near (); }
	FORCEINLINE Vector3d Vector3d::Round_Floor() const { return Native.Round_Floor(); }
	FORCEINLINE Vector3d Vector3d::Round_Ceil () const { return Native.Round_Ceil (); }
	FORCEINLINE Vector3d Vector3d::Round_Trunc() const { return Native.Round_Trunc(); }

	FORCEINLINE Vector3d Vector3d::FMod(Vector3d const & div) const { return Native.FMod(div.Native).InsertZero<3>(); }
	FORCEINLINE Vector3d Vector3d::FMod(double           div) const { return Native.FMod(div)       .InsertZero<3>(); }

	FORCEINLINE Vector3d Vector3d::RotatedAbout(Vector3d const & axis, double radians) const
	{
		Vector3d parallel = axis * this->Dot(axis);
		Vector3d perp     = (*this) - parallel;

		double cosTheta = std::cos(radians);
		double sinTheta = std::sin(radians);

		return perp * cosTheta + axis.Cross(*this) * sinTheta + parallel;
	}

	FORCEINLINE double & Vector3d::operator[](int idx)       { return Native[idx]; }
	FORCEINLINE double   Vector3d::operator[](int idx) const { return Native[idx]; }

	FORCEINLINE Vector3d Vector3d::Sin () { return Vector3d(std::sin (X), std::sin (Y), std::sin (Z)); }
	FORCEINLINE Vector3d Vector3d::Cos () { return Vector3d(std::cos (X), std::cos (Y), std::cos (Z)); }
	FORCEINLINE Vector3d Vector3d::Tan () { return Vector3d(std::tan (X), std::tan (Y), std::tan (Z)); }

	FORCEINLINE Vector3d Vector3d::ASin() { return Vector3d(std::asin(X), std::asin(Y), std::asin(Z)); }
	FORCEINLINE Vector3d Vector3d::ACos() { return Vector3d(std::acos(X), std::acos(Y), std::acos(Z)); }
	FORCEINLINE Vector3d Vector3d::ATan() { return Vector3d(std::atan(X), std::atan(Y), std::atan(Z)); }

	FORCEINLINE Vector3d Vector3d::SinH() { return Vector3d(std::sinh(X), std::sinh(Y), std::sinh(Z)); }
	FORCEINLINE Vector3d Vector3d::CosH() { return Vector3d(std::cosh(X), std::cosh(Y), std::cosh(Z)); }
	FORCEINLINE Vector3d Vector3d::TanH() { return Vector3d(std::tanh(X), std::tanh(Y), std::tanh(Z)); }

	/*static*/ FORCEINLINE Vector3d Vector3d::Zero()     { return Vector4d::Zero(); }
	/*static*/ FORCEINLINE Vector3d Vector3d::One()      { return Vector3d( 1.0_d); }
	/*static*/ FORCEINLINE Vector3d Vector3d::NegOne()   { return Vector3d(-1.0_d); }
	/*static*/ FORCEINLINE Vector3d Vector3d::UnitX()    { return Vector4d::UnitX(); }
	/*static*/ FORCEINLINE Vector3d Vector3d::UnitY()    { return Vector4d::UnitY(); }
	/*static*/ FORCEINLINE Vector3d Vector3d::UnitZ()    { return Vector4d::UnitZ(); }
	/*static*/ FORCEINLINE Vector3d Vector3d::NegUnitX() { return Vector4d::NegUnitX(); }
	/*static*/ FORCEINLINE Vector3d Vector3d::NegUnitY() { return Vector4d::NegUnitY(); }
	/*static*/ FORCEINLINE Vector3d Vector3d::NegUnitZ() { return Vector4d::NegUnitZ(); }

	/*static*/ FORCEINLINE Vector3d Vector3d::PositiveInfinity() { return Vector3d(-std::numeric_limits<double>::infinity()); }
	/*static*/ FORCEINLINE Vector3d Vector3d::NegativeInfinity() { return Vector3d( std::numeric_limits<double>::infinity()); }
	/*static*/ FORCEINLINE Vector3d Vector3d::NaN()              { return Vector3d( std::numeric_limits<double>::quiet_NaN()); }

	FORCEINLINE Vector3d operator-(Vector3d const & lhs) { return Vector3d::UnsafeCast((-lhs.Native).InsertZero<3>()); } // Eliminate -0.0 in fourth element

	FORCEINLINE Vector3d operator+(Vector3d const & lhs, Vector3d const & rhs) { return Vector3d::UnsafeCast((lhs.Native + rhs.Native)); }
	FORCEINLINE Vector3d operator-(Vector3d const & lhs, Vector3d const & rhs) { return Vector3d::UnsafeCast((lhs.Native - rhs.Native).InsertZero<3>()); } // Eliminate -0.0 in fourth element
	FORCEINLINE Vector3d operator*(Vector3d const & lhs, Vector3d const & rhs) { return Vector3d::UnsafeCast((lhs.Native * rhs.Native)); }
	FORCEINLINE Vector3d operator/(Vector3d const & lhs, Vector3d const & rhs) { return Vector3d::UnsafeCast((lhs.Native / rhs.Native.XYZ1()).InsertZero<3>()); } // Eliminate Divide By Zero in fourth element

	FORCEINLINE Vector3d operator+(Vector3d const & lhs, double rhs) { return lhs + Vector3d(rhs); }
	FORCEINLINE Vector3d operator-(Vector3d const & lhs, double rhs) { return lhs - Vector3d(rhs); }
	FORCEINLINE Vector3d operator*(Vector3d const & lhs, double rhs) { return lhs * Vector3d(rhs); }
	FORCEINLINE Vector3d operator/(Vector3d const & lhs, double rhs) { return lhs / Vector3d(rhs); }

	FORCEINLINE Vector3d operator+(double lhs, Vector3d const & rhs) { return Vector3d(lhs) + rhs; }
	FORCEINLINE Vector3d operator-(double lhs, Vector3d const & rhs) { return Vector3d(lhs) - rhs; }
	FORCEINLINE Vector3d operator*(double lhs, Vector3d const & rhs) { return Vector3d(lhs) * rhs; }
	FORCEINLINE Vector3d operator/(double lhs, Vector3d const & rhs) { return Vector3d(lhs) / rhs; }

	FORCEINLINE Vector3d & operator+=(Vector3d & lhs, Vector3d const & rhs) { lhs = lhs + rhs; return lhs; }
	FORCEINLINE Vector3d & operator-=(Vector3d & lhs, Vector3d const & rhs) { lhs = lhs - rhs; return lhs; }
	FORCEINLINE Vector3d & operator*=(Vector3d & lhs, Vector3d const & rhs) { lhs = lhs * rhs; return lhs; }
	FORCEINLINE Vector3d & operator/=(Vector3d & lhs, Vector3d const & rhs) { lhs = lhs / rhs; return lhs; }

	FORCEINLINE Vector3d & operator+=(Vector3d & lhs, double rhs) { lhs = lhs + rhs; return lhs; }
	FORCEINLINE Vector3d & operator-=(Vector3d & lhs, double rhs) { lhs = lhs - rhs; return lhs; }
	FORCEINLINE Vector3d & operator*=(Vector3d & lhs, double rhs) { lhs = lhs * rhs; return lhs; }
	FORCEINLINE Vector3d & operator/=(Vector3d & lhs, double rhs) { lhs = lhs / rhs; return lhs; }

	FORCEINLINE bool operator<  (Vector3d const & lhs, Vector3d const & rhs) { return lhs.Native  < rhs.Native; }

	FORCEINLINE bool operator==(Vector3d const & lhs, Vector3d const & rhs) { return lhs.Native == rhs.Native; }
	FORCEINLINE bool operator!=(Vector3d const & lhs, Vector3d const & rhs) { return lhs.Native != rhs.Native; }

	FORCEINLINE bool operator==(Vector3d const & lhs, double rhs) { return lhs == Vector3d(rhs); }
	FORCEINLINE bool operator!=(Vector3d const & lhs, double rhs) { return lhs != Vector3d(rhs); }

	FORCEINLINE bool operator==(double lhs, Vector3d const & rhs) { return Vector3d(lhs) == rhs; }
	FORCEINLINE bool operator!=(double lhs, Vector3d const & rhs) { return Vector3d(lhs) != rhs; }
}

namespace std {
	FORCEINLINE ::HANDYMATH_NS::Vector3d min(::HANDYMATH_NS::Vector3d const & a, ::HANDYMATH_NS::Vector3d const & b) { return a.Min(b); }
	FORCEINLINE ::HANDYMATH_NS::Vector3d max(::HANDYMATH_NS::Vector3d const & a, ::HANDYMATH_NS::Vector3d const & b) { return a.Max(b); }
}

MAKE_HASHABLE(HANDYMATH_NS::Vector3d, t.X, t.Y, t.Z)
