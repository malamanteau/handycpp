
/// See ../../License.txt for license info.

#pragma once

#include "../Core.hpp"
#include "AABB2.hpp"

namespace HANDYMATH_NS
{
	struct Xform3
	{
		Matrix4 Matrix;

		Xform3();

		// Implicit:
		Xform3(Matrix4 const & v);

		Xform3(Xform3 const & rhs)             = default;
		Xform3 & operator=(Xform3 const & rhs) = default;

		operator Matrix4 const &() const;
		operator Matrix4() const;

		bool    IsEmpty()    const;
		Matrix4 ViewMatrix() const;
		Xform3  Inverted()   const;

		Vector3 Apply(Vector2 const & xy)  const;
		Vector3 Apply(Vector3 const & xyz) const;
		Matrix4 Apply(Matrix4 const & m)   const;

		static Xform3 RotateX(float degCCW);
		static Xform3 RotateY(float degCCW);
		static Xform3 RotateZ(float degCCW);

		static Xform3 RotateAxis  (Vector3 const & axis, float degCCW);

		static Xform3 RotateXAbout(Vector2 const & pointYZ, float degCCW); // Rotate about a point on the projective YZ plane.
		static Xform3 RotateXAbout(Vector3 const & point,   float degCCW); // Rotate about a point on the projective YZ plane.
		static Xform3 RotateYAbout(Vector2 const & pointXZ, float degCCW); // Rotate about a point on the projective XZ plane.
		static Xform3 RotateYAbout(Vector3 const & point,   float degCCW); // Rotate about a point on the projective XZ plane.
		static Xform3 RotateZAbout(Vector2 const & pointXY, float degCCW); // Rotate about a point on the projective XY plane.
		static Xform3 RotateZAbout(Vector3 const & point,   float degCCW); // Rotate about a point on the projective XY plane.

		static Xform3 RotateAxisAbout(Vector3 const & axis, Vector3 const & point, float degCCW); 

		static Xform3 MoveX(float deltaX);
		static Xform3 MoveY(float deltaY);
		static Xform3 MoveZ(float deltaZ);

		static Xform3 MoveXY(float deltaX, float deltaY);
		static Xform3 MoveXY(Vector2 const & deltaXY);

		static Xform3 MoveXYZ(float deltaX, float deltaY, float deltaZ);
		static Xform3 MoveXYZ(Vector3 const & deltaXYZ);

		static Xform3 ScaleX(float cfX);
		static Xform3 ScaleY(float cfY);
		static Xform3 ScaleZ(float cfZ);

		static Xform3 ScaleXY(float cfXY);
		static Xform3 ScaleXY(float cfX, float cfY);
		static Xform3 ScaleXY(Vector2 const & cfXY);

		static Xform3 ScaleXYZ(float cfXYZ);
		static Xform3 ScaleXYZ(float cfX, float cfY, float cfZ);
		static Xform3 ScaleXYZ(Vector3 const & cfXYZ);

		static Xform3 ScaleXAbout(float pointX, float cfX);
		static Xform3 ScaleYAbout(float pointY, float cfY);
		static Xform3 ScaleZAbout(float pointZ, float cfZ);

		static Xform3 ScaleXAbout(Vector2 const & point,  float cfX);
		static Xform3 ScaleYAbout(Vector2 const & point,  float cfY);
		static Xform3 ScaleZAbout(Vector2 const & point,  float cfZ);

		static Xform3 ScaleXAbout(Vector3 const & point,  float cfX);
		static Xform3 ScaleYAbout(Vector3 const & point,  float cfY);
		static Xform3 ScaleZAbout(Vector3 const & point,  float cfZ);

		static Xform3 ScaleXYAbout(Vector2 const & point, float cfXY);
		static Xform3 ScaleXYAbout(Vector3 const & point, float cfXY);
		static Xform3 ScaleXYAbout(Vector2 const & point, float cfX, float cfY);
		static Xform3 ScaleXYAbout(Vector3 const & point, float cfX, float cfY);
		static Xform3 ScaleXYAbout(Vector2 const & point, Vector2 const & cfXY);
		static Xform3 ScaleXYAbout(Vector3 const & point, Vector2 const & cfXY);

		static Xform3 ScaleXYZAbout(Vector3 const & point, float cfXYZ);
		static Xform3 ScaleXYZAbout(Vector3 const & point, float cfX, float cfY, float cfZ);
		static Xform3 ScaleXYZAbout(Vector3 const & point, Vector3 const & cfXYZ);

		static Xform3 BoundToBound (AABB2 const & pre, AABB2 const & rhs);
		static Xform3 BoundToBoundX(AABB2 const & pre, AABB2 const & rhs);
		static Xform3 BoundToBoundY(AABB2 const & pre, AABB2 const & rhs);

