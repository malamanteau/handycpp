
/// See ../../License.txt for license info.

#pragma once

#include <cmath>
#include <vector>

#include "AABB2d.hpp"
#include "Line2.hpp"

namespace HANDYMATH_NS
{
	enum class LineClipResult : uint32_t
	{
		Reject         = 0b001,
		ClippedNeither = 0,
		ClippedA       = 0b010,
		ClippedB       = 0b100,
		ClippedBoth    = ClippedA | ClippedB
	};
	HANDY_ENABLE_BITMASK_OPERATORS(LineClipResult)

	/// https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm

	class LineClipper
	{
	public:
		static constexpr uint32_t INSIDE = 0b0000;
		static constexpr uint32_t LEFT   = 0b0001;
		static constexpr uint32_t RIGHT  = 0b0010;
		static constexpr uint32_t BOTTOM = 0b0100;
		static constexpr uint32_t TOP    = 0b1000;

		AABB2d Bounds;

		FORCEINLINE LineClipper(AABB2d bounds) : Bounds(bounds) { }

		/// Compute the bit code for a point (x, y) using the clip rectangle
		/// bounded diagonally by (xmin, ymin), and (xmax, ymax)
		FORCEINLINE uint32_t ComputeOutCode(Vector2d const & p)
		{
			uint32_t code;

			code = INSIDE; // initialised as being inside of [[clip window]]

			if      (p.X < Bounds.Min.X) code |= LEFT;   // to the left of clip window
			else if (p.X > Bounds.Max.X) code |= RIGHT;  // to the right of clip window

			if      (p.Y < Bounds.Min.Y) code |= BOTTOM; // below the clip window
			else if (p.Y > Bounds.Max.Y) code |= TOP;    // above the clip window

			return code;
		}


		FORCEINLINE LineClipResult CohenSutherland(Vector2d & a, Vector2d & b, uint32_t outcode0, uint32_t outcode1)
		{
			LineClipResult result = LineClipResult::ClippedNeither;

			while (true)
			{
				if (!(outcode0 | outcode1))
					return result;
				else if (outcode0 & outcode1) 
					return LineClipResult::Reject;

				Vector2d nu;

				uint32_t outcodeOut = outcode0 ? outcode0 : outcode1;

				if (outcodeOut & TOP)         // point is above the clip window
					nu = Vector2d(a.X + (b.X - a.X) * (Bounds.Max.Y - a.Y) / (b.Y - a.Y),
										Bounds.Max.Y);

				else if (outcodeOut & BOTTOM) // point is below the clip window
					nu = Vector2d(a.X + (b.X - a.X) * (Bounds.Min.Y - a.Y) / (b.Y - a.Y), 
										Bounds.Min.Y);

				else if (outcodeOut & RIGHT) // point is to the right of clip window
					nu = Vector2d(Bounds.Max.X,
										a.Y + (b.Y - a.Y) * (Bounds.Max.X - a.X) / (b.X - a.X));

				else if (outcodeOut & LEFT)  // point is to the left of clip window
					nu = Vector2d(Bounds.Min.X, 
										a.Y + (b.Y - a.Y) * (Bounds.Min.X - a.X) / (b.X - a.X));

				// Now we move outside point to intersection point to clip and get ready for next pass.
				if (outcodeOut == outcode0)
				{
					result |= LineClipResult::ClippedA;
					a = nu;
					outcode0 = ComputeOutCode(a);
				}
				else
				{
					result |= LineClipResult::ClippedB;
					b = nu;
					outcode1 = ComputeOutCode(b);
				}
			}

			return result;
		}

		/// Cohen–Sutherland clipping algorithm clips a line from line.A to line.B 
		/// against a rectangle with diagonal from (xmin, ymin) to (xmax, ymax).
		FORCEINLINE LineClipResult CohenSutherland(Vector2d & a, Vector2d & b)
		{
			// compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
			return CohenSutherland(a, b, ComputeOutCode(a), ComputeOutCode(b));
		}
	};

	class PolylineClipper
	{
		std::vector<Vector2d> * m_line;
		std::vector<uint32_t>         m_outcodes;
		LineClipper                   m_clipper;

		size_t m_index = 0;
	public:

		FORCEINLINE PolylineClipper(std::vector<Vector2d> * line, AABB2d bounds) : m_line(line), m_clipper(bounds) 
		{
			m_outcodes.reserve(m_line->size());

			for (auto const & item : *m_line)
				m_outcodes.emplace_back(m_clipper.ComputeOutCode(item));
		}

		/// Will clear and fill ret with the next segment. When false is returned, 
		/// you are done, no more segments, and the contents of ret are undefined.
		FORCEINLINE bool NextSegment(std::vector<Vector2d> & ret)
		{
			ret.clear();

			for (; m_index < m_line->size() - 1; ++m_index)
			{
				/// if (m_outcodes[m_index] == 0) // Is Inside

				/// Copy
				Vector2d pointA = m_line->at(m_index);
				Vector2d pointB = m_line->at(m_index + 1);

				/// Modify a and b
				LineClipResult lcr = m_clipper.CohenSutherland(pointA, pointB, m_outcodes.at(m_index), m_outcodes.at(m_index + 1)); 

				if (lcr == LineClipResult::Reject)
				{
					if (ret.size() > 1)
						return true;

					continue;
				}

				ret.push_back(pointA);

				if (m_index == m_line->size() - 2)
				{
					ret.push_back(pointB);
					++m_index;
					return ret.size() > 1;
				}

				if ((lcr & LineClipResult::ClippedB) == LineClipResult::ClippedB)
				{
					ret.push_back(pointB);
					++m_index;
					return ret.size() > 1;
				}
			}

			return ret.size() > 1;
		}
	};
}

