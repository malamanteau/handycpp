
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
	class Vector3i;

	class CVector2i /// Compact version of Vector2i, for storage, rather than computation.
	{
	public:
		int32_t X = 0, Y = 0;

		CVector2i() = default;
		CVector2i(int32_t xIn, int32_t yIn) : X(xIn), Y(yIn) { }

		CVector2i & operator=(CVector2i const &) = default;
	};

	class HANDYMATH_ALIGN16 Vector2i 
	{
		friend class Vector4i;
		friend class Vector3i;
		friend class Vector2;

		// IMPLICIT, PRIVATE. 
		// Make these public and suffer my wrath. This cast is unsafe, and doesn't sanitize Native.ZW to zero!
		Vector2i(Vector4i const & native);
		operator Vector4i() const;

	public:
		static constexpr int32_t NumElements  =  2_i32;
		static constexpr int32_t ElementBytes =  4_i32;
		static constexpr int32_t StrideBytes  = 16_i32;
		static constexpr int32_t UsefulBytes  = NumElements * ElementBytes;

		union
		{
			Vector4i               Native;
			struct { int32_t       X, Y, _zeroA, _zeroB; };
			std::array<int32_t, 4> Array;
			CVector2i              Compact;
			struct { int32_t       x, y; };
		};


		Vector2i(CVector2i const & v2ci);
		operator CVector2i() const;
				
		Vector2i();
		Vector2i(Vector2i const & v);

		explicit 
		Vector2i(int32_t xy);
		Vector2i(int32_t xIn, int32_t yIn);
		Vector2i( size_t xIn,  size_t yIn);

		explicit 
		Vector2i(int32_t const * ary);

		// implicit
		Vector2i(std::tuple<int32_t, int32_t> const & v);
		Vector2i(std::pair <int32_t, int32_t> const & v);


		Vector2i & operator=(Vector2i const &) = default;

		// Conversions:
		Vector3i XYZ (int32_t z)            const;
		Vector4i XYZW(int32_t z, int32_t w) const;
		Vector4i XYZW(Vector2i const & zw)  const;
		Vector2  ToFloat() const;
		// ---

		static Vector2i   FromArray(int32_t const * ary);
		void            CopyToArray(int32_t       * ary) const;

		template<int i0, int i1>                 Vector2i Shuffle    () const;
		template<int i0, int i1, int i2>         Vector3i ShuffleXY0 () const;
		template<int i0, int i1, int i2, int i3> Vector4i ShuffleXY00() const;

		template<int i> Vector2i Insert(int32_t f) const;
		template<int i> Vector2i InsertZero()    const;

		Vector2i InsertX(int32_t xIn) const;
		Vector2i InsertY(int32_t yIn) const;

		// These have all been optimized to be faster than using Shuffle<>()!
		Vector4i X000() const;
		Vector4i XY00() const; // THIS OPERATION IS 100% FREE, IT WILL BE COMPLETELY ERASED WHEN INLINED!
		Vector4i XY01() const;
		Vector4i XY10() const;
		Vector4i XY11() const;

		Vector3i X00()  const;
		Vector3i XY0()  const; // THIS OPERATION IS 100% FREE, IT WILL BE COMPLETELY ERASED WHEN INLINED!
		Vector3i XY1()  const;
		// ---

		Vector2i XX() const;
		Vector2i YY() const;
		Vector2i YX() const;

		Vector2i Min(Vector2i const & rhs) const;
		Vector2i Max(Vector2i const & rhs) const;

		Vector2i Min(int32_t rhs) const;
		Vector2i Max(int32_t rhs) const;

		int32_t  Min()     const;
		int32_t  Max()     const;
		int32_t  Sum()     const;
		int32_t  Product() const;

		Vector2i Abs()              const;
		Vector2i Sign()             const; //Returns, per component, the sign of the input vector: input >= 0.0 ? 1.0 : -1.0

		template <CMPI op> bool Compare(int32_t          rhs) const;
		template <CMPI op> bool Compare(Vector2i const & rhs) const;

		Vector2i To(Vector2i const & rhs) const;

		int32_t  TaxicabDistance          (Vector2i const & rhs) const;
		int32_t  TaxicabLength()    const;

		int32_t  Dot  (Vector2i const & rhs) const;
		
		Vector2i Ortho() const;

		Vector2i Power      (int32_t n) const;

		bool    HasMaxValue() const;
		bool    HasMinValue() const;
		bool    HasMinMaxValue()         const;

		Vector2i Clamp(Vector2i const & min, Vector2i const & max) const;
		Vector2i Clamp(int32_t   const   min, int32_t   const   max) const;

		int32_t & operator[](int idx);
		int32_t   operator[](int idx) const;

		static Vector2i Zero();
		static Vector2i One();
		static Vector2i NegOne();
		static Vector2i UnitX();
		static Vector2i UnitY();
		static Vector2i NegUnitX();
		static Vector2i NegUnitY();
		static Vector2i MaxValue();
		static Vector2i MinValue();
		static Vector2i FFFFFFFF();

		static Vector2i UnsafeCast(Vector4i const & v); // Certain operators need to cast from Vector4i to Vector2i without any checking. File this under "Stupid Hacks".

		template <class SerialOp> void serial(SerialOp & ser)
		{
			ser
				(X)
				(Y);
		}
	};

	/*static*/ FORCEINLINE Vector2i Vector2i::UnsafeCast(Vector4i const & v) { return Vector2i(v); }

	Vector2i operator-(Vector2i const & lhs);

	Vector2i operator+(Vector2i const & lhs, Vector2i const & rhs);
	Vector2i operator-(Vector2i const & lhs, Vector2i const & rhs);
	Vector2i operator*(Vector2i const & lhs, Vector2i const & rhs);
	Vector2i operator/(Vector2i const & lhs, Vector2i const & rhs);

	Vector2i operator+(Vector2i const & lhs, int32_t rhs);
	Vector2i operator-(Vector2i const & lhs, int32_t rhs);
	Vector2i operator*(Vector2i const & lhs, int32_t rhs);
	Vector2i operator/(Vector2i const & lhs, int32_t rhs);

	Vector2i operator+(int32_t lhs, Vector2i const & rhs);
	Vector2i operator-(int32_t lhs, Vector2i const & rhs);
	Vector2i operator*(int32_t lhs, Vector2i const & rhs);
	Vector2i operator/(int32_t lhs, Vector2i const & rhs);
	
	Vector2i & operator+=(Vector2i & lhs, Vector2i const & rhs);
	Vector2i & operator-=(Vector2i & lhs, Vector2i const & rhs);
	Vector2i & operator*=(Vector2i & lhs, Vector2i const & rhs);
	Vector2i & operator/=(Vector2i & lhs, Vector2i const & rhs);
	
	Vector2i & operator+=(Vector2i & lhs, int32_t rhs);
	Vector2i & operator-=(Vector2i & lhs, int32_t rhs);
	Vector2i & operator*=(Vector2i & lhs, int32_t rhs);
	Vector2i & operator/=(Vector2i & lhs, int32_t rhs);

	bool operator< (Vector2i const & lhs, Vector2i const & rhs);

	bool operator==(Vector2i const & lhs, Vector2i const & rhs);
	bool operator!=(Vector2i const & lhs, Vector2i const & rhs);

	bool operator==(Vector2i const & lhs, int32_t rhs);
	bool operator!=(Vector2i const & lhs, int32_t rhs);

	bool operator==(int32_t lhs, Vector2i const & rhs);
	bool operator!=(int32_t lhs, Vector2i const & rhs);
}

