
/// See ../../License.txt for license info.

#pragma once

#include <cmath>
#include <vector>

#include "../Core/Vector3.hpp"

namespace HANDYMATH_NS
{

struct AABB3
{
	Vector3 Min;
	Vector3 Max;

	AABB3();
	AABB3(Vector3 const & point);
	AABB3(std::vector<Vector3> const & points);
	AABB3(Vector3 const & min, Vector3 const & max);

	bool    IsEverything() const;
	bool    IsNothing()    const;
	bool    IsPoint()      const;

	Vector3 PointCenter()  const;

	Vector3 Size()     const;
	Vector3 HalfSize() const;

	float   Volume()      const;
	float   SurfaceArea() const;

	void    AddPoint(Vector3 const & p);
	void    AddPoints(std::vector<Vector3> const & ps);
	void    AddAABB(AABB3 const & aabb);

	bool    Envelopes (AABB3 const & aabb) const;
	bool    Intersects(AABB3 const & aabb) const;
	bool    Intersects(Vector3 const & v) const;

	static AABB3 Everything();
	static AABB3 Nothing();
};

FORCEINLINE AABB3::AABB3()                                    : Min(Vector3::NaN()), Max(Vector3::NaN()) { }
FORCEINLINE AABB3::AABB3(Vector3 const & point)               : AABB3()               { AddPoint(point); }
FORCEINLINE AABB3::AABB3(std::vector<Vector3> const & points) : AABB3() { AddPoints(points); }
FORCEINLINE AABB3::AABB3(Vector3 const & min, Vector3 const & max) : Min(min), Max(max) { }

FORCEINLINE bool AABB3::IsEverything() const { return Min.IsNegativeInfinity() && Max.IsPositiveInfinity(); }
FORCEINLINE bool AABB3::IsNothing()    const { return Min.IsNaN() || Max.IsNaN(); }
FORCEINLINE bool AABB3::IsPoint()      const { return Min == Max; }

FORCEINLINE Vector3 AABB3::PointCenter() const { return (Min + Max) * 0.5f; }

FORCEINLINE Vector3 AABB3::Size()        const { return (Max - Min); }
FORCEINLINE Vector3 AABB3::HalfSize()    const { return (Max - Min) * 0.5f; }

FORCEINLINE float AABB3::Volume()          const { return Size().Product(); }
FORCEINLINE float AABB3::SurfaceArea()     const { Vector3 sz = Size(); return 2.0f * (sz.X * sz.Y + sz.Y * sz.Z + sz.X * sz.Z); }

FORCEINLINE void AABB3::AddPoint(Vector3 const & point)
{
	if (point.HasNaN())
		throw std::runtime_error("Cannot add NaN to AABB2.");

	if (IsEverything())
		return;

	if (IsNothing())
	{
		Min = Max = point;
		return;
	}

	Min = Min.Min(point);
	Max = Max.Max(point);
}

FORCEINLINE void AABB3::AddPoints(std::vector<Vector3> const & points)
{
	if (IsEverything())
		return;

	for (auto const & point : points)
		AddPoint(point);
}

FORCEINLINE void AABB3::AddAABB(AABB3 const & aabb)
{
	if (aabb.IsNothing() || IsEverything())
		return;

	if (aabb.IsEverything() || IsNothing())
	{
		*this = aabb;
		return;
	}

	AddPoint(aabb.Min);
	AddPoint(aabb.Max);
}

FORCEINLINE bool AABB3::Envelopes(AABB3 const & aabb) const
{
	if (IsNothing() || aabb.IsNothing())
		return false;

	if (IsEverything())
		return true;

	return Min.X <= aabb.Min.X && Min.Y <= aabb.Min.Y && Min.Z <= aabb.Min.Z &&
	       Max.X >= aabb.Max.X && Max.Y >= aabb.Max.Y && Max.Z >= aabb.Max.Z;
}

FORCEINLINE bool AABB3::Intersects(AABB3 const & aabb) const
{
	if (IsNothing() || aabb.IsNothing())
		return false;

	if (IsEverything() || aabb.IsEverything())
		return true;

	return !(Max.X < aabb.Min.X || Min.X > aabb.Max.X ||
	         Max.Y < aabb.Min.Y || Min.Y > aabb.Max.Y ||
	         Max.Z < aabb.Min.Z || Min.Z > aabb.Max.Z);
}

FORCEINLINE bool AABB3::Intersects(Vector3 const & v) const
{
	return Intersects(AABB3(v));
}

FORCEINLINE /*static*/ AABB3 AABB3::Everything() { return AABB3(Vector3::NegativeInfinity(), Vector3::PositiveInfinity()); }
FORCEINLINE /*static*/ AABB3 AABB3::Nothing()    { return AABB3(Vector3::NaN(), Vector3::NaN()); }

}
