
/// See ../../License.txt for license info.

#pragma once

#include "../Core/Vector3.hpp"
#include "../Core/Vector4.hpp"
#include "../Core/Matrix4.hpp"

namespace HANDYMATH_NS
{
	
struct OrthoFrame3
{
	Vector3 Right;
	Vector3 Up;
	Vector3 Forward;
	Vector3 Position;

	OrthoFrame3();

	Matrix4 Matrix();
	//Matrix4 ViewMatrix() const;

	void Reset();
	void ResetOrientation();
	void ResetOrientationAndScale();
	void ResetScale();
	void ResetPosition();
	
	void Orthogonalize();
	void OrthoNormalize();
	
	void RotateAroundUp(float degrees);
	void RotateAroundRight(float degrees);
	void RotateAroundForward(float degrees);
	void RotateX(float degrees);
	void RotateY(float degrees);
	void RotateZ(float degrees);

	void Rotate(Vector3 const & axis, float degrees);

	void Move(float amountX, float amountY, float amountZ);
	void Move(Vector3 direction);

	void MoveTo(Vector3 position);
	void MoveTo(float x, float y, float z);

	void MoveX(float distance);
	void MoveY(float distance);
	void MoveZ(float distance);

	void MoveForward(float distance);
	void MoveRight(float distance);
	void MoveUp(float distance);	

	Vector3 GetScale() const;

	void Scale  (float s);
	void Scale  (float sRight, float sUp, float sForward);
	void Scale  (Vector3 const & s);
	void ScaleTo(float s);
	void ScaleTo(Vector3 const & s);
	void ScaleTo(float sRight, float sUp, float sForward);

	void ScaleRightTo  (float r);
	void ScaleUpTo     (float u);
	void ScaleForwardTo(float f);

