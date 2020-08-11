
/// See ../../License.txt for license info.

#pragma once

#include <cmath>
#include <vector>

#include "../Core/Base.hpp"

namespace HANDYMATH_NS
{
	FORCEINLINE uint64_t Factorial(uint64_t n)             { return (n == 1_u64 || n == 0_u64) ? 1_u64 : Factorial(n - 1_u64) * n; }
	FORCEINLINE uint64_t NChooseK (uint64_t n, uint64_t k) { return Factorial(n) / (Factorial(k) * Factorial(n - k)); }

	FORCEINLINE double Smoothstep(double x, int order) 
	{
		if (x <= 0.0) return 0.0;
		if (x >= 1.0) return 1.0;
		switch (order) 
		{
			case 0: return x;
			case 1: return   -2.0 * std::pow(x, 3) +    3.0 * std::pow(x, 2);
			case 2: return    6.0 * std::pow(x, 5) -   15.0 * std::pow(x, 4) +   10.0 * std::pow(x,3);
			case 3: return  -20.0 * std::pow(x, 7) +   70.0 * std::pow(x, 6) -   84.0 * std::pow(x,5) +   35.0 * std::pow(x,4);
			case 4: return   70.0 * std::pow(x, 9) -  315.0 * std::pow(x, 8) +  540.0 * std::pow(x,7) -  420.0 * std::pow(x,6) +  126.0 * std::pow(x,5);
			case 5: return -252.0 * std::pow(x,11) + 1386.0 * std::pow(x,10) - 3080.0 * std::pow(x,9) + 3465.0 * std::pow(x,8) - 1980.0 * std::pow(x,7) + 462.0 * std::pow(x,6);
			default: 
				double sum = 0.0;
				for (int n = 0; n <= order; n++) 
				{
					double sign = 1.0 - 2.0 * double(n % 2); //This is -1.0^n
					sum += sign * double(NChooseK(order + n, n) * NChooseK(2 * order + 1, order - n)) * std::pow(x, order + n + 1);
				}
				return sum;
		}
	}
}