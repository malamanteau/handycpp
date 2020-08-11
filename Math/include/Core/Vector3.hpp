
/// See ../../License.txt for license info.

#pragma once

#include <ostream>
#include <cmath>
#include <cstdint>
#include <limits>
#include <array>
#include <cstring>  // memcpy

#include "Base.hpp"

#include "Vector4.hpp"

// TODO, Exp, and Log

namespace HANDYMATH_NS {

	class Vector4;
	class Vector2;

	class CVector3 /// Compact version of Vector3, for storage, rather than computation.
	{
	public:
		float X = 0.0f, Y = 0.0f, Z = 0.0f;

		CVector3() = default;
		CVector3(float xIn, float yIn, float zIn) : X(xIn), Y(yIn), Z(zIn) { }

		CVector3 & operator=(CVector3 const &) = default;
	};

	class HANDYMATH_ALIGN16 Vector3 
	{
		friend class Vector4;
		friend class Vector3i;
		friend class Vector3d;
		friend class Vector2;

		// IMPLICIT, PRIVATE. 
		// Make these public and suffer my wrath. This cast is unsafe, and doesn't sanitize Native.W to zero!
		Vector3(Vector4 const & native);
		operator Vector4() const;

	public:
		static constexpr int32_t NumElements  =  3_i32;
		static constexpr int32_t ElementBytes =  4_i32;
		static constexpr int32_t StrideBytes  = 16_i32;
		static constexpr int32_t UsefulBytes  = NumElements * ElementBytes;

		union
		{
			Vector4              Native;
			struct { float       X, Y, Z, _zero; };
			std::array<float, 4> Array;
			CVector3             Compact;
			struct { float       x, y, z; };
		};


		Vector3(CVector3 const & v3c);
		operator CVector3() const;

		Vector3();
		Vector3(Vector3 const & v);

		explicit 
		Vector3(float xyz);
		Vector3(float xIn, float yIn, float zIn);

		explicit 
		Vector3(float const * ary);

		Vector3 & operator=(Vector3 const &) = default;

		// Conversions:
		Vector2  XY()          const;
		Vector4  XYZW(float w) const;
		Vector3i ToInteger()  const;
		// ---

		static Vector3 FromArray  (float const * ary);
		void           CopyToArray(float       * ary) const;

		template<int i0, int i1, int i2>         Vector3 Shuffle()     const;
		template<int i0, int i1, int i2, int i3> Vector4 ShuffleXYZ0() const;

		template<int i> Vector3 Insert(float f) const;
		template<int i> Vector3 InsertZero()    const;

		Vector3 InsertX(float xIn) const;
		Vector3 InsertY(float yIn) const;
		Vector3 InsertZ(float zIn) const;

		// These have all been optimized to be faster than using Shuffle<>()!
		Vector4 X000() const;
		Vector4 XY00() const;
		Vector4 XY01() const;
		Vector4 XY10() const;
		Vector4 XY11() const;
		Vector4 XYZ0() const; // THIS OPERATION IS 100% FREE, IT WILL BE COMPLETELY ERASED WHEN INLINED!
		Vector4 XYZ1() const;

		Vector3 X00() const;
		Vector3 XY0() const;
		Vector3 XY1() const;

		Vector3 YY0() const;
		Vector3 ZZ0() const;
		Vector3 Z0Z() const;

		Vector3 ZC0(Vector3 const & abcd) const;
		Vector3 Z0C(Vector3 const & abcd) const;
		// ---

		Vector3 XXX() const;
		Vector3 YYY() const;
		Vector3 ZZZ() const;

		Vector3 ZYX() const;

		Vector3 Min(Vector3 const & rhs) const;
		Vector3 Max(Vector3 const & rhs) const;

		Vector3 Min(float rhs) const;
		Vector3 Max(float rhs) const;

		float   Min()     const;
		float   Max()     const;
		float   Sum()     const;
		float   Product() const;
		float   Mean()    const;

		Vector3 Abs()              const;
		Vector3 Sign()             const; //Returns, per component, the sign of the input vector: input >= 0.0 ? 1.0 : -1.0
		Vector3 Half()             const;
		Vector3 Reciprocal()       const;
		Vector3 ReciprocalApprox() const;
		Vector3 Sqrt()             const;
		Vector3 RSqrt()            const;
		Vector3 RSqrtApprox()      const;

