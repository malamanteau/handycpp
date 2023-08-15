
/// See ../License.txt for license info.

#include <iostream>
#include <chrono>
#include <functional>

#include "Tests.hpp"

#ifdef _MSC_VER
	#pragma optimize("", off)
	template <typename T>
	inline void escape(T* p) 
	{
		*reinterpret_cast<char       volatile*>(p) =
		*reinterpret_cast<char const volatile*>(p);
	}
	#pragma optimize("", on)
#else
	// Only works on GGC/Clang: Boo.
	void clobber()       { asm volatile("" : :        : "memory"); }
	void escape(void* p) { asm volatile("" : : "g"(p) : "memory"); }
#endif

struct ScopedNanoTimer
{
	std::chrono::high_resolution_clock::time_point t0;
	std::function<void(int64_t)> cb;

	ScopedNanoTimer(std::function<void(int64_t)> callback)
	:	t0(std::chrono::high_resolution_clock::now()), 
		cb(callback)
	{ }

	~ScopedNanoTimer(void)
	{
		auto  t1 = std::chrono::high_resolution_clock::now();
		auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(t1-t0).count();
		
		cb(nanos);
	}
};

#define RunEscOut(t1, e)                \
{                                       \
	Results results;                    \
	ScopedNanoTimer Timer(results);     \
	auto te = e;                        \
	std::cout << t1 << te << "  ";      \
	for (int i = 0; i < 250000000; i++) \
	{                                   \
		auto var = e;                   \
		escape(&var);                   \
	}                                   \
	std::cout << std::endl;             \
} 

#define RunEscOut2(t1, e, e2)           \
{                                       \
	Results results;                    \
	ScopedNanoTimer Timer(results);     \
	auto te = e;                        \
	std::cout << t1 << te << "  ";      \
	for (int i = 0; i < 250000000; i++) \
	{                                   \
		auto var = e;                   \
		escape(&var);                   \
		escape(&e2);                    \
	}                                   \
	std::cout << std::endl;             \
} 

int main4I()
{
	struct Results
	{
		void operator()(int64_t ns)
		{
			//std::cout << "Ops        : " << N << std::endl;
			//std::cout << "Nanos      : " << ns << std::endl;
			double s = (((double)ns) / 1000000000.0_d);

			if (s > 0.05f)
				std::cout << "---Seconds    : " << s << std::endl;
			//std::cout << "ns per op  : " << (ns/N) << std::endl;
		}
	};

	auto ex  = Math::Vector4i(10_i32, 20_i32, 30_i32, 40_i32);

	auto var = ex;

	RunEscOut(".X: ", ex.X);
	RunEscOut(".Y: ", ex.Y);
	RunEscOut(".Z: ", ex.Z);
	RunEscOut(".W: ", ex.W);

	std::cout << std::endl;

	RunEscOut("XYZW: ", ex);

	std::cout << std::endl;

	RunEscOut("InsertX: ", ex.InsertX(42));
	RunEscOut("InsertY: ", ex.InsertY(42));
	RunEscOut("InsertZ: ", ex.InsertZ(42));
	RunEscOut("InsertW: ", ex.InsertW(42));

	std::cout << std::endl;

	RunEscOut("InsertZero: ", ex.InsertZero<2>());
	RunEscOut("Insert    : ", ex.Insert<2>(84));

	std::cout << std::endl;

	//RunEscOut("WXYZ: ", ex.WXYZ());
	RunEscOut("ZWXY: ", ex.ZWXY());
	//RunEscOut("YZWX: ", ex.YZWX());
	RunEscOut("WZYX: ", ex.WZYX());

	std::cout << std::endl;

	RunEscOut("X000: ", ex.X000());
	RunEscOut("XY00: ", ex.XY00());
	RunEscOut("XY01: ", ex.XY01());
	RunEscOut("XYZ0: ", ex.XYZ0());
	RunEscOut("XYZ1: ", ex.XYZ1());

	Math::Vector4i abcd(1, 2, 3, 4);

	std::cout << std::endl << "ABCD: " << abcd << std::endl << std::endl;

	RunEscOut("XXYY: ", ex.XXYY());
	RunEscOut("ZZWW: ", ex.ZZWW());

	RunEscOut("XAYB: ", ex.XAYB(abcd));
	RunEscOut("ZCWD: ", ex.ZCWD(abcd));

	std::cout << std::endl;

	auto zwxy = ex.ZWXY();

	RunEscOut("Min(XYZW, ZWXY): ", ex.Min(zwxy));
	RunEscOut("Max(XYZW, ZWXY): ", ex.Max(zwxy));
	RunEscOut("Min(XYZW, 25)  : ", ex.Min(25));
	RunEscOut("Max(XYZW, 25)  : ", ex.Max(25));

	std::cout << std::endl;

	ex = Math::Vector4i(-10, -20, 30, 40);

	RunEscOut("Set to : ", ex);

	std::cout << std::endl;

	RunEscOut("HMin     : ", ex.Min());
	RunEscOut("HMax     : ", ex.Max());
	RunEscOut("HSum     : ", ex.Sum());
	RunEscOut("HProduct : ", ex.Product());

	std::cout << std::endl;

	RunEscOut("Abs        : ", ex.Abs());

	std::cout << std::endl;

	RunEscOut("To         : ", ex.To(-ex));

	std::cout << std::endl;

	RunEscOut("TaxicabDistance : ", ex.TaxicabDistance(-ex));
	RunEscOut("TaxicabLength   : ", ex.TaxicabLength());

	std::cout << std::endl;

	RunEscOut("Dot   : ", ex.Dot(-ex));

	std::cout << std::endl;

	RunEscOut("Power: ",       ex.Power(2));

	std::cout << std::endl;

	RunEscOut("HasMaxValue: ", ex.HasMaxValue());
	RunEscOut("HasMinValue: ", ex.HasMinValue());
	RunEscOut("HasMinMaxValue: ", ex.HasMinMaxValue());

	std::cout << std::endl;

	RunEscOut("Clamp: ", ex.Clamp(20, 30));
	RunEscOut("Clamp: ", ex.Clamp(Math::Vector4i(20), Math::Vector4i(30)));

	std::cout << std::endl;

	RunEscOut("XXXX: ", ex.XXXX());
	RunEscOut("YYYY: ", ex.YYYY());
	RunEscOut("ZZZZ: ", ex.ZZZZ());
	RunEscOut("WWWW: ", ex.WWWW());

	std::cout << std::endl;

	//Math::Vector4i ijkl(2, 4, 6, 8);

	//std::cout << std::endl << "ijkl: " << ijkl << std::endl << std::endl;

	return 0;
} 

