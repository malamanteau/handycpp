
/// See ../../License.txt for license info.

#pragma once

#include <ostream>
#include <cmath>
#include <cstdint>
#include <limits>
#include <array>
#include <cstring>  // memcpy

#include "Base.hpp"

// TODO EXP2

namespace HANDYMATH_NS {

	class Bitset128;

	class Vector3i;
	class Vector2i;

	class Vector4;
	class Vector3;
	class Vector2;

	class HANDYMATH_ALIGN16 Vector4i 
	{
		friend class Vector4;
		friend class Vector3i;
		friend class Vector2i;

	public:
		static constexpr int32_t NumElements  =  4_i32;
		static constexpr int32_t ElementBytes =  4_i32;
		static constexpr int32_t StrideBytes  = 16_i32;
		static constexpr int32_t UsefulBytes  = NumElements * ElementBytes;

		union
		{
			ARCHIT4 Native;
			struct { int32_t X, Y, Z, W; };
			struct { int32_t x, y, z, w; };
			std::array<int32_t, 4> Array;
		};

		// implicit
		Vector4i(ARCHIT4 const & native);
		operator ARCHIT4() const;

		Vector4i();
		Vector4i(Vector4i const & v);
		
		explicit 
		Vector4i(int32_t xyzw);
		Vector4i(int32_t xIn, int32_t yIn, int32_t zIn, int32_t wIn);
		Vector4i( size_t xIn,  size_t yIn,  size_t zIn,  size_t wIn);

		explicit 
		Vector4i(int32_t const * ary);

		Vector4i & operator=(Vector4i const &) = default;

		// Conversions:
		Vector2i XY () const;
		Vector2i ZW () const;
		Vector3i XYZ() const;
		Vector4  ToFloat() const;
		// ---

		static Vector4i FromArray  (int32_t const * ary);
		void            CopyToArray(int32_t       * ary) const;

		Vector4  PS128() const;

		template<int i0, int i1, int i2, int i3> Vector4i Shuffle() const;

		template<int i> Vector4i Insert(int32_t f) const;
		template<int i> Vector4i InsertZero()    const;
		
		Vector4i InsertX(int32_t xIn) const;
		Vector4i InsertY(int32_t yIn) const;
		Vector4i InsertZ(int32_t zIn) const;
		Vector4i InsertW(int32_t wIn) const;

		// These have been optimized faster than using Shuffle<>()!
		Vector4i X000() const;
		Vector4i XY00() const;
		Vector4i XY01() const;
		Vector4i XY10() const;
		Vector4i XY11() const;
		Vector4i XYZ0() const;
		Vector4i XYZ1() const;

		Vector4i XXYY() const;
		Vector4i ZZWW() const;

		Vector4i XYXY() const;
		Vector4i ZWZW() const;

		Vector4i ZCWD(Vector4i const & abcd) const;
		Vector4i XYAB(Vector4i const & abcd) const; // TODO Scalar
		Vector4i ZWCD(Vector4i const & abcd) const; // TODO Scalar
		Vector4i XAYB(Vector4i const & abcd) const;
		// ---

		Vector4i XXXX() const;
		Vector4i YYYY() const;
		Vector4i ZZZZ() const;
		Vector4i WWWW() const;

		Vector4i WZYX() const;
		Vector4i ZWXY() const;

		Vector4i Min(Vector4i const & rhs) const;
		Vector4i Max(Vector4i const & rhs) const;

		Vector4i Min(int32_t rhs) const;
		Vector4i Max(int32_t rhs) const;

		int32_t  Min()     const;
		int32_t  Max()     const;
		int32_t  Sum()     const;
		int32_t  Product() const;

		Vector4i Abs()  const;
		Vector4i Sign() const; //Returns, per component, the sign of the input vector: input >= 0 ? 1 : -1

		template <CMPI op> bool Compare(int32_t          rhs) const;
		template <CMPI op> bool Compare(Vector4i const & rhs) const;

		Vector4i To(Vector4i const & rhs) const;

		int32_t  TaxicabDistance(Vector4i const & rhs) const;
		int32_t  TaxicabLength() const;

		int32_t  Dot(Vector4i const & rhs) const;
			
		Vector4i Power(int32_t n) const;

		Vector4i BitwiseAndNot(Vector4i const & rhs) const;

		bool     HasMaxValue() const;
		bool     HasMinValue() const;
		bool     HasMinMaxValue() const;

		Vector4i Clamp(Vector4i const & min, Vector4i const & max) const;
		Vector4i Clamp(int32_t min, int32_t max) const;

		int32_t & operator[](int32_t idx);
		int32_t   operator[](int32_t idx) const;

		static Vector4i Zero();
		static Vector4i One();
		static Vector4i NegOne();
		static Vector4i UnitX();
		static Vector4i UnitY();
		static Vector4i UnitZ();
		static Vector4i UnitW();
		static Vector4i UnitXW();
		static Vector4i UnitYW();
		static Vector4i UnitZW();
		static Vector4i NegUnitX();
		static Vector4i NegUnitY();
		static Vector4i NegUnitZ();
		static Vector4i NegUnitW();
		static Vector4i MaxValue();
		static Vector4i MinValue();
		static Vector4i FFFFFFFF();

		template <class SerialOp> void serial(SerialOp & ser)
		{
			ser
				(X)
				(Y)
				(Z)
				(W);
		}
	};

	Vector4i operator-(Vector4i const & lhs);
	Vector4i operator~(Vector4i const & lhs);