		Vector3 To(Vector3 const & rhs) const;

		float   Distance          (Vector3 const & rhs) const;
		float   DistanceSquared   (Vector3 const & rhs) const;

		float   Length()           const;
		float   LengthSquared()    const;

		Vector3 Normalized() const;

		float   Angle(Vector3 const & rhs) const;
		float   Dot  (Vector3 const & rhs) const;
		Vector3 Cross(Vector3 const & rhs) const;

		Vector3 Power      (float n) const;
		Vector3 PowerApprox(float n) const;

		bool    IsFacing(Vector3 const & rhs) const;

		bool    HasNaN()              const;
		bool    HasPositiveInfinity() const;
		bool    HasNegativeInfinity() const;

		bool    IsNaN()               const;
		bool    IsPositiveInfinity()  const;
		bool    IsNegativeInfinity()  const;

		bool    IsUnbounded()         const;

		template <CMP op> bool Compare(float           rhs) const;
		template <CMP op> bool Compare(Vector3 const & rhs) const;

		Vector3 Clamp(Vector3 const & min, Vector3 const & max) const;
		Vector3 Clamp(float   const   min, float   const   max) const;

		Vector3 MultiplyAdd   (Vector3 const & m2, Vector3 const &  a) const; // ((*this) * rhs) + a
		Vector3 MultiplySub   (Vector3 const & m2, Vector3 const &  a) const; // ((*this) * rhs) - a

		Vector3 Lerp      (Vector3 const & rhs, float           t) const;
		Vector3 Lerp      (Vector3 const & rhs, Vector3 const & t) const;
		Vector3 SmoothStep(Vector3 const & rhs, float           t) const;
		Vector3 SmoothStep(Vector3 const & rhs, Vector3 const & t) const;

		Vector3 Round_Near () const;
		Vector3 Round_Floor() const;
		Vector3 Round_Ceil () const;
		Vector3 Round_Trunc() const;

		Vector3 FMod(Vector3 const & div) const;
		Vector3 FMod(float           div) const;

		Vector3 RotatedAbout(Vector3 const & axis, float radians) const;

		float & operator[](int idx);
		float   operator[](int idx) const;

		Vector3 Sin();
		Vector3 Cos();
		Vector3 Tan();

		Vector3 ASin();
		Vector3 ACos();
		Vector3 ATan();

		Vector3 SinH();
		Vector3 CosH();
		Vector3 TanH();

		static Vector3 Zero();
		static Vector3 One();
		static Vector3 NegOne();
		static Vector3 UnitX();
		static Vector3 UnitY();
		static Vector3 UnitZ();
		static Vector3 NegUnitX();
		static Vector3 NegUnitY();
		static Vector3 NegUnitZ();
		static Vector3 PositiveInfinity();
		static Vector3 NegativeInfinity();
		static Vector3 NaN();

		static Vector3 UnsafeCast(Vector4 const & v); // Certain operators need to cast from Vector4 to Vector3 without any checking. File this under "Stupid Hacks".
	};

	/*static*/ FORCEINLINE Vector3 Vector3::UnsafeCast(Vector4 const & v) { return Vector3(v); }

	Vector3 operator+(Vector3 const & lhs, float rhs);
	Vector3 operator-(Vector3 const & lhs, float rhs);
	Vector3 operator*(Vector3 const & lhs, float rhs);
	Vector3 operator/(Vector3 const & lhs, float rhs);

	Vector3 operator+(float lhs, Vector3 const & rhs);
	Vector3 operator-(float lhs, Vector3 const & rhs);
	Vector3 operator*(float lhs, Vector3 const & rhs);
	Vector3 operator/(float lhs, Vector3 const & rhs);
	
	Vector3 & operator+=(Vector3 & lhs, Vector3 const & rhs);
	Vector3 & operator-=(Vector3 & lhs, Vector3 const & rhs);
	Vector3 & operator*=(Vector3 & lhs, Vector3 const & rhs);
	Vector3 & operator/=(Vector3 & lhs, Vector3 const & rhs);
	
	Vector3 & operator+=(Vector3 & lhs, float rhs);
	Vector3 & operator-=(Vector3 & lhs, float rhs);
	Vector3 & operator*=(Vector3 & lhs, float rhs);
	Vector3 & operator/=(Vector3 & lhs, float rhs);

