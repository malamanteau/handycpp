
/// See ../../License.txt for license info.

#pragma once

#include <cmath>
#include <tuple>

#include "Base.hpp"
#include "Vector3.hpp"

namespace HANDYMATH_NS {

	class CMatrix3 /// Compact version of Matrix3, for storage, rather than computation.
	{
	public:
		CVector3 XCol;
		CVector3 YCol;
		CVector3 ZCol;

		CMatrix3() = default;
		CMatrix3(CVector3 const & xCol, CVector3 const & yCol, CVector3 const & zCol) : XCol(xCol), YCol(yCol), ZCol(zCol) { }

		CMatrix3 & operator=(CMatrix3 const &) = default;
	};

	class Matrix3 
	{
	public:
		static constexpr int32_t ElementBytes      =  4_i32;
		static constexpr int32_t StrideBytes       = 48_i32;
		static constexpr int32_t NumColumns        =  3_i32;
		static constexpr int32_t ColumnUsefulBytes = 12_i32;
		static constexpr int32_t ColumnStrideBytes = 16_i32;

		Vector3 XCol;
		Vector3 YCol;
		Vector3 ZCol;

		//union 
		//{
		//	struct 
		//	{
		//		Vector3 XCol;
		//		Vector3 YCol;
		//		Vector3 ZCol;
		//	};

		//	struct
		//	{
		//		float M00; // Col 0 (1st), Row 0 (1st)
		//		float M01; // Col 0 (1st), Row 1 (2nd)
		//		float M02; // Col 0 (1st), Row 2 (3rd)
		//		float _unusedA; // Space, Col 0, Row 3
		//		float M10; // Col 1 (2nd), Row 0 (1st)
		//		float M11; // Col 1 (2nd), Row 1 (2nd)
		//		float M12; // Col 1 (2nd), Row 2 (3rd)
		//		float _unusedB; // Space, Col 1, Row 3
		//		float M20; // Col 2 (3rd), Row 0 (1st)
		//		float M21; // Col 2 (3rd), Row 1 (2nd)
		//		float M22; // Col 2 (3rd), Row 2 (3rd)
		//		float _unusedC; // Space, Col 2, Row 3
		//	};

		//	float Array[12];
		//};

		CMatrix3 Compact() const;

		Matrix3();
		Matrix3(Matrix3 const & m);
		Matrix3(Vector3 const & xCol, Vector3 const & yCol, Vector3 const & zCol);
		Matrix3(
			float m00, float m01, float m02,
			float m10, float m11, float m12,
			float m20, float m21, float m22);

		explicit Matrix3(float f9);
		explicit Matrix3(Vector3 const & xyzCol);
		explicit Matrix3(float const * ary);

		static Matrix3 FromArray  (float const * ary);
		void           CopyToArray(float       * ary) const;

		Matrix3 Transpose()        const;
		float   Determinant()      const;
		Matrix3 Inverse()          const;
		Matrix3 Normal()           const; // Inverted Transpose

		Vector3 Row0() const;
		Vector3 Row1() const;
		Vector3 Row2() const;
		Vector3 Diagonal() const;

		bool IsIdentity()  const;
		bool IsSymmetric() const;

		template <CMP op> bool Compare(float           rhs) const;
		template <CMP op> bool Compare(Matrix3 const & rhs) const;

		float & operator[](int idx);
		float   operator[](int idx) const;

		static Matrix3 Zero();
		static Matrix3 One();
		static Matrix3 Identity();

		template <class SerialOp> void serial(SerialOp & ser)
		{
			ser
				(XCol)
				(YCol)
				(ZCol);
		}
	};

	/// COMPONENT-WISE MULTIPLICATION!
	Matrix3   operator^ (Matrix3 const & lhs, Matrix3 const & rhs);
	Matrix3 & operator^=(Matrix3       & lhs, Matrix3 const & rhs);

	Vector3   operator* (Matrix3 const & lhs, Vector3 const & rhs);
	Matrix3   operator* (Matrix3 const & lhs, Matrix3 const & rhs);
	Matrix3 & operator*=(Matrix3       & lhs, Matrix3 const & rhs);


	// TODO ADD SCALAR AND COMPONENT-WISE OPERATORS!


	FORCEINLINE CMatrix3 Matrix3::Compact() const
	{
		return CMatrix3(XCol.Compact, YCol.Compact, ZCol.Compact);
	}

	FORCEINLINE Matrix3 operator^ (Matrix3 const & lhs, Matrix3 const & rhs)
	{
		return Matrix3(
			lhs.XCol * rhs.XCol,
			lhs.YCol * rhs.YCol,
			lhs.ZCol * rhs.ZCol);

	}

	FORCEINLINE Matrix3 & operator^=(Matrix3       & lhs, Matrix3 const & rhs)
	{
		lhs = lhs ^ rhs; 
		return lhs; 
	}

