
/// See ../../License.txt for license info.

#pragma once

#include <ostream>
#include <cmath>
#include <cstdint>
#include <limits>
#include <array>
#include <cstring>  // memcpy

#include "Base.hpp"

#include "Vector4i.hpp"

// TODO EXP2

namespace HANDYMATH_NS {

	class Vector4i;
	class Vector2i;

	class CVector3i /// Compact version of Vector3i, for storage, rather than computation.
	{
	public:
		int32_t X = 0, Y = 0, Z = 0;

		CVector3i() = default;
		CVector3i(int32_t xIn, int32_t yIn, int32_t zIn) : X(xIn), Y(yIn), Z(zIn) { }

		CVector3i & operator=(CVector3i const &) = default;
	};

	class HANDYMATH_ALIGN16 Vector3i 
	{
		friend class Vector4i;
		friend class Vector3;
		friend class Vector2i;

		// IMPLICIT, PRIVATE. 
		// Make these public and suffer my wrath. This cast is unsafe, and doesn't sanitize Native.W to zero!
		Vector3i(Vector4i const & native);
		operator Vector4i() const;

	public:
		static constexpr int32_t NumElements  =  3_i32;
		static constexpr int32_t ElementBytes =  4_i32;
		static constexpr int32_t StrideBytes  = 16_i32;
		static constexpr int32_t UsefulBytes  = NumElements * ElementBytes;

		union
		{
			Vector4i                Native;
			struct { int32_t        X, Y, Z, _zero; };
			std::array<int32_t,  4> Array;
			CVector3i               Compact;
			struct { int32_t        x, y, z; };
		};

		Vector3i(CVector3i const & v3ci);
		operator CVector3i() const;

		Vector3i();
		Vector3i(Vector3i const & v);

		explicit 
		Vector3i(int32_t xyz);
		Vector3i(int32_t xIn, int32_t yIn, int32_t zIn);
		Vector3i(size_t  xIn, size_t  yIn, size_t  zIn);

		explicit 
		Vector3i(int32_t const * ary);

		Vector3i & operator=(Vector3i const &) = default;

		// Conversions:
		Vector2i XY()                      const;
		Vector4i XYZW(int32_t w)           const;
		//Vector4i XYZW(Vector2i const & zw) const;
		Vector3  ToFloat() const;
		// ---

		static Vector3i FromArray  (int32_t const * ary);
		void           CopyToArray(int32_t       * ary) const;

		template<int i0, int i1, int i2>         Vector3i Shuffle()     const;
		template<int i0, int i1, int i2, int i3> Vector4i ShuffleXYZ0() const;

		template<int i> Vector3i Insert(int32_t f) const;
		template<int i> Vector3i InsertZero()    const;

		Vector3i InsertX(int32_t xIn) const;
		Vector3i InsertY(int32_t yIn) const;
		Vector3i InsertZ(int32_t zIn) const;

		// These have all been optimized to be faster than using Shuffle<>()!
		Vector4i X000() const;
		Vector4i XY00() const;
		Vector4i XY01() const;
		Vector4i XY10() const;
		Vector4i XY11() const;
		Vector4i XYZ0() const; // THIS OPERATION IS 100% FREE, IT WILL BE COMPLETELY ERASED WHEN INLINED!
		Vector4i XYZ1() const;

		Vector3i X00() const;
		Vector3i XY0() const;
		Vector3i XY1() const;

		Vector3i ZZ0() const;
		Vector3i Z0Z() const;

		Vector3i ZC0(Vector3i const & abcd) const;
		Vector3i Z0C(Vector3i const & abcd) const;
		// ---

		Vector3i XXX() const;
		Vector3i YYY() const;
		Vector3i ZZZ() const;

		Vector3i ZYX() const;

		Vector3i Min(Vector3i const & rhs) const;
		Vector3i Max(Vector3i const & rhs) const;

		Vector3i Min(int32_t rhs) const;
		Vector3i Max(int32_t rhs) const;

		int32_t  Min()     const;
		int32_t  Max()     const;
		int32_t  Sum()     const;
		int32_t  Product() const;