int main4D()
{
	struct Results
	{
		void operator()(int64_t ns)
		{
			//std::cout << "Ops        : " << N << std::endl;
			//std::cout << "Nanos      : " << ns << std::endl;
			double s = (((double)ns) / 1000000000.0_d);

			if (s > 0.05f)
				std::cout << "---Seconds    : " << s << std::endl;
			//std::cout << "ns per op  : " << (ns/N) << std::endl;
		}
	};

	auto ex  = Math::Vector4d(10.0f, 20.0f, 30.0f, 40.0f);

	auto var = ex;

	RunEscOut(".X: ", ex.X);
	RunEscOut(".Y: ", ex.Y);
	RunEscOut(".Z: ", ex.Z);
	RunEscOut(".W: ", ex.W);

	std::cout << std::endl;

	RunEscOut("XYZW: ", ex);

	std::cout << std::endl;

	RunEscOut("InsertX: ", ex.InsertX(42.0f));
	RunEscOut("InsertY: ", ex.InsertY(42.0f));
	RunEscOut("InsertZ: ", ex.InsertZ(42.0f));
	RunEscOut("InsertW: ", ex.InsertW(42.0f));

	std::cout << std::endl;

	RunEscOut("InsertZero: ", ex.InsertZero<2>());
	RunEscOut("Insert    : ", ex.Insert<2>(84.0f));

	std::cout << std::endl;

	//RunEscOut("WXYZ: ", ex.WXYZ());
	RunEscOut("ZWXY: ", ex.ZWXY());
	//RunEscOut("YZWX: ", ex.YZWX());
	RunEscOut("WZYX: ", ex.WZYX());

	std::cout << std::endl;

	/*RunEscOut("XYZZ: ", ex.XYZZ());
	RunEscOut("XXXW: ", ex.XXXW());
	RunEscOut("YYYW: ", ex.YYYW());
	RunEscOut("ZZZW: ", ex.ZZZW());
	RunEscOut("XXWW: ", ex.XXWW());

	std::cout << std::endl;*/

	RunEscOut("X000: ", ex.X000());
	RunEscOut("XY00: ", ex.XY00());
	RunEscOut("XY01: ", ex.XY01());
	RunEscOut("XYZ0: ", ex.XYZ0());
	RunEscOut("XYZ1: ", ex.XYZ1());

	std::cout << std::endl;

	RunEscOut("XXZZ: ", ex.XXZZ());
	RunEscOut("YYWW: ", ex.YYWW());

	Math::Vector4d abcd(1.0f, 2.0f, 3.0f, 4.0f);

	std::cout << std::endl << "ABCD: " << abcd << std::endl << std::endl;

	RunEscOut("YBWD: ", ex.YBWD(abcd));
	RunEscOut("XAZC: ", ex.XAZC(abcd));

	std::cout << std::endl;

	RunEscOut("ZCWD: ", ex.ZCWD(abcd));
	RunEscOut("XYAB: ", ex.XYAB(abcd));
	RunEscOut("ZWCD: ", ex.ZWCD(abcd));
	RunEscOut("XAYB: ", ex.XAYB(abcd));

	std::cout << std::endl;

	RunEscOut("ShuffleAABB_0000: ", (ex.ShuffleAABB<0, 0, 0, 0>(abcd)));
	RunEscOut("ShuffleAABB_1111: ", (ex.ShuffleAABB<1, 1, 1, 1>(abcd)));
	RunEscOut("ShuffleAABB_2222: ", (ex.ShuffleAABB<2, 2, 2, 2>(abcd)));
	RunEscOut("ShuffleAABB_3333: ", (ex.ShuffleAABB<3, 3, 3, 3>(abcd)));
	RunEscOut("ShuffleAABB_0123: ", (ex.ShuffleAABB<0, 1, 2, 3>(abcd)));
	RunEscOut("ShuffleAABB_3210: ", (ex.ShuffleAABB<3, 2, 1, 0>(abcd)));
	RunEscOut("ShuffleAABB_2323: ", (ex.ShuffleAABB<2, 3, 2, 3>(abcd)));

	std::cout << std::endl;


	auto zwxy = ex.ZWXY();

	RunEscOut("Min(XYZW, ZWXY): ", ex.Min(zwxy));
	RunEscOut("Max(XYZW, ZWXY): ", ex.Max(zwxy));
	RunEscOut("Min(XYZW, 25)  : ", ex.Min(25.0f));
	RunEscOut("Max(XYZW, 25)  : ", ex.Max(25.0f));

	std::cout << std::endl;

	ex = Math::Vector4d(-10, -20, 30, 40);

	RunEscOut("Set to : ", ex);

	std::cout << std::endl;

	RunEscOut("HMin     : ", ex.Min());
	RunEscOut("HMax     : ", ex.Max());
	RunEscOut("HSum     : ", ex.Sum());
	RunEscOut("HProduct : ", ex.Product());
	RunEscOut("HMean    : ", ex.Mean());

	std::cout << std::endl;

	RunEscOut("Sign       : ", ex.Sign());

	RunEscOut("Abs        : ", ex.Abs());
	RunEscOut("Half       : ", ex.Half());
	RunEscOut("Reciprocal : ", ex.Reciprocal());
	RunEscOut("RecipApprox: ", ex.ReciprocalApprox());
	RunEscOut("Sqrt       : ", ex.Sqrt());
	RunEscOut("RSqrt      : ", ex.RSqrt());
	RunEscOut("RSqrtApprox: ", ex.RSqrtApprox());

	std::cout << std::endl;

	RunEscOut("To         : ", ex.To(-ex));

	std::cout << std::endl;

	RunEscOut("Distance   : ", ex.Distance(-ex));
	RunEscOut("DistanceSq : ", ex.DistanceSquared(-ex));
	RunEscOut("Length   : ", ex.Length());
	RunEscOut("LengthSq : ", ex.LengthSquared());

	RunEscOut("Normalized : ", ex.Normalized());

	std::cout << std::endl;

	RunEscOut("Angle : ", ex.Angle(-ex));
	RunEscOut("Dot   : ", ex.Dot(-ex));

	std::cout << std::endl;

	//RunEscOut("PowerApprox: ", ex.PowerApprox(0.5f));

	std::cout << std::endl;

	RunEscOut("HasNaN: ", ex.HasNaN());
	RunEscOut("HasPositiveInfinity: ", ex.HasPositiveInfinity());
	RunEscOut("HasNegativeInfinity: ", ex.HasNegativeInfinity());
	RunEscOut("IsUnbounded: ", ex.IsUnbounded());

	std::cout << std::endl;

	//RunEscOut("WithinEpsilon: ", ex.WithinEpsilon(ex));
	//RunEscOut("WithinEpsilon: ", ex.WithinEpsilon(ex, 0.1f));

	std::cout << std::endl;

	RunEscOut("Clamp: ", ex.Clamp(20.0f, 30.0f));
	RunEscOut("Clamp: ", ex.Clamp(Math::Vector4d(20.0f), Math::Vector4d(30.0f)));

	std::cout << std::endl;

	RunEscOut("XXXX: ", ex.XXXX());
	RunEscOut("YYYY: ", ex.YYYY());
	RunEscOut("ZZZZ: ", ex.ZZZZ());
	RunEscOut("WWWW: ", ex.WWWW());

	std::cout << std::endl;

	RunEscOut("MultiplyAdd: ", ex.MultiplyAdd(ex, -ex));
	RunEscOut("MultiplySub: ", ex.MultiplySub(ex, -ex));

	Math::Vector4d ijkl(0.2f, 0.4f, 1.6f, 1.8f);

	std::cout << std::endl << "ijkl: " << ijkl << std::endl << std::endl;

	RunEscOut("Lerp: ",       ex.Lerp      (2.0f * ijkl, 0.99f));
	RunEscOut("SmoothStep: ", ex.SmoothStep(2.0f * ijkl, 0.99f));

	std::cout << std::endl;

	RunEscOut("Round_Near : ", ijkl.Round_Near());
	RunEscOut("Round_Floor: ", ijkl.Round_Floor());
	RunEscOut("Round_Ceil : ", ijkl.Round_Ceil());
	RunEscOut("Round_Trunc: ", ijkl.Round_Trunc());

	RunEscOut("FMod1  : ", ijkl.FMod(1.0f));
	RunEscOut("FModv1 : ", ijkl.FMod(Math::Vector4d(1.0f)));

	std::cout << std::endl;

	uint64_t testsdfs = 0b00'10101'0101'0101;

	return 0;
} 