// Not supposed to add overloads to std technically... but I want my templates that use std::to_string, min, max, etc to work, so oh well then.
namespace std {
	FORCEINLINE ::HANDYMATH_NS::Vector2i min(::HANDYMATH_NS::Vector2i const & a, ::HANDYMATH_NS::Vector2i const & b);
	FORCEINLINE ::HANDYMATH_NS::Vector2i max(::HANDYMATH_NS::Vector2i const & a, ::HANDYMATH_NS::Vector2i const & b);

	FORCEINLINE std::string to_string(::HANDYMATH_NS::Vector2i const & v)
	{
		return "("s + std::to_string(v.X) + ", "s + std::to_string(v.Y) + ")"s;
	}
}

static inline std::ostream & operator<<(std::ostream & os, const ::HANDYMATH_NS::Vector2i & v)  
{  
	os << "(" << v.X << ", " << v.Y << ")";  
	return os;  
}  

namespace HANDYMATH_NS {

	// implicit
	FORCEINLINE Vector2i::Vector2i(Vector4i const & native) : Native(native) { }
	FORCEINLINE Vector2i::operator Vector4i() const { return Native; }
			
	// implicit
	FORCEINLINE Vector2i::Vector2i(CVector2i const & v2ci) : Native(v2ci.X, v2ci.Y, 0, 0) { }
	FORCEINLINE Vector2i::operator CVector2i() const { return Compact; }