		//static Xform3 BoundToBound(AABB3 const & pre, AABB3 const & rhs);
		//static Xform3 BoundToBoundX(AABB3 const & pre, AABB3 const & rhs);
		//static Xform3 BoundToBoundY(AABB3 const & pre, AABB3 const & rhs);
		//static Xform3 BoundToBoundZ(AABB3 const & pre, AABB3 const & rhs);

		template <class SerialOp> void serial(SerialOp & ser)
		{
			ser
				(Matrix.XCol)
				(Matrix.YCol)
				(Matrix.ZCol)
				(Matrix.WCol);
		}
	};

	FORCEINLINE Xform3::Xform3() { }
	FORCEINLINE Xform3::Xform3(Matrix4 const & v) : Matrix(v) { }
	FORCEINLINE Xform3::operator Matrix4   const &() const { return Matrix; }
	FORCEINLINE Xform3::operator Matrix4() const           { return Matrix; }
	
	FORCEINLINE bool Xform3::IsEmpty() const { return Matrix.IsIdentity(); }

	FORCEINLINE Xform3 Xform3::Inverted() const { return Matrix.InverseAffine(); }

	FORCEINLINE Vector3 Xform3::Apply(Vector2 const & xy)  const { return (Matrix *  xy.XY01()).XYZ(); }
	FORCEINLINE Vector3 Xform3::Apply(Vector3 const & xyz) const { return (Matrix * xyz.XYZ1()).XYZ(); }
	FORCEINLINE Matrix4 Xform3::Apply(Matrix4 const & m)   const { return Matrix * m; }

	// BEGIN STATIC METHODS
	FORCEINLINE Xform3 Xform3::RotateX(float degCCW) { return Matrix4::RotateXAxis(HANDY_NS::Deg2Rad(degCCW)); }
	FORCEINLINE Xform3 Xform3::RotateY(float degCCW) { return Matrix4::RotateYAxis(HANDY_NS::Deg2Rad(degCCW)); }
	FORCEINLINE Xform3 Xform3::RotateZ(float degCCW) { return Matrix4::RotateZAxis(HANDY_NS::Deg2Rad(degCCW)); }

	FORCEINLINE Xform3 Xform3::RotateAxis  (Vector3 const & axis, float degCCW) { return Matrix4::RotateAboutAxis(axis, HANDY_NS::Deg2Rad(degCCW)); }

	FORCEINLINE Xform3 Xform3::RotateXAbout(Vector2 const & pointYZ, float degCCW) { Vector3 a(0_f, pointYZ.X, pointYZ.Y); return Matrix4::Translate(a) * Matrix4::RotateXAxis(HANDY_NS::Deg2Rad(degCCW)) * Matrix4::Translate(-a); }
	FORCEINLINE Xform3 Xform3::RotateYAbout(Vector2 const & pointXZ, float degCCW) { Vector3 a(pointXZ.X, 0_f, pointXZ.Y); return Matrix4::Translate(a) * Matrix4::RotateYAxis(HANDY_NS::Deg2Rad(degCCW)) * Matrix4::Translate(-a); }
	FORCEINLINE Xform3 Xform3::RotateZAbout(Vector2 const & pointXY, float degCCW) { Vector3 a(pointXY.X, pointXY.Y, 0_f); return Matrix4::Translate(a) * Matrix4::RotateYAxis(HANDY_NS::Deg2Rad(degCCW)) * Matrix4::Translate(-a); }

	FORCEINLINE Xform3 Xform3::RotateXAbout(Vector3 const & point, float degCCW) { return Matrix4::Translate(point) * Matrix4::RotateXAxis(HANDY_NS::Deg2Rad(degCCW)) * Matrix4::Translate(-point); }
	FORCEINLINE Xform3 Xform3::RotateYAbout(Vector3 const & point, float degCCW) { return Matrix4::Translate(point) * Matrix4::RotateYAxis(HANDY_NS::Deg2Rad(degCCW)) * Matrix4::Translate(-point); }
	FORCEINLINE Xform3 Xform3::RotateZAbout(Vector3 const & point, float degCCW) { return Matrix4::Translate(point) * Matrix4::RotateZAxis(HANDY_NS::Deg2Rad(degCCW)) * Matrix4::Translate(-point); }

	FORCEINLINE Xform3 Xform3::RotateAxisAbout(Vector3 const & axis, Vector3 const & point, float degCCW) { return Matrix4::Translate(point) * Matrix4::RotateAboutAxis(axis, HANDY_NS::Deg2Rad(degCCW)) * Matrix4::Translate(-point); }

