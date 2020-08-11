
#include "../../../Handy.hpp"

// This class is exported from the TestLib.dll
class IN_DLL TestClass
{
public:
	TestClass();

};

struct TestStruct
{
	bool Reply;
};

EXTERN_C_BEGIN

	IN_DLL extern int32_t TestInt;

	IN_DLL int32_t TestFunction(int32_t i);

	IN_DLL TestStruct TestFunction2(TestStruct ts);

EXTERN_C_END