	FORCEINLINE Vector2i::Vector2i() : Native() { }
	FORCEINLINE Vector2i::Vector2i(Vector2i const & v) : Native(v.Native) { }

	//explicit
	FORCEINLINE Vector2i::Vector2i(int32_t xy)                : Native(xy, xy, 0, 0) { }
	FORCEINLINE Vector2i::Vector2i(int32_t xIn, int32_t yIn)  : Native(xIn ,  yIn, 0, 0) { }
	FORCEINLINE Vector2i::Vector2i(size_t  xIn, size_t  yIn)  : Native(int32_t(xIn), int32_t(yIn), 0, 0) { }

	// implicit
	FORCEINLINE Vector2i::Vector2i(std::tuple<int32_t, int32_t> const & v) : Native(std::get<0>(v), std::get<1>(v), 0, 0) { }
	FORCEINLINE Vector2i::Vector2i(std::pair <int32_t, int32_t> const & v) : Native(       v.first,       v.second, 0, 0) { }

	//explicit
	FORCEINLINE Vector2i::Vector2i(int32_t const * ary) : Native(ary[0], ary[1], 0, 0) { }

	/*static*/ FORCEINLINE Vector2i Vector2i::  FromArray(int32_t const * ary) { return Vector2i(ary); }
	FORCEINLINE void               Vector2i::CopyToArray(int32_t       * ary) const { memcpy(ary, &Array[0], sizeof(int32_t) * 2); } // TODO: Would memcpy be faster?

	template<int i0, int i1>                 Vector2i Vector2i::Shuffle    () const { return Native.Shuffle<i0, i1,  2, 3>(); }
	template<int i0, int i1, int i2>         Vector3i Vector2i::ShuffleXY0 () const { return Native.Shuffle<i0, i1, i2, 3>(); }
	template<int i0, int i1, int i2, int i3> Vector4i Vector2i::ShuffleXY00() const { return Native.Shuffle<i0, i1, i2, i3>(); }

	template<int i> FORCEINLINE Vector2i Vector2i::Insert(int32_t f) const { return Native.Insert<i>(f); }
	template<int i> FORCEINLINE Vector2i Vector2i::InsertZero()    const { return Native.InsertZero<i>(); }

	FORCEINLINE Vector2i Vector2i::InsertX(int32_t xIn) const { return Native.InsertX(xIn); }
	FORCEINLINE Vector2i Vector2i::InsertY(int32_t yIn) const { return Native.InsertY(yIn); }

	FORCEINLINE Vector4i Vector2i::X000() const { return Native.X000(); }
	FORCEINLINE Vector4i Vector2i::XY00() const { return Native; }
	FORCEINLINE Vector4i Vector2i::XY01() const { return Native.XY01(); }
	FORCEINLINE Vector4i Vector2i::XY10() const { return Native.XY10(); }
	FORCEINLINE Vector4i Vector2i::XY11() const { return Native.XY11(); }