int main4F()
{
	struct Results
	{
		void operator()(int64_t ns)
		{
			//std::cout << "Ops        : " << N << std::endl;
			//std::cout << "Nanos      : " << ns << std::endl;
			double s = (((double)ns) / 1000000000.0_d);

			if (s > 0.05f)
				std::cout << "---Seconds    : " << s << std::endl;
			//std::cout << "ns per op  : " << (ns/N) << std::endl;
		}
	};

	auto ex  = Math::Vector4(10.0f, 20.0f, 30.0f, 40.0f);

	auto var = ex;

	RunEscOut(".X: ", ex.X);
	RunEscOut(".Y: ", ex.Y);
	RunEscOut(".Z: ", ex.Z);
	RunEscOut(".W: ", ex.W);

	std::cout << std::endl;

	RunEscOut("XYZW: ", ex);

	std::cout << std::endl;

	RunEscOut("InsertX: ", ex.InsertX(42.0f));
	RunEscOut("InsertY: ", ex.InsertY(42.0f));
	RunEscOut("InsertZ: ", ex.InsertZ(42.0f));
	RunEscOut("InsertW: ", ex.InsertW(42.0f));

	std::cout << std::endl;

	RunEscOut("InsertZero: ", ex.InsertZero<2>());
	RunEscOut("Insert    : ", ex.Insert<2>(84.0f));

	std::cout << std::endl;

	//RunEscOut("WXYZ: ", ex.WXYZ());
	RunEscOut("ZWXY: ", ex.ZWXY());
	//RunEscOut("YZWX: ", ex.YZWX());
	RunEscOut("WZYX: ", ex.WZYX());

	std::cout << std::endl;

	/*RunEscOut("XYZZ: ", ex.XYZZ());
	RunEscOut("XXXW: ", ex.XXXW());
	RunEscOut("YYYW: ", ex.YYYW());
	RunEscOut("ZZZW: ", ex.ZZZW());
	RunEscOut("XXWW: ", ex.XXWW());

	std::cout << std::endl;*/

	RunEscOut("X000: ", ex.X000());
	RunEscOut("XY00: ", ex.XY00());
	RunEscOut("XY01: ", ex.XY01());
	RunEscOut("XYZ0: ", ex.XYZ0());
	RunEscOut("XYZ1: ", ex.XYZ1());

	std::cout << std::endl;

	RunEscOut("XXZZ: ", ex.XXZZ());
	RunEscOut("YYWW: ", ex.YYWW());

	Math::Vector4 abcd(1.0f, 2.0f, 3.0f, 4.0f);

	std::cout << std::endl << "ABCD: " << abcd << std::endl << std::endl;

	RunEscOut("ZCWD: ", ex.ZCWD(abcd));
	RunEscOut("XYAB: ", ex.XYAB(abcd));
	RunEscOut("ZWCD: ", ex.ZWCD(abcd));
	RunEscOut("XAYB: ", ex.XAYB(abcd));

	std::cout << std::endl;

	RunEscOut("ShuffleAABB_0000: ", (ex.ShuffleAABB<0, 0, 0, 0>(abcd)));
	RunEscOut("ShuffleAABB_1111: ", (ex.ShuffleAABB<1, 1, 1, 1>(abcd)));
	RunEscOut("ShuffleAABB_2222: ", (ex.ShuffleAABB<2, 2, 2, 2>(abcd)));
	RunEscOut("ShuffleAABB_3333: ", (ex.ShuffleAABB<3, 3, 3, 3>(abcd)));
	RunEscOut("ShuffleAABB_0123: ", (ex.ShuffleAABB<0, 1, 2, 3>(abcd)));
	RunEscOut("ShuffleAABB_3210: ", (ex.ShuffleAABB<3, 2, 1, 0>(abcd)));
	RunEscOut("ShuffleAABB_2323: ", (ex.ShuffleAABB<2, 3, 2, 3>(abcd)));

	std::cout << std::endl;


	auto zwxy = ex.ZWXY();

	RunEscOut("Min(XYZW, ZWXY): ", ex.Min(zwxy));
	RunEscOut("Max(XYZW, ZWXY): ", ex.Max(zwxy));
	RunEscOut("Min(XYZW, 25)  : ", ex.Min(25.0f));
	RunEscOut("Max(XYZW, 25)  : ", ex.Max(25.0f));
	
	std::cout << std::endl;

	ex = Math::Vector4(-10, -20, 30, 40);

	RunEscOut("Set to : ", ex);

	std::cout << std::endl;

	RunEscOut("HMin     : ", ex.Min());
	RunEscOut("HMax     : ", ex.Max());
	RunEscOut("HSum     : ", ex.Sum());
	RunEscOut("HProduct : ", ex.Product());
	RunEscOut("HMean    : ", ex.Mean());

	std::cout << std::endl;

	RunEscOut("Abs        : ", ex.Abs());
	RunEscOut("Sign       : ", ex.Sign());
	RunEscOut("Half       : ", ex.Half());
	RunEscOut("Reciprocal : ", ex.Reciprocal());
	RunEscOut("RecipApprox: ", ex.ReciprocalApprox());
	RunEscOut("Sqrt       : ", ex.Sqrt());
	RunEscOut("RSqrt      : ", ex.RSqrt());
	RunEscOut("RSqrtApprox: ", ex.RSqrtApprox());

	std::cout << std::endl;

	RunEscOut("To         : ", ex.To(-ex));

	std::cout << std::endl;

	RunEscOut("Distance   : ", ex.Distance(-ex));
	RunEscOut("DistanceSq : ", ex.DistanceSquared(-ex));
	RunEscOut("Length   : ", ex.Length());
	RunEscOut("LengthSq : ", ex.LengthSquared());

	RunEscOut("Normalized : ", ex.Normalized());

	std::cout << std::endl;

	RunEscOut("Angle : ", ex.Angle(-ex));
	RunEscOut("Dot   : ", ex.Dot(-ex));

	std::cout << std::endl;

	//RunEscOut("PowerApprox: ", ex.PowerApprox(0.5f));

	std::cout << std::endl;

	RunEscOut("HasNaN: ", ex.HasNaN());
	RunEscOut("HasPositiveInfinity: ", ex.HasPositiveInfinity());
	RunEscOut("HasNegativeInfinity: ", ex.HasNegativeInfinity());
	RunEscOut("IsUnbounded: ", ex.IsUnbounded());

	std::cout << std::endl;

	//RunEscOut("WithinEpsilon: ", ex.WithinEpsilon(ex));
	//RunEscOut("WithinEpsilon: ", ex.WithinEpsilon(ex, 0.1f));

	std::cout << std::endl;

	RunEscOut("Clamp: ", ex.Clamp(20.0f, 30.0f));
	RunEscOut("Clamp: ", ex.Clamp(Math::Vector4(20.0f), Math::Vector4(30.0f)));

	std::cout << std::endl;

	RunEscOut("XXXX: ", ex.XXXX());
	RunEscOut("YYYY: ", ex.YYYY());
	RunEscOut("ZZZZ: ", ex.ZZZZ());
	RunEscOut("WWWW: ", ex.WWWW());

	std::cout << std::endl;

	RunEscOut("MultiplyAdd: ", ex.MultiplyAdd(ex, -ex));
	RunEscOut("MultiplySub: ", ex.MultiplySub(ex, -ex));

	Math::Vector4 ijkl(0.2f, 0.4f, 1.6f, 1.8f);
		
	std::cout << std::endl << "ijkl: " << ijkl << std::endl << std::endl;

	RunEscOut("Lerp: ",       ex.Lerp      (2.0f * ijkl, 0.99f));
	RunEscOut("SmoothStep: ", ex.SmoothStep(2.0f * ijkl, 0.99f));

	std::cout << std::endl;

	RunEscOut("Round_Near : ", ijkl.Round_Near());
	RunEscOut("Round_Floor: ", ijkl.Round_Floor());
	RunEscOut("Round_Ceil : ", ijkl.Round_Ceil());
	RunEscOut("Round_Trunc: ", ijkl.Round_Trunc());
	
	RunEscOut("FMod1  : ", ijkl.FMod(1.0f));
	RunEscOut("FModv1 : ", ijkl.FMod(Math::Vector4(1.0f)));

	std::cout << std::endl;

	uint64_t testsdfs = 0b00'10101'0101'0101;

	return 0;
} 