	FORCEINLINE Xform3 Xform3::MoveX(float deltaX) { return Matrix4::Translate(deltaX, 0_f, 0_f); }
	FORCEINLINE Xform3 Xform3::MoveY(float deltaY) { return Matrix4::Translate(0_f, deltaY, 0_f); }
	FORCEINLINE Xform3 Xform3::MoveZ(float deltaZ) { return Matrix4::Translate(0_f, 0_f, deltaZ); }

	FORCEINLINE Xform3 Xform3::MoveXY(float deltaX, float deltaY) { return Matrix4::Translate(deltaX, deltaY); }
	FORCEINLINE Xform3 Xform3::MoveXY(Vector2 const & deltaXY)    { return Matrix4::Translate(deltaXY.XY0()); }

	FORCEINLINE Xform3 Xform3::MoveXYZ(float deltaX, float deltaY, float deltaZ) { return Matrix4::Translate(deltaX, deltaY, deltaZ); }
	FORCEINLINE Xform3 Xform3::MoveXYZ(Vector3 const & deltaXYZ)                 { return Matrix4::Translate(deltaXYZ); }

	FORCEINLINE Xform3 Xform3::ScaleX(float cfX) { return Matrix4::Scale(cfX, 1_f, 1_f); }
	FORCEINLINE Xform3 Xform3::ScaleY(float cfY) { return Matrix4::Scale(1_f, cfY, 1_f); }
	FORCEINLINE Xform3 Xform3::ScaleZ(float cfZ) { return Matrix4::Scale(1_f, 1_f, cfZ); }

	FORCEINLINE Xform3 Xform3::ScaleXY(float cfXY)           { return Matrix4::Scale(cfXY,   cfXY,   1_f); }
	FORCEINLINE Xform3 Xform3::ScaleXY(float cfX, float cfY) { return Matrix4::Scale(cfX,    cfY,    1_f); }
	FORCEINLINE Xform3 Xform3::ScaleXY(Vector2 const & cfXY) { return Matrix4::Scale(cfXY.XY1()         ); }

	FORCEINLINE Xform3 Xform3::ScaleXYZ(float cfXYZ)                     { return Matrix4::Scale(cfXYZ,   cfXYZ,   cfXYZ); }
	FORCEINLINE Xform3 Xform3::ScaleXYZ(float cfX, float cfY, float cfZ) { return Matrix4::Scale(cfX,     cfY,     cfZ); }
	FORCEINLINE Xform3 Xform3::ScaleXYZ(Vector3 const & cfXYZ)           { return Matrix4::Scale(cfXYZ); }

	FORCEINLINE Xform3 Xform3::ScaleXAbout(float pointX, float cfX) { return Matrix4::Translate(pointX, 0_f, 0_f) * Matrix4::Scale(cfX, 1_f, 1_f) * Matrix4::Translate(-pointX, 0_f, 0_f); }
	FORCEINLINE Xform3 Xform3::ScaleYAbout(float pointY, float cfY) { return Matrix4::Translate(0_f, pointY, 0_f) * Matrix4::Scale(1_f, cfY, 1_f) * Matrix4::Translate(-pointY, 0_f, 0_f); }
	FORCEINLINE Xform3 Xform3::ScaleZAbout(float pointZ, float cfZ) { return Matrix4::Translate(0_f, 0_f, pointZ) * Matrix4::Scale(1_f, 1_f, cfZ) * Matrix4::Translate(-pointZ, 0_f, 0_f); }

	FORCEINLINE Xform3 Xform3::ScaleXAbout(Vector2 const & point,  float cfX) { return Matrix4::Translate(point) * Matrix4::Scale(cfX, 1_f, 1_f) * Matrix4::Translate(-point); }
	FORCEINLINE Xform3 Xform3::ScaleYAbout(Vector2 const & point,  float cfY) { return Matrix4::Translate(point) * Matrix4::Scale(1_f, cfY, 1_f) * Matrix4::Translate(-point); }
	FORCEINLINE Xform3 Xform3::ScaleZAbout(Vector2 const & point,  float cfZ) { return Matrix4::Translate(point) * Matrix4::Scale(1_f, 1_f, cfZ) * Matrix4::Translate(-point); }

	FORCEINLINE Xform3 Xform3::ScaleXAbout(Vector3 const & point,  float cfX) { return Matrix4::Translate(point) * Matrix4::Scale(cfX, 1_f, 1_f) * Matrix4::Translate(-point); }
	FORCEINLINE Xform3 Xform3::ScaleYAbout(Vector3 const & point,  float cfY) { return Matrix4::Translate(point) * Matrix4::Scale(1_f, cfY, 1_f) * Matrix4::Translate(-point); }
	FORCEINLINE Xform3 Xform3::ScaleZAbout(Vector3 const & point,  float cfZ) { return Matrix4::Translate(point) * Matrix4::Scale(1_f, 1_f, cfZ) * Matrix4::Translate(-point); }
	