	FORCEINLINE Vector3i Vector2i::X00() const { return Vector3i(Native.X000()); }
	FORCEINLINE Vector3i Vector2i::XY0() const { return Vector3i(Native); }
	FORCEINLINE Vector3i Vector2i::XY1() const { return Vector3i(Native.XY10()); }

	FORCEINLINE Vector2i Vector2i::XX() const { return Native.Shuffle<0, 0, 2, 2>(); } // XXZZ
	FORCEINLINE Vector2i Vector2i::YY() const { return Native.Shuffle<1, 1, 3, 3>(); } // YYWW
	FORCEINLINE Vector2i Vector2i::YX() const { return Native.Shuffle<1, 0, 3, 2>(); }
	
	FORCEINLINE Vector2i Vector2i::Min(Vector2i const & rhs) const { return Native.Min(rhs.Native); }
	FORCEINLINE Vector2i Vector2i::Max(Vector2i const & rhs) const { return Native.Max(rhs.Native); }

	FORCEINLINE Vector2i Vector2i::Min(int32_t rhs) const { return Min(Vector2i(rhs)); }
	FORCEINLINE Vector2i Vector2i::Max(int32_t rhs) const { return Max(Vector2i(rhs)); }

	FORCEINLINE int32_t  Vector2i::Min()     const { return Native.XXYY().Min(); }
	FORCEINLINE int32_t  Vector2i::Max()     const { return Native.XXYY().Max(); }
	FORCEINLINE int32_t  Vector2i::Sum()     const { return Native.Sum(); }
	FORCEINLINE int32_t  Vector2i::Product() const { return Native.XY11().Product(); }

	FORCEINLINE Vector2i Vector2i::Abs()              const { return Native.Abs(); }
	FORCEINLINE Vector2i Vector2i::Sign()             const { return Native.Sign().XY00(); }

	template <CMPI op>
	FORCEINLINE bool Vector2i::Compare(int32_t rhs) const
	{
		return Compare<op>(Vector2i(rhs));
	}

	template <CMPI op>
	FORCEINLINE bool Vector2i::Compare(Vector2i const & rhs) const
	{
		/// These ops require no masking, since lhs.zw is already equal to rhs.zw (both zero), and therefore will not affect the result for the XYZ components.
		if (op == CMPI::AnyLT)   { return Native.Compare<op>(rhs.Native); }
		if (op == CMPI::AnyGT)   { return Native.Compare<op>(rhs.Native); }
		if (op == CMPI::AllLEQ)  { return Native.Compare<op>(rhs.Native); }
		if (op == CMPI::AllGEQ)  { return Native.Compare<op>(rhs.Native); }
		if (op == CMPI::AllEQ)   { return Native.Compare<op>(rhs.Native); }
		if (op == CMPI::AnyNEQ)  { return Native.Compare<op>(rhs.Native); }
		if (op == CMPI::Ordinal) { return Native.Compare<op>(rhs.Native); }

		/// These ops require lhs.zw to be greater than rhs.zw to avoid affecting the result for xyz
		if (op == CMPI::AllGT)   { return Native.XY11().Compare<op>(rhs); }
		if (op == CMPI::AnyLEQ)  { return Native.XY11().Compare<op>(rhs); }

		/// These ops require lhs.zw to be less than rhs.zw to avoid affecting the result for xyz
		if (op == CMPI::AllLT)   { return Native.Compare<op>(rhs.Native.XY11()); }
		if (op == CMPI::AnyGEQ)  { return Native.Compare<op>(rhs.Native.XY11()); }

		/// These ops require lhs.zw to not be equal to rhs.zw to avoid affecting the result for xyz
		if (op == CMPI::AnyEQ)   { return Native.Compare<op>(rhs.Native.XY11()); }
		if (op == CMPI::AllNEQ)  { return Native.Compare<op>(rhs.Native.XY11()); }
	}