	Vector4i operator+(Vector4i const & lhs, Vector4i const & rhs);
	Vector4i operator-(Vector4i const & lhs, Vector4i const & rhs);
	Vector4i operator*(Vector4i const & lhs, Vector4i const & rhs);
	Vector4i operator/(Vector4i const & lhs, Vector4i const & rhs);
	Vector4i operator&(Vector4i const & lhs, Vector4i const & rhs);
	Vector4i operator|(Vector4i const & lhs, Vector4i const & rhs);
	Vector4i operator^(Vector4i const & lhs, Vector4i const & rhs);
	Vector4i operator%(Vector4i const & lhs, Vector4i const & rhs);

	Vector4i operator+(Vector4i const & lhs, int32_t rhs);
	Vector4i operator-(Vector4i const & lhs, int32_t rhs);
	Vector4i operator*(Vector4i const & lhs, int32_t rhs);
	Vector4i operator/(Vector4i const & lhs, int32_t rhs);
	Vector4i operator&(Vector4i const & lhs, int32_t rhs);
	Vector4i operator|(Vector4i const & lhs, int32_t rhs);
	Vector4i operator^(Vector4i const & lhs, int32_t rhs);
	Vector4i operator%(Vector4i const & lhs, int32_t rhs);

	Vector4i operator>>(Vector4i const & lhs, int32_t rhs);
	Vector4i operator<<(Vector4i const & lhs, int32_t rhs);

	Vector4i operator>>(Vector4i const & lhs, Vector4i const & rhs);
	Vector4i operator<<(Vector4i const & lhs, Vector4i const & rhs);

	Vector4i operator+(int32_t lhs, Vector4i const & rhs);
	Vector4i operator-(int32_t lhs, Vector4i const & rhs);
	Vector4i operator*(int32_t lhs, Vector4i const & rhs);
	Vector4i operator/(int32_t lhs, Vector4i const & rhs);
	Vector4i operator&(int32_t lhs, Vector4i const & rhs);
	Vector4i operator|(int32_t lhs, Vector4i const & rhs);
	Vector4i operator^(int32_t lhs, Vector4i const & rhs);
	Vector4i operator%(int32_t lhs, Vector4i const & rhs);

	Vector4i & operator+=(Vector4i & lhs, Vector4i const & rhs);
	Vector4i & operator-=(Vector4i & lhs, Vector4i const & rhs);
	Vector4i & operator*=(Vector4i & lhs, Vector4i const & rhs);
	Vector4i & operator/=(Vector4i & lhs, Vector4i const & rhs);
	Vector4i & operator&=(Vector4i & lhs, Vector4i const & rhs);
	Vector4i & operator|=(Vector4i & lhs, Vector4i const & rhs);
	Vector4i & operator^=(Vector4i & lhs, Vector4i const & rhs);
	Vector4i & operator%=(Vector4i & lhs, Vector4i const & rhs);

	Vector4i & operator+=(Vector4i & lhs, int32_t rhs);
	Vector4i & operator-=(Vector4i & lhs, int32_t rhs);
	Vector4i & operator*=(Vector4i & lhs, int32_t rhs);
	Vector4i & operator/=(Vector4i & lhs, int32_t rhs);
	Vector4i & operator&=(Vector4i & lhs, int32_t rhs);
	Vector4i & operator|=(Vector4i & lhs, int32_t rhs);
	Vector4i & operator^=(Vector4i & lhs, int32_t rhs);
	Vector4i & operator%=(Vector4i & lhs, int32_t rhs);

	// Postfix
	Vector4i   operator++(Vector4i & lhs, int i);
	Vector4i   operator--(Vector4i & lhs, int i);

	// Prefix
	Vector4i & operator++(Vector4i & lhs);
	Vector4i & operator--(Vector4i & lhs);

	bool operator< (Vector4i const & v1, Vector4i const & v2);

	bool operator==(Vector4i const & lhs, Vector4i const & rhs);
	bool operator!=(Vector4i const & lhs, Vector4i const & rhs);

	bool operator==(Vector4i const & lhs, int32_t rhs);
	bool operator!=(Vector4i const & lhs, int32_t rhs);

	bool operator==(int32_t lhs, Vector4i const & rhs);
	bool operator!=(int32_t lhs, Vector4i const & rhs);
}

namespace std {
	inline ::HANDYMATH_NS::Vector4i min(::HANDYMATH_NS::Vector4i const & a, ::HANDYMATH_NS::Vector4i const & b);
	inline ::HANDYMATH_NS::Vector4i max(::HANDYMATH_NS::Vector4i const & a, ::HANDYMATH_NS::Vector4i const & b);
}

static inline std::ostream & operator<<(std::ostream & os, const ::HANDYMATH_NS::Vector4i & v)  
{  
	os << "(" << v.X << ", " << v.Y << ", " << v.Z << ", " << v.W << ")";  
	return os;  
}  

namespace HANDYMATH_NS {

	FORCEINLINE Vector4i::Vector4i(ARCHIT4 const & native) : Native(native)   { }
	FORCEINLINE Vector4i::operator ARCHIT4() const { return Native; }

	FORCEINLINE Vector4i::Vector4i() : Native(Vector4i::Zero()) { }
	FORCEINLINE Vector4i::Vector4i(Vector4i const & v) : Native(v.Native) { }

