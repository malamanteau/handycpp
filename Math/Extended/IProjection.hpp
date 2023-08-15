
/// See ../../License.txt for license info.

#pragma once

#include "../Core.hpp"

namespace HANDYMATH_NS {

	class IProjection
	{
	public:
		virtual Matrix4 Matrix(float zNear, float zFar) const = 0;
	};

}