	FORCEINLINE Vector2i Vector2i::To(Vector2i const & rhs) const { return Native.To(rhs); }

	FORCEINLINE int32_t  Vector2i::TaxicabDistance(Vector2i const & rhs) const { return Native.TaxicabDistance(rhs.Native); }
	FORCEINLINE int32_t  Vector2i::TaxicabLength() const { return Native.TaxicabLength(); }

	FORCEINLINE int32_t  Vector2i::Dot  (Vector2i const & rhs) const { return Native.Dot(rhs.Native); }

	FORCEINLINE Vector2i Vector2i::Ortho() const { return YX() * NegUnitY(); } // Vector2i(Y, -X);

	FORCEINLINE Vector2i Vector2i::Power      (int32_t n) const { return Native.Power(n); }

	FORCEINLINE bool    Vector2i::HasMaxValue()    const { return Native.HasMaxValue(); }
	FORCEINLINE bool    Vector2i::HasMinValue()    const { return Native.HasMinValue(); }
	FORCEINLINE bool    Vector2i::HasMinMaxValue() const { return Native.HasMinMaxValue(); }

	FORCEINLINE Vector2i Vector2i::Clamp(Vector2i const & min, Vector2i const & max) const { return Native.Clamp(min.Native, max.Native).InsertZero<3>(); }
	FORCEINLINE Vector2i Vector2i::Clamp(int32_t  const   min, int32_t  const   max) const { return Native.Clamp(min,        max)       .InsertZero<3>(); }

	FORCEINLINE int32_t & Vector2i::operator[](int idx)       { return Native[idx]; }
	FORCEINLINE int32_t   Vector2i::operator[](int idx) const { return Native[idx]; }

	/*static*/ FORCEINLINE Vector2i Vector2i::Zero()     { return Vector4i::Zero(); }
	/*static*/ FORCEINLINE Vector2i Vector2i::One()      { return Vector2i( 1); }
	/*static*/ FORCEINLINE Vector2i Vector2i::NegOne()   { return Vector2i(-1); }
	/*static*/ FORCEINLINE Vector2i Vector2i::UnitX()    { return Vector4i::UnitX(); }
	/*static*/ FORCEINLINE Vector2i Vector2i::UnitY()    { return Vector4i::UnitY(); }
	/*static*/ FORCEINLINE Vector2i Vector2i::NegUnitX() { return Vector4i::NegUnitX(); }
	/*static*/ FORCEINLINE Vector2i Vector2i::NegUnitY() { return Vector4i::NegUnitY(); }

	/*static*/ FORCEINLINE Vector2i Vector2i::MaxValue() { return Vector2i(std::numeric_limits<int32_t>::max()); }
	/*static*/ FORCEINLINE Vector2i Vector2i::MinValue() { return Vector2i(std::numeric_limits<int32_t>::min()); }
	/*static*/ FORCEINLINE Vector2i Vector2i::FFFFFFFF() { return Vector4i::FFFFFFFF().XY00(); }

	FORCEINLINE Vector2i operator-(Vector2i const & lhs) { return Vector2i::UnsafeCast((-lhs.Native).XY00()); } // Eliminate -0.0 in third/fourth element

	FORCEINLINE Vector2i operator+(Vector2i const & lhs, Vector2i const & rhs) { return Vector2i::UnsafeCast((lhs.Native + rhs.Native)); }
	FORCEINLINE Vector2i operator-(Vector2i const & lhs, Vector2i const & rhs) { return Vector2i::UnsafeCast((lhs.Native - rhs.Native).XY00()); } // Eliminate -0.0 in third/fourth element
	FORCEINLINE Vector2i operator*(Vector2i const & lhs, Vector2i const & rhs) { return Vector2i::UnsafeCast((lhs.Native * rhs.Native)); }
	FORCEINLINE Vector2i operator/(Vector2i const & lhs, Vector2i const & rhs) { return Vector2i::UnsafeCast((lhs.Native / rhs.Native.XY11()).XY00()); } // Eliminate Divide By Zero in third/fourth element