		Vector3i Abs()              const;
		Vector3i Sign()             const; //Returns, per component, the sign of the input vector: input >= 0 ? 1 : -1

		template <CMPI op> bool Compare(int32_t          rhs) const;
		template <CMPI op> bool Compare(Vector3i const & rhs) const;

		Vector3i To(Vector3i const & rhs) const;

		int32_t  TaxicabDistance(Vector3i const & rhs) const;
		int32_t  TaxicabLength() const;

		int32_t  Dot  (Vector3i const & rhs) const;

		Vector3i Power      (int32_t n) const;

		bool     HasMaxValue()    const;
		bool     HasMinValue()    const;
		bool     HasMinMaxValue() const;

		Vector3i Clamp(Vector3i const & min, Vector3i const & max) const;
		Vector3i Clamp(int32_t   const   min, int32_t   const   max) const;

		int32_t & operator[](int idx);
		int32_t   operator[](int idx) const;

		static Vector3i Zero();
		static Vector3i One();
		static Vector3i NegOne();
		static Vector3i UnitX();
		static Vector3i UnitY();
		static Vector3i UnitZ();
		static Vector3i NegUnitX();
		static Vector3i NegUnitY();
		static Vector3i NegUnitZ();
		static Vector3i MaxValue();
		static Vector3i MinValue();
		static Vector3i FFFFFFFF();

		static Vector3i UnsafeCast(Vector4i const & v); // Certain operators need to cast from Vector4i to Vector3i without any checking. File this under "Stupid Hacks".

		template <class SerialOp> void serial(SerialOp & ser)
		{
			ser
				(X)
				(Y)
				(Z);
		}
	};

	/*static*/ FORCEINLINE Vector3i Vector3i::UnsafeCast(Vector4i const & v) { return Vector3i(v); }

	Vector3i operator+(Vector3i const & lhs, int32_t rhs);
	Vector3i operator-(Vector3i const & lhs, int32_t rhs);
	Vector3i operator*(Vector3i const & lhs, int32_t rhs);
	Vector3i operator/(Vector3i const & lhs, int32_t rhs);

	Vector3i operator+(int32_t lhs, Vector3i const & rhs);
	Vector3i operator-(int32_t lhs, Vector3i const & rhs);
	Vector3i operator*(int32_t lhs, Vector3i const & rhs);
	Vector3i operator/(int32_t lhs, Vector3i const & rhs);
	
	Vector3i & operator+=(Vector3i & lhs, Vector3i const & rhs);
	Vector3i & operator-=(Vector3i & lhs, Vector3i const & rhs);
	Vector3i & operator*=(Vector3i & lhs, Vector3i const & rhs);
	Vector3i & operator/=(Vector3i & lhs, Vector3i const & rhs);
	
	Vector3i & operator+=(Vector3i & lhs, int32_t rhs);
	Vector3i & operator-=(Vector3i & lhs, int32_t rhs);
	Vector3i & operator*=(Vector3i & lhs, int32_t rhs);
	Vector3i & operator/=(Vector3i & lhs, int32_t rhs);

	bool operator< (Vector3i const & lhs, Vector3i const & rhs);

	bool operator==(Vector3i const & lhs, Vector3i const & rhs);
	bool operator!=(Vector3i const & lhs, Vector3i const & rhs);

	bool operator==(Vector3i const & lhs, int32_t rhs);
	bool operator!=(Vector3i const & lhs, int32_t rhs);

	bool operator==(int32_t lhs, Vector3i const & rhs);
	bool operator!=(int32_t lhs, Vector3i const & rhs);
}

namespace std {
	inline ::HANDYMATH_NS::Vector3i min(::HANDYMATH_NS::Vector3i const & a, ::HANDYMATH_NS::Vector3i const & b);
	inline ::HANDYMATH_NS::Vector3i max(::HANDYMATH_NS::Vector3i const & a, ::HANDYMATH_NS::Vector3i const & b);
}

static inline std::ostream & operator<<(std::ostream & os, const ::HANDYMATH_NS::Vector3i & v)  
{  
	os << "(" << v.X << ", " << v.Y << ", " << v.Z << ")";  
	return os;  
}  

