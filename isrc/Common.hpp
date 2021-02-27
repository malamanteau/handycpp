
/// ========================================================================
/// UNLICENSE
/// 
/// This is free and unencumbered software released into the public domain.
/// Anyone is free to copy, modify, publish, use, compile, sell, or
/// distribute this software, either in source code form or as a compiled
/// binary, for any purpose, commercial or non-commercial, and by any
/// means.
///
/// In jurisdictions that recognize copyright laws, the author or authors
/// of this software dedicate any and all copyright interest in the
/// software to the public domain. We make this dedication for the benefit
/// of the public at large and to the detriment of our heirs and
/// successors. We intend this dedication to be an overt act of
/// relinquishment in perpetuity of all present and future rights to this
/// software under copyright law.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
/// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
/// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
/// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
/// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
/// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
/// OTHER DEALINGS IN THE SOFTWARE.
///
/// For more information, please refer to <http://unlicense.org/>
/// ========================================================================

#pragma once

#include <algorithm>
#include <atomic>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <mutex>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <thread>

#include "../Handy.hpp"

#if defined IS_WINDOWS && !defined(_WIN32_WINNT)
	#define _WIN32_WINNT _WIN32_WINNT_VISTA
#endif

#if defined IS_LINUX || defined IS_OSX
	#include <unistd.h>
#elif defined IS_WINDOWS
	#include <windows.h>
	#include <io.h>
	#include <memory>

	// Only defined in windows 10 onwards, redefining in lower windows since it
	// doesn't gets used in lower versions
	// https://docs.microsoft.com/en-us/windows/console/getconsolemode
	#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
		#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
	#endif

#endif

#ifdef _WIN32
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#include <windows.h>
	#include <io.h>
	#define $win(...) __VA_ARGS__
	#define $welse(...)
#else
	#include <dlfcn.h>
	#include <unistd.h>
	#include <sys/ioctl.h>
	#define $win(...)
	#define $welse(...) __VA_ARGS__
#endif

#ifdef _MSC_VER
	#define $msvc(...)  __VA_ARGS__
	#define $melse(...)
#else
	#define $msvc(...)
	#define $melse(...)  __VA_ARGS__
#endif

#if defined IS_WINDOWS
	#include <objbase.h>
#elif defined IS_APPLE
	#include <CoreFoundation/CFUUID.h>
#elif defined IS_NIX
	#include <uuid/uuid.h>
#endif