	bool operator< (Vector3 const & lhs, Vector3 const & rhs);

	bool operator==(Vector3 const & lhs, Vector3 const & rhs);
	bool operator!=(Vector3 const & lhs, Vector3 const & rhs);

	bool operator==(Vector3 const & lhs, float rhs);
	bool operator!=(Vector3 const & lhs, float rhs);

	bool operator==(float lhs, Vector3 const & rhs);
	bool operator!=(float lhs, Vector3 const & rhs);

	//---


}

namespace std {
	FORCEINLINE ::HANDYMATH_NS::Vector3 min(::HANDYMATH_NS::Vector3 const & a, ::HANDYMATH_NS::Vector3 const & b);
	FORCEINLINE ::HANDYMATH_NS::Vector3 max(::HANDYMATH_NS::Vector3 const & a, ::HANDYMATH_NS::Vector3 const & b);

	FORCEINLINE std::string to_string(::HANDYMATH_NS::Vector3 const & v)
	{
		return "("s + std::to_string(v.X) + ", "s + std::to_string(v.Y) + ", "s + std::to_string(v.Z) + ")"s;
	}
}

static inline std::ostream & operator<<(std::ostream & os, const ::HANDYMATH_NS::Vector3 & v)  
{  
	os << "(" << v.X << ", " << v.Y << ", " << v.Z << ")";  
	return os;  
}  

namespace HANDYMATH_NS {

	// implicit
	FORCEINLINE Vector3::Vector3(Vector4 const & native) : Native(native) { }
	FORCEINLINE Vector3::operator Vector4() const { return Native; }
	
	// implicit
	FORCEINLINE Vector3::Vector3(CVector3 const & v3c) : Native(v3c.X, v3c.Y, v3c.Z, 0.0f) { }
	FORCEINLINE Vector3::operator CVector3() const { return Compact; }

	FORCEINLINE Vector3::Vector3() : Native() { }
	FORCEINLINE Vector3::Vector3(Vector3 const & v) : Native(v.Native) { }

	//explicit
	FORCEINLINE Vector3::Vector3(float xyz)                       : Native(xyz, xyz, xyz, 0.0f) { }
	FORCEINLINE Vector3::Vector3(float xIn, float yIn, float zIn) : Native(xIn, yIn, zIn, 0.0f) { }

	//explicit
	FORCEINLINE Vector3::Vector3(float const * ary)         : Native(ary[0], ary[1], ary[2], 0.0f) { }

	/*static*/ FORCEINLINE Vector3 Vector3::  FromArray(float const * ary) { return Vector3(ary); }
	FORCEINLINE void               Vector3::CopyToArray(float       * ary) const { memcpy(ary, &Array[0], sizeof(float) * 3); } // TODO: Would memcpy be faster?

	template<int i0, int i1, int i2>         
	FORCEINLINE Vector3 Vector3::Shuffle()     const { return Native.Shuffle<i0, i1, i2,  3>(); }
	
	template<int i0, int i1, int i2, int i3> 
	FORCEINLINE Vector4 Vector3::ShuffleXYZ0() const { return Native.Shuffle<i0, i1, i2, i3>(); }

	template<int i> FORCEINLINE Vector3 Vector3::Insert(float f) const { return Native.Insert<i>(f); }
	template<int i> FORCEINLINE Vector3 Vector3::InsertZero()    const { return Native.InsertZero<i>(); }

	FORCEINLINE Vector3 Vector3::InsertX(float xIn) const { return Native.InsertX(xIn); }
	FORCEINLINE Vector3 Vector3::InsertY(float yIn) const { return Native.InsertY(yIn); }
	FORCEINLINE Vector3 Vector3::InsertZ(float zIn) const { return Native.InsertZ(zIn); }

	FORCEINLINE Vector4 Vector3::X000() const { return Native.X000(); }
	FORCEINLINE Vector4 Vector3::XY00() const { return Native.XY00(); }
	FORCEINLINE Vector4 Vector3::XY01() const { return Native.XY01(); }
	FORCEINLINE Vector4 Vector3::XY10() const { return Native.XY10(); }
	FORCEINLINE Vector4 Vector3::XY11() const { return Native.XY11(); }
	FORCEINLINE Vector4 Vector3::XYZ0() const { return Native;        }
	FORCEINLINE Vector4 Vector3::XYZ1() const { return Native.XYZ1(); }

