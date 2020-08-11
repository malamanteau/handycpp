
#include "TestLib.hpp"

EXTERN_C_BEGIN
	IN_DLL int32_t TestInt = 0; // This is an example of an exported variable

	IN_DLL int32_t TestFunction(int32_t i) // This is an example of an exported function.
	{
		return i + 1;
	}

	IN_DLL TestStruct TestFunction2(TestStruct ts)
	{
		ts.Reply = !ts.Reply;
		return ts;
	}
EXTERN_C_END

TestClass::TestClass() // This is the constructor of a class that has been exported. See TestLib.h for the class definition
{
	return;
}

/// Defines the entry point for the DLL application.
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

