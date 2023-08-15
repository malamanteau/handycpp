#pragma once

#include "HandyBase.hpp"
#include "HandyCompat.hpp"
#include "HandyEncoding.hpp"
#include "HandyGuid.hpp"
#include "HandyHash.hpp"
#include "HandyLoader.hpp"
#include "HandyMemory.hpp"
#include "HandyPIMPL.hpp"
#include "HandyConsole.hpp"
#include "HandyRange.hpp"
#include "HandyResult.hpp"
#include "HandySerDe.hpp"
#include "HandyFile.hpp"
#include "HandyMMFile.hpp"
#include "HandyString.hpp"
#include "HandySystemInfo.hpp"
#include "HandyThreadUtils.hpp"
#include "HandyTryGet.hpp"
#include "HandyUtils.hpp"

#if !defined HANDY_NO_DEQUE
	#include "HandyDeque.hpp"
#endif
/// NOTE  : For best performance, it is highly recommended to 
///         enable "fastcall aka /Gv" and "/arch:SSE2" flags for 
///         the best possible performance.
///
/// NOTE 2: If you're using C++, /Gv does not apply by default to 
///         member functions. You'll have to annotate them 
///         manually with either VECTORCALL or FORCEINLINE. 
///
/// NOTE 3: Methods that take a math vector type should be 
///         FORCEINLINE iff (if and only if) taken by const-ref, 
///         or VECTORCALL iff (if and only if) taken by value.

#if !defined HANDY_NO_MATH_NS
	#include "HandyMath.hpp"
#endif

/// This should be manually included in your main file. It's a 
/// lot of code that you're only going to use in just one place!
//#include "HandyArgs.hpp"

/// These have to be included individually:
//#include "HandyCalamity.hpp"
//#include "HandyPacket.hpp"
//#include "HandySHA1.hpp"
//#include "stb_image.h" <- the source of is is already compiled as part of Handy.cpp