	FORCEINLINE Vector3 Vector3::X00() const { return Native.X000(); }
	FORCEINLINE Vector3 Vector3::XY0() const { return Native.XY00(); }
	FORCEINLINE Vector3 Vector3::XY1() const { return Native.InsertZ(1.0f); }
	
	FORCEINLINE Vector3 Vector3::YY0() const { return Native.YYWW(); }

	FORCEINLINE Vector3 Vector3::ZZ0() const { return Native.ZZWW(); }
	FORCEINLINE Vector3 Vector3::Z0Z() const { return Native.ZWZW(); }

	FORCEINLINE Vector3 Vector3::ZC0(Vector3 const & abc) const { return Native.ZCWD(abc); }
	FORCEINLINE Vector3 Vector3::Z0C(Vector3 const & abc) const { return Native.ZWCD(abc); }

	FORCEINLINE Vector3 Vector3::XXX() const { return Native.Shuffle<0, 0, 0, 3>(); }
	FORCEINLINE Vector3 Vector3::YYY() const { return Native.Shuffle<1, 1, 1, 3>(); }
	FORCEINLINE Vector3 Vector3::ZZZ() const { return Native.Shuffle<2, 2, 2, 3>(); }
	
	FORCEINLINE Vector3 Vector3::ZYX() const { return Native.Shuffle<2, 1, 0, 3>(); }//.ZYXW(); }

	FORCEINLINE Vector3 Vector3::Min(Vector3 const & rhs) const { return Native.Min(rhs.Native); }
	FORCEINLINE Vector3 Vector3::Max(Vector3 const & rhs) const { return Native.Max(rhs.Native); }

	FORCEINLINE Vector3 Vector3::Min(float rhs) const { return Min(Vector3(rhs)); }
	FORCEINLINE Vector3 Vector3::Max(float rhs) const { return Max(Vector3(rhs)); }

	FORCEINLINE float   Vector3::Min()     const { return Native.Shuffle<0, 1, 2, 2>().Min(); }
	FORCEINLINE float   Vector3::Max()     const { return Native.Shuffle<0, 1, 2, 2>().Max(); }
	FORCEINLINE float   Vector3::Sum()     const { return Native.Sum(); }
	FORCEINLINE float   Vector3::Product() const { return Native.XYZ1().Product(); }
	FORCEINLINE float   Vector3::Mean()    const { return Sum() * 0.333333333333333333333f; }

	FORCEINLINE Vector3 Vector3::Abs()              const { return Native.Abs(); }
	FORCEINLINE Vector3 Vector3::Sign()             const { return Native.Sign().XY00(); }
	FORCEINLINE Vector3 Vector3::Half()             const { return Native.Half(); }
	FORCEINLINE Vector3 Vector3::Reciprocal()       const { return Native.Reciprocal()      .XYZ0(); } // Erase the Divide By Zero result!
	FORCEINLINE Vector3 Vector3::ReciprocalApprox() const { return Native.ReciprocalApprox().XYZ0(); } // Erase the Divide By Zero result!
	FORCEINLINE Vector3 Vector3::Sqrt()             const { return Native.Sqrt(); }
	FORCEINLINE Vector3 Vector3::RSqrt()            const { return Native.RSqrt()      .XYZ0(); } // Erase the Divide By Zero result!
	FORCEINLINE Vector3 Vector3::RSqrtApprox()      const { return Native.RSqrtApprox().XYZ0(); } // Erase the Divide By Zero result!

	FORCEINLINE Vector3 Vector3::To(Vector3 const & rhs) const { return Native.To(rhs); }

	FORCEINLINE float   Vector3::Distance       (Vector3 const & rhs) const { return Native.Distance(rhs.Native); }
	FORCEINLINE float   Vector3::DistanceSquared(Vector3 const & rhs) const { return Native.DistanceSquared(rhs.Native); }

	FORCEINLINE float   Vector3::Length()        const { return Native.Length(); }
	FORCEINLINE float   Vector3::LengthSquared() const { return Native.LengthSquared(); }

	FORCEINLINE Vector3 Vector3::Normalized() const { return Native.Normalized(); }

	FORCEINLINE float   Vector3::Angle(Vector3 const & rhs) const { return Native.Angle(rhs.Native); }
	FORCEINLINE float   Vector3::Dot  (Vector3 const & rhs) const { return Native.Dot  (rhs.Native); }