	FORCEINLINE Xform3 Xform3::ScaleXYAbout(Vector2 const & point, float cfXY)           { return Matrix4::Translate(point) * Matrix4::Scale(cfXY,   cfXY,   1_f) * Matrix4::Translate(-point); }
	FORCEINLINE Xform3 Xform3::ScaleXYAbout(Vector2 const & point, float cfX, float cfY) { return Matrix4::Translate(point) * Matrix4::Scale(cfX,    cfY,    1_f) * Matrix4::Translate(-point); }
	FORCEINLINE Xform3 Xform3::ScaleXYAbout(Vector2 const & point, Vector2 const & cfXY) { return Matrix4::Translate(point) * Matrix4::Scale(cfXY.XY1()         ) * Matrix4::Translate(-point); }

	FORCEINLINE Xform3 Xform3::ScaleXYAbout(Vector3 const & point, float cfXY)           { return Matrix4::Translate(point) * Matrix4::Scale(cfXY,   cfXY,   1_f) * Matrix4::Translate(-point); }
	FORCEINLINE Xform3 Xform3::ScaleXYAbout(Vector3 const & point, float cfX, float cfY) { return Matrix4::Translate(point) * Matrix4::Scale(cfX,    cfY,    1_f) * Matrix4::Translate(-point); }
	FORCEINLINE Xform3 Xform3::ScaleXYAbout(Vector3 const & point, Vector2 const & cfXY) { return Matrix4::Translate(point) * Matrix4::Scale(cfXY.XY1()         ) * Matrix4::Translate(-point); }

	FORCEINLINE Xform3 Xform3::ScaleXYZAbout(Vector3 const & point, float cfXYZ)                     { return Matrix4::Translate(point) * Matrix4::Scale(cfXYZ)         * Matrix4::Translate(-point); }
	FORCEINLINE Xform3 Xform3::ScaleXYZAbout(Vector3 const & point, float cfX, float cfY, float cfZ) { return Matrix4::Translate(point) * Matrix4::Scale(cfX, cfY, cfZ) * Matrix4::Translate(-point); }
	FORCEINLINE Xform3 Xform3::ScaleXYZAbout(Vector3 const & point, Vector3 const & cfXYZ)           { return Matrix4::Translate(point) * Matrix4::Scale(cfXYZ)         * Matrix4::Translate(-point); }

	FORCEINLINE Xform3 Xform3::BoundToBound (AABB2 const & pre, AABB2 const & post) { return Matrix4::Translate(post.PointSW()) * Matrix4::Scale((post.Size() / pre.Size()).XY1()) * Matrix4::Translate(-pre.PointSW()); }
	FORCEINLINE Xform3 Xform3::BoundToBoundX(AABB2 const & pre, AABB2 const & post) { return Matrix4::Translate(     post.PointSW().X, 0_f) * Matrix4::Scale(     (post.Size() / pre.Size()).X, 1_f, 1_f) * Matrix4::Translate(     -pre.PointSW().X, 0_f); }
	FORCEINLINE Xform3 Xform3::BoundToBoundY(AABB2 const & pre, AABB2 const & post) { return Matrix4::Translate(0_f, post.PointSW().Y     ) * Matrix4::Scale(1_f, (post.Size() / pre.Size()).Y, 1_f     ) * Matrix4::Translate(0_f, -pre.PointSW().Y     ); }

	//FORCEINLINE Xform3 Xform3::BoundToBound(AABB3 const & pre, AABB3 const & post) { }
	//FORCEINLINE Xform3 Xform3::BoundToBoundX(AABB3 const & pre, AABB3 const & post) { }
	//FORCEINLINE Xform3 Xform3::BoundToBoundY(AABB3 const & pre, AABB3 const & post) { }
	//FORCEINLINE Xform3 Xform3::BoundToBoundZ(AABB3 const & pre, AABB3 const & post) { }
	// END STATIC METHODS

	//OPERATORS
	FORCEINLINE Xform3 operator+(Xform3 const & lhs, Xform3 const & rhs) { return rhs.Matrix * lhs.Matrix; }
	FORCEINLINE Xform3 operator-(Xform3 const & lhs, Xform3 const & rhs) { return rhs.Inverted().Matrix * lhs.Matrix; }
	FORCEINLINE Xform3 operator+(Xform3 const & xf) { return xf; }
	FORCEINLINE Xform3 operator-(Xform3 const & xf) { return xf.Inverted(); }
	FORCEINLINE bool operator==(Xform3 const & lhs, Xform3 const & rhs) { return lhs.Matrix.Compare<CMP::AllWE>(rhs.Matrix); }
	FORCEINLINE bool operator!=(Xform3 const & lhs, Xform3 const & rhs) { return !operator==(lhs, rhs); }
}