	FORCEINLINE Matrix3 operator*(Matrix3 const & lhs, Matrix3 const & rhs)
	{
		Matrix3 lt = lhs.Transpose();

		Vector3 lx = lt.XCol;
		Vector3 ly = lt.YCol;
		Vector3 lz = lt.ZCol;

		Vector3 rx = rhs.XCol;
		Vector3 ry = rhs.YCol;
		Vector3 rz = rhs.ZCol;

		return Matrix3(
			lx.Dot(rx),
			ly.Dot(rx),
			lz.Dot(rx),

			lx.Dot(ry),
			ly.Dot(ry),
			lz.Dot(ry),

			lx.Dot(rz),
			ly.Dot(rz),
			lz.Dot(rz));
	}

	FORCEINLINE Vector3 operator*(Matrix3 const & lhs, Vector3 const & rhs)
	{
		#if 0 // Not sure which is faster, will have to benchmark and test

		Vector3 x = lhs.XCol;
		Vector3 y = lhs.YCol;
		Vector3 z = lhs.ZCol;

		return 
			x.MultiplyAdd(        rhs.XXXX(), 
				y.MultiplyAdd(    rhs.YYYY(), 
					(z *      rhs.ZZZZ())));
		#else

		Matrix3 tx = lhs.Transpose();

		return Vector3(
			tx.XCol.Dot(rhs),
			tx.YCol.Dot(rhs),
			tx.ZCol.Dot(rhs));
		#endif
	}

	FORCEINLINE Matrix3 & operator*=(Matrix3 & lhs, Matrix3 const & rhs)
	{ 
		lhs = lhs * rhs; 
		return lhs; 
	}
	
	FORCEINLINE Matrix3::Matrix3() : Matrix3(Vector3::UnitX(), Vector3::UnitY(), Vector3::UnitZ()) { }
	FORCEINLINE Matrix3::Matrix3(Matrix3 const & m) : XCol(m.XCol), YCol(m.YCol), ZCol(m.ZCol) { }

	FORCEINLINE Matrix3::Matrix3(Vector3 const & xCol, Vector3 const & yCol, Vector3 const & zCol) 
		: XCol(xCol), YCol(yCol), ZCol(zCol) { }


	FORCEINLINE Matrix3::Matrix3(float f9) : XCol(f9), YCol(f9), ZCol(f9) { }

	FORCEINLINE Matrix3::Matrix3(
		float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22) 
		: XCol(m00, m01, m02)
		, YCol(m10, m11, m12)
		, ZCol(m20, m21, m22)
	{ }

	FORCEINLINE Matrix3::Matrix3(Vector3 const & xyzCol) : XCol(xyzCol), YCol(xyzCol), ZCol(xyzCol) { }
	FORCEINLINE Matrix3::Matrix3(float const * ary)      : XCol(ary),    YCol(ary+3),  ZCol(ary+6)  { }

	FORCEINLINE Matrix3 Matrix3::  FromArray(float const * ary) { return Matrix3(ary); }
	FORCEINLINE void    Matrix3::CopyToArray(float       * ary) const 
	{ 
		XCol.CopyToArray(ary);
		YCol.CopyToArray(ary +  3);
		ZCol.CopyToArray(ary +  6);
	}

	FORCEINLINE Matrix3 Matrix3::Transpose()   const { return Matrix3(Row0(), Row1(), Row2()); }
	FORCEINLINE float   Matrix3::Determinant() const { return XCol.Dot(YCol.Cross(ZCol)); }
	FORCEINLINE Matrix3 Matrix3::Inverse()     const { return Normal().Transpose(); }

	FORCEINLINE Matrix3 Matrix3::Normal() const
	{
		Vector3 invDet(1_f / Determinant());

		return Matrix3(
			YCol.Cross(ZCol) * invDet,
			ZCol.Cross(XCol) * invDet,
			XCol.Cross(YCol) * invDet);
	}

	FORCEINLINE Vector3 Matrix3::Row0()     const { return Vector3(XCol.X, YCol.X, ZCol.X); }
	FORCEINLINE Vector3 Matrix3::Row1()     const { return Vector3(XCol.Y, YCol.Y, ZCol.Y); }
	FORCEINLINE Vector3 Matrix3::Row2()     const { return Vector3(XCol.Z, YCol.Z, ZCol.Z); }
	FORCEINLINE Vector3 Matrix3::Diagonal() const { return Vector3(XCol.X, YCol.Y, ZCol.Z); }

	FORCEINLINE bool Matrix3::IsIdentity() const
	{
		return
			XCol.Compare<CMP::AllWE>(Vector3::UnitX()) && // W first to catch Translation matrices early.
			YCol.Compare<CMP::AllWE>(Vector3::UnitY()) &&
			ZCol.Compare<CMP::AllWE>(Vector3::UnitZ());
	}