namespace HANDYMATH_NS {

	// implicit
	FORCEINLINE Vector3i::Vector3i(Vector4i const & native) : Native(native) { }
	FORCEINLINE Vector3i::operator Vector4i() const { return Native; }
	
	// implicit
	FORCEINLINE Vector3i::Vector3i(CVector3i const & v3ci) : Native(v3ci.X, v3ci.Y, v3ci.Z, 0) { }
	FORCEINLINE Vector3i::operator CVector3i() const { return Compact; }

	FORCEINLINE Vector3i::Vector3i() : Native() { }
	FORCEINLINE Vector3i::Vector3i(Vector3i const & v) : Native(v.Native) { }

	//explicit
	FORCEINLINE Vector3i::Vector3i(int32_t xyz)                           : Native(xyz, xyz, xyz, 0_i32) { }
	FORCEINLINE Vector3i::Vector3i(int32_t xIn, int32_t yIn, int32_t zIn) : Native(xIn, yIn, zIn, 0_i32) { }
	FORCEINLINE Vector3i::Vector3i(size_t  xIn, size_t  yIn, size_t  zIn) : Native(int32_t(xIn), int32_t(yIn), int32_t(zIn), 0_i32) { }

	//explicit
	FORCEINLINE Vector3i::Vector3i(int32_t const * ary)         : Native(ary[0], ary[1], ary[2], 0_i32) { }

	/*static*/ FORCEINLINE Vector3i Vector3i::  FromArray(int32_t const * ary) { return Vector3i(ary); }
	FORCEINLINE void               Vector3i::CopyToArray(int32_t       * ary) const { memcpy(ary, &Array[0], sizeof(int32_t) * 3); } // TODO: Would memcpy be faster?

	template<int i0, int i1, int i2>         
	FORCEINLINE Vector3i Vector3i::Shuffle()     const { return Native.Shuffle<i0, i1, i2,  3>(); }
	
	template<int i0, int i1, int i2, int i3> 
	FORCEINLINE Vector4i Vector3i::ShuffleXYZ0() const { return Native.Shuffle<i0, i1, i2, i3>(); }

	template<int i> FORCEINLINE Vector3i Vector3i::Insert(int32_t f) const { return Native.Insert<i>(f); }
	template<int i> FORCEINLINE Vector3i Vector3i::InsertZero()    const { return Native.InsertZero<i>(); }

	FORCEINLINE Vector3i Vector3i::InsertX(int32_t xIn) const { return Native.InsertX(xIn); }
	FORCEINLINE Vector3i Vector3i::InsertY(int32_t yIn) const { return Native.InsertY(yIn); }
	FORCEINLINE Vector3i Vector3i::InsertZ(int32_t zIn) const { return Native.InsertZ(zIn); }

	FORCEINLINE Vector4i Vector3i::X000() const { return Native.X000(); }
	FORCEINLINE Vector4i Vector3i::XY00() const { return Native.XY00(); }
	FORCEINLINE Vector4i Vector3i::XY01() const { return Native.XY01(); }
	FORCEINLINE Vector4i Vector3i::XY10() const { return Native.XY10(); }
	FORCEINLINE Vector4i Vector3i::XY11() const { return Native.XY11(); }
	FORCEINLINE Vector4i Vector3i::XYZ0() const { return Native;        }
	FORCEINLINE Vector4i Vector3i::XYZ1() const { return Native.XYZ1(); }

	FORCEINLINE Vector3i Vector3i::X00() const { return Native.X000(); }
	FORCEINLINE Vector3i Vector3i::XY0() const { return Native.XY00(); }
	FORCEINLINE Vector3i Vector3i::XY1() const { return Native.InsertZ(1_i32); }

	FORCEINLINE Vector3i Vector3i::ZZ0() const { return Native.ZZWW(); }
	FORCEINLINE Vector3i Vector3i::Z0Z() const { return Native.ZWZW(); }

	FORCEINLINE Vector3i Vector3i::ZC0(Vector3i const & abc) const { return Native.ZCWD(abc); }
	FORCEINLINE Vector3i Vector3i::Z0C(Vector3i const & abc) const { return Native.ZWCD(abc); }