	#if defined USE_SIMD
	FORCEINLINE Vector4i::Vector4i(int32_t xyzw) : Native(_mm_set1_epi32(xyzw)) { }
	FORCEINLINE Vector4i::Vector4i(int32_t xIn, int32_t yIn, int32_t zIn, int32_t wIn) : Native(_mm_setr_epi32(xIn, yIn, zIn, wIn)) { }
	FORCEINLINE Vector4i::Vector4i(int32_t const * ary) : Native(_mm_loadu_si128((__m128i const *)ary)) { }
	#else
	FORCEINLINE Vector4i::Vector4i(int32_t xyzw) : Native { xyzw, xyzw, xyzw, xyzw } { }
	FORCEINLINE Vector4i::Vector4i(int32_t xIn, int32_t yIn, int32_t zIn, int32_t wIn) : Native { xIn, yIn, zIn, wIn } { }
	FORCEINLINE Vector4i::Vector4i(int32_t const * ary) : Native { ary[0], ary[1], ary[2], ary[3] } { }
	#endif

	FORCEINLINE Vector4i::Vector4i(size_t xIn, size_t yIn, size_t zIn, size_t wIn) : Vector4i(int32_t(xIn), int32_t(yIn), int32_t(zIn), int32_t(wIn)) { }


	FORCEINLINE Vector4i Vector4i::FromArray(int32_t const * ary) { return Vector4i(ary); }

	FORCEINLINE void Vector4i::CopyToArray(int32_t * ary) const 
	{
		SIMD_IMP    (_mm_storeu_si128((__m128i *)ary, Native);)
		NON_SIMD_IMP(memcpy(ary, &Array[0], sizeof(int32_t) * 4);)
	}

	template<int i0, int i1, int i2, int i3> FORCEINLINE Vector4i Vector4i::Shuffle() const
	{
		SIMD_IMP    (return _mm_shuffle_epi32(Native, RESHUFFLE(i0, i1, i2, i3));)
		NON_SIMD_IMP(return Vector4i(Array[i0], Array[i1], Array[i2], Array[i3]);)
	}

	template<int i> FORCEINLINE Vector4i Vector4i::Insert(int32_t f) const
	{
		SIMD_IMP_SSE41   (return _mm_insert_epi32(Native, f, i);)
		SIMD_IMP_NO_SSE41(auto n = *this; n.Array[i] = f; return n;)
		NON_SIMD_IMP     (auto n = *this; n.Array[i] = f; return n;)
	}

	template<int i> FORCEINLINE Vector4i Vector4i::InsertZero() const
	{
		SIMD_IMP_SSE41   (return _mm_insert_epi32(Native, 0_i32, i);)
		SIMD_IMP_NO_SSE41(auto n = *this; n.Array[i] = 0_i32; return n;)
		NON_SIMD_IMP     (auto n = *this; n.Array[i] = 0_i32; return n;)
	}

	FORCEINLINE Vector4i Vector4i::InsertX(int32_t xIn) const { return Insert<0>(xIn); }
	FORCEINLINE Vector4i Vector4i::InsertY(int32_t yIn) const { return Insert<1>(yIn); }
	FORCEINLINE Vector4i Vector4i::InsertZ(int32_t zIn) const { return Insert<2>(zIn); }
	FORCEINLINE Vector4i Vector4i::InsertW(int32_t wIn) const { return Insert<3>(wIn); }

	FORCEINLINE Vector4i Vector4i::X000() const { return XY00().InsertZero<1>(); }
	FORCEINLINE Vector4i Vector4i::XY00() const
	{
		SIMD_IMP    (return _mm_move_epi64(Native);)
		NON_SIMD_IMP(return Vector4i(X, Y, 0_i32, 0_i32);)
	}
	FORCEINLINE Vector4i Vector4i::XY01() const { return XY00().InsertW(1_i32); }
	FORCEINLINE Vector4i Vector4i::XY10() const { return XY00().InsertZ(1_i32); }
	FORCEINLINE Vector4i Vector4i::XY11() const { return InsertZ(1_i32).InsertW(1_i32); }
	FORCEINLINE Vector4i Vector4i::XYZ0() const { return InsertZero<3>(); }
	FORCEINLINE Vector4i Vector4i::XYZ1() const { return InsertW(1_i32); }

	FORCEINLINE Vector4i Vector4i::XXYY() const 
	{
		SIMD_IMP    (return _mm_unpacklo_epi32(Native, Native);)
		NON_SIMD_IMP(return Vector4i(X, X, Y, Y);)
	}

	FORCEINLINE Vector4i Vector4i::ZZWW() const 
	{
		SIMD_IMP    (return _mm_unpackhi_epi32(Native, Native);)
		NON_SIMD_IMP(return Vector4i(Z, Z, W, W);)
	}

	FORCEINLINE Vector4i Vector4i::XYXY() const 
	{
		SIMD_IMP    (return _mm_unpacklo_epi64(Native, Native);)
		NON_SIMD_IMP(return Vector4i(X, Y, X, Y);)
	}
	
	FORCEINLINE Vector4i Vector4i::ZWZW() const
	{
		SIMD_IMP    (return _mm_unpackhi_epi64(Native, Native);)
		NON_SIMD_IMP(return Vector4i(Z, W, Z, W);)
	}

	FORCEINLINE Vector4i Vector4i::ZCWD(Vector4i const & abcd) const
	{
		SIMD_IMP    (return _mm_unpackhi_epi32(Native, abcd);)
		NON_SIMD_IMP(return Vector4i(Z, abcd.Z, W, abcd.W);)
	}

	FORCEINLINE Vector4i Vector4i::XYAB(Vector4i const & abcd) const
	{
		SIMD_IMP    (return _mm_unpacklo_epi64(Native, abcd);)
		NON_SIMD_IMP(return Vector4i(X, Y, abcd.X, abcd.Y);)
	}

	FORCEINLINE Vector4i Vector4i::ZWCD(Vector4i const & abcd) const
	{
		SIMD_IMP    (return _mm_unpackhi_epi64(Native, abcd);)
		NON_SIMD_IMP(return Vector4i(Z, W, abcd.Z, abcd.W);)
	}

