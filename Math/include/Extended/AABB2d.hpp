
/// See ../../License.txt for license info.

#pragma once

#include <cmath>
#include <vector>

#include "../Core/Vector2d.hpp"

namespace HANDYMATH_NS
{

	struct AABB2d
	{
		Vector2d Min;
		Vector2d Max;

		AABB2d();
		AABB2d(Vector2d const & point);
		AABB2d(std::vector<Vector2d> const & points);
		AABB2d(Vector2d const & min, Vector2d const & max);

		bool    IsEverything() const;
		bool    IsNothing()    const;
		bool    IsPoint()      const;

		Vector2d PointCenter()  const;
		Vector2d PointN()  const;
		Vector2d PointS()  const;
		Vector2d PointE()  const;
		Vector2d PointW()  const;
		Vector2d PointNW() const;
		Vector2d PointNE() const;
		Vector2d PointSE() const;
		Vector2d PointSW() const;

		Vector2d Size()     const;
		Vector2d HalfSize() const;

		double   Area()      const;
		double   Perimeter() const;

		void    AddPoint(Vector2d const & p);
		void    AddPoints(std::vector<Vector2d> const & ps);
		void    AddAABB(AABB2d const & aabb);

		bool    Envelopes (AABB2d const & aabb) const;
		bool    Intersects(AABB2d const & aabb) const;
		bool    IntersectsFastFinite(AABB2d const & aabb) const;
		bool    Intersects(Vector2d const & v) const;

		AABB2d   Intersection(AABB2d const & aabb) const;

		static AABB2d Everything();
		static AABB2d Nothing();
	};

	FORCEINLINE AABB2d::AABB2d()                                    : Min(Vector2d::NaN()), Max(Vector2d::NaN()) { }
	FORCEINLINE AABB2d::AABB2d(Vector2d const & point)               : AABB2d()               { AddPoint(point); }
	FORCEINLINE AABB2d::AABB2d(std::vector<Vector2d> const & points) : AABB2d() { AddPoints(points); }
	FORCEINLINE AABB2d::AABB2d(Vector2d const & min, Vector2d const & max) : Min(min), Max(max) { }

	FORCEINLINE bool AABB2d::IsEverything() const { return Min.IsNegativeInfinity() && Max.IsPositiveInfinity(); }
	FORCEINLINE bool AABB2d::IsNothing()    const { return Min.IsNaN() || Max.IsNaN(); }
	FORCEINLINE bool AABB2d::IsPoint()      const { return Min == Max; }

	FORCEINLINE Vector2d AABB2d::PointCenter() const { return (Min + Max) * 0.5f; }
	FORCEINLINE Vector2d AABB2d::PointN()      const { return Vector2d((Min.X + Max.X) * 0.5f, Max.Y); }
	FORCEINLINE Vector2d AABB2d::PointS()      const { return Vector2d((Min.X + Max.X) * 0.5f, Min.Y); }
	FORCEINLINE Vector2d AABB2d::PointE()      const { return Vector2d( Max.X,                (Min.Y + Max.Y) * 0.5f); }
	FORCEINLINE Vector2d AABB2d::PointW()      const { return Vector2d( Min.X,                (Min.Y + Max.Y) * 0.5f); }
	FORCEINLINE Vector2d AABB2d::PointNW()     const { return Vector2d( Min.X, Max.Y); }
	FORCEINLINE Vector2d AABB2d::PointNE()     const { return Max; }
	FORCEINLINE Vector2d AABB2d::PointSE()     const { return Vector2d(Max.X, Min.Y); }
	FORCEINLINE Vector2d AABB2d::PointSW()     const { return Min; }

	FORCEINLINE Vector2d AABB2d::Size()        const { return (Max - Min); }
	FORCEINLINE Vector2d AABB2d::HalfSize()    const { return (Max - Min) * 0.5f; }

	FORCEINLINE double AABB2d::Area()          const { return Size().Product(); }
	FORCEINLINE double AABB2d::Perimeter()     const { return Size().Sum() * 2.0f; }

	FORCEINLINE void AABB2d::AddPoint(Vector2d const & point)
	{
		if (point.HasNaN())
			throw std::runtime_error("Cannot add NaN to AABB2d.");

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

	FORCEINLINE void AABB2d::AddPoints(std::vector<Vector2d> const & points)
	{
		if (IsEverything())
			return;

		for (auto const & point : points)
			AddPoint(point);
	}

	FORCEINLINE void AABB2d::AddAABB(AABB2d const & aabb)
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


	FORCEINLINE bool AABB2d::Envelopes(AABB2d const & aabb) const
	{
		if (IsNothing() || aabb.IsNothing())
			return false;

		if (IsEverything())
			return true;

		return Min.X <= aabb.Min.X && Min.Y <= aabb.Min.Y &&
			Max.X >= aabb.Max.X && Max.Y >= aabb.Max.Y;
	}

	FORCEINLINE bool AABB2d::Intersects(AABB2d const & aabb) const
	{
		if (IsNothing() || aabb.IsNothing())
			return false;

		if (IsEverything() || aabb.IsEverything())
			return true;

		return !(Max.X < aabb.Min.X || Min.X > aabb.Max.X ||
				 Max.Y < aabb.Min.Y || Min.Y > aabb.Max.Y);
	}

	FORCEINLINE bool AABB2d::IntersectsFastFinite(AABB2d const & aabb) const
	{
		//return !(Max.Compare<Math::CMP::AnyLT>(aabb.Min) || Min.Compare<Math::CMP::AnyGT>(aabb.Max));
		return !(Max.X < aabb.Min.X || Min.X > aabb.Max.X ||
				 Max.Y < aabb.Min.Y || Min.Y > aabb.Max.Y);
	}

	FORCEINLINE AABB2d AABB2d::Intersection(AABB2d const & aabb) const
	{
		return AABB2d(Min.Max(aabb.Min), Max.Min(aabb.Max));
	}

	FORCEINLINE bool AABB2d::Intersects(Vector2d const & v) const
	{
		return Intersects(AABB2d(v));
	}

	FORCEINLINE /*static*/ AABB2d AABB2d::Everything() { return AABB2d(Vector2d::NegativeInfinity(), Vector2d::PositiveInfinity()); }
	FORCEINLINE /*static*/ AABB2d AABB2d::Nothing()    { return AABB2d(Vector2d::NaN(), Vector2d::NaN()); }

}


namespace std {

	FORCEINLINE std::string to_string(::HANDYMATH_NS::AABB2d const & v)
	{
		return "["s + std::to_string(v.Min) + "; "s + std::to_string(v.Max) + "]"s;
	}
}