	FORCEINLINE Vector3i Vector3i::XXX() const { return Native.Shuffle<0, 0, 0, 3>(); }
	FORCEINLINE Vector3i Vector3i::YYY() const { return Native.Shuffle<1, 1, 1, 3>(); }
	FORCEINLINE Vector3i Vector3i::ZZZ() const { return Native.Shuffle<2, 2, 2, 3>(); }
	
	FORCEINLINE Vector3i Vector3i::ZYX() const { return Native.Shuffle<2, 1, 0, 3>(); }//.ZYXW(); }

	FORCEINLINE Vector3i Vector3i::Min(Vector3i const & rhs) const { return Native.Min(rhs.Native); }
	FORCEINLINE Vector3i Vector3i::Max(Vector3i const & rhs) const { return Native.Max(rhs.Native); }

	FORCEINLINE Vector3i Vector3i::Min(int32_t rhs) const { return Min(Vector3i(rhs)); }
	FORCEINLINE Vector3i Vector3i::Max(int32_t rhs) const { return Max(Vector3i(rhs)); }

	FORCEINLINE int32_t  Vector3i::Min()     const { return Native.Shuffle<0, 1, 2, 2>().Min(); }
	FORCEINLINE int32_t  Vector3i::Max()     const { return Native.Shuffle<0, 1, 2, 2>().Max(); }
	FORCEINLINE int32_t  Vector3i::Sum()     const { return Native.Sum(); }
	FORCEINLINE int32_t  Vector3i::Product() const { return Native.XYZ1().Product(); }

	FORCEINLINE Vector3i Vector3i::Abs()              const { return Native.Abs(); }
	FORCEINLINE Vector3i Vector3i::Sign()             const { return Native.Sign().XY00(); }

	template <CMPI op>
	FORCEINLINE bool Vector3i::Compare(int32_t rhs) const
	{
		return Compare<op>(Vector3i(rhs));
	}

	template <CMPI op>
	FORCEINLINE bool Vector3i::Compare(Vector3i const & rhs) const
	{
		/// These ops require no masking, since lhs.w is already equal to rhs.w (both zero), and therefore will not affect the result for the XYZ components.
		if (op == CMPI::AnyLT)   { return Native.Compare<op>(rhs.Native); }
		if (op == CMPI::AnyGT)   { return Native.Compare<op>(rhs.Native); }
		if (op == CMPI::AllLEQ)  { return Native.Compare<op>(rhs.Native); }
		if (op == CMPI::AllGEQ)  { return Native.Compare<op>(rhs.Native); }
		if (op == CMPI::AllEQ)   { return Native.Compare<op>(rhs.Native); }
		if (op == CMPI::AnyNEQ)  { return Native.Compare<op>(rhs.Native); }
		if (op == CMPI::Ordinal) { return Native.Compare<op>(rhs.Native); }

		/// These ops require lhs.w to be greater than rhs.w to avoid affecting the result for xyz
		if (op == CMPI::AllGT)   { return Native.XYZ1().Compare<op>(rhs); }
		if (op == CMPI::AnyLEQ)  { return Native.XYZ1().Compare<op>(rhs); }

		/// These ops require lhs.w to be less than rhs.w to avoid affecting the result for xyz
		if (op == CMPI::AllLT)   { return Native.Compare<op>(rhs.Native.XYZ1()); }
		if (op == CMPI::AnyGEQ)  { return Native.Compare<op>(rhs.Native.XYZ1()); }

		/// These ops require lhs.w to not be equal to rhs.w to avoid affecting the result for xyz
		if (op == CMPI::AnyEQ)   { return Native.Compare<op>(rhs.Native.XYZ1()); }
		if (op == CMPI::AllNEQ)  { return Native.Compare<op>(rhs.Native.XYZ1()); }
	}

	FORCEINLINE Vector3i Vector3i::To(Vector3i const & rhs) const { return Native.To(rhs); }