	FORCEINLINE Vector3 Vector3::Cross(Vector3 const & rhs) const
	{
		/// The SIMD version is actually very slightly slower 
		/// than the scalar version in isolation, but is more 
		/// cache friendly, making it much faster when used by 
		/// other methods.
		#if defined(USE_SIMD)
			// resZ (X * rhs.Y) - (Y * rhs.X));
			// resX (Y * rhs.Z) - (Z * rhs.Y), 
			// resY (Z * rhs.X) - (X * rhs.Z),

			Vector4 lhs_yzx =     Native.Shuffle<1, 2, 0, 3>();
			Vector4 rhs_yzx = rhs.Native.Shuffle<1, 2, 0, 3>();

			Vector4 res = (Native * rhs_yzx) - (lhs_yzx * rhs.Native);
			return res.Shuffle<1, 2, 0, 3>();
		#else
			return Vector3(
				(Y * rhs.Z) - (Z * rhs.Y), 
				(Z * rhs.X) - (X * rhs.Z),
				(X * rhs.Y) - (Y * rhs.X));
		#endif
	}

	FORCEINLINE Vector3 Vector3::Power      (float n) const { return Native.Power(n); }
	FORCEINLINE Vector3 Vector3::PowerApprox(float n) const { return Native.PowerApprox(n); }

	FORCEINLINE bool Vector3::IsFacing(Vector3 const & rhs) const { return Native.IsFacing(rhs.Native); }

	FORCEINLINE bool Vector3::HasNaN()              const { return std::isnan(X) || std::isnan(Y) || std::isnan(Z); }
	FORCEINLINE bool Vector3::HasPositiveInfinity() const { return Native.HasPositiveInfinity(); }
	FORCEINLINE bool Vector3::HasNegativeInfinity() const { return Native.HasNegativeInfinity(); }

	FORCEINLINE bool Vector3::IsNaN()               const { return std::isnan(X) && std::isnan(Y) && std::isnan(Z); }
	FORCEINLINE bool Vector3::IsPositiveInfinity()  const { return Compare<CMP::AllEQ>( std::numeric_limits<float>::infinity()); }
	FORCEINLINE bool Vector3::IsNegativeInfinity()  const { return Compare<CMP::AllEQ>(-std::numeric_limits<float>::infinity()); }

	FORCEINLINE bool Vector3::IsUnbounded()         const { return Native.IsUnbounded(); }


	template <CMP op>
	FORCEINLINE bool Vector3::Compare(float rhs) const
	{
		return Compare<op>(Vector3(rhs));
	}

	template <CMP op>
	FORCEINLINE bool Vector3::Compare(Vector3 const & rhs) const
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

	FORCEINLINE Vector3 Vector3::Clamp(Vector3 const & min, Vector3 const & max) const { return Native.Clamp(min.Native, max.Native).InsertZero<3>(); }
	FORCEINLINE Vector3 Vector3::Clamp(float   const   min, float   const   max) const { return Native.Clamp(min,        max)       .InsertZero<3>(); }

	FORCEINLINE Vector3 Vector3::MultiplyAdd   (Vector3 const & m2, Vector3 const &  a) const { return Native.MultiplyAdd(m2.Native, a.Native); } // ((*this) * rhs) + a
	FORCEINLINE Vector3 Vector3::MultiplySub   (Vector3 const & m2, Vector3 const &  a) const { return Native.MultiplySub(m2.Native, a.Native); } // ((*this) * rhs) - a

	FORCEINLINE Vector3 Vector3::Lerp      (Vector3 const & rhs, float           t) const { return Native.Lerp(rhs.Native, t); }
	FORCEINLINE Vector3 Vector3::Lerp      (Vector3 const & rhs, Vector3 const & t) const { return Native.Lerp(rhs.Native, t.Native); }
	FORCEINLINE Vector3 Vector3::SmoothStep(Vector3 const & rhs, float           t) const { return Native.SmoothStep(rhs.Native, t); }
	FORCEINLINE Vector3 Vector3::SmoothStep(Vector3 const & rhs, Vector3 const & t) const { return Native.SmoothStep(rhs.Native, t.Native); }