	FORCEINLINE Vector4i Vector4i::XAYB(Vector4i const & abcd) const
	{
		SIMD_IMP    (return _mm_unpacklo_epi32(Native, abcd);)
		NON_SIMD_IMP(return Vector4i(X, abcd.X, Y, abcd.Y);)
	}

	FORCEINLINE Vector4i Vector4i::XXXX() const { return Shuffle<0, 0, 0, 0>(); }
	FORCEINLINE Vector4i Vector4i::YYYY() const { return Shuffle<1, 1, 1, 1>(); }
	FORCEINLINE Vector4i Vector4i::ZZZZ() const { return Shuffle<2, 2, 2, 2>(); }
	FORCEINLINE Vector4i Vector4i::WWWW() const { return Shuffle<3, 3, 3, 3>(); }

	FORCEINLINE Vector4i Vector4i::WZYX() const { return Shuffle<3, 2, 1, 0>(); }
	FORCEINLINE Vector4i Vector4i::ZWXY() const { return Shuffle<2, 3, 0, 1>(); }

	FORCEINLINE Vector4i Vector4i::Min(Vector4i const & rhs) const
	{
		SIMD_IMP_SSE41   (return _mm_min_epi32(Native, rhs);)
		SIMD_IMP_NO_SSE41(return Vector4i(Handy::FastMin(X, rhs.X), Handy::FastMin(Y, rhs.Y), Handy::FastMin(Z, rhs.Z), Handy::FastMin(W, rhs.W));)
		NON_SIMD_IMP     (return Vector4i(Handy::FastMin(X, rhs.X), Handy::FastMin(Y, rhs.Y), Handy::FastMin(Z, rhs.Z), Handy::FastMin(W, rhs.W));)
	}

	FORCEINLINE Vector4i Vector4i::Max(Vector4i const & rhs) const
	{
		SIMD_IMP_SSE41   (return _mm_max_epi32(Native, rhs);)
		SIMD_IMP_NO_SSE41(return Vector4i(Handy::FastMax(X, rhs.X), Handy::FastMax(Y, rhs.Y), Handy::FastMax(Z, rhs.Z), Handy::FastMax(W, rhs.W));)
		NON_SIMD_IMP     (return Vector4i(Handy::FastMax(X, rhs.X), Handy::FastMax(Y, rhs.Y), Handy::FastMax(Z, rhs.Z), Handy::FastMax(W, rhs.W));)
	}

	FORCEINLINE Vector4i Vector4i::Min(int32_t rhs) const
	{
		SIMD_IMP_SSE41   (return _mm_min_epi32(Native, _mm_set1_epi32(rhs));)
		SIMD_IMP_NO_SSE41(return Vector4i(Handy::FastMin(X, rhs), Handy::FastMin(Y, rhs), Handy::FastMin(Z, rhs), Handy::FastMin(W, rhs));)
		NON_SIMD_IMP     (return Vector4i(Handy::FastMin(X, rhs), Handy::FastMin(Y, rhs), Handy::FastMin(Z, rhs), Handy::FastMin(W, rhs));)
	}

	FORCEINLINE Vector4i Vector4i::Max(int32_t rhs) const
	{
		SIMD_IMP_SSE41   (return _mm_max_epi32(Native, _mm_set1_epi32(rhs));)
		SIMD_IMP_NO_SSE41(return Vector4i(Handy::FastMax(X, rhs), Handy::FastMax(Y, rhs), Handy::FastMax(Z, rhs), Handy::FastMax(W, rhs));)
		NON_SIMD_IMP     (return Vector4i(Handy::FastMax(X, rhs), Handy::FastMax(Y, rhs), Handy::FastMax(Z, rhs), Handy::FastMax(W, rhs));)
	}

	FORCEINLINE int32_t Vector4i::Min() const 
	{
		SIMD_IMP_SSE41   (auto abba = WZYX().Min(Native); return _mm_extract_epi32(abba.Min(abba.ZWXY()), 0);)
		SIMD_IMP_NO_SSE41(auto abba = WZYX().Min(Native); return _mm_cvtsi128_si32(abba.Min(abba.ZWXY()));)
		NON_SIMD_IMP     (return Handy::FastMin(Handy::FastMin(X, Y), Handy::FastMin(Z, W));)
	}

	FORCEINLINE int32_t Vector4i::Max() const 
	{
		SIMD_IMP_SSE41   (auto abba = WZYX().Max(Native); return _mm_extract_epi32(abba.Max(abba.ZWXY()), 0);)
		SIMD_IMP_NO_SSE41(auto abba = WZYX().Max(Native); return _mm_cvtsi128_si32(abba.Max(abba.ZWXY()));)
		NON_SIMD_IMP     (return Handy::FastMax(Handy::FastMax(X, Y), Handy::FastMax(Z, W));)
	}


	FORCEINLINE int32_t Vector4i::Sum() const
	{
		#if defined USE_SIMD
			#if defined(USE_SSSE3)
				auto temp = _mm_hadd_epi32(Native, Native);
				#if defined(USE_SSE41)
					return _mm_extract_epi32(_mm_hadd_epi32(temp, temp), 0);
				#else
					return _mm_cvtsi128_si32(_mm_hadd_epi32(temp, temp));
				#endif
			#else
				__m128i hi64  = _mm_unpackhi_epi64(Native, Native);
				__m128i sum64 = _mm_add_epi32(hi64, Native);
				__m128i hi32  = _mm_shufflelo_epi16(sum64, _MM_SHUFFLE(1, 0, 3, 2));
				__m128i sum32 = _mm_add_epi32(sum64, hi32);
				#if defined(USE_SSE41)
					return _mm_extract_epi32(sum32, 0);
				#else
					return _mm_cvtsi128_si32(sum32);
				#endif
			#endif
		#endif
		NON_SIMD_IMP(return X + Y + Z + W;)
	}

