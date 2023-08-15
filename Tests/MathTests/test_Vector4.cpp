
/// See ../License.txt for license info.

#include "TestsInternal.hpp"

std::vector<MathTestResult> HANDYMATH_NS::GetTestsVector4()
{
	Math::Vector4 v1  = Math::Vector4(  1.0f,   2.0f,      3.0f,  4.0f);
	Math::Vector4 v2  = Math::Vector4( 10.0f,  20.0f,     30.0f, 40.0f);
	Math::Vector4 v3  = Math::Vector4(-10.0f, -20.0f,     30.0f, 40.0f);
	Math::Vector4 vn  = Math::Vector4(  5.0f,   6.0f,       NAN,  8.0f);
	Math::Vector4 vip = Math::Vector4(  5.0f,   6.0f,  INFINITY,  8.0f);
	Math::Vector4 vin = Math::Vector4(  5.0f,   6.0f, -INFINITY,  8.0f);
	Math::Vector4 vs  = Math::Vector4(  1.0f,  20.0f,      3.0f, 40.0f);
	Math::Vector4 vd  = Math::Vector4( -4.2f,  -1.6f,      0.1f,  4.6f);

	Math::Vector4 vds = Math::Vector4(0.42f, 0.16f, -0.1f, -0.46f);

	float arr[] = { 1.0f, 2.0f, 3.0f, 4.0f };

	std::vector<MathTestResult> rs;


	ADDTHIS(Math::Vector4(arr),   Math::Vector4(1.0f, 2.0f, 3.0f, 4.0f));
	ADDTHIS(Math::Vector4(42.0f), Math::Vector4(42.0f, 42.0f, 42.0f, 42.0f));

	ADDTHIS(v1.X, 1_f);
	ADDTHIS(v1.Y, 2_f);
	ADDTHIS(v1.Z, 3_f);
	ADDTHIS(v1.W, 4_f);

	ADDTHIS(v1.XY(),  Math::Vector2(1.0f, 2.0f));
	ADDTHIS(v1.ZW(),  Math::Vector2(3.0f, 4.0f));
	ADDTHIS(v1.XYZ(), Math::Vector3(1.0f, 2.0f, 3.0f));

	ADDTHIS(v1.ToInteger(), Math::Vector4i(1, 2, 3, 4));
	ADDTHIS(Math::Vector4::FromArray(arr), Math::Vector4(1.0f, 2.0f, 3.0f, 4.0f));
	v1.CopyToArray(arr);
	ADDTHIS(Math::Vector4::FromArray(arr), Math::Vector4(1.0f, 2.0f, 3.0f, 4.0f));

	rs.push_back(MathTestResult::New("v1.Shuffle<2, 3, 1, 0>()", v1.Shuffle<2, 3, 1, 0>(), Math::Vector4(3.0f, 4.0f, 2.0f, 1.0f)));
	rs.push_back(MathTestResult::New("v1.Shuffle<0, 2, 3, 1>()", v1.Shuffle<0, 2, 3, 1>(), Math::Vector4(1.0f, 3.0f, 4.0f, 2.0f)));

	ADDTHIS(v1.Insert<0>(42.0f), Math::Vector4(42.0f,  2.0f,  3.0f,  4.0f));
	ADDTHIS(v1.Insert<1>(42.0f), Math::Vector4( 1.0f, 42.0f,  3.0f,  4.0f));
	ADDTHIS(v1.Insert<2>(42.0f), Math::Vector4( 1.0f,  2.0f, 42.0f,  4.0f));
	ADDTHIS(v1.Insert<3>(42.0f), Math::Vector4( 1.0f,  2.0f,  3.0f, 42.0f));

	ADDTHIS(v1.InsertZero<0>(), Math::Vector4(0.0f, 2.0f, 3.0f, 4.0f));
	ADDTHIS(v1.InsertZero<1>(), Math::Vector4(1.0f, 0.0f, 3.0f, 4.0f));
	ADDTHIS(v1.InsertZero<2>(), Math::Vector4(1.0f, 2.0f, 0.0f, 4.0f));
	ADDTHIS(v1.InsertZero<3>(), Math::Vector4(1.0f, 2.0f, 3.0f, 0.0f));

	ADDTHIS(v1.InsertX(42.0f), Math::Vector4(42.0f,  2.0f,  3.0f,  4.0f));
	ADDTHIS(v1.InsertY(42.0f), Math::Vector4( 1.0f, 42.0f,  3.0f,  4.0f));
	ADDTHIS(v1.InsertZ(42.0f), Math::Vector4( 1.0f,  2.0f, 42.0f,  4.0f));
	ADDTHIS(v1.InsertW(42.0f), Math::Vector4( 1.0f,  2.0f,  3.0f, 42.0f));

	ADDTHIS(v2.X000(), Math::Vector4(10.0f, 0.0f, 0.0f, 0.0f));
	ADDTHIS(v2.XY00(), Math::Vector4(10.0f, 20.0f, 0.0f, 0.0f));
	ADDTHIS(v2.XY01(), Math::Vector4(10.0f, 20.0f, 0.0f, 1.0f));
	ADDTHIS(v2.XY10(), Math::Vector4(10.0f, 20.0f, 1.0f, 0.0f));
	ADDTHIS(v2.XY11(), Math::Vector4(10.0f, 20.0f, 1.0f, 1.0f));
	ADDTHIS(v2.XYZ0(), Math::Vector4(10.0f, 20.0f, 30.0f, 0.0f));
	ADDTHIS(v2.XYZ1(), Math::Vector4(10.0f, 20.0f, 30.0f, 1.0f));

	ADDTHIS(v1.XXZZ(), Math::Vector4(1.0f, 1.0f, 3.0f, 3.0f));
	ADDTHIS(v1.YYWW(), Math::Vector4(2.0f, 2.0f, 4.0f, 4.0f));

	ADDTHIS(v1.XXYY(), Math::Vector4(1.0f, 1.0f, 2.0f, 2.0f));
	ADDTHIS(v1.XYXY(), Math::Vector4(1.0f, 2.0f, 1.0f, 2.0f));

	ADDTHIS(v1.ZZWW(), Math::Vector4(3.0f, 3.0f, 4.0f, 4.0f));
	ADDTHIS(v1.ZWZW(), Math::Vector4(3.0f, 4.0f, 3.0f, 4.0f));

	ADDTHIS(v1.ZCWD(v2), Math::Vector4(3.0f, 30.0f, 4.0f, 40.0f));
	ADDTHIS(v1.XYAB(v2), Math::Vector4(1.0f, 2.0f, 10.0f, 20.0f));
	ADDTHIS(v1.ZWCD(v2), Math::Vector4(3.0f, 4.0f, 30.0f, 40.0f));
	ADDTHIS(v1.XAYB(v2), Math::Vector4(1.0f, 10.0f, 2.0f, 20.0f));

	ADDTHIS(v1.XXXX(), Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	ADDTHIS(v1.YYYY(), Math::Vector4(2.0f, 2.0f, 2.0f, 2.0f));
	ADDTHIS(v1.ZZZZ(), Math::Vector4(3.0f, 3.0f, 3.0f, 3.0f));
	ADDTHIS(v1.WWWW(), Math::Vector4(4.0f, 4.0f, 4.0f, 4.0f));

	ADDTHIS(v1.WZYX(), Math::Vector4(4.0f, 3.0f, 2.0f, 1.0f));
	ADDTHIS(v1.ZWXY(), Math::Vector4(3.0f, 4.0f, 1.0f, 2.0f));

	ADDTHIS(v1.Min(v2), v1);
	ADDTHIS(v1.Max(v2), v2);

	ADDTHIS(v2.Min(v1), v1);
	ADDTHIS(v2.Max(v1), v2);

	ADDTHIS(v3.Min(), -20.0f);
	ADDTHIS(v3.Max(),  40.0f);
	ADDTHIS(v3.Sum(),     (-10.0f + -20.0f + 30.0f + 40.0f));
	ADDTHIS(v3.Product(), (-10.0f * -20.0f * 30.0f * 40.0f));
	ADDTHIS(v3.Mean(), (-10.0f + -20.0f + 30.0f + 40.0f) * 0.25f);

	ADDTHIS(v3.Abs(),              Math::Vector4(10.0f,  20.0f, 30.0f, 40.0f));
	ADDTHIS(v3.Sign(),             Math::Vector4(-1.0f,  -1.0f,  1.0f,  1.0f));
	ADDTHIS(v3.Half(),             Math::Vector4(-5.0f,          -10.0f, 15.0f, 20.0f));
	ADDTHIS(v3.Reciprocal(),       Math::Vector4(1.0f /          -10.0f,  1.0f /          -20.0f,  1.0f /           30.0f,  1.0f /           40.0f));
	ADDTHISNR(v3.ReciprocalApprox());
	ADDTHIS(v2.Sqrt(),             Math::Vector4(       std::sqrt(10.0f),        std::sqrt(20.0f),        std::sqrt(30.0f),        std::sqrt(40.0f)));
	ADDTHIS(v2.RSqrt(),            Math::Vector4(1.0f / std::sqrt(10.0f), 1.0f / std::sqrt(20.0f), 1.0f / std::sqrt(30.0f), 1.0f / std::sqrt(40.0f)));
	ADDTHISNR(v2.RSqrtApprox());

	ADDTHIS(v1.To(v3), Math::Vector4(-10.0f - 1.0f, -20.0f - 2.0f, 30.0f - 3.0f, 40.0f - 4.0f));

	ADDTHIS(v1.Distance(v3), 51.283527f);
	ADDTHIS(v1.DistanceSquared(v3), 2630.0f);

	ADDTHIS(v1.Length(), 5.477226f);
	ADDTHIS(v1.LengthSquared(), 30.0f);

	ADDTHIS(v1.Normalized(), Math::Vector4(0.182574f, 0.365148f, 0.547723f, 0.730297f));

	ADDTHIS(v1.Angle(v3), 0.841069f);
	ADDTHIS(v1.Dot(v3), 200.0f);

	ADDTHIS  (v1.Power      (3.42f), Math::Vector4(1.0f, 10.703421f, 42.830658f, 114.563225f));
	ADDTHISNR(v1.PowerApprox(3.42f));

	ADDTHIS(v1.IsFacing(v3), 1);

	ADDTHIS(vn.HasNaN(), 1);
	ADDTHIS(v1.HasNaN(), 0);

	ADDTHIS(vip.HasPositiveInfinity(), 1);
	ADDTHIS(v1.HasPositiveInfinity(), 0);

	ADDTHIS(vin.HasNegativeInfinity(), 1);
	ADDTHIS(v1.HasNegativeInfinity(), 0);

	ADDTHIS(Math::Vector4(NAN, NAN, NAN, NAN).IsNaN(), 1);
	ADDTHIS(v1.IsNaN(), 0);

	ADDTHIS(Math::Vector4(INFINITY, INFINITY, INFINITY, INFINITY).IsPositiveInfinity(), 1);
	ADDTHIS(v1.IsPositiveInfinity(), 0);

	ADDTHIS(Math::Vector4(-INFINITY, -INFINITY, -INFINITY, -INFINITY).IsNegativeInfinity(), 1);
	ADDTHIS(v1.IsNegativeInfinity(), 0);

	ADDTHIS( v1.IsUnbounded(), 0);
	ADDTHIS( vn.IsUnbounded(), 0);
	ADDTHIS(vip.IsUnbounded(), 1);
	ADDTHIS(vin.IsUnbounded(), 1);

	rs.push_back(MathTestResult::New("Compare<Math::CMP::AllEQ>A", v1.Compare<Math::CMP::AllEQ>(v1), 1));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::AllEQ>B", v1.Compare<Math::CMP::AllEQ>(vs), 0));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::AnyEQ>A", v1.Compare<Math::CMP::AnyEQ>(vs), 1));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::AnyEQ>B", v1.Compare<Math::CMP::AnyEQ>(v2), 0));

	rs.push_back(MathTestResult::New("Compare<Math::CMP::AllNEQ>A", v1.Compare<Math::CMP::AllNEQ>(v2), 1));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::AllNEQ>B", v1.Compare<Math::CMP::AllNEQ>(v1), 0));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::AnyNEQ>A", v1.Compare<Math::CMP::AnyNEQ>(vs), 1));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::AnyNEQ>B", v1.Compare<Math::CMP::AnyNEQ>(v1), 0));

	rs.push_back(MathTestResult::New("Compare<Math::CMP::AllWE>A", v1.Compare<Math::CMP::AllWE>(v1), 1));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::AllWE>B", v1.Compare<Math::CMP::AllWE>(vs), 0));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::AnyWE>A", v1.Compare<Math::CMP::AnyWE>(vs), 1));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::AnyWE>B", v1.Compare<Math::CMP::AnyWE>(v2), 0));

	rs.push_back(MathTestResult::New("Compare<Math::CMP::AllNWE>A", v1.Compare<Math::CMP::AllNWE>(v2), 1));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::AllNWE>B", v1.Compare<Math::CMP::AllNWE>(v1), 0));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::AnyNWE>A", v1.Compare<Math::CMP::AnyNWE>(vs), 1));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::AnyNWE>B", v1.Compare<Math::CMP::AnyNWE>(v1), 0));

	rs.push_back(MathTestResult::New("Compare<Math::CMP::AllGEQ>A", v2.Compare<Math::CMP::AllGEQ>(v1), 1));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::AllGEQ>B", vs.Compare<Math::CMP::AllGEQ>(v1), 1));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::AnyGEQ>A", vs.Compare<Math::CMP::AnyGEQ>(v1), 1));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::AnyGEQ>B", v1.Compare<Math::CMP::AnyGEQ>(v2), 0));

	rs.push_back(MathTestResult::New("Compare<Math::CMP::AllGT>A", v2.Compare<Math::CMP::AllGT>(v1), 1));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::AllGT>B", vs.Compare<Math::CMP::AllGT>(v1), 0));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::AnyGT>A", vs.Compare<Math::CMP::AnyGT>(v1), 1));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::AnyGT>B", v1.Compare<Math::CMP::AnyGT>(v2), 0));

	rs.push_back(MathTestResult::New("Compare<Math::CMP::AllLEQ>A", v1.Compare<Math::CMP::AllLEQ>(v2), 1));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::AllLEQ>B", v1.Compare<Math::CMP::AllLEQ>(vs), 1));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::AnyLEQ>A", v1.Compare<Math::CMP::AnyLEQ>(vs), 1));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::AnyLEQ>B", v2.Compare<Math::CMP::AnyLEQ>(v1), 0));

	rs.push_back(MathTestResult::New("Compare<Math::CMP::AllLT>A", v1.Compare<Math::CMP::AllLT>(v2), 1));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::AllLT>B", v1.Compare<Math::CMP::AllLT>(vs), 0));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::AnyLT>A", v1.Compare<Math::CMP::AnyLT>(vs), 1));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::AnyLT>B", v2.Compare<Math::CMP::AnyLT>(v1), 0));

	rs.push_back(MathTestResult::New("Compare<Math::CMP::Ordinal>A", v1.Compare<Math::CMP::Ordinal>(v2), 1));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::Ordinal>B", vs.Compare<Math::CMP::Ordinal>(v1), 0));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::Ordinal>A", vs.Compare<Math::CMP::Ordinal>(v2), 1));
	rs.push_back(MathTestResult::New("Compare<Math::CMP::Ordinal>B", v2.Compare<Math::CMP::Ordinal>(v1), 0));

	rs.push_back(MathTestResult::New("ClampA", Math::Vector4(10.0f).Clamp(Math::Vector4( 5.0f), Math::Vector4( 8.0f)), Math::Vector4( 8.0f)));
	rs.push_back(MathTestResult::New("ClampB", Math::Vector4(10.0f).Clamp(Math::Vector4(12.0f), Math::Vector4(16.0f)), Math::Vector4(12.0f)));
	rs.push_back(MathTestResult::New("ClampC", Math::Vector4(10.0f).Clamp(Math::Vector4( 9.0f), Math::Vector4(11.0f)), Math::Vector4(10.0f)));

	rs.push_back(MathTestResult::New("MultiplyAdd",v1.MultiplyAdd(v2, v3), Math::Vector4( 0.0f, 20.0f, 120.0f, 200.0f)));
	rs.push_back(MathTestResult::New("MultiplySub",v1.MultiplySub(v2, v3), Math::Vector4(20.0f, 60.0f,  60.0f, 120.0f)));

	rs.push_back(MathTestResult::New("LerpA", v1.Lerp(v2,               0.5f) , Math::Vector4(5.5f, 11.0f, 16.5f, 22.0f)));
	rs.push_back(MathTestResult::New("LerpB", v1.Lerp(v2, Math::Vector4(0.5f)), Math::Vector4(5.5f, 11.0f, 16.5f, 22.0f)));

	rs.push_back(MathTestResult::New("SmoothStepA", v1.SmoothStep(v2,               0.5f) , Math::Vector4(5.5f, 11.0f, 16.5f, 22.0f)));
	rs.push_back(MathTestResult::New("SmoothStepB", v1.SmoothStep(v2, Math::Vector4(0.5f)), Math::Vector4(5.5f, 11.0f, 16.5f, 22.0f)));

	ADDTHIS(vd.Round_Near(),  Math::Vector4(-4.0f, -2.0f, 0.0f, 5.0f));
	ADDTHIS(vd.Round_Floor(), Math::Vector4(-5.0f, -2.0f, 0.0f, 4.0f));
	ADDTHIS(vd.Round_Ceil(),  Math::Vector4(-4.0f, -1.0f, 1.0f, 5.0f));
	ADDTHIS(vd.Round_Trunc(), Math::Vector4(-4.0f, -1.0f, 0.0f, 4.0f));

	ADDTHIS(vd.FMod(1.0f),                Math::Vector4(-0.2f, -0.6f, 0.1f, 0.6f));
	ADDTHIS(vd.FMod(Math::Vector4(2.0f)), Math::Vector4(-0.2f, -1.6f, 0.1f, 0.6f));

	ADDTHIS(v1[0], 1_f);
	ADDTHIS(v1[1], 2_f);
	ADDTHIS(v1[2], 3_f);
	ADDTHIS(v1[3], 4_f);

	ADDTHIS(v1.Sin(),   Math::Vector4(0.841471f,  0.909297f,  0.141120f,  -0.756802f));
	ADDTHIS(v1.Cos(),   Math::Vector4(0.540302f, -0.416147f, -0.989992f,  -0.653644f));
	ADDTHIS(v1.Tan(),   Math::Vector4(1.557408f, -2.185040f, -0.142547f,   1.157821f));

	ADDTHIS(vds.ASin(), Math::Vector4(0.433445f,  0.160691f, -0.100167f,  -0.477995f));
	ADDTHIS(vds.ACos(), Math::Vector4(1.137351f,  1.410106f,  1.670964f,   2.048792f));
	ADDTHIS(vds.ATan(), Math::Vector4(0.397628f,  0.158655f, -0.099669f,  -0.431139f));

	ADDTHIS(v1.SinH(),  Math::Vector4(1.175201f,  3.626860f,  10.017875f, 27.289917f));
	ADDTHIS(v1.CosH(),  Math::Vector4(1.543081f,  3.762196f,  10.067662f, 27.308233f));
	ADDTHIS(v1.TanH(),  Math::Vector4(0.761594f,  0.964028f,   0.995055f,  0.999329f));

	ADDTHIS(Math::Vector4::Zero(),             Math::Vector4( 0.0f,  0.0f,  0.0f,  0.0f));
	ADDTHIS(Math::Vector4::One(),              Math::Vector4( 1.0f,  1.0f,  1.0f,  1.0f));
	ADDTHIS(Math::Vector4::NegOne(),           Math::Vector4(-1.0f, -1.0f, -1.0f, -1.0f));
	ADDTHIS(Math::Vector4::UnitX(),            Math::Vector4( 1.0f,  0.0f,  0.0f,  0.0f));
	ADDTHIS(Math::Vector4::UnitY(),            Math::Vector4( 0.0f,  1.0f,  0.0f,  0.0f));
	ADDTHIS(Math::Vector4::UnitZ(),            Math::Vector4( 0.0f,  0.0f,  1.0f,  0.0f));
	ADDTHIS(Math::Vector4::UnitW(),            Math::Vector4( 0.0f,  0.0f,  0.0f,  1.0f));
	ADDTHIS(Math::Vector4::UnitXW(),           Math::Vector4( 1.0f,  0.0f,  0.0f,  1.0f));
	ADDTHIS(Math::Vector4::UnitYW(),           Math::Vector4( 0.0f,  1.0f,  0.0f,  1.0f));
	ADDTHIS(Math::Vector4::UnitZW(),           Math::Vector4( 0.0f,  0.0f,  1.0f,  1.0f));
	ADDTHIS(Math::Vector4::NegUnitX(),         Math::Vector4(-1.0f,  0.0f,  0.0f,  0.0f));
	ADDTHIS(Math::Vector4::NegUnitY(),         Math::Vector4( 0.0f, -1.0f,  0.0f,  0.0f));
	ADDTHIS(Math::Vector4::NegUnitZ(),         Math::Vector4( 0.0f,  0.0f, -1.0f,  0.0f));
	ADDTHIS(Math::Vector4::NegUnitW(),         Math::Vector4( 0.0f,  0.0f,  0.0f, -1.0f));
	ADDTHIS(Math::Vector4::PositiveInfinity(), Math::Vector4(INFINITY, INFINITY, INFINITY, INFINITY));
	ADDTHIS(Math::Vector4::NegativeInfinity(), Math::Vector4(-INFINITY, -INFINITY, -INFINITY, -INFINITY));
	ADDTHISNR(Math::Vector4::NaN());
	ADDTHISNR(Math::Vector4::FFFFFFFF());

	ADDTHIS(-v1, Math::Vector4(-1.0f, -2.0f, -3.0f, -4.0f));
	ADDTHISNR(~v1);

	ADDTHIS(v1 + v2, Math::Vector4(11.0f,  22.0f,  33.0f,  44.0f));
	ADDTHIS(v1 - v2, Math::Vector4(-9.0f, -18.0f, -27.0f, -36.0f));
	ADDTHIS(v1 * v2, Math::Vector4(10.0f,  40.0f,  90.0f, 160.0f));
	ADDTHIS(v1 / v2, Math::Vector4( 0.1f,   0.1f,   0.1f,   0.1f));
	ADDTHISNR(v1 & v2);
	ADDTHISNR(v1 | v2);
	ADDTHISNR(v1 ^ v2);

	ADDTHIS(v1 + 42.0f, Math::Vector4( 43.0f,       44.0f,       45.0f,       46.0f));
	ADDTHIS(v1 - 42.0f, Math::Vector4(-41.0f,      -40.0f,      -39.0f,      -38.0f));
	ADDTHIS(v1 * 42.0f, Math::Vector4( 42.0f,       84.0f,      126.0f,      168.0f));
	ADDTHIS(v1 / 42.0f, Math::Vector4(  0.023810f,   0.047619f,   0.071429f,   0.095238f));
	ADDTHISNR(v1 & 42.0f);
	ADDTHISNR(v1 | 42.0f);
	ADDTHISNR(v1 ^ 42.0f);

	return rs;
}