	FORCEINLINE Vector3 Vector3::Round_Near () const { return Native.Round_Near (); }
	FORCEINLINE Vector3 Vector3::Round_Floor() const { return Native.Round_Floor(); }
	FORCEINLINE Vector3 Vector3::Round_Ceil () const { return Native.Round_Ceil (); }
	FORCEINLINE Vector3 Vector3::Round_Trunc() const { return Native.Round_Trunc(); }

	FORCEINLINE Vector3 Vector3::FMod(Vector3 const & div) const { return Native.FMod(div.Native).InsertZero<3>(); }
	FORCEINLINE Vector3 Vector3::FMod(float           div) const { return Native.FMod(div)       .InsertZero<3>(); }

	FORCEINLINE Vector3 Vector3::RotatedAbout(Vector3 const & axis, float radians) const
	{
		Vector3 parallel = axis * this->Dot(axis);
		Vector3 perp     = (*this) - parallel;

		float cosTheta = std::cos(radians);
		float sinTheta = std::sin(radians);

		return perp * cosTheta + axis.Cross(*this) * sinTheta + parallel;
	}

	FORCEINLINE float & Vector3::operator[](int idx)       { return Native[idx]; }
	FORCEINLINE float   Vector3::operator[](int idx) const { return Native[idx]; }

	FORCEINLINE Vector3 Vector3::Sin () { return Vector3(std::sin (X), std::sin (Y), std::sin (Z)); }
	FORCEINLINE Vector3 Vector3::Cos () { return Vector3(std::cos (X), std::cos (Y), std::cos (Z)); }
	FORCEINLINE Vector3 Vector3::Tan () { return Vector3(std::tan (X), std::tan (Y), std::tan (Z)); }

	FORCEINLINE Vector3 Vector3::ASin() { return Vector3(std::asin(X), std::asin(Y), std::asin(Z)); }
	FORCEINLINE Vector3 Vector3::ACos() { return Vector3(std::acos(X), std::acos(Y), std::acos(Z)); }
	FORCEINLINE Vector3 Vector3::ATan() { return Vector3(std::atan(X), std::atan(Y), std::atan(Z)); }

	FORCEINLINE Vector3 Vector3::SinH() { return Vector3(std::sinh(X), std::sinh(Y), std::sinh(Z)); }
	FORCEINLINE Vector3 Vector3::CosH() { return Vector3(std::cosh(X), std::cosh(Y), std::cosh(Z)); }
	FORCEINLINE Vector3 Vector3::TanH() { return Vector3(std::tanh(X), std::tanh(Y), std::tanh(Z)); }

	/*static*/ FORCEINLINE Vector3 Vector3::Zero()     { return Vector4::Zero(); }
	/*static*/ FORCEINLINE Vector3 Vector3::One()      { return Vector3( 1.0f); }
	/*static*/ FORCEINLINE Vector3 Vector3::NegOne()   { return Vector3(-1.0f); }
	/*static*/ FORCEINLINE Vector3 Vector3::UnitX()    { return Vector4::UnitX(); }
	/*static*/ FORCEINLINE Vector3 Vector3::UnitY()    { return Vector4::UnitY(); }
	/*static*/ FORCEINLINE Vector3 Vector3::UnitZ()    { return Vector4::UnitZ(); }
	/*static*/ FORCEINLINE Vector3 Vector3::NegUnitX() { return Vector4::NegUnitX(); }
	/*static*/ FORCEINLINE Vector3 Vector3::NegUnitY() { return Vector4::NegUnitY(); }
	/*static*/ FORCEINLINE Vector3 Vector3::NegUnitZ() { return Vector4::NegUnitZ(); }

	/*static*/ FORCEINLINE Vector3 Vector3::PositiveInfinity() { return Vector3(-std::numeric_limits<float>::infinity()); }
	/*static*/ FORCEINLINE Vector3 Vector3::NegativeInfinity() { return Vector3( std::numeric_limits<float>::infinity()); }
	/*static*/ FORCEINLINE Vector3 Vector3::NaN()              { return Vector3( std::numeric_limits<float>::quiet_NaN()); }

	FORCEINLINE Vector3 operator-(Vector3 const & lhs) { return Vector3::UnsafeCast((-lhs.Native).InsertZero<3>()); } // Eliminate -0.0 in fourth element