	FORCEINLINE int32_t Vector4i::Product() const
	{
		    SIMD_IMP(auto abba = WZYX() * Vector4i(Native); return _mm_cvtsi128_si32(abba * abba.ZWXY());)
		NON_SIMD_IMP(return X * Y * Z * W;)
	}

	FORCEINLINE Vector4i Vector4i::Abs() const
	{
		SIMD_IMP_SSSE3   (return _mm_abs_epi32(Native);)
		SIMD_IMP_NO_SSSE3(Vector4i temp = (*this) >> 31; return ((*this) ^ temp) - temp;)
		NON_SIMD_IMP     (return Vector4i(std::abs(X), std::abs(Y), std::abs(Z), std::abs(W));)
	}

	FORCEINLINE Vector4i Vector4i::Sign() const
	{
		return Vector4i(X >= 0 ? 1 : -1, Y >= 0 ? 1 : -1, Z >= 0 ? 1 : -1, W >= 0 ? 1 : -1);
	}

	template <CMPI op>
	FORCEINLINE bool Vector4i::Compare(int32_t rhs) const
	{
		return Compare<op>(Vector4i(rhs));
	}

	template <CMPI op>
	FORCEINLINE bool Vector4i::Compare(Vector4i const & rhs) const
	{
		/// IMPORTANT: If any of these are upgraded, the implementation in the Vector2-3* versions need to be changed too.
		if (op == CMPI::AnyLT)  { return X <  rhs.X || Y <  rhs.Y || Z <  rhs.Z || W <  rhs.W; }
		if (op == CMPI::AllLT)  { return X <  rhs.X && Y <  rhs.Y && Z <  rhs.Z && W <  rhs.W; }
		if (op == CMPI::AnyLEQ) { return X <= rhs.X || Y <= rhs.Y || Z <= rhs.Z || W <= rhs.W; }
		if (op == CMPI::AllLEQ) { return X <= rhs.X && Y <= rhs.Y && Z <= rhs.Z && W <= rhs.W; }
		if (op == CMPI::AnyGT)  { return X >  rhs.X || Y >  rhs.Y || Z >  rhs.Z || W >  rhs.W; }
		if (op == CMPI::AllGT)  { return X >  rhs.X && Y >  rhs.Y && Z >  rhs.Z && W >  rhs.W; }
		if (op == CMPI::AnyGEQ) { return X >= rhs.X || Y >= rhs.Y || Z >= rhs.Z || W >= rhs.W; }
		if (op == CMPI::AllGEQ) { return X >= rhs.X && Y >= rhs.Y && Z >= rhs.Z && W >= rhs.W; }
		if (op == CMPI::AnyEQ)  { return X == rhs.X || Y == rhs.Y || Z == rhs.Z || W == rhs.W; }
		if (op == CMPI::AllEQ)
		{
			SIMD_IMP_SSE41   (auto vcmp = _mm_xor_si128(Native, rhs); return _mm_testz_si128(vcmp, vcmp) != 0;)
			SIMD_IMP_NO_SSE41(return 0xFFFF_i32 == _mm_movemask_epi8(_mm_cmpeq_epi8(Native, rhs));)
			NON_SIMD_IMP     (return X == rhs.X && Y == rhs.Y && Z == rhs.Z && W == rhs.W;)
		}

		if (op == CMPI::AnyNEQ)
		{
			SIMD_IMP_SSE41   (auto vcmp = _mm_xor_si128(Native, rhs); return _mm_testz_si128(vcmp, vcmp) == 0;)
			SIMD_IMP_NO_SSE41(return 0xFFFF_i32 != _mm_movemask_epi8(_mm_cmpeq_epi8(Native, rhs));)
			NON_SIMD_IMP     (return X != rhs.X || Y != rhs.Y || Z != rhs.Z || W != rhs.W;)
		}
		if (op == CMPI::AllNEQ) { return X != rhs.X && Y != rhs.Y && Z != rhs.Z && W != rhs.W; }

		// Used for storing Vectors in collections. Compare NWE=>LT, X, then Y, then.... Default op for operator< on vector types.
		if (op == CMPI::Ordinal)
		{
			if (X != rhs.X) return X < rhs.X;
			if (Y != rhs.Y) return Y < rhs.Y;
			if (Z != rhs.Z) return Z < rhs.Z;
			if (W != rhs.W) return W < rhs.W;
			return false;
		} 

	}

	FORCEINLINE Vector4i Vector4i::To(Vector4i const & rhs) const { return rhs - (*this); }

	FORCEINLINE int32_t  Vector4i::TaxicabDistance(Vector4i const & rhs) const { return (rhs - (*this)).TaxicabLength(); }
	FORCEINLINE int32_t  Vector4i::TaxicabLength() const { return Abs().Sum(); }

	FORCEINLINE int32_t  Vector4i::Dot(Vector4i const & rhs) const { return ((*this) * rhs).Sum(); }

	FORCEINLINE Vector4i Vector4i::Power(int32_t n) const { return Vector4i((int32_t)std::pow(X, n), (int32_t)std::pow(Y, n), (int32_t)std::pow(Z, n), (int32_t)std::pow(W, n)); }

	FORCEINLINE Vector4i Vector4i::BitwiseAndNot(Vector4i const & rhs) const
	{
		    SIMD_IMP(return _mm_andnot_si128(rhs, Native);)
		NON_SIMD_IMP(return (*this) & (~rhs);)
	}
	
