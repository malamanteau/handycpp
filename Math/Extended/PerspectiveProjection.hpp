
/// See ../../License.txt for license info.

#pragma once

#include "IProjection.hpp"

namespace HANDYMATH_NS {

enum class PerspectiveProjectionType
{
	FixedFOVX,
	FixedFOVY,
	MinimumFOVXY,
	Manual
};
	
class PerspectiveProjection : public IProjection
{
public:
	Vector2                     ViewportDimensions = Vector2::Zero();
	Vector2                     FOVXY              = Vector2::Zero();
	PerspectiveProjectionType   Mode               = PerspectiveProjectionType::Manual;
		
	PerspectiveProjection() = default;
		
	PerspectiveProjection(Vector2 viewportDimensions, Vector2 const & fovXY, PerspectiveProjectionType mode)
		: ViewportDimensions(viewportDimensions)
		, FOVXY(fovXY)
		, Mode (mode)
	{ }
		
	Vector2 EffectiveAngles(float znear, float zfar) const
	{
		if ((Mode == PerspectiveProjectionType::FixedFOVX && FOVXY.X <= 0.0f) ||
			(Mode == PerspectiveProjectionType::FixedFOVY && FOVXY.Y <= 0.0f))
			throw;
			
		float ratio = ViewportDimensions.X / ViewportDimensions.Y;
			
		float fW = std::tan(Handy::Deg2Rad(FOVXY.X * 0.5f)) * znear;
		float fH = std::tan(Handy::Deg2Rad(FOVXY.Y * 0.5f)) * znear;
			
		switch (Mode)
		{
			case PerspectiveProjectionType::FixedFOVX:
				fH = fW / ratio;
				break;
					
			case PerspectiveProjectionType::FixedFOVY:
				fW = fH * ratio;
				break;
					
			case PerspectiveProjectionType::MinimumFOVXY :
				if (fW / ratio > fH)
					fH = fW / ratio;
				else if (fH * ratio > fW)
					fW = fH * ratio;
				break;
					
			case PerspectiveProjectionType::Manual:
				//No Change!
				break;
		}
			
		return Vector2(fW, fH);
	}
		
	virtual 
	Matrix4 Matrix(float zNear, float zFar) const override
	{
		Vector2 fWH = EffectiveAngles(zNear, zFar);
			
		float left   = -fWH.X;
		float right  =  fWH.X;
		float top    =  fWH.Y;
		float bottom = -fWH.Y; 

		Matrix4 result = Matrix4::Identity();

		float invRightLeft = 1.0f / (right - left);
		float invTopBottom = 1.0f / (top - bottom);
		float invFarNear   = 1.0f / (zFar - zNear);

		result.XCol.X = 2.0f * zNear    * invRightLeft;
		result.YCol.Y = 2.0f * zNear    * invTopBottom;
		result.ZCol.Z = -(zFar + zNear) * invFarNear;

		result.ZCol.X = (right + left) * invRightLeft;
		result.ZCol.Y = (top + bottom) * invTopBottom;
		result.ZCol.W = -1_f;
		result.WCol.Z = -2_f * zFar * zNear * invFarNear;
		result.WCol.W = 0_f;

		return result;
	}
};
	

}