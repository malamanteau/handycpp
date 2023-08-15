
/// See ../../License.txt for license info.

#pragma once

#include <cmath>
#include <tuple>

#include "Matrix3.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

namespace HANDYMATH_NS {

	class Matrix4 
	{
	public:
		static constexpr int32_t ElementBytes =  4_i32;
		static constexpr int32_t NumElements  = 16_i32;
		static constexpr int32_t StrideBytes  = 64_i32;
		static constexpr int32_t NumColumns        =  4_i32;
		static constexpr int32_t ColumnStrideBytes = 16_i32;

		Vector4 XCol;
		Vector4 YCol;
		Vector4 ZCol;
		Vector4 WCol;

		//union 
		//{
		//	struct 
		//	{
		//		Vector4 XCol;
		//		Vector4 YCol;
		//		Vector4 ZCol;
		//		Vector4 WCol;
		//	};

		//	struct
		//	{
		//		float M00; // Col 0 (1st), Row 0 (1st)
		//		float M01; // Col 0 (1st), Row 1 (2nd)
		//		float M02; // Col 0 (1st), Row 2 (3rd)
		//		float M03; // Col 0 (1st), Row 3 (4th)
		//		float M10; // Col 1 (2nd), Row 0 (1st)
		//		float M11; // Col 1 (2nd), Row 1 (2nd)
		//		float M12; // Col 1 (2nd), Row 2 (3rd)
		//		float M13; // Col 1 (2nd), Row 3 (4th)
		//		float M20; // Col 2 (3rd), Row 0 (1st)
		//		float M21; // Col 2 (3rd), Row 1 (2nd)
		//		float M22; // Col 2 (3rd), Row 2 (3rd)
		//		float M23; // Col 2 (3rd), Row 3 (4th)
		//		float M30; // Col 3 (4th), Row 0 (1st)
		//		float M31; // Col 3 (4th), Row 1 (2nd)
		//		float M32; // Col 3 (4th), Row 2 (3rd)
		//		float M33; // Col 3 (4th), Row 3 (4th)
		//	};

		//	float Array[16];
		//};
		//
		Matrix4();
		Matrix4(Matrix4 const & m);
		Matrix4(Vector4 const & xCol, Vector4 const & yCol, Vector4 const & zCol, Vector4 const & wCol);
		Matrix4(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);

		explicit Matrix4(float f16);
		explicit Matrix4(Vector4 const & xyzwCol);
		explicit Matrix4(float const * ary);
		
		static Matrix4 FromArray  (float const * ary);
		void           CopyToArray(float       * ary) const;
		
		Matrix4 Transpose()     const;
		float   Determinant()   const;
		Matrix4 Inverse()       const;
		Matrix4 InverseAffine() const; // Find the inverse, given that the matrix is affine, VERY FAST.
		Matrix4 Normal()        const; // Inverted Transpose
		Matrix4 View()          const;


		Matrix4 Inverse_Determinant(float & outDet) const;          // Fastest option if you already need both anyways.
		Matrix4 InverseTranspose_Determinant(float & outDet) const; // Fastest option if you already need both anyways, and were also planning to transpose.

		Vector4 Row0() const;
		Vector4 Row1() const;
		Vector4 Row2() const;
		Vector4 Row3() const;
		Vector4 Diagonal() const;

		// Conversions:
		Matrix3 SubMatrix3() const; // Mus
		// ---

		bool IsIdentity() const;
		bool IsSymmetric() const;
	
		template <CMP op> bool Compare(float           rhs) const;
		template <CMP op> bool Compare(Matrix4 const & rhs) const;

		float & operator[](int idx);
		float   operator[](int idx) const;

		static Matrix4 Zero();
		static Matrix4 One();
		static Matrix4 Identity();

		static Matrix4 Translate(Vector2 const & t);
		static Matrix4 Translate(Vector3 const & t);
		static Matrix4 Translate(Vector4 const & t);
		static Matrix4 Translate(float x, float y, float z);
		static Matrix4 Translate(float x, float y);
		static Matrix4 RotateAboutZAxisXYPoint(float thetaRadians, Vector2 const & point);
		static Matrix4 RotateAboutAxisPoint   (float thetaradians, Vector3 const & axis, Vector3 const & point);

		static Matrix4 Scale(float v);
		static Matrix4 Scale(float sX, float sY, float sZ);
		static Matrix4 Scale(Vector3 const & v);
		static Matrix4 RotateXAxis(float thetaRadians);
		static Matrix4 RotateYAxis(float thetaRadians);
		static Matrix4 RotateZAxis(float thetaRadians);
		static Matrix4 RotateAboutAxis(Vector3 const & axis, float theta);
		static Matrix4 Skew2D(float x_sub_y, float y_sub_x);
		
		//static Matrix4 FromAxisRotation(float radians, Vector4 axis);

		template <class SerialOp> void serial(SerialOp & ser)
		{
			ser
				(XCol)
				(YCol)
				(ZCol)
				(WCol);
		}
	};

	/// COMPONENT-WISE MULTIPLICATION!
	Matrix4   operator^ (Matrix4 const & lhs, Matrix4 const & rhs);
	Matrix4 & operator^=(Matrix4       & lhs, Matrix4 const & rhs);

	Vector4   operator* (Matrix4 const & lhs, Vector4 const & rhs);
	Matrix4   operator* (Matrix4 const & lhs, Matrix4 const & rhs);
	Matrix4 & operator*=(Matrix4       & lhs, Matrix4 const & rhs);