	FORCEINLINE bool Vector4i::HasMaxValue() const { return Compare<CMPI::AnyEQ>(std::numeric_limits<int32_t>::max()); }
	FORCEINLINE bool Vector4i::HasMinValue() const { return Compare<CMPI::AnyEQ>(std::numeric_limits<int32_t>::min()); }
	FORCEINLINE bool Vector4i::HasMinMaxValue() const { return HasMaxValue() || HasMinValue(); }

	FORCEINLINE Vector4i Vector4i::Clamp(Vector4i const & min, Vector4i const & max) const { return Max(min).Min(max); }
	FORCEINLINE Vector4i Vector4i::Clamp(int32_t          min, int32_t          max) const { return Max(min).Min(max); }

	FORCEINLINE int32_t & Vector4i::operator[](int32_t idx)       { return Array[idx]; }
	FORCEINLINE int32_t   Vector4i::operator[](int32_t idx) const { return Array[idx]; }

	/*static*/ FORCEINLINE Vector4i Vector4i::Zero()
	{
		    SIMD_IMP(return _mm_setzero_si128();)
		NON_SIMD_IMP(return Vector4i(0_i32);)
	}
	/*static*/ FORCEINLINE Vector4i Vector4i::One()      { return Vector4i( 1_i32); }
	/*static*/ FORCEINLINE Vector4i Vector4i::NegOne()   { return Vector4i(-1_i32); }
	/*static*/ FORCEINLINE Vector4i Vector4i::UnitX()    { return Vector4i( 1_i32,  0_i32,  0_i32,  0_i32); }
	/*static*/ FORCEINLINE Vector4i Vector4i::UnitY()    { return Vector4i( 0_i32,  1_i32,  0_i32,  0_i32); }
	/*static*/ FORCEINLINE Vector4i Vector4i::UnitZ()    { return Vector4i( 0_i32,  0_i32,  1_i32,  0_i32); }
	/*static*/ FORCEINLINE Vector4i Vector4i::UnitW()    { return Vector4i( 0_i32,  0_i32,  0_i32,  1_i32); }
	/*static*/ FORCEINLINE Vector4i Vector4i::UnitXW()   { return Vector4i( 1_i32,  0_i32,  0_i32,  1_i32); }
	/*static*/ FORCEINLINE Vector4i Vector4i::UnitYW()   { return Vector4i( 0_i32,  1_i32,  0_i32,  1_i32); }
	/*static*/ FORCEINLINE Vector4i Vector4i::UnitZW()   { return Vector4i( 0_i32,  0_i32,  1_i32,  1_i32); }
	/*static*/ FORCEINLINE Vector4i Vector4i::NegUnitX() { return Vector4i(-1_i32,  0_i32,  0_i32,  0_i32); }
	/*static*/ FORCEINLINE Vector4i Vector4i::NegUnitY() { return Vector4i( 0_i32, -1_i32,  0_i32,  0_i32); }
	/*static*/ FORCEINLINE Vector4i Vector4i::NegUnitZ() { return Vector4i( 0_i32,  0_i32, -1_i32,  0_i32); }
	/*static*/ FORCEINLINE Vector4i Vector4i::NegUnitW() { return Vector4i( 0_i32,  0_i32,  0_i32, -1_i32); }
	/*static*/ FORCEINLINE Vector4i Vector4i::MaxValue() { return Vector4i( std::numeric_limits<int32_t>::max()); }
	/*static*/ FORCEINLINE Vector4i Vector4i::MinValue() { return Vector4i( std::numeric_limits<int32_t>::min()); }
	/*static*/ FORCEINLINE Vector4i Vector4i::FFFFFFFF()
	{
		    SIMD_IMP(return _mm_cmpeq_epi8(Zero(), Zero());)
		NON_SIMD_IMP(return Vector4i( 0xFFFFFFFF_i32);)
	}

	FORCEINLINE Vector4i operator-(Vector4i const & lhs) 
	{
		    SIMD_IMP(return _mm_sub_epi32(_mm_setzero_si128(), lhs);)
		NON_SIMD_IMP(return Vector4i(-lhs.X, -lhs.Y, -lhs.Z, -lhs.W);)
	}

	FORCEINLINE Vector4i operator~(Vector4i const & lhs)
	{
		    SIMD_IMP(return _mm_andnot_si128(lhs, Vector4i::FFFFFFFF());)
		NON_SIMD_IMP(return Vector4i(~lhs.X, ~lhs.Y, ~lhs.Z, ~lhs.W);)
	}

	FORCEINLINE Vector4i operator+(Vector4i const & lhs, Vector4i const & rhs)
	{
		    SIMD_IMP(return _mm_add_epi32(lhs, rhs);)
		NON_SIMD_IMP(return Vector4i(lhs.X + rhs.X, lhs.Y + rhs.Y, lhs.Z + rhs.Z, lhs.W + rhs.W);)
	}

	FORCEINLINE Vector4i operator-(Vector4i const & lhs, Vector4i const & rhs)
	{
		    SIMD_IMP(return _mm_sub_epi32(lhs, rhs);)
		NON_SIMD_IMP(return Vector4i(lhs.X - rhs.X, lhs.Y - rhs.Y, lhs.Z - rhs.Z, lhs.W - rhs.W);)
	}

