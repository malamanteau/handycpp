# HandyMath
Header-only SIMD and scalar vector math library, a refactored version of the vectorial library by scoopr (https://github.com/scoopr/vectorial).

## Features
- Supports SSE, and Scalar implementations (NEON is in progress).
- Most basic vector and matrix maths are available, as well as transformations and AABBs.

## Installation / Usage
- By default, the library will be found in the `Math::` namespace. If you want something different, define `HANDYMATH_NS` to what you want the namespace to be. I usually use `Math::` since it's clean.

## Intrinsics
If you don't want any intrinsics AT ALL, make sure you define `USE_SCALAR`.
You can also force a certain type/level of intrinsics with the following macros: 
```
USE_AVX2 
USE_FMA3 
USE_AVX 
USE_SSE42 
USE_SSE41
USE_SSSE3 
USE_SSE3
USE_SSE2
USE_SSE
or
USE_NEON (only partly implemented, don't use this yet)
```
If none of these are defined, the library will default to SSE2 on x64 platforms, and scalar for all the rest. 

## Interactions with ImGui (https://github.com/ocornut/imgui)
You must include Handy/HandyMath BEFORE imgui. This implements implicit casts to and from the `ImVec2` and `ImVec4` types with `Vector2`, and `Vector4`.

## Core Vector Types

<details><summary>Vector2 Class Reference</summary>
	
```
Public Member Functions
	Vector2 (CVector2 const &v2c)
	operator CVector2 () const
	Vector2 (Vector2 const &v)
	Vector2 (float xy)
	Vector2 (float x, float y)
	Vector2 (float const *ary)
	Vector2 & operator= (Vector2 const &)=default
	Vector3 XYZ (float z) const
	Vector4 XYZW (float z, float w) const
	Vector4 XYZW (Vector2 const &zw) const
	Vector2i ToInteger () const
	void CopyToArray (float *ary) const
	template<int i0, int i1> Vector2 Shuffle () const
	template<int i0, int i1, int i2> Vector3 ShuffleXY0 () const
	template<int i0, int i1, int i2, int i3> Vector4 ShuffleXY00 () const
	template<int i> Vector2 Insert (float f) const
	template<int i> Vector2 InsertZero () const
	Vector2 InsertX (float x) const
	Vector2 InsertY (float y) const
	Vector4 X000 () const
	Vector4 XY00 () const
	Vector4 XY01 () const
	Vector4 XY10 () const
	Vector4 XY11 () const
	Vector3 X00 () const
	Vector3 XY0 () const
	Vector3 XY1 () const
	Vector2 XX () const
	Vector2 YY () const
	Vector2 YX () const
	Vector2 Min (Vector2 const &rhs) const
	Vector2 Max (Vector2 const &rhs) const
	Vector2 Min (float rhs) const
	Vector2 Max (float rhs) const
	float Min () const
	float Max () const
	float Sum () const
	float Product () const
	float Mean () const
	Vector2 Abs () const
	Vector2 Half () const
	Vector2 Reciprocal () const
	Vector2 ReciprocalApprox () const
	Vector2 Sqrt () const
	Vector2 RSqrt () const
	Vector2 RSqrtApprox () const
	Vector2 To (Vector2 const &rhs) const
	float Distance (Vector2 const &rhs) const
	float DistanceSquared (Vector2 const &rhs) const
	float Length () const
	float LengthSquared () const
	Vector2 Normalized () const
	float Angle (Vector2 const &rhs) const
	float Dot (Vector2 const &rhs) const
	Vector2 OrthoCW () const
	Vector2 OrthoCCW () const
	Vector2 Power (float n) const
	Vector2 PowerApprox (float n) const
	bool IsFacing (Vector2 const &rhs) const
	bool HasNaN () const
	bool HasPositiveInfinity () const
	bool HasNegativeInfinity () const
	bool IsNaN () const
	bool IsPositiveInfinity () const
	bool IsNegativeInfinity () const
	bool IsUnbounded () const
	bool ExactlyEqual (Vector2 const &rhs) const
	bool WithinEpsilon (Vector2 const &rhs) const
	Vector2 Clamp (Vector2 const &min, Vector2 const &max) const
	Vector2 Clamp (float const min, float const max) const
	Vector2 MultiplyAdd (Vector2 const &m2, Vector2 const &a) const
	Vector2 MultiplySub (Vector2 const &m2, Vector2 const &a) const
	Vector2 MultiplyAccum (Vector2 const &m1, Vector2 const &m2) const
	Vector2 MultiplyDeduct (Vector2 const &m1, Vector2 const &m2) const
	Vector2 Lerp (Vector2 const &rhs, float t) const
	Vector2 Lerp (Vector2 const &rhs, Vector2 const &t) const
	Vector2 SmoothStep (Vector2 const &rhs, float t) const
	Vector2 SmoothStep (Vector2 const &rhs, Vector2 const &t) const
	Vector2 Round_Near () const
	Vector2 Round_Floor () const
	Vector2 Round_Ceil () const
	Vector2 Round_Trunc () const
	Vector2 FMod (Vector2 const &div) const
	Vector2 FMod (float div) const
	float & operator[] (int idx)
	float operator[] (int idx) const
	template<int i> FORCEINLINE Vector2 Insert (float f) const
	template<int i> FORCEINLINE Vector2 InsertZero () const
Static Public Member Functions
	static Vector2 FromArray (float const *ary)
	static Vector2 Zero ()
	static Vector2 One ()
	static Vector2 NegOne ()
	static Vector2 UnitX ()
	static Vector2 UnitY ()
	static Vector2 NegUnitX ()
	static Vector2 NegUnitY ()
	static Vector2 PositiveInfinity ()
	static Vector2 NegativeInfinity ()
	static Vector2 NaN ()
	static Vector2 UnsafeCast (Vector4 const &v)
Public Attributes
	union {
	  struct {
	    union {
	      struct {
	        float X
	        float Y
	      } 
	      CVector2 Compact
	    } 
	    float _zeroA
	    float _zeroB
	  } 
	  Vector4 Native
	  std::array< float, 4 > Array
	}; 
Static Public Attributes
	static constexpr int32_t NumElements = 2_i32
	static constexpr int32_t ElementBytes = 4_i32
	static constexpr int32_t StrideBytes = 16_i32
	static constexpr int32_t UsefulBytes = NumElements * ElementBytes
```
</details>

<details><summary>Vector2d Class Reference</summary>

```
Public Member Functions
	Vector2d (CVector2d const &v2c)
	operator CVector2d () const
	Vector2d (Vector2d const &v)
	Vector2d (double xy)
	Vector2d (double x, double y)
	Vector2d (double const *ary)
	Vector2d & operator= (Vector2d const &)=default
	Vector3d XYZ (double z) const
	Vector4d XYZW (double z, double w) const
	Vector4d XYZW (Vector2d const &zw) const
	void CopyToArray (double *ary) const
	template<int i0, int i1> Vector2d Shuffle () const
	template<int i0, int i1, int i2> Vector3d ShuffleXY0 () const
	template<int i0, int i1, int i2, int i3> Vector4d ShuffleXY00 () const
	template<int i> Vector2d Insert (double f) const
	template<int i> Vector2d InsertZero () const
	Vector2d InsertX (double x) const
	Vector2d InsertY (double y) const
	Vector4d X000 () const
	Vector4d XY00 () const
	Vector4d XY01 () const
	Vector4d XY10 () const
	Vector4d XY11 () const
	Vector3d X00 () const
	Vector3d XY0 () const
	Vector3d XY1 () const
	Vector2d XX () const
	Vector2d YY () const
	Vector2d YX () const
	Vector2d Min (Vector2d const &rhs) const
	Vector2d Max (Vector2d const &rhs) const
	Vector2d Min (double rhs) const
	Vector2d Max (double rhs) const
	double Min () const
	double Max () const
	double Sum () const
	double Product () const
	double Mean () const
	Vector2d Abs () const
	Vector2d Half () const
	Vector2d Reciprocal () const
	Vector2d ReciprocalApprox () const
	Vector2d Sqrt () const
	Vector2d RSqrt () const
	Vector2d RSqrtApprox () const
	Vector2d To (Vector2d const &rhs) const
	double Distance (Vector2d const &rhs) const
	double DistanceSquared (Vector2d const &rhs) const
	double Length () const
	double LengthSquared () const
	Vector2d Normalized () const
	double Angle (Vector2d const &rhs) const
	double Dot (Vector2d const &rhs) const
	Vector2d OrthoCW () const
	Vector2d OrthoCCW () const
	Vector2d Power (double n) const
	Vector2d PowerApprox (double n) const
	bool IsFacing (Vector2d const &rhs) const
	bool HasNaN () const
	bool HasPositiveInfinity () const
	bool HasNegativeInfinity () const
	bool IsNaN () const
	bool IsPositiveInfinity () const
	bool IsNegativeInfinity () const
	bool IsUnbounded () const
	bool ExactlyEqual (Vector2d const &rhs) const
	bool WithinEpsilon (Vector2d const &rhs) const
	Vector2d Clamp (Vector2d const &min, Vector2d const &max) const
	Vector2d Clamp (double const min, double const max) const
	Vector2d MultiplyAdd (Vector2d const &m2, Vector2d const &a) const
	Vector2d MultiplySub (Vector2d const &m2, Vector2d const &a) const
	Vector2d MultiplyAccum (Vector2d const &m1, Vector2d const &m2) const
	Vector2d MultiplyDeduct (Vector2d const &m1, Vector2d const &m2) const
	Vector2d Lerp (Vector2d const &rhs, double t) const
	Vector2d Lerp (Vector2d const &rhs, Vector2d const &t) const
	Vector2d SmoothStep (Vector2d const &rhs, double t) const
	Vector2d SmoothStep (Vector2d const &rhs, Vector2d const &t) const
	Vector2d Round_Near () const
	Vector2d Round_Floor () const
	Vector2d Round_Ceil () const
	Vector2d Round_Trunc () const
	Vector2d FMod (Vector2d const &div) const
	Vector2d FMod (double div) const
	double & operator[] (int idx)
	double operator[] (int idx) const
	template<int i> FORCEINLINE Vector2d Insert (double f) const
	template<int i> FORCEINLINE Vector2d InsertZero () const
Static Public Member Functions
	static Vector2d FromArray (double const *ary)
	static Vector2d Zero ()
	static Vector2d One ()
	static Vector2d NegOne ()
	static Vector2d UnitX ()
	static Vector2d UnitY ()
	static Vector2d NegUnitX ()
	static Vector2d NegUnitY ()
	static Vector2d PositiveInfinity ()
	static Vector2d NegativeInfinity ()
	static Vector2d NaN ()
	static Vector2d UnsafeCast (Vector4d const &v)
Public Attributes
	union {
	  struct {
	    union {
	      struct {
	        double X
	        double Y
	      } 
	      CVector2d Compact
	    } 
	    double _zeroA
	    double _zeroB
	  } 
	  Vector4d Native
	  std::array< double, 4 > Array
	}; 
Static Public Attributes
	static constexpr int32_t NumElements = 2_i32
	static constexpr int32_t ElementBytes = 8_i32
	static constexpr int32_t StrideBytes = 32_i32
	static constexpr int32_t UsefulBytes = NumElements * ElementBytes
```
</details>

<details><summary>Vector2i Class Reference</summary>

```
Public Member Functions
	Vector2i (CVector2i const &v2ci)
	operator CVector2i () const
	Vector2i (Vector2i const &v)
	Vector2i (int32_t xy)
	Vector2i (int32_t x, int32_t y)
	Vector2i (size_t x, size_t y)
	Vector2i (int32_t const *ary)
	Vector2i & operator= (Vector2i const &)=default
	Vector3i XYZ (int32_t z) const
	Vector4i XYZW (int32_t z, int32_t w) const
	Vector4i XYZW (Vector2i const &zw) const
	Vector2 ToFloat () const
	void CopyToArray (int32_t *ary) const
	template<int i0, int i1> Vector2i Shuffle () const
	template<int i0, int i1, int i2> Vector3i ShuffleXY0 () const
	template<int i0, int i1, int i2, int i3> Vector4i ShuffleXY00 () const
	template<int i> Vector2i Insert (int32_t f) const
	template<int i> Vector2i InsertZero () const
	Vector2i InsertX (int32_t x) const
	Vector2i InsertY (int32_t y) const
	Vector4i X000 () const
	Vector4i XY00 () const
	Vector4i XY01 () const
	Vector4i XY10 () const
	Vector4i XY11 () const
	Vector3i X00 () const
	Vector3i XY0 () const
	Vector3i XY1 () const
	Vector2i XX () const
	Vector2i YY () const
	Vector2i YX () const
	Vector2i Min (Vector2i const &rhs) const
	Vector2i Max (Vector2i const &rhs) const
	Vector2i Min (int32_t rhs) const
	Vector2i Max (int32_t rhs) const
	int32_t Min () const
	int32_t Max () const
	int32_t Sum () const
	int32_t Product () const
	Vector2i Abs () const
	Vector2i To (Vector2i const &rhs) const
	int32_t TaxicabDistance (Vector2i const &rhs) const
	int32_t TaxicabLength () const
	int32_t Dot (Vector2i const &rhs) const
	Vector2i Ortho () const
	Vector2i Power (int32_t n) const
	bool HasMaxValue () const
	bool HasMinValue () const
	bool HasMinMaxValue () const
	Vector2i Clamp (Vector2i const &min, Vector2i const &max) const
	Vector2i Clamp (int32_t const min, int32_t const max) const
	int32_t & operator[] (int idx)
	int32_t operator[] (int idx) const
	template<int i> FORCEINLINE Vector2i Insert (int32_t f) const
	template<int i> FORCEINLINE Vector2i InsertZero () const
Static Public Member Functions
	static Vector2i FromArray (int32_t const *ary)
	static Vector2i Zero ()
	static Vector2i One ()
	static Vector2i NegOne ()
	static Vector2i UnitX ()
	static Vector2i UnitY ()
	static Vector2i NegUnitX ()
	static Vector2i NegUnitY ()
	static Vector2i MaxValue ()
	static Vector2i MinValue ()
	static Vector2i FFFFFFFF ()
	static Vector2i UnsafeCast (Vector4i const &v)
Public Attributes
	union {
	  struct {
	    union {
	      struct {
	        int32_t X
	        int32_t Y
	      } 
	      CVector2i Compact
	    } 
	    int32_t _zeroA
	    int32_t _zeroB
	  } 
	  Vector4i Native
	  std::array< int32_t, 4 > Array
	}; 
Static Public Attributes
	static constexpr int32_t NumElements = 2_i32
	static constexpr int32_t ElementBytes = 4_i32
	static constexpr int32_t StrideBytes = 16_i32
	static constexpr int32_t UsefulBytes = NumElements * ElementBytes
Friends
	class Vector4i
	class Vector3i
	class Vector2
```
</details>

<details><summary>Vector3 Class Reference</summary>

```
Public Member Functions
	Vector3 (CVector3 const &v3c)
	operator CVector3 () const
	Vector3 (Vector3 const &v)
	Vector3 (float xyz)
	Vector3 (float x, float y, float z)
	Vector3 (float const *ary)
	Vector3 & operator= (Vector3 const &)=default
	Vector2 XY () const
	Vector4 XYZW (float w) const
	Vector3i ToInteger () const
	void CopyToArray (float *ary) const
	template<int i0, int i1, int i2> Vector3 Shuffle () const
	template<int i0, int i1, int i2, int i3> Vector4 ShuffleXYZ0 () const
	template<int i> Vector3 Insert (float f) const
	template<int i> Vector3 InsertZero () const
	Vector3 InsertX (float x) const
	Vector3 InsertY (float y) const
	Vector3 InsertZ (float z) const
	Vector4 X000 () const
	Vector4 XY00 () const
	Vector4 XY01 () const
	Vector4 XY10 () const
	Vector4 XY11 () const
	Vector4 XYZ0 () const
	Vector4 XYZ1 () const
	Vector3 X00 () const
	Vector3 XY0 () const
	Vector3 XY1 () const
	Vector3 YY0 () const
	Vector3 ZZ0 () const
	Vector3 Z0Z () const
	Vector3 ZC0 (Vector3 const &abcd) const
	Vector3 Z0C (Vector3 const &abcd) const
	Vector3 XXX () const
	Vector3 YYY () const
	Vector3 ZZZ () const
	Vector3 ZYX () const
	Vector3 Min (Vector3 const &rhs) const
	Vector3 Max (Vector3 const &rhs) const
	Vector3 Min (float rhs) const
	Vector3 Max (float rhs) const
	float Min () const
	float Max () const
	float Sum () const
	float Product () const
	float Mean () const
	Vector3 Abs () const
	Vector3 Half () const
	Vector3 Reciprocal () const
	Vector3 ReciprocalApprox () const
	Vector3 Sqrt () const
	Vector3 RSqrt () const
	Vector3 RSqrtApprox () const
	Vector3 To (Vector3 const &rhs) const
	float Distance (Vector3 const &rhs) const
	float DistanceSquared (Vector3 const &rhs) const
	float Length () const
	float LengthSquared () const
	Vector3 Normalized () const
	float Angle (Vector3 const &rhs) const
	float Dot (Vector3 const &rhs) const
	Vector3 Cross (Vector3 const &rhs) const
	Vector3 Power (float n) const
	Vector3 PowerApprox (float n) const
	bool IsFacing (Vector3 const &rhs) const
	bool HasNaN () const
	bool HasPositiveInfinity () const
	bool HasNegativeInfinity () const
	bool IsNaN () const
	bool IsPositiveInfinity () const
	bool IsNegativeInfinity () const
	bool IsUnbounded () const
	bool ExactlyEqual (Vector3 const &rhs) const
	bool WithinEpsilon (Vector3 const &rhs) const
	Vector3 Clamp (Vector3 const &min, Vector3 const &max) const
	Vector3 Clamp (float const min, float const max) const
	Vector3 MultiplyAdd (Vector3 const &m2, Vector3 const &a) const
	Vector3 MultiplySub (Vector3 const &m2, Vector3 const &a) const
	Vector3 MultiplyAccum (Vector3 const &m1, Vector3 const &m2) const
	Vector3 MultiplyDeduct (Vector3 const &m1, Vector3 const &m2) const
	Vector3 Lerp (Vector3 const &rhs, float t) const
	Vector3 Lerp (Vector3 const &rhs, Vector3 const &t) const
	Vector3 SmoothStep (Vector3 const &rhs, float t) const
	Vector3 SmoothStep (Vector3 const &rhs, Vector3 const &t) const
	Vector3 Round_Near () const
	Vector3 Round_Floor () const
	Vector3 Round_Ceil () const
	Vector3 Round_Trunc () const
	Vector3 FMod (Vector3 const &div) const
	Vector3 FMod (float div) const
	Vector3 RotatedAbout (Vector3 const &axis, float radians) const
	float & operator[] (int idx)
	float operator[] (int idx) const
	template<int i0, int i1, int i2> FORCEINLINE Vector3 Shuffle () const
	template<int i0, int i1, int i2, int i3> FORCEINLINE Vector4 ShuffleXYZ0 () const
	template<int i> FORCEINLINE Vector3 Insert (float f) const
	template<int i> FORCEINLINE Vector3 InsertZero () const
Static Public Member Functions
	static Vector3 FromArray (float const *ary)
	static Vector3 Zero ()
	static Vector3 One ()
	static Vector3 NegOne ()
	static Vector3 UnitX ()
	static Vector3 UnitY ()
	static Vector3 UnitZ ()
	static Vector3 NegUnitX ()
	static Vector3 NegUnitY ()
	static Vector3 NegUnitZ ()
	static Vector3 PositiveInfinity ()
	static Vector3 NegativeInfinity ()
	static Vector3 NaN ()
	static Vector3 UnsafeCast (Vector4 const &v)
Public Attributes
	union {
	  struct {
	    union {
	      struct {
	        float X
	        float Y
	        float Z
	      } 
	      CVector3 Compact
	    } 
	    float _zero
	  } 
	  Vector4 Native
	  std::array< float, 4 > Array
	}; 
Static Public Attributes
	static constexpr int32_t NumElements = 3_i32
	static constexpr int32_t ElementBytes = 4_i32
	static constexpr int32_t StrideBytes = 16_i32
	static constexpr int32_t UsefulBytes = NumElements * ElementBytes
```
</details>

<details><summary>Vector3d Class Reference</summary>

```
Public Member Functions
	Vector3d (CVector3d const &v3c)
	operator CVector3d () const
	Vector3d (Vector3d const &v)
	Vector3d (double xyz)
	Vector3d (double x, double y, double z)
	Vector3d (double const *ary)
	Vector3d & operator= (Vector3d const &)=default
	Vector2d XY () const
	Vector4d XYZW (double w) const
	void CopyToArray (double *ary) const
	template<int i0, int i1, int i2> Vector3d Shuffle () const
	template<int i0, int i1, int i2, int i3> Vector4d ShuffleXYZ0 () const
	template<int i> Vector3d Insert (double f) const
	template<int i> Vector3d InsertZero () const
	Vector3d InsertX (double x) const
	Vector3d InsertY (double y) const
	Vector3d InsertZ (double z) const
	Vector4d X000 () const
	Vector4d XY00 () const
	Vector4d XY01 () const
	Vector4d XY10 () const
	Vector4d XY11 () const
	Vector4d XYZ0 () const
	Vector4d XYZ1 () const
	Vector3d X00 () const
	Vector3d XY0 () const
	Vector3d XY1 () const
	Vector3d YY0 () const
	Vector3d ZZ0 () const
	Vector3d Z0Z () const
	Vector3d ZC0 (Vector3d const &abcd) const
	Vector3d Z0C (Vector3d const &abcd) const
	Vector3d XXX () const
	Vector3d YYY () const
	Vector3d ZZZ () const
	Vector3d ZYX () const
	Vector3d Min (Vector3d const &rhs) const
	Vector3d Max (Vector3d const &rhs) const
	Vector3d Min (double rhs) const
	Vector3d Max (double rhs) const
	double Min () const
	double Max () const
	double Sum () const
	double Product () const
	double Mean () const
	Vector3d Abs () const
	Vector3d Half () const
	Vector3d Reciprocal () const
	Vector3d ReciprocalApprox () const
	Vector3d Sqrt () const
	Vector3d RSqrt () const
	Vector3d RSqrtApprox () const
	Vector3d To (Vector3d const &rhs) const
	double Distance (Vector3d const &rhs) const
	double DistanceSquared (Vector3d const &rhs) const
	double Length () const
	double LengthSquared () const
	Vector3d Normalized () const
	double Angle (Vector3d const &rhs) const
	double Dot (Vector3d const &rhs) const
	Vector3d Cross (Vector3d const &rhs) const
	Vector3d Power (double n) const
	Vector3d PowerApprox (double n) const
	bool IsFacing (Vector3d const &rhs) const
	bool HasNaN () const
	bool HasPositiveInfinity () const
	bool HasNegativeInfinity () const
	bool IsNaN () const
	bool IsPositiveInfinity () const
	bool IsNegativeInfinity () const
	bool IsUnbounded () const
	bool ExactlyEqual (Vector3d const &rhs) const
	bool WithinEpsilon (Vector3d const &rhs) const
	Vector3d Clamp (Vector3d const &min, Vector3d const &max) const
	Vector3d Clamp (double const min, double const max) const
	Vector3d MultiplyAdd (Vector3d const &m2, Vector3d const &a) const
	Vector3d MultiplySub (Vector3d const &m2, Vector3d const &a) const
	Vector3d MultiplyAccum (Vector3d const &m1, Vector3d const &m2) const
	Vector3d MultiplyDeduct (Vector3d const &m1, Vector3d const &m2) const
	Vector3d Lerp (Vector3d const &rhs, double t) const
	Vector3d Lerp (Vector3d const &rhs, Vector3d const &t) const
	Vector3d SmoothStep (Vector3d const &rhs, double t) const
	Vector3d SmoothStep (Vector3d const &rhs, Vector3d const &t) const
	Vector3d Round_Near () const
	Vector3d Round_Floor () const
	Vector3d Round_Ceil () const
	Vector3d Round_Trunc () const
	Vector3d FMod (Vector3d const &div) const
	Vector3d FMod (double div) const
	Vector3d RotatedAbout (Vector3d const &axis, double radians) const
	double & operator[] (int idx)
	double operator[] (int idx) const
	template<int i0, int i1, int i2> FORCEINLINE Vector3d Shuffle () const
	template<int i0, int i1, int i2, int i3> FORCEINLINE Vector4d ShuffleXYZ0 () const
	template<int i> FORCEINLINE Vector3d Insert (double f) const
	template<int i> FORCEINLINE Vector3d InsertZero () const
Static Public Member Functions
	static Vector3d FromArray (double const *ary)
	static Vector3d Zero ()
	static Vector3d One ()
	static Vector3d NegOne ()
	static Vector3d UnitX ()
	static Vector3d UnitY ()
	static Vector3d UnitZ ()
	static Vector3d NegUnitX ()
	static Vector3d NegUnitY ()
	static Vector3d NegUnitZ ()
	static Vector3d PositiveInfinity ()
	static Vector3d NegativeInfinity ()
	static Vector3d NaN ()
	static Vector3d UnsafeCast (Vector4d const &v)
Public Attributes
	union {
	  struct {
	    union {
	      struct {
	        double X
	        double Y
	        double Z
	      } 
	      CVector3d Compact
	    } 
	    double _zero
	  } 
	  Vector4d Native
	  std::array< double, 4 > Array
	}; 
Static Public Attributes
	static constexpr int32_t NumElements = 3_i32
	static constexpr int32_t ElementBytes = 8_i32
	static constexpr int32_t StrideBytes = 32_i32
	static constexpr int32_t UsefulBytes = NumElements * ElementBytes
```
</details>

<details><summary>Vector3i Class Reference</summary>

```
Public Member Functions
	Vector3i (CVector3i const &v3ci)
	operator CVector3i () const
	Vector3i (Vector3i const &v)
	Vector3i (int32_t xyz)
	Vector3i (int32_t x, int32_t y, int32_t z)
	Vector3i (size_t x, size_t y, size_t z)
	Vector3i (int32_t const *ary)
	Vector3i & operator= (Vector3i const &)=default
	Vector2i XY () const
	Vector4i XYZW (int32_t w) const
	Vector3 ToFloat () const
	void CopyToArray (int32_t *ary) const
	template<int i0, int i1, int i2> Vector3i Shuffle () const
	template<int i0, int i1, int i2, int i3> Vector4i ShuffleXYZ0 () const
	template<int i> Vector3i Insert (int32_t f) const
	template<int i> Vector3i InsertZero () const
	Vector3i InsertX (int32_t x) const
	Vector3i InsertY (int32_t y) const
	Vector3i InsertZ (int32_t z) const
	Vector4i X000 () const
	Vector4i XY00 () const
	Vector4i XY01 () const
	Vector4i XY10 () const
	Vector4i XY11 () const
	Vector4i XYZ0 () const
	Vector4i XYZ1 () const
	Vector3i X00 () const
	Vector3i XY0 () const
	Vector3i XY1 () const
	Vector3i ZZ0 () const
	Vector3i Z0Z () const
	Vector3i ZC0 (Vector3i const &abcd) const
	Vector3i Z0C (Vector3i const &abcd) const
	Vector3i XXX () const
	Vector3i YYY () const
	Vector3i ZZZ () const
	Vector3i ZYX () const
	Vector3i Min (Vector3i const &rhs) const
	Vector3i Max (Vector3i const &rhs) const
	Vector3i Min (int32_t rhs) const
	Vector3i Max (int32_t rhs) const
	int32_t Min () const
	int32_t Max () const
	int32_t Sum () const
	int32_t Product () const
	Vector3i Abs () const
	Vector3i To (Vector3i const &rhs) const
	int32_t TaxicabDistance (Vector3i const &rhs) const
	int32_t TaxicabLength () const
	int32_t Dot (Vector3i const &rhs) const
	Vector3i Power (int32_t n) const
	bool HasMaxValue () const
	bool HasMinValue () const
	bool HasMinMaxValue () const
	Vector3i Clamp (Vector3i const &min, Vector3i const &max) const
	Vector3i Clamp (int32_t const min, int32_t const max) const
	int32_t & operator[] (int idx)
	int32_t operator[] (int idx) const
	template<int i0, int i1, int i2> FORCEINLINE Vector3i Shuffle () const
	template<int i0, int i1, int i2, int i3> FORCEINLINE Vector4i ShuffleXYZ0 () const
	template<int i> FORCEINLINE Vector3i Insert (int32_t f) const
	template<int i> FORCEINLINE Vector3i InsertZero () const
Static Public Member Functions
	static Vector3i FromArray (int32_t const *ary)
	static Vector3i Zero ()
	static Vector3i One ()
	static Vector3i NegOne ()
	static Vector3i UnitX ()
	static Vector3i UnitY ()
	static Vector3i UnitZ ()
	static Vector3i NegUnitX ()
	static Vector3i NegUnitY ()
	static Vector3i NegUnitZ ()
	static Vector3i MaxValue ()
	static Vector3i MinValue ()
	static Vector3i FFFFFFFF ()
	static Vector3i UnsafeCast (Vector4i const &v)
Public Attributes
	union {
	  struct {
	    union {
	      struct {
	        int32_t X
	        int32_t Y
	        int32_t Z
	      } 
	      CVector3i Compact
	    } 
	    int32_t _zero
	  } 
	  Vector4i Native
	  std::array< int32_t, 4 > Array
	}; 
Static Public Attributes
	static constexpr int32_t NumElements = 3_i32
	static constexpr int32_t ElementBytes = 4_i32
	static constexpr int32_t StrideBytes = 16_i32
	static constexpr int32_t UsefulBytes = NumElements * ElementBytes
```
</details>

<details><summary>Vector4 Class Reference</summary>

```
Public Member Functions
	Vector4 (ARCHFT4 const &native)
	operator ARCHFT4 () const
	Vector4 (Vector4 const &v)
	Vector4 (float xyzw)
	Vector4 (float x, float y, float z, float w)
	Vector4 (float const *ary)
	Vector4 & operator= (Vector4 const &)=default
	Vector2 XY () const
	Vector2 ZW () const
	Vector3 XYZ () const
	Vector4i ToInteger () const
	void CopyToArray (float *ary) const
	Vector4i SI128 () const
	template<int i0, int i1, int i2, int i3> Vector4 Shuffle () const
	template<int i0, int i1, int i2, int i3> Vector4 ShuffleAABB (Vector4 const &b) const
	template<int i> Vector4 Insert (float f) const
	template<int i> Vector4 InsertZero () const
	Vector4 InsertX (float x) const
	Vector4 InsertY (float y) const
	Vector4 InsertZ (float z) const
	Vector4 InsertW (float w) const
	Vector4 X000 () const
	Vector4 XY00 () const
	Vector4 XY01 () const
	Vector4 XY10 () const
	Vector4 XY11 () const
	Vector4 XYZ0 () const
	Vector4 XYZ1 () const
	Vector4 XXZZ () const
	Vector4 YYWW () const
	Vector4 XXYY () const
	Vector4 XYXY () const
	Vector4 ZZWW () const
	Vector4 ZWZW () const
	Vector4 ZCWD (Vector4 const &abcd) const
	Vector4 XYAB (Vector4 const &abcd) const
	Vector4 ZWCD (Vector4 const &abcd) const
	Vector4 XAYB (Vector4 const &abcd) const
	Vector4 XXXX () const
	Vector4 YYYY () const
	Vector4 ZZZZ () const
	Vector4 WWWW () const
	Vector4 WZYX () const
	Vector4 ZWXY () const
	Vector4 Min (Vector4 const &rhs) const
	Vector4 Max (Vector4 const &rhs) const
	Vector4 Min (float rhs) const
	Vector4 Max (float rhs) const
	float Min () const
	float Max () const
	float Sum () const
	float Product () const
	float Mean () const
	Vector4 Abs () const
	Vector4 Half () const
	Vector4 Reciprocal () const
	Vector4 ReciprocalApprox () const
	Vector4 Sqrt () const
	Vector4 RSqrt () const
	Vector4 RSqrtApprox () const
	Vector4 To (Vector4 const &rhs) const
	float Distance (Vector4 const &rhs) const
	float DistanceSquared (Vector4 const &rhs) const
	float Length () const
	float LengthSquared () const
	Vector4 Normalized () const
	float Angle (Vector4 const &rhs) const
	float Dot (Vector4 const &rhs) const
	Vector4 Power (float n) const
	Vector4 PowerApprox (float n) const
	bool IsFacing (Vector4 const &rhs) const
	bool HasNaN () const
	bool HasPositiveInfinity () const
	bool HasNegativeInfinity () const
	bool IsNaN () const
	bool IsPositiveInfinity () const
	bool IsNegativeInfinity () const
	bool IsUnbounded () const
	bool ExactlyEqual (Vector4 const &rhs) const
	bool WithinEpsilon (Vector4 const &rhs) const
	Vector4 Clamp (Vector4 const &min, Vector4 const &max) const
	Vector4 Clamp (float const min, float const max) const
	Vector4 MultiplyAdd (Vector4 const &m2, Vector4 const &a) const
	Vector4 MultiplySub (Vector4 const &m2, Vector4 const &a) const
	Vector4 MultiplyAccum (Vector4 const &m1, Vector4 const &m2) const
	Vector4 MultiplyDeduct (Vector4 const &m1, Vector4 const &m2) const
	Vector4 Lerp (Vector4 const &rhs, float t) const
	Vector4 Lerp (Vector4 const &rhs, Vector4 const &t) const
	Vector4 SmoothStep (Vector4 const &rhs, float t) const
	Vector4 SmoothStep (Vector4 const &rhs, Vector4 const &t) const
	Vector4 Round_Near () const
	Vector4 Round_Floor () const
	Vector4 Round_Ceil () const
	Vector4 Round_Trunc () const
	Vector4 FMod (Vector4 const &div) const
	Vector4 FMod (float div) const
	float & operator[] (int idx)
	float operator[] (int idx) const
	template<int i0, int i1, int i2, int i3> FORCEINLINE Vector4 Shuffle () const
	template<int i0, int i1, int i2, int i3> FORCEINLINE Vector4 ShuffleAABB (Vector4 const &b) const
	template<int i> FORCEINLINE Vector4 Insert (float f) const
	template<int i> FORCEINLINE Vector4 InsertZero () const
Static Public Member Functions
	static Vector4 FromArray (float const *ary)
	static Vector4 Zero ()
	static Vector4 One ()
	static Vector4 NegOne ()
	static Vector4 UnitX ()
	static Vector4 UnitY ()
	static Vector4 UnitZ ()
	static Vector4 UnitW ()
	static Vector4 UnitXW ()
	static Vector4 UnitYW ()
	static Vector4 UnitZW ()
	static Vector4 NegUnitX ()
	static Vector4 NegUnitY ()
	static Vector4 NegUnitZ ()
	static Vector4 NegUnitW ()
	static Vector4 PositiveInfinity ()
	static Vector4 NegativeInfinity ()
	static Vector4 NaN ()
	static Vector4 FFFFFFFF ()
Public Attributes
	union {
	  struct {
	    float X
	    float Y
	    float Z
	    float W
	  } 
	  ARCHFT4 Native
	  std::array< float, 4 > Array
	}; 
Static Public Attributes
	static constexpr int32_t NumElements = 4_i32
	static constexpr int32_t ElementBytes = 4_i32
	static constexpr int32_t StrideBytes = 16_i32
	static constexpr int32_t UsefulBytes = NumElements * ElementBytes
```
</details>

<details><summary>Vector4d Class Reference</summary>

```
Public Member Functions
	Vector4d (ARCHDT4 const &native)
	operator ARCHDT4 () const
	Vector4d (Vector4d const &v)
	Vector4d (double xyzw)
	Vector4d (double x, double y, double z, double w)
	Vector4d (double const *ary)
	Vector4d & operator= (Vector4d const &)=default
	Vector2d XY () const
	Vector2d ZW () const
	Vector3d XYZ () const
	void CopyToArray (double *ary) const
	template<int i0, int i1, int i2, int i3> Vector4d Shuffle () const
	template<int i0, int i1, int i2, int i3> Vector4d ShuffleAABB (Vector4d const &b) const
	template<int i> Vector4d Insert (double f) const
	template<int i> Vector4d InsertZero () const
	Vector4d InsertX (double x) const
	Vector4d InsertY (double y) const
	Vector4d InsertZ (double z) const
	Vector4d InsertW (double w) const
	Vector4d X000 () const
	Vector4d XY00 () const
	Vector4d XY01 () const
	Vector4d XY10 () const
	Vector4d XY11 () const
	Vector4d XYZ0 () const
	Vector4d XYZ1 () const
	Vector4d XXZZ () const
	Vector4d YYWW () const
	Vector4d XXYY () const
	Vector4d XYXY () const
	Vector4d ZZWW () const
	Vector4d ZWZW () const
	Vector4d ZCWD (Vector4d const &abcd) const
	Vector4d XYAB (Vector4d const &abcd) const
	Vector4d ZWCD (Vector4d const &abcd) const
	Vector4d XAYB (Vector4d const &abcd) const
	Vector4d XXXX () const
	Vector4d YYYY () const
	Vector4d ZZZZ () const
	Vector4d WWWW () const
	Vector4d WZYX () const
	Vector4d ZWXY () const
	Vector4d Min (Vector4d const &rhs) const
	Vector4d Max (Vector4d const &rhs) const
	Vector4d Min (double rhs) const
	Vector4d Max (double rhs) const
	double Min () const
	double Max () const
	double Sum () const
	double Product () const
	double Mean () const
	Vector4d Abs () const
	Vector4d Half () const
	Vector4d Reciprocal () const
	Vector4d ReciprocalApprox () const
	Vector4d Sqrt () const
	Vector4d RSqrt () const
	Vector4d RSqrtApprox () const
	Vector4d To (Vector4d const &rhs) const
	double Distance (Vector4d const &rhs) const
	double DistanceSquared (Vector4d const &rhs) const
	double Length () const
	double LengthSquared () const
	Vector4d Normalized () const
	double Angle (Vector4d const &rhs) const
	double Dot (Vector4d const &rhs) const
	Vector4d Power (double n) const
	Vector4d PowerApprox (double n) const
	bool IsFacing (Vector4d const &rhs) const
	bool HasNaN () const
	bool HasPositiveInfinity () const
	bool HasNegativeInfinity () const
	bool IsNaN () const
	bool IsPositiveInfinity () const
	bool IsNegativeInfinity () const
	bool IsUnbounded () const
	bool ExactlyEqual (Vector4d const &rhs) const
	bool WithinEpsilon (Vector4d const &rhs) const
	Vector4d Clamp (Vector4d const &min, Vector4d const &max) const
	Vector4d Clamp (double const min, double const max) const
	Vector4d MultiplyAdd (Vector4d const &m2, Vector4d const &a) const
	Vector4d MultiplySub (Vector4d const &m2, Vector4d const &a) const
	Vector4d MultiplyAccum (Vector4d const &m1, Vector4d const &m2) const
	Vector4d MultiplyDeduct (Vector4d const &m1, Vector4d const &m2) const
	Vector4d Lerp (Vector4d const &rhs, double t) const
	Vector4d Lerp (Vector4d const &rhs, Vector4d const &t) const
	Vector4d SmoothStep (Vector4d const &rhs, double t) const
	Vector4d SmoothStep (Vector4d const &rhs, Vector4d const &t) const
	Vector4d Round_Near () const
	Vector4d Round_Floor () const
	Vector4d Round_Ceil () const
	Vector4d Round_Trunc () const
	Vector4d FMod (Vector4d const &div) const
	Vector4d FMod (double div) const
	double & operator[] (int idx)
	double operator[] (int idx) const
	template<int i0, int i1, int i2, int i3> FORCEINLINE Vector4d Shuffle () const
	template<int i0, int i1, int i2, int i3> FORCEINLINE Vector4d ShuffleAABB (Vector4d const &b) const
	template<int i> FORCEINLINE Vector4d Insert (double f) const
	template<int i> FORCEINLINE Vector4d InsertZero () const
Static Public Member Functions
	static Vector4d FromArray (double const *ary)
	static Vector4d Zero ()
	static Vector4d One ()
	static Vector4d NegOne ()
	static Vector4d UnitX ()
	static Vector4d UnitY ()
	static Vector4d UnitZ ()
	static Vector4d UnitW ()
	static Vector4d UnitXW ()
	static Vector4d UnitYW ()
	static Vector4d UnitZW ()
	static Vector4d NegUnitX ()
	static Vector4d NegUnitY ()
	static Vector4d NegUnitZ ()
	static Vector4d NegUnitW ()
	static Vector4d PositiveInfinity ()
	static Vector4d NegativeInfinity ()
	static Vector4d NaN ()
	static Vector4d FFFFFFFF ()
Public Attributes
	union {
	  struct {
	    double X
	    double Y
	    double Z
	    double W
	  } 
	  ARCHDT4 Native
	  std::array< double, 4 > Array
	}; 
Static Public Attributes
	static constexpr int32_t NumElements = 4_i32
	static constexpr int32_t ElementBytes = 8_i32
	static constexpr int32_t StrideBytes = 32_i32
	static constexpr int32_t UsefulBytes = NumElements * ElementBytes
```
</details>

<details><summary>Vector4i Class Reference</summary>

```
Public Member Functions
	Vector4i (ARCHIT4 const &native)
	operator ARCHIT4 () const
	Vector4i (Vector4i const &v)
	Vector4i (int32_t xyzw)
	Vector4i (int32_t x, int32_t y, int32_t z, int32_t w)
	Vector4i (size_t x, size_t y, size_t z, size_t w)
	Vector4i (int32_t const *ary)
	Vector4i & operator= (Vector4i const &)=default
	Vector2i XY () const
	Vector2i ZW () const
	Vector3i XYZ () const
	Vector4 ToFloat () const
	void CopyToArray (int32_t *ary) const
	Vector4 PS128 () const
	template<int i0, int i1, int i2, int i3> Vector4i Shuffle () const
	template<int i> Vector4i Insert (int32_t f) const
	template<int i> Vector4i InsertZero () const
	Vector4i InsertX (int32_t x) const
	Vector4i InsertY (int32_t y) const
	Vector4i InsertZ (int32_t z) const
	Vector4i InsertW (int32_t w) const
	Vector4i X000 () const
	Vector4i XY00 () const
	Vector4i XY01 () const
	Vector4i XY10 () const
	Vector4i XY11 () const
	Vector4i XYZ0 () const
	Vector4i XYZ1 () const
	Vector4i XXYY () const
	Vector4i ZZWW () const
	Vector4i XYXY () const
	Vector4i ZWZW () const
	Vector4i ZCWD (Vector4i const &abcd) const
	Vector4i XYAB (Vector4i const &abcd) const
	Vector4i ZWCD (Vector4i const &abcd) const
	Vector4i XAYB (Vector4i const &abcd) const
	Vector4i XXXX () const
	Vector4i YYYY () const
	Vector4i ZZZZ () const
	Vector4i WWWW () const
	Vector4i WZYX () const
	Vector4i ZWXY () const
	Vector4i Min (Vector4i const &rhs) const
	Vector4i Max (Vector4i const &rhs) const
	Vector4i Min (int32_t rhs) const
	Vector4i Max (int32_t rhs) const
	int32_t Min () const
	int32_t Max () const
	int32_t Sum () const
	int32_t Product () const
	Vector4i Abs () const
	Vector4i To (Vector4i const &rhs) const
	int32_t TaxicabDistance (Vector4i const &rhs) const
	int32_t TaxicabLength () const
	int32_t Dot (Vector4i const &rhs) const
	Vector4i Power (int32_t n) const
	Vector4i BitwiseAndNot (Vector4i const &rhs) const
	bool HasMaxValue () const
	bool HasMinValue () const
	bool HasMinMaxValue () const
	Vector4i Clamp (Vector4i const &min, Vector4i const &max) const
	Vector4i Clamp (int32_t min, int32_t max) const
	int32_t & operator[] (int32_t idx)
	int32_t operator[] (int32_t idx) const
	template<int i0, int i1, int i2, int i3> FORCEINLINE Vector4i Shuffle () const
	template<int i> FORCEINLINE Vector4i Insert (int32_t f) const
	template<int i> FORCEINLINE Vector4i InsertZero () const
Static Public Member Functions
	static Vector4i FromArray (int32_t const *ary)
	static Vector4i Zero ()
	static Vector4i One ()
	static Vector4i NegOne ()
	static Vector4i UnitX ()
	static Vector4i UnitY ()
	static Vector4i UnitZ ()
	static Vector4i UnitW ()
	static Vector4i UnitXW ()
	static Vector4i UnitYW ()
	static Vector4i UnitZW ()
	static Vector4i NegUnitX ()
	static Vector4i NegUnitY ()
	static Vector4i NegUnitZ ()
	static Vector4i NegUnitW ()
	static Vector4i MaxValue ()
	static Vector4i MinValue ()
	static Vector4i FFFFFFFF ()
Public Attributes
	union {
	  struct {
	    int32_t X
	    int32_t Y
	    int32_t Z
	    int32_t W
	  } 
	  ARCHIT4 Native
	  std::array< int32_t, 4 > Array
	}; 
Static Public Attributes
	static constexpr int32_t NumElements = 4_i32
	static constexpr int32_t ElementBytes = 4_i32
	static constexpr int32_t StrideBytes = 16_i32
	static constexpr int32_t UsefulBytes = NumElements * ElementBytes

```
</details>

## Core Matrix Types

<details><summary>Matrix3 Class Reference</summary>

```
Public Member Functions
	CMatrix3 Compact () const
	Matrix3 (Matrix3 const &m)
	Matrix3 (Vector3 const &xCol, Vector3 const &yCol, Vector3 const &zCol)
	Matrix3 (float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
	Matrix3 (float f9)
	Matrix3 (Vector3 const &xyzCol)
	Matrix3 (float const *ary)
	void CopyToArray (float *ary) const
	Matrix3 Transpose () const
	float Determinant () const
	Matrix3 Inverse () const
	Matrix3 Normal () const
	Vector3 Row0 () const
	Vector3 Row1 () const
	Vector3 Row2 () const
	Vector3 Diagonal () const
	bool IsIdentity () const
	bool IsSymmetric () const
	bool ExactlyEqual (Matrix3 const &rhs) const
	bool WithinEpsilon (Matrix3 const &rhs) const
	float & operator[] (int idx)
	float operator[] (int idx) const
Static Public Member Functions
	static Matrix3 FromArray (float const *ary)
	static Matrix3 Zero ()
	static Matrix3 One ()
	static Matrix3 Identity ()
Public Attributes
	union {
	  struct {
	    Vector3 XCol
	    Vector3 YCol
	    Vector3 ZCol
	  } 
	  struct {
	    float M00
	    float M01
	    float M02
	    float _unusedA
	    float M10
	    float M11
	    float M12
	    float _unusedB
	    float M20
	    float M21
	    float M22
	    float _unusedC
	  } 
	  float Array [12]
	}; 
Static Public Attributes
	static constexpr int32_t NumElements = 9_i32
	static constexpr int32_t NumElementSpaces = 12_i32
	static constexpr int32_t ColumnStrideBytes = 16_i32
	static constexpr int32_t StrideBytes = 48_i32
```
</details>

<details><summary>Matrix4 Class Reference</summary>

```
Public Member Functions
	Matrix4 (Matrix4 const &m)
	Matrix4 (Vector4 const &xCol, Vector4 const &yCol, Vector4 const &zCol, Vector4 const &wCol)
	Matrix4 (float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
	Matrix4 (float f16)
	Matrix4 (Vector4 const &xyzwCol)
	Matrix4 (float const *ary)
	void CopyToArray (float *ary) const
	Matrix4 Transpose () const
	float Determinant () const
	Matrix4 Inverse () const
	Matrix4 InverseAffine () const
	Matrix4 Normal () const
	Matrix4 View () const
	Matrix4 Inverse_Determinant (float &outDet) const
	Matrix4 InverseTranspose_Determinant (float &outDet) const
	Vector4 Row0 () const
	Vector4 Row1 () const
	Vector4 Row2 () const
	Vector4 Row3 () const
	Vector4 Diagonal () const
	Matrix3 SubMatrix3 () const
	bool IsIdentity () const
	bool IsSymmetric () const
	bool ExactlyEqual (Matrix4 const &rhs) const
	bool WithinEpsilon (Matrix4 const &rhs) const
	float & operator[] (int idx)
	float operator[] (int idx) const
Static Public Member Functions
	static Matrix4 FromArray (float const *ary)
	static Matrix4 Zero ()
	static Matrix4 One ()
	static Matrix4 Identity ()
	static Matrix4 Translate (Vector2 const &t)
	static Matrix4 Translate (Vector3 const &t)
	static Matrix4 Translate (Vector4 const &t)
	static Matrix4 Translate (float x, float y, float z)
	static Matrix4 Translate (float x, float y)
	static Matrix4 RotateAboutZAxisXYPoint (float thetaRadians, Vector2 const &point)
	static Matrix4 RotateAboutAxisPoint (float thetaradians, Vector3 const &axis, Vector3 const &point)
	static Matrix4 Scale (float v)
	static Matrix4 Scale (float sX, float sY, float sZ)
	static Matrix4 Scale (Vector3 const &v)
	static Matrix4 RotateXAxis (float thetaRadians)
	static Matrix4 RotateYAxis (float thetaRadians)
	static Matrix4 RotateZAxis (float thetaRadians)
	static Matrix4 RotateAboutAxis (Vector3 const &axis, float theta)
	static Matrix4 Skew2D (float x_sub_y, float y_sub_x)
Public Attributes
	union {
	  struct {
	    Vector4 XCol
	    Vector4 YCol
	    Vector4 ZCol
	    Vector4 WCol
	  } 
	  struct {
	    float M00
	    float M01
	    float M02
	    float M03
	    float M10
	    float M11
	    float M12
	    float M13
	    float M20
	    float M21
	    float M22
	    float M23
	    float M30
	    float M31
	    float M32
	    float M33
	  } 
	  float Array [16]
	}; 
Static Public Attributes
	static constexpr int32_t NumElements = 16_i32
	static constexpr int32_t ElementBytes = 4_i32
	static constexpr int32_t StrideBytes = 64_i32
	static constexpr int32_t UsefulBytes = NumElements * ElementBytes
```
</details>

## Compact Vector/Matrix Types

<details><summary>CVector2 Class Reference</summary>

```
Compact version of Vector2, for storage, rather than computation. 
Public Member Functions
	CVector2 (float x, float y)
	CVector2 & operator= (CVector2 const &)=default
Public Attributes
	float X = 0.0f
	float Y = 0.0f
```
</details>

<details><summary>CVector2d Class Reference</summary>
	
```
Compact version of Vector2d, for storage, rather than computation. 
Public Member Functions
	CVector2d (double x, double y)
	CVector2d & operator= (CVector2d const &)=default
Public Attributes
	double X = 0.0_d
	double Y = 0.0_d
```
</details>

<details><summary>CVector2i Class Reference</summary>

```
Compact version of Vector2i, for storage, rather than computation. 
Public Member Functions
	CVector2i (int32_t x, int32_t y)
	CVector2i & operator= (CVector2i const &)=default
Public Attributes
	int32_t X = 0
	int32_t Y = 0
```
</details>

<details><summary>CVector3 Class Reference</summary>

```
Compact version of Vector3, for storage, rather than computation. 
Public Member Functions
	CVector3 (float x, float y, float z)
	CVector3 & operator= (CVector3 const &)=default
Public Attributes
	float X = 0.0f
	float Y = 0.0f
	float Z = 0.0f
```
</details>

<details><summary>CVector3d Class Reference</summary>

```
Compact version of Vector3, for storage, rather than computation. 
Public Member Functions
	CVector3d (double x, double y, double z)
	CVector3d & operator= (CVector3d const &)=default
Public Attributes
	double X = 0.0_d
	double Y = 0.0_d
	double Z = 0.0_d
```
</details>

<details><summary>CVector3i Class Reference</summary>

```
Compact version of Vector3i, for storage, rather than computation. 
Public Member Functions
	CVector3i (int32_t x, int32_t y, int32_t z)
	CVector3i & operator= (CVector3i const &)=default
Public Attributes
	int32_t X = 0
	int32_t Y = 0
	int32_t Z = 0
```
</details>

<details><summary>CMatrix3 Class Reference</summary>

```
Compact version of Matrix3, for storage, rather than computation. 
Public Member Functions
	CMatrix3 (CVector3 const &xCol, CVector3 const &yCol, CVector3 const &zCol)
	CMatrix3 & operator= (CMatrix3 const &)=default
Public Attributes
	CVector3 XCol
	CVector3 YCol
	CVector3 ZCol
```
</details>

### Unnecessary Compact Types
`CVector4/i/d` and `CMatrix4` are not needed and not implemented, because `Vector4/i/d` and `Matrix4` have no padding bytes. They are already compact.

## RGBA Classes

<details><summary>RGB888 Class Reference</summary>

```
Public Member Functions
	RGB888 (uint8_t v)
	RGB888 (uint8_t r, uint8_t g, uint8_t b)
	RGB888 & operator= (RGB888 const &)=default
	RGB888 Max (RGB888 const &v) const
	RGB888 Min (RGB888 const &v) const
	bool IsZero () const
	RGB888 Clamped (RGB888 const &min, RGB888 const &max) const
	RGBA8888 RGBA (uint8_t a=255) const
	uint8_t & operator[] (int idx)
	uint8_t operator[] (int idx) const
Static Public Member Functions
	static RGB888 Black ()
	static RGB888 White ()
	static RGB888 Red ()
	static RGB888 Green ()
	static RGB888 Blue ()
	static RGB888 Cyan ()
	static RGB888 Magenta ()
	static RGB888 Yellow ()
Public Attributes
	union {
	  struct {
	    uint8_t R
	    uint8_t G
	    uint8_t B
	    uint8_t _zero
	  } 
	  uint8_t Array [4]
	  uint32_t UInt
	  int32_t Int
	}; 
```
</details>

<details><summary>RGBA8888 Class Reference</summary>

```
Public Member Functions
	RGBA8888 (uint8_t v)
	RGBA8888 (uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	RGBA8888 & operator= (RGBA8888 const &)=default
	RGBA8888 Max (RGBA8888 const &v) const
	RGBA8888 Min (RGBA8888 const &v) const
	bool IsZero () const
	RGBA8888 Clamped (RGBA8888 const &min, RGBA8888 const &max) const
	RGB888 RGB () const
	uint8_t & operator[] (int idx)
	uint8_t operator[] (int idx) const
Static Public Member Functions
	static RGBA8888 ClearBlack ()
	static RGBA8888 ClearWhite ()
	static RGBA8888 ClearRed ()
	static RGBA8888 ClearGreen ()
	static RGBA8888 ClearBlue ()
	static RGBA8888 ClearCyan ()
	static RGBA8888 ClearMagenta ()
	static RGBA8888 ClearYellow ()
	static RGBA8888 Black ()
	static RGBA8888 White ()
	static RGBA8888 Red ()
	static RGBA8888 Green ()
	static RGBA8888 Blue ()
	static RGBA8888 Cyan ()
	static RGBA8888 Magenta ()
	static RGBA8888 Yellow ()
Public Attributes
	union {
	  struct {
	    uint8_t R
	    uint8_t G
	    uint8_t B
	    uint8_t A
	  } 
	  uint8_t Array [4]
	  uint32_t UInt
	  int32_t Int
	}; 
```
</details>

## Projections

<details><summary>IProjection Class Reference</summary>

```
Inherited by OrthoProjection, and PerspectiveProjection.
Public Member Functions
	virtual Matrix4 Matrix (Vector2 const &viewportDimensions, float zNear, float zFar) const =0
```
</details>

<details><summary>OrthoProjection Class Reference</summary>

```
Inherits IProjection.
Public Member Functions
	Vector2 Dimensions () const
	OrthoProjectionType Mode () const
	OrthoProjection (Vector2 const &dimensions, OrthoProjectionType mode)
	void Set (Vector2 const &dimensions, OrthoProjectionType mode)
	Vector2 EffectiveDimensions (Vector2 const &viewportDimensions) const
	Matrix4 Matrix (Vector2 const &viewportDimensions, float zNear, float zFar) const override
```
</details>

<details><summary>PerspectiveProjection Class Reference</summary>

```
Inherits IProjection.
Public Member Functions
	Vector2 FOVXY () const
	PerspectiveProjectionType Mode () const
	PerspectiveProjection (float const fovY)
	PerspectiveProjection (Vector2 const &fovXY, PerspectiveProjectionType mode)
	void Set (float const fovY)
	void Set (Vector2 const &fovXY, PerspectiveProjectionType mode)
	Vector2 EffectiveAngles (Vector2 const &viewportDimensions, float znear, float zfar) const
	virtual Matrix4 Matrix (Vector2 const &viewportDimensions, float zNear, float zFar) const override
```
</details>

## Transformation Classes

<details><summary>Xform3 Class Reference</summary>

```
Public Member Functions
	Xform3 (Matrix4 const &v)
	operator Matrix4 const & ()
	operator Matrix4 () const
	bool IsEmpty () const
	Matrix4 ViewMatrix () const
	Xform3 Inverted () const
	Vector3 Apply (Vector2 const &xy) const
	Vector3 Apply (Vector3 const &xyz) const
	Matrix4 Apply (Matrix4 const &m) const
Static Public Member Functions
	static Xform3 RotateX (float degCCW)
	static Xform3 RotateY (float degCCW)
	static Xform3 RotateZ (float degCCW)
	static Xform3 RotateAxis (Vector3 const &axis, float degCCW)
	static Xform3 RotateXAbout (Vector2 const &pointYZ, float degCCW)
	static Xform3 RotateXAbout (Vector3 const &point, float degCCW)
	static Xform3 RotateYAbout (Vector2 const &pointXZ, float degCCW)
	static Xform3 RotateYAbout (Vector3 const &point, float degCCW)
	static Xform3 RotateZAbout (Vector2 const &pointXY, float degCCW)
	static Xform3 RotateZAbout (Vector3 const &point, float degCCW)
	static Xform3 RotateAxisAbout (Vector3 const &axis, Vector3 const &point, float degCCW)
	static Xform3 MoveX (float deltaX)
	static Xform3 MoveY (float deltaY)
	static Xform3 MoveZ (float deltaZ)
	static Xform3 MoveXY (float deltaX, float deltaY)
	static Xform3 MoveXY (Vector2 const &deltaXY)
	static Xform3 MoveXYZ (float deltaX, float deltaY, float deltaZ)
	static Xform3 MoveXYZ (Vector3 const &deltaXYZ)
	static Xform3 ScaleX (float cfX)
	static Xform3 ScaleY (float cfY)
	static Xform3 ScaleZ (float cfZ)
	static Xform3 ScaleXY (float cfXY)
	static Xform3 ScaleXY (float cfX, float cfY)
	static Xform3 ScaleXY (Vector2 const &cfXY)
	static Xform3 ScaleXYZ (float cfXYZ)
	static Xform3 ScaleXYZ (float cfX, float cfY, float cfZ)
	static Xform3 ScaleXYZ (Vector3 const &cfXYZ)
	static Xform3 ScaleXAbout (float pointX, float cfX)
	static Xform3 ScaleYAbout (float pointY, float cfY)
	static Xform3 ScaleZAbout (float pointZ, float cfZ)
	static Xform3 ScaleXAbout (Vector2 const &point, float cfX)
	static Xform3 ScaleYAbout (Vector2 const &point, float cfY)
	static Xform3 ScaleZAbout (Vector2 const &point, float cfZ)
	static Xform3 ScaleXAbout (Vector3 const &point, float cfX)
	static Xform3 ScaleYAbout (Vector3 const &point, float cfY)
	static Xform3 ScaleZAbout (Vector3 const &point, float cfZ)
	static Xform3 ScaleXYAbout (Vector2 const &point, float cfXY)
	static Xform3 ScaleXYAbout (Vector3 const &point, float cfXY)
	static Xform3 ScaleXYAbout (Vector2 const &point, float cfX, float cfY)
	static Xform3 ScaleXYAbout (Vector3 const &point, float cfX, float cfY)
	static Xform3 ScaleXYAbout (Vector2 const &point, Vector2 const &cfXY)
	static Xform3 ScaleXYAbout (Vector3 const &point, Vector2 const &cfXY)
	static Xform3 ScaleXYZAbout (Vector3 const &point, float cfXYZ)
	static Xform3 ScaleXYZAbout (Vector3 const &point, float cfX, float cfY, float cfZ)
	static Xform3 ScaleXYZAbout (Vector3 const &point, Vector3 const &cfXYZ)
	static Xform3 BoundToBound (AABB2 const &pre, AABB2 const &rhs)
	static Xform3 BoundToBoundX (AABB2 const &pre, AABB2 const &rhs)
	static Xform3 BoundToBoundY (AABB2 const &pre, AABB2 const &rhs)
Public Attributes
	Matrix4 const Matrix
Operators
	Xform3 operator+(Xform3 const & lhs, Xform3 const & rhs)
	Xform3 operator-(Xform3 const & lhs, Xform3 const & rhs)
	Xform3 operator+(Xform3 const & xf)
	Xform3 operator-(Xform3 const & xf)
	bool operator==(Xform3 const & lhs, Xform3 const & rhs)
	bool operator!=(Xform3 const & lhs, Xform3 const & rhs)
```
</details>

<details><summary>OrthoFrame3 Class Reference</summary>

```
Public Member Functions
	Matrix4 Matrix ()
	void Reset ()
	void ResetOrientation ()
	void ResetOrientationAndScale ()
	void ResetScale ()
	void ResetPosition ()
	void Orthogonalize ()
	void OrthoNormalize ()
	void RotateAroundUp (float degrees)
	void RotateAroundRight (float degrees)
	void RotateAroundForward (float degrees)
	void RotateX (float degrees)
	void RotateY (float degrees)
	void RotateZ (float degrees)
	void Rotate (Vector3 const &axis, float degrees)
	void Move (float amountX, float amountY, float amountZ)
	void Move (Vector3 direction)
	void MoveTo (Vector3 position)
	void MoveTo (float x, float y, float z)
	void MoveX (float distance)
	void MoveY (float distance)
	void MoveZ (float distance)
	void MoveForward (float distance)
	void MoveRight (float distance)
	void MoveUp (float distance)
	Vector3 GetScale () const
	void Scale (float s)
	void Scale (float sRight, float sUp, float sForward)
	void Scale (Vector3 const &s)
	void ScaleTo (float s)
	void ScaleTo (Vector3 const &s)
	void ScaleTo (float sRight, float sUp, float sForward)
	void ScaleRightTo (float r)
	void ScaleUpTo (float u)
	void ScaleForwardTo (float f)
Public Attributes
	Vector3 Right
	Vector3 Up
	Vector3 Forward
	Vector3 Position
```
</details>
	
## Axis-Aligned Bounding Boxes

<details><summary>AABB2 Class Reference</summary>

```
Public Member Functions
	AABB2 (Vector2 const &point)
	AABB2 (std::vector< Vector2 > const &points)
	AABB2 (Vector2 const &min, Vector2 const &max)
	bool IsEverything () const
	bool IsNothing () const
	bool IsPoint () const
	Vector2 PointCenter () const
	Vector2 PointN () const
	Vector2 PointS () const
	Vector2 PointE () const
	Vector2 PointW () const
	Vector2 PointNW () const
	Vector2 PointNE () const
	Vector2 PointSE () const
	Vector2 PointSW () const
	Vector2 Size () const
	Vector2 HalfSize () const
	float Area () const
	float Perimeter () const
	void AddPoint (Vector2 const &p)
	void AddPoints (std::vector< Vector2 > const &ps)
	void AddAABB (AABB2 const &aabb)
	bool Envelopes (AABB2 const &aabb) const
	bool Intersects (AABB2 const &aabb) const
	bool Intersects (Vector2 const &v) const
	AABB2 Intersection (AABB2 const &aabb) const
Static Public Member Functions
	static AABB2 Everything ()
	static AABB2 Nothing ()
Public Attributes
	Vector2 Min
	Vector2 Max
```
</details>

<details><summary>AABB2i Class Reference</summary>

```
Public Member Functions
	AABB2i (Vector2i const &point)
	AABB2i (std::vector< Vector2i > const &points)
	AABB2i (Vector2i const &min, Vector2i const &max)
	bool IsEverything () const
	bool IsNothing () const
	bool IsPoint () const
	Vector2i PointNW () const
	Vector2i PointNE () const
	Vector2i PointSE () const
	Vector2i PointSW () const
	Vector2i Size () const
	int32_t Area () const
	int32_t Perimeter () const
	void AddPoint (Vector2i const &p)
	void AddPoints (std::vector< Vector2i > const &ps)
	void AddAABB (AABB2i const &aabb)
	bool Envelopes (AABB2i const &aabb) const
	bool Intersects (AABB2i const &aabb) const
	bool Intersects (Vector2i const &v) const
	AABB2i Intersection (AABB2i const &aabb) const
Static Public Member Functions
	static AABB2i Everything ()
	static AABB2i Nothing ()
Public Attributes
	Vector2i Min
	Vector2i Max
```
</details>
 
<details><summary>AABB3 Class Reference</summary>

```
Public Member Functions
	AABB3 (Vector3 const &point)
	AABB3 (std::vector< Vector3 > const &points)
	AABB3 (Vector3 const &min, Vector3 const &max)
	bool IsEverything () const
	bool IsNothing () const
	bool IsPoint () const
	Vector3 PointCenter () const
	Vector3 Size () const
	Vector3 HalfSize () const
	float Volume () const
	float SurfaceArea () const
	void AddPoint (Vector3 const &p)
	void AddPoints (std::vector< Vector3 > const &ps)
	void AddAABB (AABB3 const &aabb)
	bool Envelopes (AABB3 const &aabb) const
	bool Intersects (AABB3 const &aabb) const
	bool Intersects (Vector3 const &v) const
Static Public Member Functions
	static AABB3 Everything ()
	static AABB3 Nothing ()
Public Attributes
	Vector3 Min
	Vector3 Max
```
</details>
 
## Additional Functionality
 
<details><summary>Free Functions</summary>

```
int64_t Popcount(uint64_t num)
int32_t Popcount(uint32_t num)
```
</details>