int main3F()
{
	struct Results
	{
		void operator()(int64_t ns)
		{
			//std::cout << "Ops        : " << N << std::endl;
			//std::cout << "Nanos      : " << ns << std::endl;
			double s = (((double)ns) / 1000000000.0_d);

			if (s > 0.05f)
				std::cout << "---Seconds    : " << s << std::endl;
			//std::cout << "ns per op  : " << (ns/N) << std::endl;
		}
	};

	auto ex  = Math::Vector3(10.0f, 20.0f, 30.0f);

	auto var = ex;

	RunEscOut(".X: ", ex.X);
	RunEscOut(".Y: ", ex.Y);
	RunEscOut(".Z: ", ex.Z);

	std::cout << std::endl;

	RunEscOut("XYZW: ", ex);

	std::cout << std::endl;

	RunEscOut("InsertX: ", ex.InsertX(42.0f));
	RunEscOut("InsertY: ", ex.InsertY(42.0f));
	RunEscOut("InsertZ: ", ex.InsertZ(42.0f));

	std::cout << std::endl;

	RunEscOut("InsertZero: ", ex.InsertZero<2>());
	RunEscOut("Insert    : ", ex.Insert<2>(84.0f));

	std::cout << std::endl;

	RunEscOut("X000: ", ex.X000());
	RunEscOut("XY00: ", ex.XY00());
	RunEscOut("XY01: ", ex.XY01());
	RunEscOut("XY11: ", ex.XY11());
	RunEscOut("XYZ0: ", ex.XYZ0());
	RunEscOut("XYZ1: ", ex.XYZ1());

	std::cout << std::endl;

	//RunEscOut("XXZZ: ", ex.XXZZ());
	//RunEscOut("YY00: ", ex.YY00());

	Math::Vector4 abcd(1.0f, 2.0f, 3.0f, 4.0f);

	std::cout << std::endl << "ABCD: " << abcd << std::endl << std::endl;

	//RunEscOut("ZC00: ", ex.ZC00(abcd));
	//RunEscOut("XYAB: ", ex.XYAB(abcd));
	//RunEscOut("Z0C0: ", ex.Z0C0(abcd));
	//RunEscOut("XAYB: ", ex.XAYB(abcd));

	std::cout << std::endl;

	RunEscOut("X00: ", ex.X00());
	RunEscOut("XY0: ", ex.XY0());
	RunEscOut("XY1: ", ex.XY1());

	RunEscOut("YY0: ", ex.YY0());

	RunEscOut("ZZ0: ", ex.ZZ0());
	RunEscOut("Z0Z: ", ex.Z0Z());

	RunEscOut("ZC0: ", ex.ZC0(Math::Vector3(101.0f, 202.0f, 303.0f)));
	RunEscOut("Z0C: ", ex.Z0C(Math::Vector3(101.0f, 202.0f, 303.0f)));

	std::cout << std::endl;

	auto zwxy = ex.ZYX();

	RunEscOut("Min(XYZW, ZWXY): ", ex.Min(zwxy));
	RunEscOut("Max(XYZW, ZWXY): ", ex.Max(zwxy));
	RunEscOut("Min(XYZW, 25)  : ", ex.Min(25.0f));
	RunEscOut("Max(XYZW, 25)  : ", ex.Max(25.0f));

	std::cout << std::endl;

	ex = Math::Vector3(-10, -20, 30);

	RunEscOut("Set to : ", ex);

	std::cout << std::endl;

	RunEscOut("HMin     : ", ex.Min());
	RunEscOut("HMax     : ", ex.Max());
	RunEscOut("HSum     : ", ex.Sum());
	RunEscOut("HProduct : ", ex.Product());
	RunEscOut("HMean    : ", ex.Mean());

	std::cout << std::endl;

	RunEscOut("Abs        : ", ex.Abs());
	RunEscOut("Half       : ", ex.Half());
	RunEscOut("Reciprocal : ", ex.Reciprocal());
	RunEscOut("RecipApprox: ", ex.ReciprocalApprox());
	RunEscOut("Sqrt       : ", ex.Sqrt());
	RunEscOut("RSqrt      : ", ex.RSqrt());
	RunEscOut("RSqrtApprox: ", ex.RSqrtApprox());

	std::cout << std::endl;

	RunEscOut("To         : ", ex.To(-ex));

	std::cout << std::endl;

	RunEscOut("Distance   : ", ex.Distance(-ex));
	RunEscOut("DistanceSq : ", ex.DistanceSquared(-ex));
	RunEscOut("Length     : ", ex.Length());
	RunEscOut("LengthSq   : ", ex.LengthSquared());

	RunEscOut("Normalized : ", ex.Normalized());

	std::cout << std::endl;

	RunEscOut("Angle : ", ex.Angle(-ex));
	RunEscOut("Dot   : ", ex.Dot(-ex));

	RunEscOut("Cross : ", ex.Cross(zwxy));

	std::cout << std::endl;

	RunEscOut("HasNaN: ", ex.HasNaN());
	RunEscOut("HasPositiveInfinity: ", ex.HasPositiveInfinity());
	RunEscOut("HasNegativeInfinity: ", ex.HasNegativeInfinity());
	RunEscOut("IsUnbounded: ", ex.IsUnbounded());

	std::cout << std::endl;

	//RunEscOut("WithinEpsilon: ", ex.WithinEpsilon(ex));
	//RunEscOut("WithinEpsilon: ", ex.WithinEpsilon(ex, 0.1f));

	std::cout << std::endl;

	RunEscOut("Clamp: ", ex.Clamp(20.0f, 30.0f));
	RunEscOut("Clamp: ", ex.Clamp(Math::Vector3(20.0f), Math::Vector3(30.0f)));

	std::cout << std::endl;

	RunEscOut("XXX: ", ex.XXX());
	RunEscOut("YYY: ", ex.YYY());
	RunEscOut("ZZZ: ", ex.ZZZ());

	std::cout << std::endl;

	RunEscOut("MultiplyAdd: ", ex.MultiplyAdd(ex, -ex));
	RunEscOut("MultiplySub: ", ex.MultiplySub(ex, -ex));

	Math::Vector3 ijk(0.2f, 0.4f, 1.6f);

	std::cout << std::endl << "ijkl: " << ijk << std::endl << std::endl;

	RunEscOut("Lerp: ",       ex.Lerp      (2.0f * ijk, 0.99f));
	RunEscOut("SmoothStep: ", ex.SmoothStep(2.0f * ijk, 0.99f));

	std::cout << std::endl;

	RunEscOut("Round_Near : ", ijk.Round_Near());
	RunEscOut("Round_Floor: ", ijk.Round_Floor());
	RunEscOut("Round_Ceil : ", ijk.Round_Ceil());
	RunEscOut("Round_Trunc: ", ijk.Round_Trunc());

	RunEscOut("FMod1  : ", ijk.FMod(1.0f));
	RunEscOut("FModv1 : ", ijk.FMod(Math::Vector3(1.0f)));

	std::cout << std::endl;

	uint64_t testsdfs = 0b00'10101'0101'0101;

	return 0;
} 

