
#include <iostream>
#include <string>
#include <cstdint>

#include "../../Handy.hpp"

#include "TestLib/TestLib.hpp"

class TestLibAccess : Handy::Platform::Library
{
public:
	TestLibAccess()
		: Handy::Platform::Library(Handy::Platform::Paths::ThisExecutableDirectory() / "TestLib.dll") { }

	VARIABLEPTRDEF(TestInt, int32_t);

	CDECLDEF(TestFunction, int32_t(int32_t));
	CDECLDEF(TestFunction2, TestStruct(TestStruct));
};

int main()
{
	TestLibAccess tla;

	std::cout << std::endl << std::to_string(*tla.TestInt) << std::endl;
	std::cout << std::endl << std::to_string(tla.TestFunction(41)) << std::endl;
	std::cout << std::endl << std::to_string(tla.TestFunction2(TestStruct { false }).Reply) << std::endl;
};

#include "../HandyCpp/Handy.cpp"