
/// See ../../License.txt for license info.

#pragma once

#include "Base.hpp"

#include "Vector4.hpp"
#include "Vector3.hpp"
#include "Vector2.hpp"

#include "Vector4d.hpp"
#include "Vector3d.hpp"
#include "Vector2d.hpp"

#include "Vector4i.hpp"
#include "Vector3i.hpp"
#include "Vector2i.hpp"

#include "RGB888.hpp"
#include "RGBA8888.hpp"

#include "Matrix3.hpp"
#include "Matrix4.hpp"

//#include "Bitset128.hpp"

namespace HANDYMATH_NS {

#ifdef USE_SCALAR
	FORCEINLINE Vector4i Vector4 ::SI128() const { return Vector4i(*(int32_t*)&X, *(int32_t*)&Y, *(int32_t*)&Z, *(int32_t*)&W); }
	FORCEINLINE Vector4  Vector4i::PS128() const { return Vector4 (*(float*)&X, *(float*)&Y, *(float*)&Z, *(float*)&W); }
#elif defined(USE_SIMD)
	FORCEINLINE Vector4i Vector4 ::SI128() const { return Vector4i(_mm_castps_si128(Native)); }
	FORCEINLINE Vector4  Vector4i::PS128() const { return Vector4 (_mm_castsi128_ps(Native)); }
#elif defined(USE_NEON)

#else
	#error No implementation defined
#endif

// Vector4:
FORCEINLINE Vector2 Vector4::XY()  const { return Vector2(XY00());       }
FORCEINLINE Vector2 Vector4::ZW()  const { return Vector2(ZWCD(Zero())); }
FORCEINLINE Vector3 Vector4::XYZ() const { return Vector3(XYZ0());       }

// Vector3:
FORCEINLINE Vector2 Vector3::XY()                     const { return Vector2(Native.XY00()); }
FORCEINLINE Vector4 Vector3::XYZW(float w)            const { return Native.InsertW(w);      }

// Vector2:
FORCEINLINE Vector3 Vector2::XYZ (float z)            const { return Vector3(Native.InsertZ(z));        }
FORCEINLINE Vector4 Vector2::XYZW(float z, float w)   const { return Native.XYAB(Vector2(z, w).Native); }
FORCEINLINE Vector4 Vector2::XYZW(Vector2 const & zw) const { return Native.XYAB(zw.Native);            }

// Vector4d:
FORCEINLINE Vector2d Vector4d::XY()  const { return Vector2d(X, Y); }
FORCEINLINE Vector2d Vector4d::ZW()  const { return Vector2d(Z, W); } //(ZWCD(Zero())); }
FORCEINLINE Vector3d Vector4d::XYZ() const { return Vector3d(XYZ0());       }

// Vector3d:
FORCEINLINE Vector2d Vector3d::XY()                      const { return Vector2d(X, Y); }
FORCEINLINE Vector4d Vector3d::XYZW(double w)            const { return Native.InsertW(w);      }

// Vector4i:
FORCEINLINE Vector2i Vector4i::XY()  const { return Vector2i(XY00());       }
FORCEINLINE Vector2i Vector4i::ZW()  const { return Vector2i(ZWCD(Zero())); }
FORCEINLINE Vector3i Vector4i::XYZ() const { return Vector3i(XYZ0());       }

// Vector3i:
FORCEINLINE Vector2i Vector3i::XY()            const { return Vector2i(Native.XY00()); }
FORCEINLINE Vector4i Vector3i::XYZW(int32_t w) const { return Native.InsertW(w);       }

// Vector2i:
FORCEINLINE Vector3i Vector2i::XYZ (int32_t z)            const { return Vector3i(Native.InsertZ(z));        }
FORCEINLINE Vector4i Vector2i::XYZW(int32_t z, int32_t w) const { return Native.XYAB(Vector2i(z, w).Native); }
FORCEINLINE Vector4i Vector2i::XYZW(Vector2i const & zw)  const { return Native.XYAB(zw.Native);             }

FORCEINLINE RGB888   RGBA8888::ToRGB()           const { return RGB888  (R, G, B); }
FORCEINLINE RGBA8888 RGB888  ::ToRGBA(uint8_t a) const { return RGBA8888(R, G, B, a); }

FORCEINLINE Vector4  Vector4d::ToFloat() const
{
	SIMD_IMP_AVX   (return _mm256_cvtpd_ps(Native);)
	SIMD_IMP_NO_AVX(return _mm_shuffle_ps(_mm_cvtpd_ps(NativeXY), _mm_cvtpd_ps(NativeZW), _MM_SHUFFLE(1, 0, 1, 0));)
	NON_SIMD_IMP   (return Vector4((float)X, (float)Y, (float)Z, (float)W);)
}

FORCEINLINE Vector4i Vector4 ::ToInteger() const 
{ 
	SIMD_IMP    (return Vector4i(_mm_cvttps_epi32(Native));)
	NON_SIMD_IMP(return Vector4i(int32_t(X), int32_t(Y), int32_t(Z), int32_t(W));)
}

FORCEINLINE Vector4  Vector4i::ToFloat()   const 
{
	SIMD_IMP    (return Vector4(_mm_cvtepi32_ps(Native));)
	NON_SIMD_IMP(return Vector4(float(X), float(Y), float(Z), float(W));)
}

FORCEINLINE Vector3i Vector3 ::ToInteger() const { return Native.ToInteger(); }
FORCEINLINE Vector3  Vector3i::ToFloat()   const { return Native.ToFloat(); }
FORCEINLINE Vector3  Vector3d::ToFloat()   const { return Native.ToFloat(); }

FORCEINLINE Vector2i Vector2 ::ToInteger() const { return Native.ToInteger(); }
FORCEINLINE Vector2  Vector2i::ToFloat()   const { return Native.ToFloat(); }

FORCEINLINE Matrix3  Matrix4::SubMatrix3() const { return Matrix3(XCol.XYZ(), YCol.XYZ(), ZCol.XYZ()); }

// Vector2d:
FORCEINLINE Vector2  Vector2d::ToFloat() const
{
	SIMD_IMP    (return Vector2(_mm_cvtpd_ps(Native));)
	NON_SIMD_IMP(return Vector2((float)X, (float)Y);)
}
FORCEINLINE Vector3d Vector2d::XYZ (double z)            const { return Vector3d(X, Y, z);    }
FORCEINLINE Vector4d Vector2d::XYZW(double z, double w)  const { return Vector4d(X, Y, z, w); }
FORCEINLINE Vector4d Vector2d::XYZW(Vector2d const & zw) const { return Vector4d(X, Y, zw.X, zw.Y); }
FORCEINLINE Vector4d Vector2d::X000() const { return Vector4d(X, 0.0_d, 0.0_d, 0.0_d); }
FORCEINLINE Vector4d Vector2d::XY00() const { return Vector4d(X,     Y, 0.0_d, 0.0_d); }
FORCEINLINE Vector4d Vector2d::XY01() const { return Vector4d(X,     Y, 0.0_d, 1.0_d); }
FORCEINLINE Vector4d Vector2d::XY10() const { return Vector4d(X,     Y, 1.0_d, 0.0_d); }
FORCEINLINE Vector4d Vector2d::XY11() const { return Vector4d(X,     Y, 1.0_d, 1.0_d); }
FORCEINLINE Vector3d Vector2d::X00()  const { return Vector3d(X, 0.0_d, 0.0_d); }
FORCEINLINE Vector3d Vector2d::XY0()  const { return Vector3d(X,     Y, 0.0_d); }
FORCEINLINE Vector3d Vector2d::XY1()  const { return Vector3d(X,     Y, 1.0_d); }

//// Bitset128:
//FORCEINLINE Vector4i Bitset128::V4I() const
//{
//	return Native;
//}

}