	FORCEINLINE Vector4i operator*(Vector4i const & lhs, Vector4i const & rhs)
	{
		SIMD_IMP_SSE41   (return _mm_mullo_epi32(lhs, rhs);)
		SIMD_IMP_NO_SSE41(return Vector4i(lhs.X * rhs.X, lhs.Y * rhs.Y, lhs.Z * rhs.Z, lhs.W * rhs.W);)
		NON_SIMD_IMP     (return Vector4i(lhs.X * rhs.X, lhs.Y * rhs.Y, lhs.Z * rhs.Z, lhs.W * rhs.W);)
	}

	FORCEINLINE Vector4i operator&(Vector4i const & lhs, Vector4i const & rhs)
	{
		    SIMD_IMP(return _mm_and_si128(lhs, rhs);)
		NON_SIMD_IMP(return Vector4i(lhs.X & rhs.X, lhs.Y & rhs.Y, lhs.Z & rhs.Z, lhs.W & rhs.W);)
	}

	FORCEINLINE Vector4i operator|(Vector4i const & lhs, Vector4i const & rhs)
	{
		    SIMD_IMP(return _mm_or_si128 (lhs, rhs);)
		NON_SIMD_IMP(return Vector4i(lhs.X | rhs.X, lhs.Y | rhs.Y, lhs.Z | rhs.Z, lhs.W | rhs.W);)
	}
	
	FORCEINLINE Vector4i operator^(Vector4i const & lhs, Vector4i const & rhs)
	{
		    SIMD_IMP(return _mm_xor_si128(lhs, rhs);)
		NON_SIMD_IMP(return Vector4i(lhs.X ^ rhs.X, lhs.Y ^ rhs.Y, lhs.Z ^ rhs.Z, lhs.W ^ rhs.W);)
	}

	FORCEINLINE Vector4i operator>>(Vector4i const & lhs, int32_t rhs)
	{
		    SIMD_IMP(return _mm_srai_epi32(lhs, rhs);)
		NON_SIMD_IMP(return Vector4i(lhs.X >> rhs, lhs.Y >> rhs, lhs.Z >> rhs, lhs.W >> rhs);)
	}

	FORCEINLINE Vector4i operator<<(Vector4i const & lhs, int32_t rhs)
	{
		    SIMD_IMP(return _mm_slli_epi32(lhs, rhs);)
		NON_SIMD_IMP(return Vector4i(lhs.X << rhs, lhs.Y << rhs, lhs.Z << rhs, lhs.W << rhs);)
	}

	FORCEINLINE Vector4i operator>>(Vector4i const & lhs, Vector4i const & rhs)
	{
		SIMD_IMP_AVX2   (return _mm_srlv_epi32(lhs, rhs);)
		SIMD_IMP_NO_AVX2(return Vector4i(lhs.X >> rhs.X, lhs.Y >> rhs.Y, lhs.Z >> rhs.Z, lhs.W >> rhs.W);)
		NON_SIMD_IMP    (return Vector4i(lhs.X >> rhs.X, lhs.Y >> rhs.Y, lhs.Z >> rhs.Z, lhs.W >> rhs.W);)
	}

	FORCEINLINE Vector4i operator<<(Vector4i const & lhs, Vector4i const & rhs)
	{
		SIMD_IMP_AVX2   (return _mm_sllv_epi32(lhs, rhs);)
		SIMD_IMP_NO_AVX2(return Vector4i(lhs.X << rhs.X, lhs.Y << rhs.Y, lhs.Z << rhs.Z, lhs.W << rhs.W);)
		NON_SIMD_IMP    (return Vector4i(lhs.X << rhs.X, lhs.Y << rhs.Y, lhs.Z << rhs.Z, lhs.W << rhs.W);)
	}
	
	// Nothing available in SSE to directly do these :(
	FORCEINLINE Vector4i operator/(Vector4i const & lhs, Vector4i const & rhs) { return Vector4i(lhs.X / rhs.X, lhs.Y / rhs.Y, lhs.Z / rhs.Z, lhs.W / rhs.W); }
	FORCEINLINE Vector4i operator%(Vector4i const & lhs, Vector4i const & rhs) { return Vector4i(lhs.X % rhs.X, lhs.Y % rhs.Y, lhs.Z % rhs.Z, lhs.W % rhs.W); }

	FORCEINLINE Vector4i operator+(Vector4i const & lhs, int32_t rhs) { return lhs + Vector4i(rhs); }
	FORCEINLINE Vector4i operator-(Vector4i const & lhs, int32_t rhs) { return lhs - Vector4i(rhs); }
	FORCEINLINE Vector4i operator*(Vector4i const & lhs, int32_t rhs) { return lhs * Vector4i(rhs); }
	FORCEINLINE Vector4i operator/(Vector4i const & lhs, int32_t rhs) { return lhs / Vector4i(rhs); }
	FORCEINLINE Vector4i operator&(Vector4i const & lhs, int32_t rhs) { return lhs & Vector4i(rhs); }
	FORCEINLINE Vector4i operator|(Vector4i const & lhs, int32_t rhs) { return lhs | Vector4i(rhs); }
	FORCEINLINE Vector4i operator^(Vector4i const & lhs, int32_t rhs) { return lhs ^ Vector4i(rhs); }
	FORCEINLINE Vector4i operator%(Vector4i const & lhs, int32_t rhs) { return lhs % Vector4i(rhs); }

