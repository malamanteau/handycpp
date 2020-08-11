#include "Handy.hpp"

/*
int main()
{
//	float offset = 1;         // 1 digit
	float offset = 0.1;       // 2 digit
//	float offset = 0.01;      // 3 digit
//	float offset = 0.001;     // 4 digit
//	float offset = 0.0001;    // 5 digit
//	float offset = 0.00001;   // 6 digit
//	float offset = 0.000001;  // 7 digit
//	float offset = 0.0000001; // 8 digit

	//for (auto iter : Handy::range(0, 10))
	{

		for (auto numULPS : Handy::range(419429, 419440).step(1))
		{
			float a = Handy::Pi<float>;
			float b = a - offset;

			std::cout << "Offset: " << offset << " ULPS: " << std::to_string(numULPS) << " " << (Handy::WithinEpsilon(a, b, numULPS) ? "Y" : "N" ) << std::endl;
		}

		offset /= 10;
	}


	return 0;
}
*/

int main()
{
//	double offset = 1;         // 1 digit
//	double offset = 0.1;       // 2 digit
//	double offset = 0.01;      // 3 digit
//	double offset = 0.001;     // 4 digit
//	double offset = 0.0001;    // 5 digit
//	double offset = 0.00001;   // 6 digit
//	double offset = 0.000001;  // 7 digit
//	double offset = 0.0000001; // 8 digit
//	double offset = 0.00000001; // 9 digit
	double offset = 0.000000001; // 10 digit
//	double offset = 0.0000000001; // 11 digit
//	double offset = 0.00000000001; // 12 digit
//	double offset = 0.000000000001; // 13 digit
//	double offset = 0.0000000000001; // 14 digit
//	double offset = 0.00000000000001; // 15 digit
//	double offset = 0.000000000000001; // 16 digit

	//for (auto iter : Handy::range(0, 10))
	{

		for (auto numULPS : Handy::range(2251700, 2251801).step(1))
		{
			double a = Handy::Pi<double>;
			double b = a - offset;

			std::cout << "Offset: " << offset << " ULPS: " << std::to_string(numULPS) << " " << (Handy::WithinEpsilon(a, b, numULPS) ? "Y" : "N" ) << std::endl;
		}

		offset /= 10;
	}


	return 0;
}