	template <class SerialOp> void serial(SerialOp & ser)
	{
		ser
			(Right)
			(Up)
			(Forward)
			(Position);
	}
};
	
FORCEINLINE OrthoFrame3::OrthoFrame3()
	: Right   (Vector3::UnitX()),
	  Up      (Vector3::UnitY()),
	  Forward (Vector3::UnitZ()),
	  Position(Vector3::Zero())
{ }

FORCEINLINE Vector3 OrthoFrame3::GetScale() const { return Vector3(Right.Length(), Up.Length(), Forward.Length()); }

FORCEINLINE Matrix4 OrthoFrame3::Matrix() { return Matrix4(Right.XYZ0(), Up.XYZ0(), Forward.XYZ0(), Position.XYZ1()); }

FORCEINLINE void OrthoFrame3::Reset()
{
	Right    = Vector3::UnitX();
	Up       = Vector3::UnitY();
	Forward  = Vector3::UnitZ();
	Position = Vector3::Zero();
}

FORCEINLINE void OrthoFrame3::ResetOrientation()
{
	Right   = Vector3::UnitX() * Right.Length();
	Up      = Vector3::UnitY() * Up.Length();
	Forward = Vector3::UnitZ() * Forward.Length();
}

FORCEINLINE void OrthoFrame3::ResetOrientationAndScale()
{
	Right   = Vector3::UnitX();
	Up      = Vector3::UnitY();
	Forward = Vector3::UnitZ();
}

FORCEINLINE void OrthoFrame3::ResetScale()
{
	Right   =   Right.Normalized();
	Up      =      Up.Normalized();
	Forward = Forward.Normalized();
}

FORCEINLINE void OrthoFrame3::ResetPosition()
{
	Position = Vector3::Zero();
}

FORCEINLINE void OrthoFrame3::Orthogonalize()
{
	Right = Forward.Cross(Up).Normalized() * Right.Length();
	Up    = Right.Cross(Forward).Normalized() * Up.Length();
}

FORCEINLINE void OrthoFrame3::OrthoNormalize()
{
	Forward = Forward.Normalized();
	Right   = Forward.Cross(Up).Normalized();
	Up      = Right.Cross(Forward).Normalized();
}

FORCEINLINE void OrthoFrame3::RotateAroundUp(float degrees)
{
	Right   =   Right.RotatedAbout(Up, HANDY_NS::Deg2Rad(degrees));
	Forward = Forward.RotatedAbout(Up, HANDY_NS::Deg2Rad(degrees));
}

FORCEINLINE void OrthoFrame3::RotateAroundRight(float degrees)
{
	Up      =      Up.RotatedAbout(Right, HANDY_NS::Deg2Rad(degrees));
	Forward = Forward.RotatedAbout(Right, HANDY_NS::Deg2Rad(degrees));
}

FORCEINLINE void OrthoFrame3::RotateAroundForward(float degrees)
{
	Right = Right.RotatedAbout(Forward, HANDY_NS::Deg2Rad(degrees));
	Up    =    Up.RotatedAbout(Forward, HANDY_NS::Deg2Rad(degrees));
}

FORCEINLINE void OrthoFrame3::RotateX(float degrees)
{
	Right   =   Right.RotatedAbout(Vector3::UnitX(), HANDY_NS::Deg2Rad(degrees));
	Up      =      Up.RotatedAbout(Vector3::UnitX(), HANDY_NS::Deg2Rad(degrees));
	Forward = Forward.RotatedAbout(Vector3::UnitX(), HANDY_NS::Deg2Rad(degrees));
}

FORCEINLINE void OrthoFrame3::RotateY(float degrees)
{
	Right   =   Right.RotatedAbout(Vector3::UnitX(), HANDY_NS::Deg2Rad(degrees));
	Up      =      Up.RotatedAbout(Vector3::UnitX(), HANDY_NS::Deg2Rad(degrees));
	Forward = Forward.RotatedAbout(Vector3::UnitX(), HANDY_NS::Deg2Rad(degrees));
}

FORCEINLINE void OrthoFrame3::RotateZ(float degrees)
{
	Right   =   Right.RotatedAbout(Vector3::UnitX(), HANDY_NS::Deg2Rad(degrees));
	Up      =      Up.RotatedAbout(Vector3::UnitX(), HANDY_NS::Deg2Rad(degrees));
	Forward = Forward.RotatedAbout(Vector3::UnitX(), HANDY_NS::Deg2Rad(degrees));
}


FORCEINLINE void OrthoFrame3::Rotate(Vector3 const & axis, float degrees)
{
	Right   =   Right.RotatedAbout(axis, HANDY_NS::Deg2Rad(degrees));
	Up      =      Up.RotatedAbout(axis, HANDY_NS::Deg2Rad(degrees));
	Forward = Forward.RotatedAbout(axis, HANDY_NS::Deg2Rad(degrees));
}

FORCEINLINE void OrthoFrame3::Move(float amountX, float amountY, float amountZ) { Position += Vector3(amountX, amountY, amountZ); }
FORCEINLINE void OrthoFrame3::Move(Vector3 direction) { Position += direction; }

FORCEINLINE void OrthoFrame3::MoveTo(Vector3 position) { Position = position; }
FORCEINLINE void OrthoFrame3::MoveTo(float x, float y, float z) { Position = Vector3(x, y, z); }

FORCEINLINE void OrthoFrame3::MoveX(float distance) { Position.X += distance; }
FORCEINLINE void OrthoFrame3::MoveY(float distance) { Position.Y += distance; }
FORCEINLINE void OrthoFrame3::MoveZ(float distance) { Position.Z += distance; }

FORCEINLINE void OrthoFrame3::MoveForward(float distance) { Position = Position + Forward.Normalized() * distance; }
FORCEINLINE void OrthoFrame3::MoveRight  (float distance) { Position = Position +   Right.Normalized() * distance; }
FORCEINLINE void OrthoFrame3::MoveUp     (float distance) { Position = Position +      Up.Normalized() * distance; }

FORCEINLINE void OrthoFrame3::Scale  (float s)            { Vector3 s3(s); Right *= s3;     Up *= s3;  Forward *= s3; }
FORCEINLINE void OrthoFrame3::Scale  (float sRight, float sUp, float sForward) { Right *= sRight; Up *= sUp; Forward *= sForward; }
FORCEINLINE void OrthoFrame3::Scale  (Vector3 const & s)                 { Right *= s.X;    Up *= s.Y; Forward *= s.Z; }
FORCEINLINE void OrthoFrame3::ScaleTo(float s)                                 { ResetScale(); Scale(s); }
FORCEINLINE void OrthoFrame3::ScaleTo(Vector3 const & s)                 { ResetScale(); Scale(s); }
FORCEINLINE void OrthoFrame3::ScaleTo(float sRight, float sUp, float sForward) { ResetScale(); Scale(sRight, sUp, sForward);  }

FORCEINLINE void OrthoFrame3::ScaleRightTo  (float r) { Right   = Right  .Normalized() * r; }
FORCEINLINE void OrthoFrame3::ScaleUpTo     (float u) { Up      = Up     .Normalized() * u; }
FORCEINLINE void OrthoFrame3::ScaleForwardTo(float f) { Forward = Forward.Normalized() * f; }

}