	FORCEINLINE int32_t  Vector3i::TaxicabDistance(Vector3i const & rhs) const { return Native.TaxicabDistance(rhs.Native); }
	FORCEINLINE int32_t  Vector3i::TaxicabLength() const { return Native.TaxicabLength(); }

	FORCEINLINE int32_t  Vector3i::Dot(Vector3i const & rhs) const { return Native.Dot  (rhs.Native); }

	FORCEINLINE Vector3i Vector3i::Power(int32_t n) const { return Native.Power(n); }

	FORCEINLINE bool     Vector3i::HasMaxValue()    const { return Native.HasMaxValue(); }
	FORCEINLINE bool     Vector3i::HasMinValue()    const { return Native.HasMinValue(); }
	FORCEINLINE bool     Vector3i::HasMinMaxValue() const { return Native.HasMinMaxValue(); }

	FORCEINLINE Vector3i Vector3i::Clamp(Vector3i  const & min, Vector3i const & max) const { return Native.Clamp(min.Native, max.Native).InsertZero<3>(); }
	FORCEINLINE Vector3i Vector3i::Clamp(int32_t   const   min, int32_t  const   max) const { return Native.Clamp(min,        max)       .InsertZero<3>(); }

	FORCEINLINE int32_t & Vector3i::operator[](int idx)       { return Native[idx]; }
	FORCEINLINE int32_t   Vector3i::operator[](int idx) const { return Native[idx]; }

	/*static*/ FORCEINLINE Vector3i Vector3i::Zero()     { return Vector4i::Zero(); }
	/*static*/ FORCEINLINE Vector3i Vector3i::One()      { return Vector3i( 1_i32); }
	/*static*/ FORCEINLINE Vector3i Vector3i::NegOne()   { return Vector3i(-1_i32); }
	/*static*/ FORCEINLINE Vector3i Vector3i::UnitX()    { return Vector4i::UnitX(); }
	/*static*/ FORCEINLINE Vector3i Vector3i::UnitY()    { return Vector4i::UnitY(); }
	/*static*/ FORCEINLINE Vector3i Vector3i::UnitZ()    { return Vector4i::UnitZ(); }
	/*static*/ FORCEINLINE Vector3i Vector3i::NegUnitX() { return Vector4i::NegUnitX(); }
	/*static*/ FORCEINLINE Vector3i Vector3i::NegUnitY() { return Vector4i::NegUnitY(); }
	/*static*/ FORCEINLINE Vector3i Vector3i::NegUnitZ() { return Vector4i::NegUnitZ(); }

	/*static*/ FORCEINLINE Vector3i Vector3i::MaxValue() { return Vector3i( std::numeric_limits<int32_t>::max()); }
	/*static*/ FORCEINLINE Vector3i Vector3i::MinValue() { return Vector3i( std::numeric_limits<int32_t>::min()); }
	/*static*/ FORCEINLINE Vector3i Vector3i::FFFFFFFF() { return Vector4i::FFFFFFFF().InsertZero<3>(); }

	FORCEINLINE Vector3i operator-(Vector3i const & lhs) { return Vector3i::UnsafeCast((-lhs.Native).InsertZero<3>()); } // Eliminate -0.0 in fourth element

	FORCEINLINE Vector3i operator+(Vector3i const & lhs, Vector3i const & rhs) { return Vector3i::UnsafeCast((lhs.Native + rhs.Native)); }
	FORCEINLINE Vector3i operator-(Vector3i const & lhs, Vector3i const & rhs) { return Vector3i::UnsafeCast((lhs.Native - rhs.Native).InsertZero<3>()); } // Eliminate -0.0 in fourth element
	FORCEINLINE Vector3i operator*(Vector3i const & lhs, Vector3i const & rhs) { return Vector3i::UnsafeCast((lhs.Native * rhs.Native)); }
	FORCEINLINE Vector3i operator/(Vector3i const & lhs, Vector3i const & rhs) { return Vector3i::UnsafeCast((lhs.Native / rhs.Native.XYZ1()).InsertZero<3>()); } // Eliminate Divide By Zero in fourth element

