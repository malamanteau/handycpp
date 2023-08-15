
/// See ../../License.txt for license info.

#pragma once

#include "../Core/Base.hpp"
#include "../Core/Vector2d.hpp"

namespace HANDYMATH_NS {

	class Line2
	{
	public:
		Vector2 A;
		Vector2 B;

		Line2(Vector2 const & a, Vector2 const & b)
			: A(a)
			, B(b)
		{

		}

		template <class SerialOp> void serial(SerialOp & ser)
		{
			ser
				(A)
				(B);
		}
	};

	class Line2d
	{
	public:
		Vector2d A;
		Vector2d B;

		Line2d(Vector2d const & a, Vector2d const & b)
			: A(a)
			, B(b)
		{

		}
	};
}