auto minitest()
{

}

int mainM44F()
{
	struct Results
	{
		void operator()(int64_t ns)
		{
			//std::cout << "Ops        : " << N << std::endl;
			//std::cout << "Nanos      : " << ns << std::endl;
			double s = (((double)ns) / 1000000000.0_d);

			if (s > 0.05f)
				std::cout << "---Seconds    : " << s << std::endl;
			//std::cout << "ns per op  : " << (ns/N) << std::endl;
		}
	};

	Math::Matrix4 m(2.1f, 6.3f, 10.5f,  1.1f,
	                3.3f, 7.9f, 11.1f,  1.3f,
	                4.1f, 8.2f, 12.4f,  1.2f,
	                5.6f, 9.9f, 13.13f, 1.13f);

	Math::Matrix4 target(
		-1.48088f,   0.97228f,   0.31483f,  -4.83744f, // <-XCol
		 0.04850f,   0.36593f,  -0.60652f,   3.60110f, // <-YCol
		 2.06317f,  -2.38688f,   0.46071f,   5.33391f, // <-ZCol
		-0.80522f,   1.16730f,  -0.09791f,  -4.21322f);// <-WCol
	
	target = target.Transpose();

	std::cout << "Matrix M :"      << m      << std::endl << std::endl;
	std::cout << "Target Inverse:" << target << std::endl << std::endl;

	RunEscOut("Inverse : \r\n", (m.Inverse()));

	std::cout << std::endl << std::endl;

	RunEscOut("Determinant : \r\n", (m.Determinant()));

	std::cout << std::endl << std::endl;

	float det;
	RunEscOut2("InverseDeterminant : \r\n", (m.Inverse_Determinant(det)), det);

	std::cout << det << std::endl << std::endl;
	
	RunEscOut("Transpose : \r\n", (m.Transpose()));



	return 0;
}