	// TODO ADD SCALAR AND COMPONENT-WISE OPERATORS!

	FORCEINLINE Matrix4 operator^ (Matrix4 const & lhs, Matrix4 const & rhs)
	{
		return Matrix4(
			lhs.XCol * rhs.XCol,
			lhs.YCol * rhs.YCol,
			lhs.ZCol * rhs.ZCol,
			lhs.WCol * rhs.WCol);

	}

	FORCEINLINE Matrix4 & operator^=(Matrix4 & lhs, Matrix4 const & rhs)
	{
		lhs = lhs ^ rhs; 
		return lhs; 
	}

	FORCEINLINE Matrix4 operator*(Matrix4 const & lhs, Matrix4 const & rhs)
	{
		Matrix4 lt = lhs.Transpose();

		Vector4 lx = lt.XCol;
		Vector4 ly = lt.YCol;
		Vector4 lz = lt.ZCol;
		Vector4 lw = lt.WCol;

		Vector4 rx = rhs.XCol;
		Vector4 ry = rhs.YCol;
		Vector4 rz = rhs.ZCol;
		Vector4 rw = rhs.WCol;

		return Matrix4(
			lx.Dot(rx),
			ly.Dot(rx),
			lz.Dot(rx),
			lw.Dot(rx),

			lx.Dot(ry),
			ly.Dot(ry),
			lz.Dot(ry),
			lw.Dot(ry),

			lx.Dot(rz),
			ly.Dot(rz),
			lz.Dot(rz),
			lw.Dot(rz),

			lx.Dot(rw),
			ly.Dot(rw),
			lz.Dot(rw),
			lw.Dot(rw));
	}

	FORCEINLINE Vector4 operator*(Matrix4 const & lhs, Vector4 const & rhs)
	{
		#if 0 // Not sure which is faster, will have to benchmark

		Vector4 x = lhs.XCol;
		Vector4 y = lhs.YCol;
		Vector4 z = lhs.ZCol;
		Vector4 w = lhs.WCol;

		return 
			x.MultiplyAdd(        rhs.XXXX(), 
				y.MultiplyAdd(    rhs.YYYY(), 
					z.MultiplyAdd(rhs.ZZZZ(), 
						(w *      rhs.WWWW()))));
		#else

		Matrix4 tx = lhs.Transpose();

		return Vector4(
			tx.XCol.Dot(rhs),
			tx.YCol.Dot(rhs),
			tx.ZCol.Dot(rhs),
			tx.WCol.Dot(rhs));

		#endif
	}

	FORCEINLINE Matrix4 & operator*=(Matrix4 & lhs, Matrix4 const & rhs)
	{ 
		lhs = lhs * rhs; 
		return lhs; 
	}
	
	FORCEINLINE Matrix4::Matrix4() : Matrix4(Vector4::UnitX(), Vector4::UnitY(), Vector4::UnitZ(), Vector4::UnitW()) { }
	FORCEINLINE Matrix4::Matrix4(Matrix4 const & m) : XCol(m.XCol), YCol(m.YCol), ZCol(m.ZCol), WCol(m.WCol) { }
	
	FORCEINLINE Matrix4::Matrix4(Vector4 const & xCol, Vector4 const & yCol, Vector4 const & zCol, Vector4 const & wCol) 
		: XCol(xCol), YCol(yCol), ZCol(zCol), WCol(wCol) { }


	FORCEINLINE Matrix4::Matrix4(float f16) : XCol(f16), YCol(f16), ZCol(f16), WCol(f16) { }

