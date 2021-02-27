
/// See ../../License.txt for license info.

#pragma once

#include <limits>
#include <cstdint>

#include "Base.hpp"

namespace HANDYMATH_NS
{

struct RGB888;

struct alignas(4) RGBA8888
{
	union
	{
		uint32_t UInt;
		int32_t   Int; 
		struct { uint8_t R, G, B, A; };
		std::array<uint8_t, 4> Array;
	};
	
	RGBA8888();
	RGBA8888(RGBA8888 const & rhs) : UInt(rhs.UInt) { }

	explicit RGBA8888(uint32_t v);

	RGBA8888(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	~RGBA8888()                             = default;
	RGBA8888 & operator =(RGBA8888 const & rhs)
	{
		UInt = rhs.UInt;
		return *this;
	}

	RGBA8888 Max(RGBA8888 const & v) const;
	RGBA8888 Min(RGBA8888 const & v) const;

	bool IsZero() const;

	RGBA8888 Clamped(RGBA8888 const & min, RGBA8888 const & max) const;

	//RGBA8888 Pow(uint8_t const n) const
	//{
	//	return RGBA8888(std::pow(R, n), std::pow(G, n), std::pow(B, n), std::pow(A, n));
	//}

	RGB888 ToRGB() const;

	uint8_t & operator[](int idx);
	uint8_t   operator[](int idx) const;

	static RGBA8888 ClearBlack();
	static RGBA8888 ClearWhite();

	static RGBA8888 ClearRed();
	static RGBA8888 ClearGreen();
	static RGBA8888 ClearBlue();

	static RGBA8888 ClearCyan();
	static RGBA8888 ClearMagenta();
	static RGBA8888 ClearYellow();

	static RGBA8888 Black();
	static RGBA8888 White();

	static RGBA8888 Red();
	static RGBA8888 Green();
	static RGBA8888 Blue();

	static RGBA8888 Cyan();
	static RGBA8888 Magenta();
	static RGBA8888 Yellow();
	static RGBA8888 Gray();
};

bool operator<(RGBA8888 const & lhs, RGBA8888 const & rhs) ;

RGBA8888 operator+(RGBA8888 const & lhs);

RGBA8888 operator+(RGBA8888 const & lhs, RGBA8888 const & rhs);
RGBA8888 operator-(RGBA8888 const & lhs, RGBA8888 const & rhs);
RGBA8888 operator*(RGBA8888 const & lhs, RGBA8888 const & rhs);
RGBA8888 operator/(RGBA8888 const & lhs, RGBA8888 const & rhs);
RGBA8888 operator%(RGBA8888 const & lhs, RGBA8888 const & rhs);

RGBA8888 & operator+=(RGBA8888 const & lhs, RGBA8888 const & rhs);
RGBA8888 & operator-=(RGBA8888 const & lhs, RGBA8888 const & rhs);
RGBA8888 & operator*=(RGBA8888 const & lhs, RGBA8888 const & rhs);
RGBA8888 & operator/=(RGBA8888 const & lhs, RGBA8888 const & rhs);
RGBA8888 & operator%=(RGBA8888 const & lhs, RGBA8888 const & rhs);

bool operator ==(RGBA8888 const & lhs, RGBA8888 const & rhs);
bool operator !=(RGBA8888 const & lhs, RGBA8888 const & rhs);

// ---

FORCEINLINE RGBA8888::RGBA8888() : UInt(0u) { }
FORCEINLINE RGBA8888::RGBA8888(uint32_t v) : UInt(v) { }
FORCEINLINE RGBA8888::RGBA8888(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : R(r), G(g), B(b), A(a) { }

FORCEINLINE RGBA8888 RGBA8888::Max(RGBA8888 const & v) const
{
	return RGBA8888(
		Handy::FastMax(R, v.R),
		Handy::FastMax(G, v.G),
		Handy::FastMax(B, v.B),
		Handy::FastMax(A, v.A));
}

FORCEINLINE RGBA8888 RGBA8888::Min(RGBA8888 const & v) const
{
	return RGBA8888(
		Handy::FastMin(R, v.R),
		Handy::FastMin(G, v.G),
		Handy::FastMin(B, v.B),
		Handy::FastMin(A, v.A));
}

FORCEINLINE bool RGBA8888::IsZero() const { return UInt == 0; }

FORCEINLINE RGBA8888 RGBA8888::Clamped(RGBA8888 const & minV, RGBA8888 const & maxV) const { return this->Max(minV).Min(maxV); }

FORCEINLINE bool operator<(RGBA8888 const & lhs, RGBA8888 const & rhs) { return lhs.UInt < rhs.UInt; }

FORCEINLINE RGBA8888 operator+(RGBA8888 const & lhs) { return lhs; }
FORCEINLINE RGBA8888 operator+(RGBA8888 const & lhs, RGBA8888 const & rhs) { return RGBA8888(lhs.R + rhs.R, lhs.G + rhs.G, lhs.B + rhs.B, lhs.A + rhs.A); }
FORCEINLINE RGBA8888 operator-(RGBA8888 const & lhs, RGBA8888 const & rhs) { return RGBA8888(lhs.R - rhs.R, lhs.G - rhs.G, lhs.B - rhs.B, lhs.A - rhs.A); }
FORCEINLINE RGBA8888 operator*(RGBA8888 const & lhs, RGBA8888 const & rhs) { return RGBA8888(lhs.R * rhs.R, lhs.G * rhs.G, lhs.B * rhs.B, lhs.A * rhs.A); }
FORCEINLINE RGBA8888 operator/(RGBA8888 const & lhs, RGBA8888 const & rhs) { return RGBA8888(lhs.R / rhs.R, lhs.G / rhs.G, lhs.B / rhs.B, lhs.A / rhs.A); }
FORCEINLINE RGBA8888 operator%(RGBA8888 const & lhs, RGBA8888 const & rhs) { return RGBA8888(lhs.R % rhs.R, lhs.G % rhs.G, lhs.B % rhs.B, lhs.A % rhs.A); }

FORCEINLINE RGBA8888 & operator+=(RGBA8888 & lhs, RGBA8888 const & rhs) { lhs = lhs + rhs; return lhs; }
FORCEINLINE RGBA8888 & operator-=(RGBA8888 & lhs, RGBA8888 const & rhs) { lhs = lhs - rhs; return lhs; }
FORCEINLINE RGBA8888 & operator*=(RGBA8888 & lhs, RGBA8888 const & rhs) { lhs = lhs * rhs; return lhs; }
FORCEINLINE RGBA8888 & operator/=(RGBA8888 & lhs, RGBA8888 const & rhs) { lhs = lhs / rhs; return lhs; }
FORCEINLINE RGBA8888 & operator%=(RGBA8888 & lhs, RGBA8888 const & rhs) { lhs = lhs % rhs; return lhs; }

FORCEINLINE bool      operator ==(RGBA8888 const & lhs, RGBA8888 const & rhs) { return lhs.UInt == rhs.UInt; }
FORCEINLINE bool      operator !=(RGBA8888 const & lhs, RGBA8888 const & rhs) { return !operator==(lhs, rhs); }

FORCEINLINE uint8_t & RGBA8888::operator[](int idx)       { return (&R)[idx]; }
FORCEINLINE uint8_t   RGBA8888::operator[](int idx) const { return (&R)[idx]; }

FORCEINLINE /*static*/ RGBA8888 RGBA8888::ClearBlack()   { return RGBA8888(  0u,   0u,   0u,   0u); }
FORCEINLINE /*static*/ RGBA8888 RGBA8888::ClearWhite()   { return RGBA8888(  0u,   0u,   0u,   0u); }

FORCEINLINE /*static*/ RGBA8888 RGBA8888::ClearRed()     { return RGBA8888(255u,   0u,   0u,   0u); }
FORCEINLINE /*static*/ RGBA8888 RGBA8888::ClearGreen()   { return RGBA8888(  0u, 255u,   0u,   0u); }
FORCEINLINE /*static*/ RGBA8888 RGBA8888::ClearBlue()    { return RGBA8888(  0u,   0u, 255u,   0u); }

FORCEINLINE /*static*/ RGBA8888 RGBA8888::ClearCyan()    { return RGBA8888(  0u, 255u, 255u,   0u); }
FORCEINLINE /*static*/ RGBA8888 RGBA8888::ClearMagenta() { return RGBA8888(255u,   0u, 255u,   0u); }
FORCEINLINE /*static*/ RGBA8888 RGBA8888::ClearYellow()  { return RGBA8888(255u, 255u,   0u,   0u); }

FORCEINLINE /*static*/ RGBA8888 RGBA8888::Black()        { return RGBA8888(  0u,   0u,   0u, 255u); }
FORCEINLINE /*static*/ RGBA8888 RGBA8888::White()        { return RGBA8888(255u, 255u, 255u, 255u); }

FORCEINLINE /*static*/ RGBA8888 RGBA8888::Red()          { return RGBA8888(255u,   0u,   0u, 255u); }
FORCEINLINE /*static*/ RGBA8888 RGBA8888::Green()        { return RGBA8888(  0u, 255u,   0u, 255u); }
FORCEINLINE /*static*/ RGBA8888 RGBA8888::Blue()         { return RGBA8888(  0u,   0u, 255u, 255u); }

FORCEINLINE /*static*/ RGBA8888 RGBA8888::Cyan()         { return RGBA8888(  0u, 255u, 255u, 255u); }
FORCEINLINE /*static*/ RGBA8888 RGBA8888::Magenta()      { return RGBA8888(255u,   0u, 255u, 255u); }
FORCEINLINE /*static*/ RGBA8888 RGBA8888::Yellow()       { return RGBA8888(255u, 255u,   0u, 255u); }

FORCEINLINE /*static*/ RGBA8888 RGBA8888::Gray()         { return RGBA8888(127u, 127u, 127u, 255u); }

}

MAKE_HASHABLE(HANDYMATH_NS::RGBA8888, t.UInt)
