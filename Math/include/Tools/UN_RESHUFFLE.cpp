
/// See ../../License.txt for license info.

/////////////////////////////////
// FOR USE AT http://cpp.sh/   //
/////////////////////////////////

// Example program
#include <iostream>
#include <string>
#include <array>
#include <emmintrin.h>

//#define _MM_SHUFFLE(fp3,fp2,fp1,fp0) (((fp3) << 6) | ((fp2) << 4) | ((fp1) << 2) | ((fp0)))
#define RESHUFFLE(A1, A2, A3, A4) _MM_SHUFFLE(A4, A3, A2, A1)

struct alignas(16) Vector4 
{
	union
	{
		struct // Allowed in MSVC and GCC, not sure about Clang or ICC
		{
			float X, Y, Z, W;
		};

		__m128 Native;

		std::array<float, 4> Array;
	};
};

void UN_RESHUFFLE(uint32_t num) // RESHUFFLE(fp0,fp1,fp2,fp3) = num;
{
	uint32_t fp0 = (num     ) & 0b11;
	uint32_t fp1 = (num >> 2) & 0b11;
	uint32_t fp2 = (num >> 4) & 0b11;
	uint32_t fp3 = (num >> 6) & 0b11;

	std::cout 
		<< num << " == " << RESHUFFLE(fp0, fp1, fp2, fp3) << std::endl
		<< "RESHUFFLE(" << fp0 << ", " << fp1 << ", " << fp2 << ", " << fp3 << ")" << std::endl;
}

int main()
{
	UN_RESHUFFLE(_MM_SHUFFLE(3, 0, 2, 1));

	Vector4 v;
	v.X = 1.0f; v.Y = 2.0f; v.Z = 3.0f; v.W = 4.0f;

	v.Native = _mm_shuffle_ps(v.Native, v.Native, _MM_SHUFFLE(3, 0, 2, 1));

	std::cout << "SHUFFLE_RESULT(" << v.X << ", " << v.Y << ", " << v.Z << ", " << v.W << ")" << std::endl;
}
