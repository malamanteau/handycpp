
/// See ../../License.txt for license info.

#pragma once

#include <cmath>
#include <vector>

#include "../Core/Vector2.hpp"

namespace HANDYMATH_NS
{

struct AABB2
{
	Vector2 Min;
	Vector2 Max;

	AABB2();
	AABB2(Vector2 const & point);
	AABB2(std::vector<Vector2> const & points);
	AABB2(Vector2 const & min, Vector2 const & max);

	bool    IsEverything() const;
	bool    IsNothing()    const;
	bool    IsPoint()      const;

	Vector2 PointCenter()  const;
	Vector2 PointN()  const;
	Vector2 PointS()  const;
	Vector2 PointE()  const;
	Vector2 PointW()  const;
	Vector2 PointNW() const;
	Vector2 PointNE() const;
	Vector2 PointSE() const;
	Vector2 PointSW() const;

	Vector2 Size()     const;
	Vector2 HalfSize() const;
	
	float   Area()      const;
	float   Perimeter() const;

	void    AddPoint(Vector2 const & p);
	void    AddPoints(std::vector<Vector2> const & ps);
	void    AddAABB(AABB2 const & aabb);

	bool    Envelopes (AABB2 const & aabb) const;
	bool    Intersects(AABB2 const & aabb) const;
	bool    Intersects(Vector2 const & v) const;
	
	AABB2   Intersection(AABB2 const & aabb) const;

	static AABB2 Everything();
	static AABB2 Nothing();
};

FORCEINLINE AABB2::AABB2()                                    : Min(Vector2::NaN()), Max(Vector2::NaN()) { }
FORCEINLINE AABB2::AABB2(Vector2 const & point)               : AABB2()               { AddPoint(point); }
FORCEINLINE AABB2::AABB2(std::vector<Vector2> const & points) : AABB2() { AddPoints(points); }
FORCEINLINE AABB2::AABB2(Vector2 const & min, Vector2 const & max) : Min(min), Max(max) { }

FORCEINLINE bool AABB2::IsEverything() const { return Min.IsNegativeInfinity() && Max.IsPositiveInfinity(); }
FORCEINLINE bool AABB2::IsNothing()    const { return Min.IsNaN() || Max.IsNaN(); }
FORCEINLINE bool AABB2::IsPoint()      const { return Min == Max; }

FORCEINLINE Vector2 AABB2::PointCenter() const { return (Min + Max) * 0.5f; }
FORCEINLINE Vector2 AABB2::PointN()      const { return Vector2((Min.X + Max.X) * 0.5f, Max.Y); }
FORCEINLINE Vector2 AABB2::PointS()      const { return Vector2((Min.X + Max.X) * 0.5f, Min.Y); }
FORCEINLINE Vector2 AABB2::PointE()      const { return Vector2( Max.X,                (Min.Y + Max.Y) * 0.5f); }
FORCEINLINE Vector2 AABB2::PointW()      const { return Vector2( Min.X,                (Min.Y + Max.Y) * 0.5f); }
FORCEINLINE Vector2 AABB2::PointNW()     const { return Vector2( Min.X, Max.Y); }
FORCEINLINE Vector2 AABB2::PointNE()     const { return Max; }
FORCEINLINE Vector2 AABB2::PointSE()     const { return Vector2(Max.X, Min.Y); }
FORCEINLINE Vector2 AABB2::PointSW()     const { return Min; }

FORCEINLINE Vector2 AABB2::Size()        const { return (Max - Min); }
FORCEINLINE Vector2 AABB2::HalfSize()    const { return (Max - Min) * 0.5f; }

FORCEINLINE float AABB2::Area()          const { return Size().Product(); }
FORCEINLINE float AABB2::Perimeter()     const { return Size().Sum() * 2.0f; }

FORCEINLINE void AABB2::AddPoint(Vector2 const & point)
{
	if (point.HasNaN())
		throw std::runtime_error("Cannot add NaN to AABB2.");

	if (IsEverything())// || (IsNothing() && point.HasNaN()))
		return;

	if (IsNothing())
	{
		Min = Max = point;
		return;
	}

	Min = Min.Min(point);
	Max = Max.Max(point);
}

FORCEINLINE void AABB2::AddPoints(std::vector<Vector2> const & points)
{
	if (IsEverything())
		return;

	for (auto const & point : points)
		AddPoint(point);
}

FORCEINLINE void AABB2::AddAABB(AABB2 const & aabb)
{
	if (aabb.IsNothing() || IsEverything())
		return;

	if (aabb.IsEverything() || IsNothing())
	{
		*this = aabb;
		return;
	}

	AddPoint(Min);
	AddPoint(Max);
}


FORCEINLINE bool AABB2::Envelopes(AABB2 const & aabb) const
{
	if (IsNothing() || aabb.IsNothing())
		return false;

	if (IsEverything())
		return true;

	return Min.X <= aabb.Min.X && Min.Y <= aabb.Min.Y &&
		Max.X >= aabb.Max.X && Max.Y >= aabb.Max.Y;
}

FORCEINLINE bool AABB2::Intersects(AABB2 const & aabb) const
{
	if (IsNothing() || aabb.IsNothing())
		return false;

	if (IsEverything() || aabb.IsEverything())
		return true;

	return !(Max.X < aabb.Min.X || Min.X > aabb.Max.X ||
		     Max.Y < aabb.Min.Y || Min.Y > aabb.Max.Y);
}

FORCEINLINE AABB2 AABB2::Intersection(AABB2 const & aabb) const
{
	return AABB2(Min.Max(aabb.Min), Max.Min(aabb.Max));
}

FORCEINLINE bool AABB2::Intersects(Vector2 const & v) const
{
	return Intersects(AABB2(v));
}

FORCEINLINE /*static*/ AABB2 AABB2::Everything() { return AABB2(Vector2::NegativeInfinity(), Vector2::PositiveInfinity()); }
FORCEINLINE /*static*/ AABB2 AABB2::Nothing()    { return AABB2(Vector2::NaN(), Vector2::NaN()); }

}