	FORCEINLINE Vector4i operator+(int32_t lhs, Vector4i const & rhs) { return Vector4i(lhs) + rhs; }
	FORCEINLINE Vector4i operator-(int32_t lhs, Vector4i const & rhs) { return Vector4i(lhs) - rhs; }
	FORCEINLINE Vector4i operator*(int32_t lhs, Vector4i const & rhs) { return Vector4i(lhs) * rhs; }
	FORCEINLINE Vector4i operator/(int32_t lhs, Vector4i const & rhs) { return Vector4i(lhs) / rhs; }
	FORCEINLINE Vector4i operator&(int32_t lhs, Vector4i const & rhs) { return Vector4i(lhs) & rhs; }
	FORCEINLINE Vector4i operator|(int32_t lhs, Vector4i const & rhs) { return Vector4i(lhs) | rhs; }
	FORCEINLINE Vector4i operator^(int32_t lhs, Vector4i const & rhs) { return Vector4i(lhs) ^ rhs; }
	FORCEINLINE Vector4i operator%(int32_t lhs, Vector4i const & rhs) { return Vector4i(lhs) % rhs; }

	FORCEINLINE Vector4i & operator+=(Vector4i & lhs, Vector4i const & rhs) { lhs = lhs + rhs; return lhs; }
	FORCEINLINE Vector4i & operator-=(Vector4i & lhs, Vector4i const & rhs) { lhs = lhs - rhs; return lhs; }
	FORCEINLINE Vector4i & operator*=(Vector4i & lhs, Vector4i const & rhs) { lhs = lhs * rhs; return lhs; }
	FORCEINLINE Vector4i & operator/=(Vector4i & lhs, Vector4i const & rhs) { lhs = lhs / rhs; return lhs; }
	FORCEINLINE Vector4i & operator&=(Vector4i & lhs, Vector4i const & rhs) { lhs = lhs & rhs; return lhs; }
	FORCEINLINE Vector4i & operator|=(Vector4i & lhs, Vector4i const & rhs) { lhs = lhs | rhs; return lhs; }
	FORCEINLINE Vector4i & operator^=(Vector4i & lhs, Vector4i const & rhs) { lhs = lhs ^ rhs; return lhs; }
	FORCEINLINE Vector4i & operator%=(Vector4i & lhs, Vector4i const & rhs) { lhs = lhs % rhs; return lhs; }

	FORCEINLINE Vector4i & operator+=(Vector4i & lhs, int32_t rhs) { lhs = lhs + rhs; return lhs; }
	FORCEINLINE Vector4i & operator-=(Vector4i & lhs, int32_t rhs) { lhs = lhs - rhs; return lhs; }
	FORCEINLINE Vector4i & operator*=(Vector4i & lhs, int32_t rhs) { lhs = lhs * rhs; return lhs; }
	FORCEINLINE Vector4i & operator/=(Vector4i & lhs, int32_t rhs) { lhs = lhs / rhs; return lhs; }
	FORCEINLINE Vector4i & operator&=(Vector4i & lhs, int32_t rhs) { lhs = lhs & rhs; return lhs; }
	FORCEINLINE Vector4i & operator|=(Vector4i & lhs, int32_t rhs) { lhs = lhs | rhs; return lhs; }
	FORCEINLINE Vector4i & operator^=(Vector4i & lhs, int32_t rhs) { lhs = lhs ^ rhs; return lhs; }
	FORCEINLINE Vector4i & operator%=(Vector4i & lhs, int32_t rhs) { lhs = lhs % rhs; return lhs; }

	// Postfix
	FORCEINLINE Vector4i   operator++(Vector4i & lhs, int i) { Vector4i t = lhs; lhs = lhs + 1_i32; return t; }
	FORCEINLINE Vector4i   operator--(Vector4i & lhs, int i) { Vector4i t = lhs; lhs = lhs - 1_i32; return t; }

	// Prefix
	FORCEINLINE Vector4i & operator++(Vector4i & lhs) { lhs = lhs + 1_i32; return lhs; }
	FORCEINLINE Vector4i & operator--(Vector4i & lhs) { lhs = lhs - 1_i32; return lhs; }

	FORCEINLINE bool operator< (Vector4i const & lhs, Vector4i const & rhs) { return lhs.Compare<CMPI::Ordinal>(rhs); }

	FORCEINLINE bool operator==(Vector4i const & lhs, Vector4i const & rhs) { return lhs.Compare<CMPI::AllEQ> (rhs); }
	FORCEINLINE bool operator!=(Vector4i const & lhs, Vector4i const & rhs) { return lhs.Compare<CMPI::AnyNEQ>(rhs); }

	FORCEINLINE bool operator==(Vector4i const & lhs, int32_t rhs) { return lhs == Vector4i(rhs); }
	FORCEINLINE bool operator!=(Vector4i const & lhs, int32_t rhs) { return lhs != Vector4i(rhs); }

	FORCEINLINE bool operator==(int32_t lhs, Vector4i const & rhs) { return Vector4i(lhs) == rhs; }
	FORCEINLINE bool operator!=(int32_t lhs, Vector4i const & rhs) { return Vector4i(lhs) != rhs; }
}

// Not supposed to add overloads to std technically... but I want my templates that use std::to_string, min, max, etc to work, so oh well then.
namespace std {
	FORCEINLINE ::HANDYMATH_NS::Vector4i min(::HANDYMATH_NS::Vector4i const & a, ::HANDYMATH_NS::Vector4i const & b) { return a.Min(b); }
	FORCEINLINE ::HANDYMATH_NS::Vector4i max(::HANDYMATH_NS::Vector4i const & a, ::HANDYMATH_NS::Vector4i const & b) { return a.Max(b); }

	FORCEINLINE std::string to_string(::HANDYMATH_NS::Vector4i const & v)
	{
		return "("s + std::to_string(v.X) + ", "s + std::to_string(v.Y) + ", "s + std::to_string(v.Z) + ", "s + std::to_string(v.W) + ")"s;
	}
}

MAKE_HASHABLE(HANDYMATH_NS::Vector4i, t.X, t.Y, t.Z, t.W)
