
/// See ../License.txt for license info.

#pragma once

#include "Tests.hpp"

#define ADDTHIS(x, ...) rs.push_back(MathTestResult::New( #x, x , __VA_ARGS__))
#define ADDTHISNR(x)    rs.push_back(MathTestResult::New( #x, x ))