	FORCEINLINE Vector3i operator+(Vector3i const & lhs, int32_t rhs) { return lhs + Vector3i(rhs); }
	FORCEINLINE Vector3i operator-(Vector3i const & lhs, int32_t rhs) { return lhs - Vector3i(rhs); }
	FORCEINLINE Vector3i operator*(Vector3i const & lhs, int32_t rhs) { return lhs * Vector3i(rhs); }
	FORCEINLINE Vector3i operator/(Vector3i const & lhs, int32_t rhs) { return lhs / Vector3i(rhs); }

	FORCEINLINE Vector3i operator+(int32_t lhs, Vector3i const & rhs) { return Vector3i(lhs) + rhs; }
	FORCEINLINE Vector3i operator-(int32_t lhs, Vector3i const & rhs) { return Vector3i(lhs) - rhs; }
	FORCEINLINE Vector3i operator*(int32_t lhs, Vector3i const & rhs) { return Vector3i(lhs) * rhs; }
	FORCEINLINE Vector3i operator/(int32_t lhs, Vector3i const & rhs) { return Vector3i(lhs) / rhs; }

	FORCEINLINE Vector3i & operator+=(Vector3i & lhs, Vector3i const & rhs) { lhs = lhs + rhs; return lhs; }
	FORCEINLINE Vector3i & operator-=(Vector3i & lhs, Vector3i const & rhs) { lhs = lhs - rhs; return lhs; }
	FORCEINLINE Vector3i & operator*=(Vector3i & lhs, Vector3i const & rhs) { lhs = lhs * rhs; return lhs; }
	FORCEINLINE Vector3i & operator/=(Vector3i & lhs, Vector3i const & rhs) { lhs = lhs / rhs; return lhs; }

	FORCEINLINE Vector3i & operator+=(Vector3i & lhs, int32_t rhs) { lhs = lhs + rhs; return lhs; }
	FORCEINLINE Vector3i & operator-=(Vector3i & lhs, int32_t rhs) { lhs = lhs - rhs; return lhs; }
	FORCEINLINE Vector3i & operator*=(Vector3i & lhs, int32_t rhs) { lhs = lhs * rhs; return lhs; }
	FORCEINLINE Vector3i & operator/=(Vector3i & lhs, int32_t rhs) { lhs = lhs / rhs; return lhs; }

	FORCEINLINE bool operator<  (Vector3i const & lhs, Vector3i const & rhs) { return lhs.Native < rhs.Native; }

	FORCEINLINE bool operator==(Vector3i const & lhs, Vector3i const & rhs) { return lhs.Native == rhs.Native; }
	FORCEINLINE bool operator!=(Vector3i const & lhs, Vector3i const & rhs) { return lhs.Native != rhs.Native; }

	FORCEINLINE bool operator==(Vector3i const & lhs, int32_t rhs) { return lhs == Vector3i(rhs); }
	FORCEINLINE bool operator!=(Vector3i const & lhs, int32_t rhs) { return lhs != Vector3i(rhs); }

	FORCEINLINE bool operator==(int32_t lhs, Vector3i const & rhs) { return Vector3i(lhs) == rhs; }
	FORCEINLINE bool operator!=(int32_t lhs, Vector3i const & rhs) { return Vector3i(lhs) != rhs; }
}

// Not supposed to add overloads to std technically... but I want my templates that use std::to_string, min, max, etc to work, so oh well then.
namespace std {
	FORCEINLINE ::HANDYMATH_NS::Vector3i min(::HANDYMATH_NS::Vector3i const & a, ::HANDYMATH_NS::Vector3i const & b) { return a.Min(b); }
	FORCEINLINE ::HANDYMATH_NS::Vector3i max(::HANDYMATH_NS::Vector3i const & a, ::HANDYMATH_NS::Vector3i const & b) { return a.Max(b); }

	FORCEINLINE std::string to_string(::HANDYMATH_NS::Vector3i const & v)
	{
		return "("s + std::to_string(v.X) + ", "s + std::to_string(v.Y) + ", "s + std::to_string(v.Z) + ")"s;
	}
}

MAKE_HASHABLE(HANDYMATH_NS::Vector3i, t.X, t.Y, t.Z)
