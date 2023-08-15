
/// See ../../License.txt for license info.

#pragma once

#include <cmath>
#include <vector>

#include "../Core/Vector2.hpp"

namespace HANDYMATH_NS
{
	class LineSimplification
	{
	public:
		static BitContinuum SimplifySimple(std::vector<Vector2d> const & points, double tolerance);

		static std::vector<bool>    DouglasPeucker(std::vector<Vector2> const & points, float epsilon);
		static std::vector<bool>    DouglasPeucker(std::vector<Vector2d> const & points, double epsilon);

	private:
		static std::vector<bool>    DouglasPeucker(std::vector<Vector2>  const & points,  float epsilon2, size_t startIndex, size_t lastIndex);
		static std::vector<bool>    DouglasPeucker(std::vector<Vector2d> const & points, double epsilon2, size_t startIndex, size_t lastIndex);

		static float PointLineDistance(Vector2 point, Vector2 p1, Vector2 p2);
		static double PointLineDistance(Vector2d point, Vector2d p1, Vector2d p2);

		static float PointLineDistance2(Vector2 point, Vector2 p1, Vector2 p2);
		static double PointLineDistance2(Vector2d point, Vector2d p1, Vector2d p2);
	};

	FORCEINLINE BitContinuum LineSimplification::SimplifySimple(std::vector<Vector2d> const & points, double tolerance)
	{
		BitContinuum ret;

		size_t const n = points.size();

		double tol2 = tolerance * tolerance;

		switch (n)
		{
			case 2: ret.set_bit(1, true); [[fallthrough]];
			case 1: ret.set_bit(0, true); [[fallthrough]];
			case 0: return ret;
		}

		size_t const nm1 = n - 1;

		ret.set_bit(0, true);
		ret.set_bit(nm1, true);

		size_t lastKeep = 0;

		for (size_t idx = 1; idx < nm1; ++idx)
		{
			if (points[lastKeep].DistanceSquared(points[idx]) < tol2)
				continue;

			lastKeep = idx;
			ret.set_bit(idx);
		}

		return std::move(ret);
	}

	FORCEINLINE float LineSimplification::PointLineDistance(Vector2 point, Vector2 p1, Vector2 p2) 
	{
		if (p1 == p2)
			return point.Distance(p1);

		Vector2 AB = p2 - p1; 
		return (AB * (point - p1)).Length() / AB.Length(); 
	}


	FORCEINLINE double LineSimplification::PointLineDistance(Vector2d point, Vector2d p1, Vector2d p2) 
	{
		if (p1 == p2)
			return point.Distance(p1);

		Vector2d AB = p2 - p1; 
		return (AB * (point - p1)).Length() / AB.Length(); 
	}


	FORCEINLINE float LineSimplification::PointLineDistance2(Vector2 point, Vector2 p1, Vector2 p2) 
	{
		if (p1 == p2)
			return point.DistanceSquared(p1);

		Vector2 AB = p2 - p1; 
		return (AB * (point - p1)).LengthSquared() / AB.LengthSquared(); 
	}


	FORCEINLINE double LineSimplification::PointLineDistance2(Vector2d point, Vector2d p1, Vector2d p2) 
	{
		if (p1 == p2)
			return point.DistanceSquared(p1);

		Vector2d AB = p2 - p1; 
		return (AB * (point - p1)).LengthSquared() / AB.LengthSquared(); 
	}


	FORCEINLINE std::vector<bool> LineSimplification::DouglasPeucker(std::vector<Vector2> const & points, float epsilon2, size_t startIndex, size_t lastIndex) 
	{
		std::vector<std::pair<size_t, size_t>> stk;
		stk.reserve(16);
		stk.emplace_back(startIndex, lastIndex);

		size_t globalStartIndex = startIndex;
		std::vector<bool> bitArray(lastIndex-startIndex+1, true);

		while (stk.size() > 0)
		{
			startIndex = stk.back().first;
			lastIndex  = stk.back().second;
			stk.pop_back();

			float dmax = 0_f;
			size_t index = startIndex;

			for (size_t i = index + 1; i < lastIndex; ++i)
			{
				if (bitArray[i - globalStartIndex]) 
				{
					float d2 = PointLineDistance2(points[i], points[startIndex], points[lastIndex]);

					if (d2 > dmax)
					{
						index = i;
						dmax = d2;
					}
				}
			}

			if (dmax > epsilon2)
			{
				stk.emplace_back(startIndex, index);
				stk.emplace_back(index, lastIndex);
			}
			else
			{
				for (size_t i = startIndex + 1; i < lastIndex; ++i)
					bitArray[i - globalStartIndex] = false;
			}
		}

		return bitArray;
	}

	FORCEINLINE std::vector<bool> LineSimplification::DouglasPeucker(std::vector<Vector2d> const & points, double epsilon2, size_t startIndex, size_t lastIndex) 
	{
		std::vector<std::pair<size_t, size_t>> stk;
		stk.reserve(16);
		stk.emplace_back(startIndex, lastIndex);

		size_t globalStartIndex = startIndex;
		std::vector<bool> bitArray(lastIndex-startIndex+1, true);

		while (stk.size() > 0)
		{
			startIndex = stk.back().first;
			lastIndex  = stk.back().second;
			stk.pop_back();

			double dmax = 0_f;
			size_t index = startIndex;

			for (size_t i = index + 1; i < lastIndex; ++i)
			{
				if (bitArray[i - globalStartIndex]) 
				{
					double d2 = PointLineDistance2(points[i], points[startIndex], points[lastIndex]);

					if (d2 > dmax)
					{
						index = i;
						dmax = d2;
					}
				}
			}

			if (dmax > epsilon2)
			{
				stk.emplace_back(startIndex, index);
				stk.emplace_back(index, lastIndex);
			}
			else
			{
				for (size_t i = startIndex + 1; i < lastIndex; ++i)
					bitArray[i - globalStartIndex] = false;
			}
		}

		return bitArray;
	}

	FORCEINLINE std::vector<bool> LineSimplification::DouglasPeucker(std::vector<Vector2> const & points, float epsilon)
	{
		return DouglasPeucker(points, epsilon * epsilon, 0, points.size() - 1);
	}


	FORCEINLINE std::vector<bool> LineSimplification::DouglasPeucker(std::vector<Vector2d> const & points, double epsilon)
	{
		return DouglasPeucker(points, epsilon * epsilon, 0, points.size() - 1);
	}









	//// square distance between 2 points
	//FORCEINLINE double getSqDist(Vector2d const & p1, Vector2d const & p2)
	//{

	//	auto dx = p1.x - p2.x;
	//	auto dy = p1.y - p2.y;

	//	return dx * dx + dy * dy;
	//}

	//// square distance from a point to a segment
	//FORCEINLINE double getSqSegDist(Vector2d const & p, Vector2d const & p1, Vector2d const & p2)
	//{

	//	auto
	//		x = p1.x,
	//		y = p1.y,
	//		dx = p2.x - x,
	//		dy = p2.y - y;

	//	if (dx !== 0 || dy !== 0) {

	//		auto t = ((p.x - x) * dx + (p.y - y) * dy) / (dx * dx + dy * dy);

	//		if (t > 1)
	//		{
	//			x = p2.x;
	//			y = p2.y;

	//		}
	//		else if (t > 0)
	//		{
	//			x += dx * t;
	//			y += dy * t;
	//		}
	//	}

	//	dx = p.x - x;
	//	dy = p.y - y;

	//	return dx * dx + dy * dy;
	//}
	//// rest of the code doesn't care about point format


}