
/// See ../../License.txt for license info.

#pragma once

#include <limits>
#include <cstdint>

#include "Base.hpp"

namespace HANDYMATH_NS
{

struct RGBA8888;

struct alignas(4) RGB888
{
	union
	{
		uint32_t UInt;
		int32_t   Int; 
		struct { uint8_t R, G, B, _zero; };
		std::array<uint8_t, 4> Array;

	};

	RGB888();

	explicit RGB888(uint8_t v);

	RGB888(uint8_t r, uint8_t g, uint8_t b);

	~RGB888()                            = default;
	RGB888 & operator =(RGB888 const &) = default;

	RGB888 Max(RGB888 const & v) const;
	RGB888 Min(RGB888 const & v) const;

	bool IsZero() const;

	RGB888 Clamped(RGB888 const & min, RGB888 const & max) const;

	//RGB888 Pow(uint8_t const n) const
	//{
	//    return RGB888(std::pow(R, n), std::pow(G, n), std::pow(B, n));
	//}

	RGBA8888 ToRGBA(uint8_t a = 255) const;

	uint8_t & operator[](int idx);
	uint8_t   operator[](int idx) const;

	static RGB888 Black();
	static RGB888 White();

	static RGB888 Red();
	static RGB888 Green();
	static RGB888 Blue();

	static RGB888 Cyan();
	static RGB888 Magenta();
	static RGB888 Yellow();
};

bool operator<(RGB888 const & lhs, RGB888 const & rhs) ;

RGB888 operator+(RGB888 const & lhs);

RGB888 operator+(RGB888 const & lhs, RGB888 const & rhs);
RGB888 operator-(RGB888 const & lhs, RGB888 const & rhs);
RGB888 operator*(RGB888 const & lhs, RGB888 const & rhs);
RGB888 operator/(RGB888 const & lhs, RGB888 const & rhs);
RGB888 operator%(RGB888 const & lhs, RGB888 const & rhs);

RGB888 & operator+=(RGB888 const & lhs, RGB888 const & rhs);
RGB888 & operator-=(RGB888 const & lhs, RGB888 const & rhs);
RGB888 & operator*=(RGB888 const & lhs, RGB888 const & rhs);
RGB888 & operator/=(RGB888 const & lhs, RGB888 const & rhs);
RGB888 & operator%=(RGB888 const & lhs, RGB888 const & rhs);

bool operator ==(RGB888 const & lhs, RGB888 const & rhs);
bool operator !=(RGB888 const & lhs, RGB888 const & rhs);

// ---

FORCEINLINE RGB888::RGB888() : UInt(0u) { }
FORCEINLINE RGB888::RGB888(uint8_t v) : R(v), G(v), B(v), _zero(0) { }
FORCEINLINE RGB888::RGB888(uint8_t r, uint8_t g, uint8_t b) : R(r), G(g), B(b), _zero(0) { }

FORCEINLINE RGB888 RGB888::Max(RGB888 const & v) const
{
	return RGB888(
		Handy::FastMax(R, v.R),
		Handy::FastMax(G, v.G),
		Handy::FastMax(B, v.B));
}

FORCEINLINE RGB888 RGB888::Min(RGB888 const & v) const
{
	return RGB888(
		Handy::FastMin(R, v.R),
		Handy::FastMin(G, v.G),
		Handy::FastMin(B, v.B));
}

FORCEINLINE bool RGB888::IsZero() const { return UInt == 0; }

FORCEINLINE RGB888 RGB888::Clamped(RGB888 const & minV, RGB888 const & maxV) const { return this->Max(minV).Min(maxV); }

FORCEINLINE bool operator<(RGB888 const & lhs, RGB888 const & rhs) { return lhs.UInt < rhs.UInt; }

FORCEINLINE RGB888 operator+(RGB888 const & lhs) { return lhs; }
FORCEINLINE RGB888 operator+(RGB888 const & lhs, RGB888 const & rhs) { return RGB888(lhs.R + rhs.R, lhs.G + rhs.G, lhs.B + rhs.B); }
FORCEINLINE RGB888 operator-(RGB888 const & lhs, RGB888 const & rhs) { return RGB888(lhs.R - rhs.R, lhs.G - rhs.G, lhs.B - rhs.B); }
FORCEINLINE RGB888 operator*(RGB888 const & lhs, RGB888 const & rhs) { return RGB888(lhs.R * rhs.R, lhs.G * rhs.G, lhs.B * rhs.B); }
FORCEINLINE RGB888 operator/(RGB888 const & lhs, RGB888 const & rhs) { return RGB888(lhs.R / rhs.R, lhs.G / rhs.G, lhs.B / rhs.B); }
FORCEINLINE RGB888 operator%(RGB888 const & lhs, RGB888 const & rhs) { return RGB888(lhs.R % rhs.R, lhs.G % rhs.G, lhs.B % rhs.B); }

FORCEINLINE RGB888 & operator+=(RGB888 & lhs, RGB888 const & rhs) { lhs = lhs + rhs; return lhs; }
FORCEINLINE RGB888 & operator-=(RGB888 & lhs, RGB888 const & rhs) { lhs = lhs - rhs; return lhs; }
FORCEINLINE RGB888 & operator*=(RGB888 & lhs, RGB888 const & rhs) { lhs = lhs * rhs; return lhs; }
FORCEINLINE RGB888 & operator/=(RGB888 & lhs, RGB888 const & rhs) { lhs = lhs / rhs; return lhs; }
FORCEINLINE RGB888 & operator%=(RGB888 & lhs, RGB888 const & rhs) { lhs = lhs % rhs; return lhs; }

FORCEINLINE bool      operator ==(RGB888 const & lhs, RGB888 const & rhs) { return lhs.UInt == rhs.UInt; }
FORCEINLINE bool      operator !=(RGB888 const & lhs, RGB888 const & rhs) { return !operator==(lhs, rhs); }

FORCEINLINE uint8_t & RGB888::operator[](int idx)       { return (&R)[idx]; }
FORCEINLINE uint8_t   RGB888::operator[](int idx) const { return (&R)[idx]; }

FORCEINLINE /*static*/ RGB888 RGB888::Black()        { return RGB888(  0u,   0u,   0u); }
FORCEINLINE /*static*/ RGB888 RGB888::White()        { return RGB888(255u, 255u, 255u); }

FORCEINLINE /*static*/ RGB888 RGB888::Red()          { return RGB888(255u,   0u,   0u); }
FORCEINLINE /*static*/ RGB888 RGB888::Green()        { return RGB888(  0u, 255u,   0u); }
FORCEINLINE /*static*/ RGB888 RGB888::Blue()         { return RGB888(  0u,   0u, 255u); }

FORCEINLINE /*static*/ RGB888 RGB888::Cyan()         { return RGB888(  0u, 255u, 255u); }
FORCEINLINE /*static*/ RGB888 RGB888::Magenta()      { return RGB888(255u,   0u, 255u); }
FORCEINLINE /*static*/ RGB888 RGB888::Yellow()       { return RGB888(255u, 255u,   0u); }

}

MAKE_HASHABLE(HANDYMATH_NS::RGB888, t.R, t.G, t.B)