	FORCEINLINE Vector2i operator+(Vector2i const & lhs, int32_t rhs) { return lhs + Vector2i(rhs); }
	FORCEINLINE Vector2i operator-(Vector2i const & lhs, int32_t rhs) { return lhs - Vector2i(rhs); }
	FORCEINLINE Vector2i operator*(Vector2i const & lhs, int32_t rhs) { return lhs * Vector2i(rhs); }
	FORCEINLINE Vector2i operator/(Vector2i const & lhs, int32_t rhs) { return lhs / Vector2i(rhs); }

	FORCEINLINE Vector2i operator+(int32_t lhs, Vector2i const & rhs) { return Vector2i(lhs) + rhs; }
	FORCEINLINE Vector2i operator-(int32_t lhs, Vector2i const & rhs) { return Vector2i(lhs) - rhs; }
	FORCEINLINE Vector2i operator*(int32_t lhs, Vector2i const & rhs) { return Vector2i(lhs) * rhs; }
	FORCEINLINE Vector2i operator/(int32_t lhs, Vector2i const & rhs) { return Vector2i(lhs) / rhs; }

	FORCEINLINE Vector2i & operator+=(Vector2i & lhs, Vector2i const & rhs) { lhs = lhs + rhs; return lhs; }
	FORCEINLINE Vector2i & operator-=(Vector2i & lhs, Vector2i const & rhs) { lhs = lhs - rhs; return lhs; }
	FORCEINLINE Vector2i & operator*=(Vector2i & lhs, Vector2i const & rhs) { lhs = lhs * rhs; return lhs; }
	FORCEINLINE Vector2i & operator/=(Vector2i & lhs, Vector2i const & rhs) { lhs = lhs / rhs; return lhs; }

	FORCEINLINE Vector2i & operator+=(Vector2i & lhs, int32_t rhs) { lhs = lhs + rhs; return lhs; }
	FORCEINLINE Vector2i & operator-=(Vector2i & lhs, int32_t rhs) { lhs = lhs - rhs; return lhs; }
	FORCEINLINE Vector2i & operator*=(Vector2i & lhs, int32_t rhs) { lhs = lhs * rhs; return lhs; }
	FORCEINLINE Vector2i & operator/=(Vector2i & lhs, int32_t rhs) { lhs = lhs / rhs; return lhs; }

	FORCEINLINE bool operator< (Vector2i const & lhs, Vector2i const & rhs) { return lhs.Native  < rhs.Native; }

	FORCEINLINE bool operator==(Vector2i const & lhs, Vector2i const & rhs) { return lhs.Native == rhs.Native; }
	FORCEINLINE bool operator!=(Vector2i const & lhs, Vector2i const & rhs) { return lhs.Native != rhs.Native; }

	FORCEINLINE bool operator==(Vector2i const & lhs, int32_t rhs) { return lhs == Vector2i(rhs); }
	FORCEINLINE bool operator!=(Vector2i const & lhs, int32_t rhs) { return lhs != Vector2i(rhs); }

	FORCEINLINE bool operator==(int32_t lhs, Vector2i const & rhs) { return Vector2i(lhs) == rhs; }
	FORCEINLINE bool operator!=(int32_t lhs, Vector2i const & rhs) { return Vector2i(lhs) != rhs; }
}

namespace std {
	FORCEINLINE ::HANDYMATH_NS::Vector2i min(::HANDYMATH_NS::Vector2i const & a, ::HANDYMATH_NS::Vector2i const & b) { return a.Min(b); }
	FORCEINLINE ::HANDYMATH_NS::Vector2i max(::HANDYMATH_NS::Vector2i const & a, ::HANDYMATH_NS::Vector2i const & b) { return a.Max(b); }
}

MAKE_HASHABLE(HANDYMATH_NS::Vector2i, t.X, t.Y)