	FORCEINLINE bool Matrix3::IsSymmetric() const { return HANDY_NS::WithinEpsilon(XCol.Z, ZCol.X); }

	template <CMP op>
	FORCEINLINE bool Matrix3::Compare(float rhs) const { return Compare<op>(Matrix3(rhs)); }

	template <CMP op>
	FORCEINLINE bool Matrix3::Compare(Matrix3 const & rhs) const
	{
		if (op == CMP::AnyLT)  { return XCol.Compare<op>(rhs.XCol) || YCol.Compare<op>(rhs.YCol) || ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AnyLEQ) { return XCol.Compare<op>(rhs.XCol) || YCol.Compare<op>(rhs.YCol) || ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AnyGT)  { return XCol.Compare<op>(rhs.XCol) || YCol.Compare<op>(rhs.YCol) || ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AnyGEQ) { return XCol.Compare<op>(rhs.XCol) || YCol.Compare<op>(rhs.YCol) || ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AnyEQ)  { return XCol.Compare<op>(rhs.XCol) || YCol.Compare<op>(rhs.YCol) || ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AnyNEQ) { return XCol.Compare<op>(rhs.XCol) || YCol.Compare<op>(rhs.YCol) || ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AnyWE)  { return XCol.Compare<op>(rhs.XCol) || YCol.Compare<op>(rhs.YCol) || ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AnyNWE) { return XCol.Compare<op>(rhs.XCol) || YCol.Compare<op>(rhs.YCol) || ZCol.Compare<op>(rhs.ZCol); }

		if (op == CMP::AllLT)  { return XCol.Compare<op>(rhs.XCol) && YCol.Compare<op>(rhs.YCol) && ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AllLEQ) { return XCol.Compare<op>(rhs.XCol) && YCol.Compare<op>(rhs.YCol) && ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AllGT)  { return XCol.Compare<op>(rhs.XCol) && YCol.Compare<op>(rhs.YCol) && ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AllGEQ) { return XCol.Compare<op>(rhs.XCol) && YCol.Compare<op>(rhs.YCol) && ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AllEQ)  { return XCol.Compare<op>(rhs.XCol) && YCol.Compare<op>(rhs.YCol) && ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AllNEQ) { return XCol.Compare<op>(rhs.XCol) && YCol.Compare<op>(rhs.YCol) && ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AllWE)  { return XCol.Compare<op>(rhs.XCol) && YCol.Compare<op>(rhs.YCol) && ZCol.Compare<op>(rhs.ZCol); }
		if (op == CMP::AllNWE) { return XCol.Compare<op>(rhs.XCol) && YCol.Compare<op>(rhs.YCol) && ZCol.Compare<op>(rhs.ZCol); }

		// Used for storing Vectors in collections. Compare NWE=>LT, X, then Y, then.... Default op for operator< on vector types.
		if (op == CMP::Ordinal)
		{
			if (XCol.Compare<CMP::AnyNWE>(rhs.XCol)) return XCol.Compare<CMP::Ordinal>(rhs.XCol);
			if (YCol.Compare<CMP::AnyNWE>(rhs.YCol)) return YCol.Compare<CMP::Ordinal>(rhs.YCol);
			if (ZCol.Compare<CMP::AnyNWE>(rhs.ZCol)) return ZCol.Compare<CMP::Ordinal>(rhs.ZCol);
			return false;
		} 
	}

	FORCEINLINE float & Matrix3::operator[](int idx)       { return (&XCol.X)[idx]; }
	FORCEINLINE float   Matrix3::operator[](int idx) const { return (&XCol.X)[idx]; }

	// Note: Don't reduce these to the single param ctor, it will be slower, due to replication.
	/*static*/ FORCEINLINE Matrix3 Matrix3::Zero()     { return Matrix3(Vector3::Zero(),  Vector3::Zero(),  Vector3::Zero());  } 
	/*static*/ FORCEINLINE Matrix3 Matrix3::One()      { return Matrix3(Vector3::One(),   Vector3::One(),   Vector3::One());   }
	/*static*/ FORCEINLINE Matrix3 Matrix3::Identity() { return Matrix3(Vector3::UnitX(), Vector3::UnitY(), Vector3::UnitZ()); }
}

static inline std::ostream & operator<<(std::ostream& os, ::HANDYMATH_NS::Matrix3 const & v) 
{
	os << std::endl
		<< "[ " << v.XCol.X << ",\t" << v.YCol.X << ",\t" << v.ZCol.X << std::endl
		<< "  " << v.XCol.Y << ",\t" << v.YCol.Y << ",\t" << v.ZCol.Y << std::endl
		<< "  " << v.XCol.Z << ",\t" << v.YCol.Z << ",\t" << v.ZCol.Z << " ]";

	return os;
}
