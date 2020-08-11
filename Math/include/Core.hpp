
/// See ../License.txt for license info.

#pragma once

#include "Core/Base.hpp"

#include "Core/BitContinuum.hpp"

#include "Core/Vector4.hpp"
#include "Core/Vector3.hpp"
#include "Core/Vector2.hpp"

#include "Core/Vector4i.hpp"
#include "Core/Vector3i.hpp"
#include "Core/Vector2i.hpp"

#include "Core/Vector4d.hpp"
#include "Core/Vector3d.hpp"
#include "Core/Vector2d.hpp"


#include "Core/Matrix4.hpp"

#include "Core/Popcount.hpp"

#include "Core/RGB888.hpp"
#include "Core/RGBA8888.hpp"

#include "Core/Conversions.hpp"

#ifdef IMGUI_API
	#error YOU MUST INCLUDE HANDY BEFORE IMGUI
#endif

/// If using imgui, this will allow implicit casts to/from HandyMath types,
///
/// ---  BUT  ---
/// 
/// YOU ___MUST___ INCLUDE HANDYMATH ___BEFORE___ IMGUI!!! 
#define IM_VEC2_CLASS_EXTRA \
FORCEINLINE   ImVec2(HANDYMATH_NS::Vector2   const & v2) : x(v2.X), y(v2.Y) { }              \
FORCEINLINE operator HANDYMATH_NS::Vector2() const { return HANDYMATH_NS::Vector2(x, y); }
//FORCEINLINE   ImVec2(HANDYMATH_NS::Vector2d   const & v2) : x(static_cast<float>(v2.X)), y(static_cast<float>(v2.Y)) { }
//FORCEINLINE operator HANDYMATH_NS::Vector2d() const { return HANDYMATH_NS::Vector2d(x, y); }

#define IM_VEC4_CLASS_EXTRA \
FORCEINLINE   ImVec4(HANDYMATH_NS::Vector4   const & v4) : x(v4.X), y(v4.Y), z(v4.Z), w(v4.W) { }  \
FORCEINLINE operator HANDYMATH_NS::Vector4() const { return HANDYMATH_NS::Vector4(x, y, z, w); }
//FORCEINLINE   ImVec4(HANDYMATH_NS::Vector4d   const & v4) : x(static_cast<float>(v4.X)), y(static_cast<float>(v4.Y)), z(static_cast<float>(v4.Z)), w(static_cast<float>(v4.W)) { }
//FORCEINLINE operator HANDYMATH_NS::Vector4d() const { return HANDYMATH_NS::Vector4d(x, y, z, w); }