//int mainNF()
//{
//	struct Results
//	{
//		void operator()(int64_t ns)
//		{
//			//std::cout << "Ops        : " << N << std::endl;
//			//std::cout << "Nanos      : " << ns << std::endl;
//			double s = (((double)ns) / 1000000000.0_d);
//
//			if (s > 0.05f)
//				std::cout << "---Seconds    : " << s << std::endl;
//			//std::cout << "ns per op  : " << (ns/N) << std::endl;
//		}
//	};
//
//	auto ex  = Math::VectorN<4>({ 10.0f, 20.0f, 30.0f, 40.0f });
//
//	auto ex2 = Math::VectorN<4>(ex);
//
//	auto ex3 = Math::VectorN<4>(50.0f);
//
//	auto ex4 = Math::VectorN<4>();
//
//
//	RunEscOut(".X: ", ex2[0]);
//	RunEscOut(".Y: ", ex2[1]);
//	RunEscOut(".Z: ", ex2[2]);
//	RunEscOut(".W: ", ex2[3]);
//
//	std::cout << std::endl;
//
//	Math::VectorN<4> abcd({ 1.0f, 2.0f, 3.0f, 4.0f });
//
//	RunEscOut(".A: ", abcd[0]);
//	RunEscOut(".B: ", abcd[1]);
//	RunEscOut(".C: ", abcd[2]);
//	RunEscOut(".D: ", abcd[3]);
//
//	std::cout << std::endl;
//
//	//auto zwxy = ex.Reverse();
//
//	//RunEscOut("Min(XYZW, ZWXY): ", ex.Min(zwxy));
//	//RunEscOut("Max(XYZW, ZWXY): ", ex.Max(zwxy));
//	//RunEscOut("Min(XYZW, 25)  : ", ex.Min(25.0f));
//	//RunEscOut("Max(XYZW, 25)  : ", ex.Max(25.0f));
//
//	//std::cout << std::endl;
//
//	ex = Math::VectorN<4>({ -10, -20, 30, 40 });
//
//	//RunEscOut("Set to : ", ex);
//
//	//std::cout << std::endl;
//
//	RunEscOut("HMin     : ", ex.Min());
//	RunEscOut("HMax     : ", ex.Max());
//	RunEscOut("HSum     : ", ex.Sum());
//	RunEscOut("HProduct : ", ex.Product());
//	RunEscOut("HMean    : ", ex.Mean());
//
//	std::cout << std::endl;
//
//	RunEscOut("Abs        : ", ex.Abs());
//	RunEscOut("Half       : ", ex.Half());
//	RunEscOut("Reciprocal : ", ex.Reciprocal());
//	RunEscOut("Sqrt       : ", ex.Sqrt());
//	RunEscOut("RSqrt      : ", ex.RSqrt());
//
//	std::cout << std::endl;
//
//	RunEscOut("To         : ", ex.To(-ex));
//
//	std::cout << std::endl;
//
//	RunEscOut("Distance   : ", ex.Distance(-ex));
//	RunEscOut("DistanceSq : ", ex.DistanceSquared(-ex));
//	RunEscOut("Length   : ",   ex.Length());
//	RunEscOut("LengthSq : ",   ex.LengthSquared());
//
//	RunEscOut("Normalized : ", ex.Normalized());
//
//	std::cout << std::endl;
//
//	RunEscOut("Angle : ", ex.Angle(-ex));
//	RunEscOut("Dot   : ", ex.Dot(-ex));
//
//	std::cout << std::endl;
//
//	//RunEscOut("PowerApprox: ", ex.PowerApprox(0.5f));
//
//	//std::cout << std::endl;
//
//	RunEscOut("HasNaN: ",              ex.HasNaN());
//	RunEscOut("HasPositiveInfinity: ", ex.HasPositiveInfinity());
//	RunEscOut("HasNegativeInfinity: ", ex.HasNegativeInfinity());
//	RunEscOut("IsUnbounded: ",         ex.IsUnbounded());
//
//	std::cout << std::endl;
//
//	RunEscOut("WithinEpsilon: ", ex.WithinEpsilon(ex));
//	//RunEscOut("WithinEpsilon: ", ex.WithinEpsilon(ex, 0.1f));
//
//	std::cout << std::endl;
//
//	RunEscOut("Clamp: ", ex.Clamp(20.0f, 30.0f));
//	RunEscOut("Clamp: ", ex.Clamp(Math::VectorN<4>(20.0f), Math::VectorN<4>(30.0f)));
//
//	std::cout << std::endl;
//
//	//RunEscOut("XXXX: ", ex.XXXX());
//	//RunEscOut("YYYY: ", ex.YYYY());
//	//RunEscOut("ZZZZ: ", ex.ZZZZ());
//	//RunEscOut("WWWW: ", ex.WWWW());
//
//	//std::cout << std::endl;
//
//	RunEscOut("MultiplyAdd: ", ex.MultiplyAdd(ex, -ex));
//	RunEscOut("MultiplySub: ", ex.MultiplySub(ex, -ex));
//
//	Math::VectorN<4> ijkl({ 0.2f, 0.4f, 1.6f, 1.8f });
//
//	std::cout << std::endl << "ijkl: " << ijkl << std::endl << std::endl;
//
//	RunEscOut("Lerp: ",       ex.Lerp      (2.0f * ijkl, 0.99f));
//	RunEscOut("SmoothStep: ", ex.SmoothStep(2.0f * ijkl, 0.99f));
//
//	std::cout << std::endl;
//
//	RunEscOut("Round_Near : ", ijkl.Round_Near());
//	RunEscOut("Round_Floor: ", ijkl.Round_Floor());
//	RunEscOut("Round_Ceil : ", ijkl.Round_Ceil());
//	RunEscOut("Round_Trunc: ", ijkl.Round_Trunc());
//
//	RunEscOut("FMod1  : ", ijkl.FMod(1.0f));
//	RunEscOut("FModv1 : ", ijkl.FMod(Math::VectorN<4>(1.0f)));
//
//	std::cout << std::endl;
//
//	uint64_t testsdfs = 0b00'10101'0101'0101;
//
//	return 0;
//} 
//

int __cdecl main()
{
	HANDY_NS::PrintSystemInfo();

	//std::cout << std::endl << std::endl << "-----------VECTORNF-----------" << std::endl << std::endl;
	//mainNF();


	std::cout << std::endl << std::endl << "-----------VECTOR4D-----------" << std::endl << std::endl;
	main4D();

	std::cout << std::endl << std::endl << "-----------VECTOR4F-----------" << std::endl << std::endl;
	main4F();

	std::cout << std::endl << std::endl << "-----------VECTOR4I-----------" << std::endl << std::endl;
	main4I();

	std::cout << std::endl << std::endl << "-----------VECTOR3F-----------" << std::endl << std::endl;
	main3F();

	std::cout << std::endl << std::endl << "-----------MATRIX44-----------" << std::endl << std::endl;
	mainM44F();



	return 0;
}
