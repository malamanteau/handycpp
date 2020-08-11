
/// See ../../License.txt for license info.

#pragma once

#include <cmath>
#include <vector>

#include "../Core/Vector2i.hpp"

namespace HANDYMATH_NS
{

	struct AABB2i
	{
		Vector2i Min;
		Vector2i Max;

		AABB2i();
		AABB2i(Vector2i const & point);
		AABB2i(std::vector<Vector2i> const & points);
		AABB2i(Vector2i const & min, Vector2i const & max);

		bool     IsEverything() const;
		bool     IsNothing()    const;
		bool     IsPoint()      const;

		Vector2i PointNW() const;
		Vector2i PointNE() const;
		Vector2i PointSE() const;
		Vector2i PointSW() const;

		Vector2i Size()     const;

		int32_t  Area()      const;
		int32_t  Perimeter() const;

		void     AddPoint(Vector2i const & p);
		void     AddPoints(std::vector<Vector2i> const & ps);
		void     AddAABB(AABB2i const & aabb);

		bool     Envelopes (AABB2i const & aabb) const;
		bool     Intersects(AABB2i const & aabb) const;
		bool     Intersects(Vector2i const & v) const;

		AABB2i   Intersection(AABB2i const & aabb) const;

		static AABB2i Everything();
		static AABB2i Nothing();
	};

	FORCEINLINE AABB2i::AABB2i()                                     : Min(Vector2i::MinValue()), Max(Vector2i::MinValue()) { }
	FORCEINLINE AABB2i::AABB2i(Vector2i const & point)               : AABB2i()               { AddPoint (point); }
	FORCEINLINE AABB2i::AABB2i(std::vector<Vector2i> const & points) : AABB2i() { AddPoints(points); }
	FORCEINLINE AABB2i::AABB2i(Vector2i const & min, Vector2i const & max)           : Min(min), Max(max) { }

	FORCEINLINE bool AABB2i::IsEverything() const { return Min == Vector2i::MinValue() && Max == Vector2i::MaxValue(); }
	FORCEINLINE bool AABB2i::IsNothing()    const { return Min == Vector2i::MinValue() && Max == Vector2i::MinValue();  }
	FORCEINLINE bool AABB2i::IsPoint()      const { return Min == Max; }

	FORCEINLINE Vector2i AABB2i::PointNW()  const { return Vector2i( Min.X, Max.Y); }
	FORCEINLINE Vector2i AABB2i::PointNE()  const { return Max; }
	FORCEINLINE Vector2i AABB2i::PointSE()  const { return Vector2i(Max.X, Min.Y); }
	FORCEINLINE Vector2i AABB2i::PointSW()  const { return Min; }

	FORCEINLINE Vector2i AABB2i::Size()     const { return (Max - Min); }

	FORCEINLINE int32_t AABB2i::Area()      const { return Size().Product(); }
	FORCEINLINE int32_t AABB2i::Perimeter() const { return Size().Sum() * 2; }

	FORCEINLINE void AABB2i::AddPoint(Vector2i const & point)
	{
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

	FORCEINLINE void AABB2i::AddPoints(std::vector<Vector2i> const & points)
	{
		if (IsEverything())
			return;

		for (auto const & point : points)
			AddPoint(point);
	}

	FORCEINLINE void AABB2i::AddAABB(AABB2i const & aabb)
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

	FORCEINLINE bool AABB2i::Envelopes(AABB2i const & aabb) const
	{
		if (IsNothing() || aabb.IsNothing())
			return false;

		if (IsEverything())
			return true;

		return Min.X <= aabb.Min.X && Min.Y <= aabb.Min.Y &&
			Max.X >= aabb.Max.X && Max.Y >= aabb.Max.Y;
	}

	FORCEINLINE bool AABB2i::Intersects(AABB2i const & aabb) const
	{
		if (IsNothing() || aabb.IsNothing())
			return false;

		if (IsEverything() || aabb.IsEverything())
			return true;

		return !(Max.X < aabb.Min.X || Min.X > aabb.Max.X ||
		         Max.Y < aabb.Min.Y || Min.Y > aabb.Max.Y);
	}

	FORCEINLINE bool AABB2i::Intersects(Vector2i const & v) const
	{
		return Intersects(AABB2i(v));
	}

	FORCEINLINE AABB2i AABB2i::Intersection(AABB2i const & aabb) const
	{
		return AABB2i(Min.Max(aabb.Min), Max.Min(aabb.Max));
	}

	FORCEINLINE /*static*/ AABB2i AABB2i::Everything() { return AABB2i(Vector2i::MinValue(), Vector2i::MaxValue()); }
	FORCEINLINE /*static*/ AABB2i AABB2i::Nothing()    { return AABB2i(Vector2i::FFFFFFFF(), Vector2i::FFFFFFFF()); }

}