	FORCEINLINE Matrix4::Matrix4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33) 
		: XCol(m00, m01, m02, m03)
		, YCol(m10, m11, m12, m13)
		, ZCol(m20, m21, m22, m23)
		, WCol(m30, m31, m32, m33)
	{ }

	FORCEINLINE Matrix4::Matrix4(Vector4 const & xyzwCol) : XCol(xyzwCol), YCol(xyzwCol), ZCol(xyzwCol), WCol(xyzwCol) { }
	FORCEINLINE Matrix4::Matrix4(float const * ary)       : XCol(ary),     YCol(ary+4),   ZCol(ary+8),   WCol(ary+12) { }

	FORCEINLINE Matrix4 Matrix4::  FromArray(float const * ary) { return Matrix4(ary); }
	FORCEINLINE void    Matrix4::CopyToArray(float       * ary) const 
	{ 
		XCol.CopyToArray(ary); 
		YCol.CopyToArray(ary +  4); 
		ZCol.CopyToArray(ary +  8); 
		WCol.CopyToArray(ary + 12); 
	}

	FORCEINLINE Matrix4 Matrix4::Transpose() const
	{
		#if defined USE_SIMD
		// In isolation this version is identical to the obvious implementation below, 
		// but the obvious implementation has cache penalties for the .Inverse*() methods.
		// So we do this instead, to avoid the cache penalties for huge benefit:
		Vector4 t0 = XCol.XAYB(YCol);
		Vector4 t1 = ZCol.XAYB(WCol);
		Vector4 t2 = XCol.ZCWD(YCol);
		Vector4 t3 = ZCol.ZCWD(WCol);

		return Matrix4(
			t0.XYAB(t1),
			t0.ZWCD(t1),
			t2.XYAB(t3),
			t2.ZWCD(t3));
		#else
		// Obvious implementation:
		return Matrix4(Row0(), Row1(), Row2(), Row3());
		#endif
	}

	FORCEINLINE float Matrix4::Determinant() const 
	{
		return
			  XCol.X * YCol.Y * ZCol.Z * WCol.W - XCol.X * YCol.Y * ZCol.W * WCol.Z
			+ XCol.X * YCol.Z * ZCol.W * WCol.Y - XCol.X * YCol.Z * ZCol.Y * WCol.W
			+ XCol.X * YCol.W * ZCol.Y * WCol.Z - XCol.X * YCol.W * ZCol.Z * WCol.Y
			+ XCol.Y * YCol.Z * ZCol.X * WCol.W - XCol.Y * YCol.Z * ZCol.W * WCol.X
			+ XCol.Y * YCol.W * ZCol.Z * WCol.X - XCol.Y * YCol.W * ZCol.X * WCol.Z
			+ XCol.Y * YCol.X * ZCol.W * WCol.Z - XCol.Y * YCol.X * ZCol.Z * WCol.W
			+ XCol.Z * YCol.W * ZCol.X * WCol.Y - XCol.Z * YCol.W * ZCol.Y * WCol.X
			+ XCol.Z * YCol.X * ZCol.Y * WCol.W - XCol.Z * YCol.X * ZCol.W * WCol.Y
			+ XCol.Z * YCol.Y * ZCol.W * WCol.X - XCol.Z * YCol.Y * ZCol.X * WCol.W
			+ XCol.W * YCol.X * ZCol.Z * WCol.Y - XCol.W * YCol.X * ZCol.Y * WCol.Z
			+ XCol.W * YCol.Y * ZCol.X * WCol.Z - XCol.W * YCol.Y * ZCol.Z * WCol.X
			+ XCol.W * YCol.Z * ZCol.Y * WCol.X - XCol.W * YCol.Z * ZCol.X * WCol.Y;
	}


	FORCEINLINE Matrix4 Matrix4::InverseAffine() const
	{
		Matrix3 A = SubMatrix3().Inverse();

		return Matrix4(A.XCol.XYZ0(), A.YCol.XYZ0(), A.ZCol.XYZ0(), (-WCol).XYZ1());
	}

	// I have tested this and it identical in performance as an inverse method that doesn't output the determinant.
	FORCEINLINE Matrix4 Matrix4::Inverse() const
	{
		float notused;
		return Inverse_Determinant(notused);
	}
	
	// I have tested this and it identical in performance as an inverse method that doesn't output the determinant.
	FORCEINLINE Matrix4 Matrix4::Normal() const
	{
		float notused;
		return InverseTranspose_Determinant(notused);
	}

	FORCEINLINE Matrix4 Matrix4::View() const
	{
		Vector4 right     =   XCol .XYZ0().Normalized();
		Vector4 up        =   YCol .XYZ0().Normalized();
		Vector4 forward   =   ZCol .XYZ0().Normalized();
		Vector4 nPosition = (-WCol).XYZ0();

		  right.InsertW(  right.Dot(nPosition));
		     up.InsertW(     up.Dot(nPosition));
		forward.InsertW(forward.Dot(nPosition));

		return Matrix4(right, up, forward, Vector4::UnitW()).Transpose();
	}

	#if defined USE_SIMD
	// Fastest options if you already need both anyways.
	FORCEINLINE Matrix4 Matrix4::Inverse_Determinant(float & outDet) const
	{
		return InverseTranspose_Determinant(outDet).Transpose();
	}

	FORCEINLINE Matrix4 Matrix4::InverseTranspose_Determinant(float & outDet) const 
	{
		Vector4 a1230 = XCol.Shuffle<1,2,3,0>();
		Vector4 a2301 = XCol.ZWXY();
		Vector4 a3012 = a1230.ZWXY();

		Vector4 b1230 = YCol.Shuffle<1,2,3,0>();
		Vector4 b2301 = YCol.ZWXY();
		Vector4 b3012 = b1230.ZWXY();

		Vector4 c1230 = ZCol.Shuffle<1,2,3,0>();
		Vector4 c2301 = ZCol.ZWXY();
		Vector4 c3012 = c1230.ZWXY();

		Vector4 d1230 = WCol.Shuffle<1,2,3,0>();
		Vector4 d2301 = WCol.ZWXY();
		Vector4 d3012 = d1230.ZWXY();

		Vector4 a3012_x_b0123 = a3012 * YCol;
		Vector4 a3012_x_b1230 = a3012 * b1230;
		Vector4 a3012_x_b2301 = a3012 * b2301;

		Vector4 c3012_x_d0123 = c3012 * WCol;
		Vector4 c3012_x_d1230 = c3012 * d1230;
		Vector4 c3012_x_d2301 = c3012 * d2301;

		Vector4 c2301_x_d1230 = c3012_x_d2301.Shuffle<3,0,1,2>();
		Vector4 a2301_x_b1230 = a3012_x_b2301.Shuffle<3,0,1,2>();

		Vector4 c1230_x_d2301 = c3012_x_d0123.ZWXY();
		Vector4 a1230_x_b2301 = a3012_x_b0123.ZWXY();

		Vector4 c1230_x_d3012 = c3012_x_d1230.ZWXY();
		Vector4 a1230_x_b3012 = a3012_x_b1230.ZWXY();

		Vector4 c2301_x_d3012 = c3012_x_d0123.Shuffle<3,0,1,2>();
		Vector4 a2301_x_b3012 = a3012_x_b0123.Shuffle<3,0,1,2>();

		// ---------------------------------------------------------------
		// The original version of vectorial had the subtractive operands 
		// backwards, resulting in the determinant having sign reversed.
		//Vector4 ab1 = a2301_x_b1230 - a1230_x_b2301;
		//Vector4 ab2 = a1230_x_b3012 - a3012_x_b1230;
		//Vector4 ab3 = a3012_x_b2301 - a2301_x_b3012;

		//Vector4 cd1 = c2301_x_d1230 - c1230_x_d2301;
		//Vector4 cd2 = c1230_x_d3012 - c3012_x_d1230;
		//Vector4 cd3 = c3012_x_d2301 - c2301_x_d3012;
		// ---------------------------------------------------------------

		Vector4 ab1 = a1230_x_b2301 - a2301_x_b1230;
		Vector4 ab2 = a3012_x_b1230 - a1230_x_b3012;
		Vector4 ab3 = a2301_x_b3012 - a3012_x_b2301;

		Vector4 cd1 = c1230_x_d2301 - c2301_x_d1230;
		Vector4 cd2 = c3012_x_d1230 - c1230_x_d3012;
		Vector4 cd3 = c2301_x_d3012 - c3012_x_d2301;

		Vector4 s1( 0.0f, -0.0f,  0.0f, -0.0f);
		Vector4 s2(-0.0f,  0.0f, -0.0f,  0.0f);

		// Should get a nice little boost on FMA3 archs!
		Vector4 c0_sum = a1230.MultiplyAdd(cd3, a2301.MultiplyAdd(cd2, a3012 * cd1));
		Vector4 c1_sum = b3012.MultiplyAdd(cd1, b2301.MultiplyAdd(cd2, b1230 * cd3));
		Vector4 c2_sum = c1230.MultiplyAdd(ab3, c2301.MultiplyAdd(ab2, c3012 * ab1));
		Vector4 c3_sum = d1230.MultiplyAdd(ab3, d2301.MultiplyAdd(ab2, d3012 * ab1));

		Vector4 d0  = c1_sum * XCol;

		Vector4  d1 = d0 + d0.ZWZW();
		float   det = d1.X - d1.Y;

		Vector4 invdet(1.0f / det);

		Vector4 o0 = (c1_sum ^ s1) * invdet;
		Vector4 o1 = (c0_sum ^ s2) * invdet;
		Vector4 o2 = (c3_sum ^ s1) * invdet;
		Vector4 o3 = (c2_sum ^ s2) * invdet;

		outDet = det;

		return Matrix4(o0, o1, o2, o3);
	}
	#else
	// Fastest options if you already need both anyways.
	FORCEINLINE Matrix4 Matrix4::InverseTranspose_Determinant(float & outDet) const
	{
		return Inverse_Determinant(outDet).Transpose();
	}

	FORCEINLINE Matrix4 Matrix4::Inverse_Determinant(float & outDet) const
	{
		float const * const m = reinterpret_cast<float const * const>(&XCol.X);

		Matrix4 ret;

		float * inv = reinterpret_cast<float *>(&ret.XCol.X);

		inv[ 0] =  m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
		inv[ 1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
		inv[ 2] =  m[1] * m[ 6] * m[15] - m[1] * m[ 7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[ 7] - m[13] * m[3] * m[ 6];
		inv[ 3] = -m[1] * m[ 6] * m[11] + m[1] * m[ 7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[ 9] * m[2] * m[ 7] + m[ 9] * m[3] * m[ 6];
		inv[ 4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
		inv[ 5] =  m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
		inv[ 6] = -m[0] * m[ 6] * m[15] + m[0] * m[ 7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[ 7] + m[12] * m[3] * m[ 6];
		inv[ 7] =  m[0] * m[ 6] * m[11] - m[0] * m[ 7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[ 8] * m[2] * m[ 7] - m[ 8] * m[3] * m[ 6];
		inv[ 8] =  m[4] * m[ 9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[ 9];
		inv[ 9] = -m[0] * m[ 9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[ 9];
		inv[10] =  m[0] * m[ 5] * m[15] - m[0] * m[ 7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[ 7] - m[12] * m[3] * m[ 5];
		inv[11] = -m[0] * m[ 5] * m[11] + m[0] * m[ 7] * m[ 9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[ 9] - m[ 8] * m[1] * m[ 7] + m[ 8] * m[3] * m[ 5];
		inv[12] = -m[4] * m[ 9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[ 9];
		inv[13] =  m[0] * m[ 9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[ 9];
		inv[14] = -m[0] * m[ 5] * m[14] + m[0] * m[ 6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[ 6] + m[12] * m[2] * m[ 5];
		inv[15] =  m[0] * m[ 5] * m[10] - m[0] * m[ 6] * m[ 9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[ 9] + m[ 8] * m[1] * m[ 6] - m[ 8] * m[2] * m[ 5];

		float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

		//if (WithinEpsilonOfZero(det))
		//	throw;

		outDet = det;

		det = 1.0f / det;

		for (int i = 0; i < 16; i++)
			inv[i] = inv[i] * det;

		return ret;
	}
	#endif


	FORCEINLINE Vector4 Matrix4::Row0()     const { return Vector4(XCol.X, YCol.X, ZCol.X, WCol.X); }
	FORCEINLINE Vector4 Matrix4::Row1()     const { return Vector4(XCol.Y, YCol.Y, ZCol.Y, WCol.Y); }
	FORCEINLINE Vector4 Matrix4::Row2()     const { return Vector4(XCol.Z, YCol.Z, ZCol.Z, WCol.Z); }
	FORCEINLINE Vector4 Matrix4::Row3()     const { return Vector4(XCol.W, YCol.W, ZCol.W, WCol.W); }
	FORCEINLINE Vector4 Matrix4::Diagonal() const { return Vector4(XCol.X, YCol.Y, ZCol.Z, WCol.W); }
	
	FORCEINLINE bool Matrix4::IsIdentity() const
	{
		return
			WCol.Compare<CMP::AllWE>(Vector4::UnitW()) && // W first to catch Translation matrices early.
			XCol.Compare<CMP::AllWE>(Vector4::UnitX()) &&
			YCol.Compare<CMP::AllWE>(Vector4::UnitY()) &&
			ZCol.Compare<CMP::AllWE>(Vector4::UnitZ());
	}

	FORCEINLINE bool Matrix4::IsSymmetric() const
	{
		Vector3 A0 (XCol.Y, XCol.Z, XCol.W);
		Vector3 B0 (YCol.X, ZCol.X, WCol.X);

		Vector3 A12(YCol.Z, YCol.W, ZCol.W);
		Vector3 B12(ZCol.Y, WCol.Y, WCol.Z);

		return A0.Compare<CMP::AllWE>(B0) && A12.Compare<CMP::AllWE>(B12);
	}

	template <CMP op>
	FORCEINLINE bool Matrix4::Compare(float rhs) const { return Compare<op>(Matrix4(rhs)); }

	template <CMP op>
	FORCEINLINE bool Matrix4::Compare(Matrix4 const & rhs) const
	{
		if (op == CMP::AnyLT)  { return WCol.Compare<op>(rhs.WCol) || XCol.Compare<op>(rhs.XCol) || YCol.Compare<op>(rhs.YCol) || ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AnyLEQ) { return WCol.Compare<op>(rhs.WCol) || XCol.Compare<op>(rhs.XCol) || YCol.Compare<op>(rhs.YCol) || ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AnyGT)  { return WCol.Compare<op>(rhs.WCol) || XCol.Compare<op>(rhs.XCol) || YCol.Compare<op>(rhs.YCol) || ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AnyGEQ) { return WCol.Compare<op>(rhs.WCol) || XCol.Compare<op>(rhs.XCol) || YCol.Compare<op>(rhs.YCol) || ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AnyEQ)  { return WCol.Compare<op>(rhs.WCol) || XCol.Compare<op>(rhs.XCol) || YCol.Compare<op>(rhs.YCol) || ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AnyNEQ) { return WCol.Compare<op>(rhs.WCol) || XCol.Compare<op>(rhs.XCol) || YCol.Compare<op>(rhs.YCol) || ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AnyWE)  { return WCol.Compare<op>(rhs.WCol) || XCol.Compare<op>(rhs.XCol) || YCol.Compare<op>(rhs.YCol) || ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AnyNWE) { return WCol.Compare<op>(rhs.WCol) || XCol.Compare<op>(rhs.XCol) || YCol.Compare<op>(rhs.YCol) || ZCol.Compare<op>(rhs.ZCol); }

		if (op == CMP::AllLT)  { return WCol.Compare<op>(rhs.WCol) && XCol.Compare<op>(rhs.XCol) && YCol.Compare<op>(rhs.YCol) && ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AllLEQ) { return WCol.Compare<op>(rhs.WCol) && XCol.Compare<op>(rhs.XCol) && YCol.Compare<op>(rhs.YCol) && ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AllGT)  { return WCol.Compare<op>(rhs.WCol) && XCol.Compare<op>(rhs.XCol) && YCol.Compare<op>(rhs.YCol) && ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AllGEQ) { return WCol.Compare<op>(rhs.WCol) && XCol.Compare<op>(rhs.XCol) && YCol.Compare<op>(rhs.YCol) && ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AllEQ)  { return WCol.Compare<op>(rhs.WCol) && XCol.Compare<op>(rhs.XCol) && YCol.Compare<op>(rhs.YCol) && ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AllNEQ) { return WCol.Compare<op>(rhs.WCol) && XCol.Compare<op>(rhs.XCol) && YCol.Compare<op>(rhs.YCol) && ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AllWE)  { return WCol.Compare<op>(rhs.WCol) && XCol.Compare<op>(rhs.XCol) && YCol.Compare<op>(rhs.YCol) && ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AllNWE) { return WCol.Compare<op>(rhs.WCol) && XCol.Compare<op>(rhs.XCol) && YCol.Compare<op>(rhs.YCol) && ZCol.Compare<op>(rhs.ZCol); }

		// Used for storing Vectors in collections. Compare NWE=>LT, X, then Y, then.... Default op for operator< on vector types.
		if (op == CMP::Ordinal)
		{
			if (XCol.Compare<CMP::AnyNWE>(rhs.XCol)) return XCol.Compare<CMP::Ordinal>(rhs.XCol);
			if (YCol.Compare<CMP::AnyNWE>(rhs.YCol)) return YCol.Compare<CMP::Ordinal>(rhs.YCol);
			if (ZCol.Compare<CMP::AnyNWE>(rhs.ZCol)) return ZCol.Compare<CMP::Ordinal>(rhs.ZCol);
			if (WCol.Compare<CMP::AnyNWE>(rhs.WCol)) return WCol.Compare<CMP::Ordinal>(rhs.WCol);
			return false;
		} 
	}

	FORCEINLINE float & Matrix4::operator[](int idx)       { return (&XCol.X)[idx]; }
	FORCEINLINE float   Matrix4::operator[](int idx) const { return (&XCol.X)[idx]; }

	/*static*/ FORCEINLINE Matrix4 Matrix4::Translate(Vector4 const & t)         { return Matrix4(Vector4::UnitX(), Vector4::UnitY(), Vector4::UnitZ(), t); }
	/*static*/ FORCEINLINE Matrix4 Matrix4::Translate(Vector3 const & t)         { return Matrix4(Vector4::UnitX(), Vector4::UnitY(), Vector4::UnitZ(), t.XYZ1()); }
	/*static*/ FORCEINLINE Matrix4 Matrix4::Translate(Vector2 const & t)         { return Matrix4(Vector4::UnitX(), Vector4::UnitY(), Vector4::UnitZ(), t.XY01()); }
	/*static*/ FORCEINLINE Matrix4 Matrix4::Translate(float x, float y, float z) { return Matrix4(Vector4::UnitX(), Vector4::UnitY(), Vector4::UnitZ(), Vector4(x, y,    z, 1.0f)); }
	/*static*/ FORCEINLINE Matrix4 Matrix4::Translate(float x, float y)          { return Matrix4(Vector4::UnitX(), Vector4::UnitY(), Vector4::UnitZ(), Vector4(x, y, 0.0f, 1.0f)); }

	/*static*/ FORCEINLINE Matrix4 Matrix4::RotateAboutZAxisXYPoint(float thetaRadians, Vector2 const & point)
	{
		return 
			Translate(point.XY00()) *
			RotateZAxis(thetaRadians) *
			Translate(-point.XY00());
	}

	/*static*/ FORCEINLINE Matrix4 Matrix4::RotateAboutAxisPoint(float thetaradians, Vector3 const & axis, Vector3 const & point)
	{
		return 
			Translate(point) *
			RotateAboutAxis(axis, thetaradians) *
			Translate(-point);
	}

	/*static*/ FORCEINLINE Matrix4 Matrix4::Scale(Vector3 const & v)
	{
		return Matrix4(
			Vector4( v.X, 0.0f, 0.0f, 0.0f), 
			Vector4(0.0f,  v.Y, 0.0f, 0.0f),
			Vector4(0.0f, 0.0f,  v.Z, 0.0f),
			Vector4::UnitW());
	}

	/*static*/ FORCEINLINE Matrix4 Matrix4::Scale(float v)
	{
		return Matrix4(
			Vector4(   v, 0.0f, 0.0f, 0.0f), 
			Vector4(0.0f,    v, 0.0f, 0.0f),
			Vector4(0.0f, 0.0f,    v, 0.0f),
			Vector4::UnitW());
	}

	/*static*/ FORCEINLINE Matrix4 Matrix4::Scale(float sX, float sY, float sZ)
	{
		return Matrix4(
			Vector4(  sX, 0.0f, 0.0f, 0.0f), 
			Vector4(0.0f,   sY, 0.0f, 0.0f),
			Vector4(0.0f, 0.0f,   sZ, 0.0f),
			Vector4::UnitW());
	}
	
	/*static*/ FORCEINLINE Matrix4 Matrix4::RotateXAxis(float thetaRadians)
	{
		float c = std::cos(thetaRadians);
		float s = std::sin(thetaRadians);

		return Matrix4(
			Vector4::UnitX(),
			Vector4(0.0f, c, -s, 0.0f),
			Vector4(0.0f, s,  c, 0.0f),
			Vector4::UnitW());
	}

	/*static*/FORCEINLINE  Matrix4 Matrix4::RotateYAxis(float thetaRadians)
	{
		float c = std::cos(thetaRadians);
		float s = std::sin(thetaRadians);

		return Matrix4(
			Vector4( c, 0.0f, s, 0.0f),
			Vector4::UnitY(),
			Vector4(-s, 0.0f, c, 0.0f),
			Vector4::UnitW());
	}

	/*static*/ FORCEINLINE Matrix4 Matrix4::RotateZAxis(float thetaRadians)
	{
		float c = std::cos(thetaRadians);
		float s = std::sin(thetaRadians);

		return Matrix4(
			Vector4(c, -s, 0.0f, 0.0f),
			Vector4(s,  c, 0.0f, 0.0f),
			Vector4::UnitZ(),
			Vector4::UnitW());
	}

	/*static*/ FORCEINLINE Matrix4 Matrix4::RotateAboutAxis(Vector3 const & axis, float theta)
	{
		Vector4 u  = axis.XYZ0().Normalized();
		Vector4 un = -u;

		float c = std::cos(theta);
		float s = std::sin(theta);

		float t = 1.0f - c;

		Vector4 rx = t * u * u.XXXX();
		Vector4 ry = t * u * u.YYYY();
		Vector4 rz = t * u * u.ZZZZ();

		rx += (s * Vector4(0.0f, un.Z, u.Y, 0.0f)).InsertX(c);
		ry += (s * Vector4(u.Z, 0.0f, un.X, 0.0f)).InsertY(c);
		rz += (s * Vector4(un.Y, u.X, 0.0f, 0.0f)).InsertZ(c);

		return Matrix4(rx, ry, rz, Vector4::UnitW());
	}

	/*static*/ FORCEINLINE Matrix4 Matrix4::Skew2D(float x_sub_y, float y_sub_x)
	{
		return Matrix4(
			Vector4(1.0f, y_sub_x, 0.0f, 0.0f),
			Vector4(x_sub_y, 1.0f, 0.0f, 0.0f),
			Vector4::UnitZ(),
			Vector4::UnitW());
	}


	// Note: Don't reduce these to the single param ctor, it will be slower, due to replication.
	/*static*/ FORCEINLINE Matrix4 Matrix4::Zero()     { return Matrix4(Vector4::Zero(),  Vector4::Zero(),  Vector4::Zero(),  Vector4::Zero());  } 
	/*static*/ FORCEINLINE Matrix4 Matrix4::One()      { return Matrix4(Vector4::One(),   Vector4::One(),   Vector4::One(),   Vector4::One());   }
	/*static*/ FORCEINLINE Matrix4 Matrix4::Identity() { return Matrix4(Vector4::UnitX(), Vector4::UnitY(), Vector4::UnitZ(), Vector4::UnitW()); }
}

static inline std::ostream & operator<<(std::ostream& os, ::HANDYMATH_NS::Matrix4 const & v) 
{
	os << std::endl
	   << "[ " << v.XCol.X << ",\t" << v.YCol.X << ",\t" << v.ZCol.X << ",\t" << v.WCol.X << std::endl
	   << "  " << v.XCol.Y << ",\t" << v.YCol.Y << ",\t" << v.ZCol.Y << ",\t" << v.WCol.Y << std::endl
	   << "  " << v.XCol.Z << ",\t" << v.YCol.Z << ",\t" << v.ZCol.Z << ",\t" << v.WCol.Z << std::endl
	   << "  " << v.XCol.W << ",\t" << v.YCol.W << ",\t" << v.ZCol.W << ",\t" << v.WCol.W << " ]";

	return os;
}

MAKE_HASHABLE(HANDYMATH_NS::Matrix4,                  \
			  t.XCol.X, t.XCol.Y, t.XCol.Z, t.XCol.W, \
			  t.YCol.X, t.YCol.Y, t.YCol.Z, t.YCol.W, \
			  t.ZCol.X, t.ZCol.Y, t.ZCol.Z, t.ZCol.W, \
			  t.WCol.X, t.WCol.Y, t.WCol.Z, t.WCol.W)


// This is a stripped down version of InverseTranspose_Determinant(). It is about 5 times faster 
// than using InverseTranspose_Determinant() and throwing away the inverse.
// Unfortunaly, this version actually ends up being (slightly) slower than the scalar version :'(
//FORCEINLINE float Matrix4::Determinant() const 
//{
	//Vector4 b1230 = YCol.Shuffle<1,2,3,0>();
	//Vector4 b2301 = YCol.ZWXY();
	//Vector4 b3012 = b1230.ZWXY();

	//Vector4 c3012 = ZCol.Shuffle<3,0,1,2>();

	//Vector4 d1230 = WCol.Shuffle<1,2,3,0>();
	//Vector4 d2301 = WCol.ZWXY();

	//Vector4 c3012_x_d0123 = c3012 * WCol;
	//Vector4 c3012_x_d1230 = c3012 * d1230;
	//Vector4 c3012_x_d2301 = c3012 * d2301;

	//Vector4 c2301_x_d1230 = c3012_x_d2301.Shuffle<3,0,1,2>();
	//Vector4 c1230_x_d2301 = c3012_x_d0123.ZWXY();
	//Vector4 c1230_x_d3012 = c3012_x_d1230.ZWXY();
	//Vector4 c2301_x_d3012 = c3012_x_d0123.Shuffle<3,0,1,2>();

	//// ---------------------------------------------------------------
	//// The original version of vectorial had the subtractive operands 
	//// backwards, resulting in the determinant having sign reversed.
	////Vector4 cd1 = c2301_x_d1230 - c1230_x_d2301;
	////Vector4 cd2 = c1230_x_d3012 - c3012_x_d1230;
	////Vector4 cd3 = c3012_x_d2301 - c2301_x_d3012;
	//// ---------------------------------------------------------------

	//Vector4 cd1 = c1230_x_d2301 - c2301_x_d1230;
	//Vector4 cd2 = c3012_x_d1230 - c1230_x_d3012;
	//Vector4 cd3 = c2301_x_d3012 - c3012_x_d2301;

	//// Should get a nice little boost on FMA3 archs!
	//Vector4 c1_sum = b3012.MultiplyAdd(cd1, b2301.MultiplyAdd(cd2, b1230 * cd3));

	//Vector4 d0  = c1_sum * XCol;
	//Vector4 d1  = d0 + d0.ZWZW();

	//return d1.X - d1.Y;
//}


// Another implementation:
//FORCEINLINE Matrix4 Matrix4::Inverse_Determinant(float & outDet) const 
//{
//	const float * src = &XCol.X;
//
//	Matrix4 DM;
//	float * dst = &DM.XCol.X;
//
//	Vector4 minor0, minor1, minor2, minor3;
//	Vector4 row0, row1, row2, row3;
//	Vector4 det, tmp1;
//
//	Vector4 agk1 = _mm_loadl_pi(tmp1, (__m64*)(src));
//
//	tmp1 = _mm_loadh_pi(agk1, (__m64*)(src + 4));
//	row1 = _mm_loadh_pi(_mm_loadl_pi(row1, (__m64*)(src+8)), (__m64*)(src+12));
//
//	row0 = _mm_shuffle_ps(tmp1, row1, 0x88);
//	row1 = _mm_shuffle_ps(row1, tmp1, 0xDD);
//
//	tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src+ 2)), (__m64*)(src+ 6));
//	row3 = _mm_loadh_pi(_mm_loadl_pi(row3, (__m64*)(src+10)), (__m64*)(src+14));
//
//	row2 = _mm_shuffle_ps(tmp1, row3, 0x88);
//	row3 = _mm_shuffle_ps(row3, tmp1, 0xDD);
//
//	tmp1 = _mm_mul_ps(row2, row3);
//	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
//
//	minor0 = _mm_mul_ps(row1, tmp1);
//	minor1 = _mm_mul_ps(row0, tmp1);
//
//	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
//
//	minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
//	minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
//	minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);
//
//	tmp1 = _mm_mul_ps(row1, row2);
//	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
//
//	minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
//	minor3 = _mm_mul_ps(row0, tmp1);
//
//	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
//
//	minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
//	minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
//	minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);
//
//	tmp1 = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
//	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
//	row2 = _mm_shuffle_ps(row2, row2, 0x4E);
//
//	minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
//	minor2 = _mm_mul_ps(row0, tmp1);
//
//	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
//
//	minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
//	minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
//	minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);
//
//	tmp1 = _mm_mul_ps(row0, row1);
//	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
//
//	minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
//	minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);
//
//	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
//
//	minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
//	minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));
//
//	tmp1 = _mm_mul_ps(row0, row3);
//	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
//
//	minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
//	minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);
//
//	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
//
//	minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
//	minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));
//
//	tmp1 = _mm_mul_ps(row0, row2);
//	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
//
//	minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
//	minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));
//
//	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
//
//	minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
//	minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);
//
//	det = _mm_mul_ps(row0, minor0);
//	det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
//	det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
//
//	tmp1 = _mm_rcp_ss(det);
//
//	det = _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
//	det = _mm_shuffle_ps(det, det, 0x00);
//
//	minor0 = _mm_mul_ps(det, minor0);
//	_mm_storel_pi((__m64*)(dst), minor0);
//	_mm_storeh_pi((__m64*)(dst+2), minor0);
//
//	minor1 = _mm_mul_ps(det, minor1);
//	_mm_storel_pi((__m64*)(dst+4), minor1);
//	_mm_storeh_pi((__m64*)(dst+6), minor1);
//
//	minor2 = _mm_mul_ps(det, minor2);
//	_mm_storel_pi((__m64*)(dst+ 8), minor2);
//	_mm_storeh_pi((__m64*)(dst+10), minor2);
//
//	minor3 = _mm_mul_ps(det, minor3);
//	_mm_storel_pi((__m64*)(dst+12), minor3);
//	_mm_storeh_pi((__m64*)(dst+14), minor3);
//
//	outDet = det.X;
//
//	return DM;
//}

//FORCEINLINE Vector4 mul2x2(Vector4 const & a, Vector4 const & b)
//{
//	return b.XXZZ() * a.XYXY() + b.YYWW() * a.ZWZW();
//}
//
//FORCEINLINE Vector4 det2x2(Vector4 const & m)
//{
//	Vector4 t = m.XXYY() * m.Shuffle<3,3,2,2>(); // * m.WWZZ()
//
//	return t.XXYY() - t.ZZWW();
//}
//
//FORCEINLINE Vector4 adj2x2(Vector4 const & m)
//{
//	Vector4 znzn(0.0f, -0.0f, -0.0f, 0.0f);
//
//	Vector4 mWYZX = m.Shuffle<3, 1, 2, 0>();
//
//	return mWYZX ^ znzn;
//}
//
//FORCEINLINE Vector4 inv2x2(Vector4 const & m)
//{
//	return adj2x2(m) * det2x2(m).Reciprocal();
//}


//// THIS METHOD IS CLEANER LOOKING BUT MUCH SLOWER!
//FORCEINLINE Matrix4 Matrix4::Inverse_Determinant(float & outDet) const 
//{
//	// Sub matrices
//	Vector4 s00 = XCol.XYAB(YCol);
//	Vector4 s10 = XCol.ZWCD(YCol);
//	Vector4 s01 = ZCol.XYAB(WCol);
//	Vector4 s11 = ZCol.ZWCD(WCol);
//	
//	// Inverse sub matrices
//	Vector4 inv11 = inv2x2(s00);
//	Vector4 inv22 = inv2x2(s11);
//
//	// Partitions
//	Vector4 _s00 = inv2x2(s00 - mul2x2(mul2x2(   s01, inv22),  s10));
//	Vector4 _s10 =       (      mul2x2(mul2x2(-inv22,   s10), _s00));
//	Vector4 _s11 = inv2x2(s11 - mul2x2(mul2x2(   s10, inv11),  s01));
//	Vector4 _s01 =       (      mul2x2(mul2x2(-inv11,   s01), _s11));
//
//	outDet = 42.0f; // TODO
//
//	// Return inverse
//	return Matrix4(
//		_s00.XYAB(_s10),
//		_s00.ZWCD(_s10),
//		_s01.XYAB(_s11),
//		_s01.ZWCD(_s11));
//}
