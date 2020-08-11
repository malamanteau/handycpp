
/// See ../License.txt for license info.

#pragma once

#include "../../Handy.hpp"

struct MathTestResult
{
	std::string Name;
	std::string Result;
	std::string Expected;

	MathTestResult(std::string name, std::string result, std::string expected)
		: Name(name),
		  Result(result),
		  Expected(expected) { }

	MathTestResult(std::string name, std::string result)
		: Name(name),
		  Result(result),
		  Expected("") { }

	template <typename TResult, typename TExpected>
	static MathTestResult New(char const * name, TResult result, TExpected expected)
	{
		return MathTestResult(name, std::to_string(result), std::to_string(expected));
	}

	template <typename TResult>
	static MathTestResult New(char const * name, TResult result)
	{
		return MathTestResult(name, std::to_string(result));
	}
};

namespace HANDYMATH_NS
{
	std::vector<MathTestResult> GetTestsVector4();
}
