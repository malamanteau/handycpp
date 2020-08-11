
/// See ../../License.txt for license info.

#pragma once

#include "IProjection.hpp"

namespace HANDYMATH_NS {

enum class OrthoProjectionType
{
	FixedWidth,
	FixedHeight,
	FixedMinWidthHeight,
	Manual
};

class OrthoProjection : public IProjection
{
public:
	Vector2             ViewportDimensions = Vector2::Zero();
	OrthoProjectionType Mode               = OrthoProjectionType::Manual;

	OrthoProjection() = default;

	OrthoProjection(Vector2 const & viewportDimensions, OrthoProjectionType mode) 
		: ViewportDimensions(viewportDimensions)
		, Mode              (mode) 
	{ }

	Vector2 EffectiveDimensions() const
	{
		float width  = ViewportDimensions.X;
		float height = ViewportDimensions.Y;

		float ratio = width / height;

		switch (Mode)
		{
			case OrthoProjectionType::FixedWidth:
				height = width / ratio;
				break;

			case OrthoProjectionType::FixedHeight:
				width = height * ratio;
				break;

			case OrthoProjectionType::FixedMinWidthHeight:
				if (width / ratio > height)
					height = width / ratio;
				else if (height * ratio > width)
					width = height * ratio;
				break;

			case OrthoProjectionType::Manual:
				break;
		}

		return Vector2(width, height);
	}
	 
	Matrix4 Matrix(float zNear, float zFar) const override
	{
		if (zFar < zNear)
			throw;

		Vector2 realSize = EffectiveDimensions();

		float left   = -realSize.X * 0.5f;
		float right  =  realSize.X * 0.5f;
		float top    =  realSize.Y * 0.5f;
		float bottom = -realSize.Y * 0.5f;

		Matrix4 result = Matrix4::Identity();

		float invRightLeft = 1.0f / (right - left);
		float invTopBottom = 1.0f / (top - bottom);
		float invFarNear   = 1.0f / (zFar - zNear);

		result.XCol.X =  2.0f * invRightLeft;
		result.YCol.Y =  2.0f * invTopBottom;
		result.ZCol.Z = -2.0f * invFarNear;

		result.WCol.X = -(right + left) * invRightLeft;
		result.WCol.Y = -(top + bottom) * invTopBottom;
		result.WCol.Z = -(zFar + zNear) * invFarNear;

		return result;
	}
};

}