	FORCEINLINE Vector3 operator+(Vector3 const & lhs, Vector3 const & rhs) { return Vector3::UnsafeCast((lhs.Native + rhs.Native)); }
	FORCEINLINE Vector3 operator-(Vector3 const & lhs, Vector3 const & rhs) { return Vector3::UnsafeCast((lhs.Native - rhs.Native).InsertZero<3>()); } // Eliminate -0.0 in fourth element
	FORCEINLINE Vector3 operator*(Vector3 const & lhs, Vector3 const & rhs) { return Vector3::UnsafeCast((lhs.Native * rhs.Native)); }
	FORCEINLINE Vector3 operator/(Vector3 const & lhs, Vector3 const & rhs) { return Vector3::UnsafeCast((lhs.Native / rhs.Native.XYZ1()).InsertZero<3>()); } // Eliminate Divide By Zero in fourth element

	FORCEINLINE Vector3 operator+(Vector3 const & lhs, float rhs) { return lhs + Vector3(rhs); }
	FORCEINLINE Vector3 operator-(Vector3 const & lhs, float rhs) { return lhs - Vector3(rhs); }
	FORCEINLINE Vector3 operator*(Vector3 const & lhs, float rhs) { return lhs * Vector3(rhs); }
	FORCEINLINE Vector3 operator/(Vector3 const & lhs, float rhs) { return lhs / Vector3(rhs); }

	FORCEINLINE Vector3 operator+(float lhs, Vector3 const & rhs) { return Vector3(lhs) + rhs; }
	FORCEINLINE Vector3 operator-(float lhs, Vector3 const & rhs) { return Vector3(lhs) - rhs; }
	FORCEINLINE Vector3 operator*(float lhs, Vector3 const & rhs) { return Vector3(lhs) * rhs; }
	FORCEINLINE Vector3 operator/(float lhs, Vector3 const & rhs) { return Vector3(lhs) / rhs; }

	FORCEINLINE Vector3 & operator+=(Vector3 & lhs, Vector3 const & rhs) { lhs = lhs + rhs; return lhs; }
	FORCEINLINE Vector3 & operator-=(Vector3 & lhs, Vector3 const & rhs) { lhs = lhs - rhs; return lhs; }
	FORCEINLINE Vector3 & operator*=(Vector3 & lhs, Vector3 const & rhs) { lhs = lhs * rhs; return lhs; }
	FORCEINLINE Vector3 & operator/=(Vector3 & lhs, Vector3 const & rhs) { lhs = lhs / rhs; return lhs; }

	FORCEINLINE Vector3 & operator+=(Vector3 & lhs, float rhs) { lhs = lhs + rhs; return lhs; }
	FORCEINLINE Vector3 & operator-=(Vector3 & lhs, float rhs) { lhs = lhs - rhs; return lhs; }
	FORCEINLINE Vector3 & operator*=(Vector3 & lhs, float rhs) { lhs = lhs * rhs; return lhs; }
	FORCEINLINE Vector3 & operator/=(Vector3 & lhs, float rhs) { lhs = lhs / rhs; return lhs; }

	FORCEINLINE bool operator<  (Vector3 const & lhs, Vector3 const & rhs) { return lhs.Native  < rhs.Native; }

	FORCEINLINE bool operator==(Vector3 const & lhs, Vector3 const & rhs) { return lhs.Native == rhs.Native; }
	FORCEINLINE bool operator!=(Vector3 const & lhs, Vector3 const & rhs) { return lhs.Native != rhs.Native; }

	FORCEINLINE bool operator==(Vector3 const & lhs, float rhs) { return lhs == Vector3(rhs); }
	FORCEINLINE bool operator!=(Vector3 const & lhs, float rhs) { return lhs != Vector3(rhs); }

	FORCEINLINE bool operator==(float lhs, Vector3 const & rhs) { return Vector3(lhs) == rhs; }
	FORCEINLINE bool operator!=(float lhs, Vector3 const & rhs) { return Vector3(lhs) != rhs; }
}

namespace std {
	FORCEINLINE ::HANDYMATH_NS::Vector3 min(::HANDYMATH_NS::Vector3 const & a, ::HANDYMATH_NS::Vector3 const & b) { return a.Min(b); }
	FORCEINLINE ::HANDYMATH_NS::Vector3 max(::HANDYMATH_NS::Vector3 const & a, ::HANDYMATH_NS::Vector3 const & b) { return a.Max(b); }
}

MAKE_HASHABLE(HANDYMATH_NS::Vector3, t.X, t.Y, t.Z)
