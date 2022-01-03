
/// =============================================================================
/// Poshlib: http://poshlib.hookatooka.com/poshlib/trac.cgi
///
/// LICENSE - BSD 2-Clause "Simplified" License 
///     (https://opensource.org/licenses/BSD-2-Clause)
/// 
/// BSD 2-Clause License
/// 
/// Copyright (c) 2004-2006, Brian Hook
/// All rights reserved.
/// 
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
/// 
/// * Redistributions of source code must retain the above copyright notice, this
///   list of conditions and the following disclaimer.
/// 
/// * Redistributions in binary form must reproduce the above copyright notice,
///   this list of conditions and the following disclaimer in the documentation
///   and/or other materials provided with the distribution.
/// 
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
/// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
/// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
/// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
/// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
/// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/// =============================================================================

/// =============================================================================
/// googletest (https://github.com/google/googletest)
///
/// LICENSE: 3-Clause BSD License
///
/// Copyright 2005, Google Inc.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are
/// met:
///
///     * Redistributions of source code must retain the above copyright
/// notice, this list of conditions and the following disclaimer.
///     * Redistributions in binary form must reproduce the above
/// copyright notice, this list of conditions and the following disclaimer
/// in the documentation and/or other materials provided with the
/// distribution.
///     * Neither the name of Google Inc. nor the names of its
/// contributors may be used to endorse or promote products derived from
/// this software without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
/// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
/// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
/// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
/// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
/// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
/// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
/// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
/// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
/// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/// =============================================================================

/// =============================================================================
/// nvidia-mesh-tools
/// https://code.google.com/p/nvidia-mesh-tools
/// https://github.com/castano/nvidia-mesh-tools/tree/master/src/nvcore
///
/// LICENSE: MIT
/// 
/// Copyright (c) 2009-2017 Ignacio Castaño
/// Copyright (c) 2007-2009 NVIDIA Corporation
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE.
///
/// =============================================================================

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

#if defined _DEBUG && defined _WIN32
	/// If this is a Windows build with the Debug CRT, include crtdbg.h
	#include <crtdbg.h>
	#include <stdlib.h>
#endif

#include <cstdint>
#include <tuple>
#include <algorithm>
#include <type_traits>
#include <string>
#include <initializer_list>
#include <cmath>
#include <complex>
#include <vector>
#include <memory>
#include <string>

#include <stdarg.h> // va_list

using namespace std::literals::string_literals;

#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)

#define Breakable for (bool brRegion = true; brRegion; brRegion = false)

//#define CONCAT_IMPL(x, y) x##y
//#define BREAKABLE_IMPL(x) for (bool CONCAT_IMPL(breakableRegion, x) = true; CONCAT_IMPL(breakableRegion, x); CONCAT_IMPL(breakableRegion, x) = false)
//#define Breakable BREAKABLE_IMPL(__COUNTER__)

#if !defined __APPLE__ && !defined _WIN32
	#include <endian.h>
#endif

#ifdef min
	#pragma message("Detected min macro! Handy does not compile with min/max macros active! Please add a define NOMINMAX and WIN32_LEAN_AND_MEAN to your compiler flags !")
	#error min macro active 
#endif
#ifdef max
	#pragma message("Detected max macro! Handy does not compile with min/max macros active! Please add a define NOMINMAX and WIN32_LEAN_AND_MEAN to your compiler flags !")
	#error max macro active 
#endif

///-----------------------------------------------
/// MACROS/Global constexprs
///

/// -------------
/// CPU Detection
#if defined __ia64 || defined _M_IA64 || defined __ia64__ 
	#define IS_IA64
	#define CPU_STRING "IA64"
#endif

#if defined __PPC__ || defined __POWERPC__  || defined powerpc || defined _POWER || defined __ppc__ || defined __powerpc__ || defined _M_PPC
	#define IS_PPC
	#if defined __powerpc64__
		#define IS_PPC_64
		#define CPU_STRING "PowerPC64"
	#else
		#define IS_PPC_32
		#define CPU_STRING "PowerPC"
	#endif
#endif

#if defined ARM || defined __arm__ || defined _ARM || defined __aarch64__
	#define IS_ARM
	#if defined __aarch64__
		#define IS_ARM_64
		#define CPU_STRING "ARM (64-bit)"
	#else
		#define IS_ARM_32
		#define CPU_STRING "ARM (32-bit or lower)"
	#endif
#endif

#if defined __X86__ || defined __i386__ || defined i386 || defined _M_IX86 || defined __386__ || defined __x86_64__ || defined _M_X64 || defined __amd64 || defined _M_AMD64
	#define IS_X86
	#if defined __x86_64__ || defined _M_X64 || defined __amd64 || defined _M_AMD64
		#define IS_X86_64
		#define IS_X64
		#define CPU_STRING "X86-64"
	#else
		#define IS_X86_32
		#define CPU_STRING "X86-32"
	#endif
#endif

#if defined __EMSCRIPTEN__
	#define IS_WASM
	#define CPU_STRING "WASM/JS-32"
#endif

#if defined IS_X86_64 || defined IS_ARM_64 || defined IS_PPC_64 || defined IS_IA64
	#define IS_64BIT
#endif

#if defined IS_PPC_32 || defined IS_ARM_32 || defined IS_X86_32 || defined IS_WASM
	#define IS_32BIT
#endif

#if defined IS_32BIT && defined IS_64BIT
	#error CPU cannot be both 32 and 64bit, detection error.
#endif 

#if !defined IS_32BIT && !defined IS_64BIT
	#error CPU cannot be neither 32 nor 64bit, detection error.
#endif 

/// -------------
/// OS Detection
#if defined __MINGW32__
	#define IS_MINGW
#endif

#if defined __CYGWIN32__
	#define IS_CYGWIN
#endif

#if defined __EMSCRIPTEN__
	#define IS_BROWSER
	#define IS_EMSCRIPTEN
	#define IS_NIX
	#define OS_STRING "Web Browser/Emscripten"
#elif defined __APPLE__
	#define IS_APPLE
	#include "TargetConditionals.h"
	#if TARGET_IPHONE_SIMULATOR
		#define IS_IOS
		#define IS_IOS_SIMULATOR
		#define OS_STRING "Apple iOS (Simulator)"
	#elif TARGET_OS_IPHONE
		#define IS_IOS
		#define OS_STRING "Apple iOS"
	#elif TARGET_OS_MAC
		#define IS_OSX
		#define IS_NIX
		#define OS_STRING "Apple MacOS X"
	#else
		#define OS_STRING "Unknown Apple platform"
	#endif
#elif defined __ANDROID__
	#define IS_ANDROID
	#define IS_LINUX
	#define IS_NIX
	#define OS_STRING "Android/Linux"
#elif defined __linux__
	#define IS_LINUX
	#define IS_NIX
	#define OS_STRING "Linux"
#elif defined __FreeBSD__
	#define IS_FREEBSD 
	#define IS_NIX
	#define IS_BSD
	#define OS_STRING "FreeBSD"
#elif defined __NetBSD__
	#define IS_NETBSD
	#define IS_NIX
	#define IS_BSD
	#define OS_STRING "NetBSD"
#elif defined __OpenBSD__
	#define IS_OPENBSD
	#define IS_NIX
	#define IS_BSD
	#define OS_STRING "OpenBSD"
#elif defined __sun__ || defined sun || defined __sun || defined __solaris__
	#define IS_SUN_OR_SOLARIS
	#define IS_NIX
	#if defined __SVR4 || defined __svr4__ || defined __solaris__
		#define IS_SOLARIS
		#define OS_STRING "Solaris"
	#else
		#define IS_SUNOS
		#define OS_STRING "SunOS"
	#endif
#elif defined __sgi__ || defined sgi || defined __sgi
	#define IS_IRIX
	#define IS_NIX
	#define OS_STRING "Irix"
#elif defined __hpux__ || defined __hpux
	#define IS_HPUX
	#define IS_NIX
	#define OS_STRING "HP-UX"
#elif defined _AIX
	#define IS_AIX
	#define IS_NIX
	#define OS_STRING "AIX"
#elif defined __unix__ // all unices not caught above
	#define IS_UNIX
	#define IS_NIX
	#define OS_STRING "Generic Unix"
#elif defined _POSIX_VERSION
	#define IS_POSIX
	#define IS_NIX
	#define OS_STRING "Generic Posix"
#elif defined GO32 && defined DJGPP && defined __MSDOS__
	#define IS_GO32
	#define OS_STRING "GO32/MS-DOS"
// NOTE: make sure you use /bt=DOS if compiling for 32-bit DOS, otherwise Watcom assumes host=target
#elif defined __WATCOMC__  && defined __386__ && defined __DOS__
	#define IS_DOS32
	#define OS_STRING "DOS/32-bit"
#elif defined _XBOX || defined _XBOX_VER
	#define IS_XBOX
	#define OS_STRING "XBOX"
#elif defined _WIN32 || defined WIN32 || defined _WIN64 || defined WIN64 || defined __NT__
	#define IS_WINDOWS

	#if defined _WIN64
		#define IS_WINDOWS64

		#if defined IS_MINGW
			#define OS_STRING "Microsoft Windows (x64 - MINGW)"
		#elif defined IS_CYGWIN
			#define OS_STRING "Microsoft Windows (x64 - CYGWIN)"
		#else
			#define OS_STRING "Microsoft Windows (x64)"
		#endif
	#else
		#define IS_WINDOWS32

		#if defined IS_MINGW
			#define OS_STRING "Microsoft Windows (Win32 - MINGW)"
		#elif defined IS_CYGWIN
			#define OS_STRING "Microsoft Windows (Win32 - CYGWIN)"
		#else
			#define OS_STRING "Microsoft Windows (Win32)"
		#endif
	#endif
#elif defined IS_MINGW
	#define OS_STRING "Microsoft Windows (MINGW - Unknown Arch)"
#elif defined IS_CYGWIN
	#define OS_STRING "Microsoft Windows (CYGWIN - Unknown Arch)"
#else
	#define OS_STRING "Unknown platform"
#endif

/// -------------
/// Compiler Detection

#if defined __ECC || defined __ICC || defined __INTEL_COMPILER
	#define IS_ICC
	#define COMPILER_STRING "Intel C/C++"
#elif defined __clang__
	#define IS_CLANG
	#if defined __EMSCRIPTEN__
		#define COMPILER_STRING "EMCC " STRINGIFY(__EMSCRIPTEN_major__) "." STRINGIFY(__EMSCRIPTEN_minor__) "." STRINGIFY(__EMSCRIPTEN_tiny__) \
			" / Clang " STRINGIFY(__clang_major__) "." STRINGIFY(__clang_minor__) "." STRINGIFY(__clang_patchlevel__)
	#else
		#define COMPILER_STRING "Clang " STRINGIFY(__clang_major__) "." STRINGIFY(__clang_minor__) "." STRINGIFY(__clang_patchlevel__)
	#endif
#elif defined __GNUC__
	#define IS_GNU
	#if defined __GNUG__
		#define IS_GPP
		#define COMPILER_STRING "Gnu G++"
	#else
		#define COMPILER_STRING "Gnu GCC"
	#endif
#elif defined _MSC_VER
	/// https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros

	#define IS_MSVC

	#if _MSC_VER >= 1920
		#define GTEQ_VS2019
		#define GTEQ_VS2017
		#define GTEQ_VS2015
		#define GTEQ_VS2010
		#define GTEQ_VS2005
		#define COMPILER_STRING "MSVC 16.0 (2019) or newer"

	#elif _MSC_VER >= 1910
		#define GTEQ_VS2017
		#define GTEQ_VS2015
		#define GTEQ_VS2010
		#define GTEQ_VS2005
		#define COMPILER_STRING "MSVC 15.0 (2017) or newer"

	#elif _MSC_VER >= 1900
		#define GTEQ_VS2015
		#define GTEQ_VS2010
		#define GTEQ_VS2005
		#define COMPILER_STRING "MSVC 14.0 (2015)"
	#elif _MSC_VER >= 1600
		#define GTEQ_VS2010
		#define GTEQ_VS2005
		#define COMPILER_STRING "MSVC 10.0 (2010)"
	#elif _MSC_VER >= 1400
		#define GTEQ_VS2005
		#define COMPILER_STRING "MSVC 8.0 (2005)"
	#else
		#define COMPILER_STRING "MSVC <8.0 (Pre-2005)"
	#endif

	#if _MSC_VER < 1920
		#define LT_VS2019
	#endif
	#if _MSC_VER < 1915
		#define LT_VS2017_3
	#endif
	#if _MSC_VER < 1910
		#define LT_VS2017
	#endif
	#if _MSC_VER < 1900
		#define LT_VS2015
	#endif
	#if _MSC_VER < 1600
		#define LT_VS2010
	#endif
	#if _MSC_VER < 1400
		#define LT_VS2005
	#endif

#endif

/// -------------
/// Language Detection
#if defined __cplusplus
	#define IS_CPP

	#if defined GTEQ_VS2017
		#define LANGUAGE_STRING "C++(VS2017)"
	#elif defined GTEQ_VS2015
		#define LANGUAGE_STRING "C++(VS2015)"
	#elif defined GTEQ_VS2010
		#define LANGUAGE_STRING "C++(VS2010)"
	#elif defined GTEQ_VS2005
		#define LANGUAGE_STRING "C++(VS2005)"
	#endif

	#if __cplusplus > 201703L
		#define GTEQ_CPP20
		#define GTEQ_CPP17
		#define GTEQ_CPP14
		#define GTEQ_CPP11
		#ifndef LANGUAGE_STRING
			#define LANGUAGE_STRING "C++20"
		#endif
	#elif __cplusplus >= 201703L
		#define GTEQ_CPP17
		#define GTEQ_CPP14
		#define GTEQ_CPP11
		#ifndef LANGUAGE_STRING
			#define LANGUAGE_STRING "C++17"
		#endif
	#elif __cplusplus >= 201402L
		#define GTEQ_CPP14
		#define GTEQ_CPP11
		#ifndef LANGUAGE_STRING
			#define LANGUAGE_STRING "C++14"
		#endif
	#elif __cplusplus >= 201103L
		#define GTEQ_CPP11
		#ifndef LANGUAGE_STRING
			#define LANGUAGE_STRING "C++11"
		#endif
	#else
		#ifndef LANGUAGE_STRING
			#define LANGUAGE_STRING "C++99"
		#endif
	#endif
#else
	#define IS_C
	#if defined __STDC_VERSION__
		#if __STDC_VERSION__ >= 199901L
			#define GTEQ_C99
			#define GTEQ_C90
			#define LANGUAGE_STRING "C (C99)"
		#else
			#define GTEQ_C90
			#define LANGUAGE_STRING "C (C90)"
		#endif
	#elif defined __STDC__
		#define LANGUAGE_STRING "C (C89)"
	#else
		#define LANGUAGE_STRING "C (Unknown Version)"
	#endif
#endif


/// -------------
/// Endianness Detection

// ----------------------------------------------------------------------------
// Try like hell to find a compiler provided endianness macro. Otherwise,
// try to infer endianness.  Basically we just go through the CPUs we know are
// little endian, and assume anything that isn't one of those is big endian.
// As a sanity check, we also do this with operating systems we know are
// little endian, such as Windows.
// DOES NOT HANDLE UNUSUAL BI-ENDIAN ARCHS.
// ----------------------------------------------------------------------------

#if defined __BYTE_ORDER && __BYTE_ORDER == __BIG_ENDIAN
	#define IS_BIG_ENDIAN
#endif

#if defined __BYTE_ORDER && __BYTE_ORDER == __LITTLE_ENDIAN
	#define IS_LITTLE_ENDIAN
#endif

#if defined __BYTE_ORDER__ && __BYTE_ORDER == __ORDER_BIG_ENDIAN__
	#define IS_BIG_ENDIAN
#endif

#if defined __BYTE_ORDER__ && __BYTE_ORDER == __ORDER_LITTLE_ENDIAN__
	#define IS_LITTLE_ENDIAN
#endif

#if defined __BIG_ENDIAN__    || defined __ARMEB__ || defined __THUMBEB__ || defined __AARCH64EB__
	#define IS_BIG_ENDIAN
#endif

#if defined __LITTLE_ENDIAN__ || defined __ARMEL__ || defined __THUMBEL__ || defined __AARCH64EL__
	#define IS_LITTLE_ENDIAN
#endif

#if !defined IS_LITTLE_ENDIAN && !defined IS_BIG_ENDIAN
	#if defined IS_X86 || defined IS_WINDOWS || defined IS_ARM || defined __ORDER_LITTLE_ENDIAN__
		#define IS_LITTLE_ENDIAN
	#else
		#error Could not determine endianness.
	#endif
#endif

#if defined IS_BIG_ENDIAN && defined IS_LITTLE_ENDIAN
	#error Invalid result from endianness test.
#endif

#if defined IS_LITTLE_ENDIAN
	#define ENDIAN_STRING "Little Endian"
#else
	#define ENDIAN_STRING "Big Endian"
#endif

/// -------------
/// Unaligned Access Detection

#if defined IS_EMSCRIPTEN || defined IS_PPC
	#define IS_UNALIGNED_ACCESS_FORBIDDEN
#elif defined IS_X86
	#define IS_UNALIGNED_ACCESS_ALLOWED
#elif defined IS_ARM
	#if defined __TARGET_ARCH_ARM && __TARGET_ARCH_ARM >= 6
		#define IS_UNALIGNED_ACCESS_ALLOWED
	#else
		#define IS_UNALIGNED_ACCESS_FORBIDDEN
	#endif
#else
	#define IS_UNALIGNED_ACCESS_UNKNOWN
#endif

/// ------
/// Fill Unknowns
#if !defined CPU_STRING
	#define CPU_STRING "Unknown CPU"
#endif

#if !defined COMPILER_STRING
	#define COMPILER_STRING "Unknown compiler"
#endif

#if !defined OS_STRING
	#define OS_STRING "Unknown OS"
#endif

#if !defined LANGUAGE_STRING
	#define LANGUAGE_STRING "Unknown version of C/C++"
#endif

/// -------

#if defined IS_GNU || defined IS_CLANG
	#if defined IS_CPP
		#define RESTRICT __restrict
	#elif defined __STDC_VERSION__
		#if __STDC_VERSION__ >= 199901L
			#define RESTRICT restrict
		#endif
	#endif

	#define LIKELY(x)    (__builtin_expect(!!(x), 1))
	#define UNLIKELY(x)  (__builtin_expect(!!(x), 0))
	#define NOTHROW      __attribute__((nothrow))
	#define FORCEINLINE  __attribute__((always_inline)) inline
	#define NORETURN     __attribute__((noreturn))
	#define NOINLINE     __attribute__((noinline))
	#define THREAD_LOCAL __thread
	#define DEPRECATED   __attribute__((deprecated))
	#define FPURE        __attribute__((pure))
	#define FCONST       __attribute__((const))

	// __attribute__((pure)) function attribute
	// Many functions have no effects except to return a value, and
	// their return value depends only on the parameters and global 
	// variables. Functions of this kind can be subject to data flow 
	// analysis and might be eliminated.
	//
	// __attribute__((const)) function attribute
	// Many functions examine only the arguments passed to them, and 
	// have no effects except for the return value. This is a much 
	// stricter class than __attribute__((pure)), because a function is 
	// not permitted to read global memory. If a function is known to 
	// operate only on its arguments then it can be subject to common 
	// sub-expression elimination and loop optimizations.

	#if defined IS_32BIT
		#define CDECL      __attribute__((cdecl))
		#define STDCALL    __attribute__((stdcall))
		#define FASTCALL   __attribute__((fastcall))
	#else
		#define CDECL
		#define STDCALL
		#define FASTCALL	
	#endif
	#define CLRCALL
	#define THISCALL
	#define VECTORCALL

	#define CURRENT_LINE   __LINE__
	#define CURRENT_FUNC   __PRETTY_FUNCTION__
	#define CURRENT_FILE   (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)


#elif defined IS_MSVC
	#ifdef CDECL
		#undef CDECL
	#endif

	#define RESTRICT __restrict

	#define LIKELY(x)   (x)
	#define UNLIKELY(x) (x)
	#define FORCEINLINE  __forceinline
	#define NOTHROW      __declspec(nothrow)
	#define NORETURN     __declspec(noreturn)
	#define NOINLINE     __declspec(noinline)
	#define THREAD_LOCAL __declspec(thread)
	#define DEPRECATED   
	#define FPURE
	#define FCONST
	#define CDECL       __cdecl
	#define CLRCALL     __clrcall
	#define STDCALL     __stdcall
	#define FASTCALL    __fastcall
	#define THISCALL    __thiscall
	#define VECTORCALL  __vectorcall 

	#define CURRENT_LINE   __LINE__
	#define CURRENT_FUNC   __FUNCTION__
	#define CURRENT_FILE   __FILE__

	#ifdef __INTELLISENSE__
		#define IS_INTELLISENSE
	#endif

#else
	#define RESTRICT

	#define LIKELY(x)   (x)
	#define UNLIKELY(x) (x)
	#define FORCEINLINE
	#define NOTHROW
	#define NORETURN
	#define NOINLINE
	#define THREAD_LOCAL
	#define DEPRECATED   
	#define FPURE
	#define FCONST
	#define CDECL
	#define CLRCALL
	#define STDCALL
	#define FASTCALL
	#define THISCALL
	#define VECTORCALL

	#define CURRENT_LINE "UNKNOWN LINE"
	#define CURRENT_FUNC "UNKNOWN FUNC"
	#define CURRENT_FILE "UNKNOWN FILE"

#endif

#ifdef IS_MSVC
	#define DLLIMPORT __declspec(dllimport)
	#define DLLEXPORT __declspec(dllexport)
#elif defined IS_GNU || defined IS_CLANG || defined IS_ICC
	#define DLLIMPORT __attribute__((visibility("default")))
	#define DLLEXPORT __attribute__((visibility("default")))
#else
	#define DLLIMPORT
	#define DLLEXPORT
#endif

#if   defined HANDY_EXPORT_DLL
	#define IN_DLL DLLEXPORT
#elif defined HANDY_IMPORT_DLL
	#define IN_DLL DLLIMPORT
#else
	#define IN_DLL
#endif


#ifdef IS_CPP
	#define EXTERN_C_BEGIN extern "C" {
	#define EXTERN_C_END }
#else
	#define EXTERN_C_BEGIN
	#define EXTERN_C_END
#endif

#if defined __cplusplus_cli
	#define IS_CLI
#endif

///-----------------------------------------------
///Win32 defines that help a lot.
#if defined IS_WINDOWS
	#if _MSC_VER < 1900
		#define snprintf _snprintf
	#endif

	#if _MSC_VER < 1500
		#define vsnprintf _vsnprintf
	#endif

	#if _MSC_VER < 1700
		#define strtoll _strtoi64
		#define strtoull _strtoui64
	#endif

	#define chdir _chdir
	#define getcwd _getcwd 

	#if _MSC_VER < 1800 // Not sure what version introduced this.
		#define va_copy(a, b) (a) = (b)
	#endif

	// Ignore gcc attributes.
	#if defined IS_MSVC
		#define __attribute__(X)
	#endif

	#if !defined __FUNC__
		#define __FUNC__ __FUNCTION__ 
	#endif


	/* // Unwanted VC++ warnings to disable.
	#pragma warning(disable : 4244)     // conversion to float, possible loss of data
	#pragma warning(disable : 4245)     // conversion from 'enum ' to 'unsigned long', signed/unsigned mismatch
	#pragma warning(disable : 4100)     // unreferenced formal parameter
	#pragma warning(disable : 4514)     // unreferenced inline function has been removed
	#pragma warning(disable : 4710)     // inline function not expanded
	#pragma warning(disable : 4127)     // Conditional expression is constant
	#pragma warning(disable : 4305)     // truncation from 'const double' to 'float'
	#pragma warning(disable : 4505)     // unreferenced local function has been removed

	#pragma warning(disable : 4702)     // unreachable code in inline expanded function
	#pragma warning(disable : 4711)     // function selected for automatic inlining
	#pragma warning(disable : 4725)     // Pentium fdiv bug

	#pragma warning(disable : 4786)     // Identifier was truncated and cannot be debugged.

	#pragma warning(disable : 4675)     // resolved overload was found by argument-dependent lookup
	*/

	#pragma warning(1 : 4705)     // Report unused local variables.
	#pragma warning(1 : 4555)     // Expression has no effect.
#endif

///-----------------------------------------------
/// Byte Swaps
///-----------------------------------------------

#if defined IS_WINDOWS
	//#define __bswap_constant_16(x) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8))
	//#define __bswap_constant_32(x) ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) | (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))
	//#define __bswap_constant_64(x)             \n
	//	( (((x) & 0xff00000000000000ull) >> 56) \n
	//	| (((x) & 0x00ff000000000000ull) >> 40) \n
	//	| (((x) & 0x0000ff0000000000ull) >> 24) \n
	//	| (((x) & 0x000000ff00000000ull) >>  8) \n
	//	| (((x) & 0x00000000ff000000ull) <<  8) \n
	//	| (((x) & 0x0000000000ff0000ull) << 24) \n
	//	| (((x) & 0x000000000000ff00ull) << 40) \n
	//	| (((x) & 0x00000000000000ffull) << 56))

	//#define __bswap_16(x) __bswap_constant_16 (x)
	//#define __bswap_32(x) __bswap_constant_32 (x)
	//#define __bswap_64(x) __bswap_constant_64 (x)
	//#define __builtin_bswap16 __bswap_constant_16 (x)
	//#define __builtin_bswap32 __bswap_constant_32 (x)
	//#define __builtin_bswap64 __bswap_constant_64 (x)

	//#define __bswap_constant_16(x) _byteswap_ushort(x)
	//#define __bswap_constant_32(x) _byteswap_ulong (x)
	//#define __bswap_constant_64(x) _byteswap_uint64(x)

	//#define __bswap_16(x) __bswap_constant_16(x)
	//#define __bswap_32(x) __bswap_constant_32(x)
	//#define __bswap_64(x) __bswap_constant_64(x)

	//#define __builtin_bswap16 __bswap_constant_16(x)
	//#define __builtin_bswap32 __bswap_constant_32(x)
	//#define __builtin_bswap64 __bswap_constant_64(x)

#endif

#if defined IS_WINDOWS
	#define bswap_16(x) _byteswap_ushort(x)
	#define bswap_32(x) _byteswap_ulong(x)
	#define bswap_64(x) _byteswap_uint64(x)
#elif defined IS_APPLE
	#include <libkern/OSByteOrder.h>
	#define bswap_16(x) OSSwapInt16(x)
	#define bswap_32(x) OSSwapInt32(x)
	#define bswap_64(x) OSSwapInt64(x)
#elif defined IS_SUN_OR_SOLARIS
	#include <sys/byteorder.h>
	#define bswap_16(x) BSWAP_16(x)
	#define bswap_32(x) BSWAP_32(x)
	#define bswap_64(x) BSWAP_64(x)
#elif defined IS_FREEBSD
	#include <sys/endian.h>
	#define bswap_16(x) bswap16(x)
	#define bswap_32(x) bswap32(x)
	#define bswap_64(x) bswap64(x)
#elif defined IS_OPENBSD
	#include <sys/types.h>
	#define bswap_16(x) swap16(x)
	#define bswap_32(x) swap32(x)
	#define bswap_64(x) swap64(x)
#elif defined IS_NETBSD
	#include <sys/types.h>
	#include <machine/bswap.h>
	#if defined __BSWAP_RENAME && !defined __bswap_32
		#define bswap_32(x) bswap32(x)
		#define bswap_64(x) bswap64(x)
	#endif
#elif __has_include(<byteswap.h>)
	#include <byteswap.h>
#endif

///-----------------------------------------------

#define STATIC_ASSERT(x) static_assert(x, #x STRINGIFY(in __FILE__: __LINE__));

#define DO_STRING_JOIN2(arg1, arg2) arg1 ## arg2
#define DO_STRING_JOIN3(arg1, arg2, arg3) arg1 ## arg2 ## arg3

// String concatenation macros.
#define STRING_JOIN2(arg1, arg2)       DO_STRING_JOIN2(arg1, arg2)
#define STRING_JOIN3(arg1, arg2, arg3) DO_STRING_JOIN3(arg1, arg2, arg3)

/// Why a do/while loop? I'm glad you asked: https://kernelnewbies.org/FAQ/DoWhile0
#if defined IS_MSVC
	#define MULTI_LINE_MACRO_BEGIN do {  
	#define MULTI_LINE_MACRO_END            \
			__pragma(warning(push))         \
			__pragma(warning(disable:4127)) \
			} while(false)                  \
			__pragma(warning(pop))  
#else
	#define MULTI_LINE_MACRO_BEGIN do {
	#define MULTI_LINE_MACRO_END } while(false)
#endif

#ifdef IS_MSVC
	#define DISABLE_MS_WARNING_BEGIN(number) \
	__pragma(warning(push))                  \
	__pragma(warning(disable: number ))

	#define DISABLE_MS_WARNING_END \
	__pragma(warning(pop))  
#else
	#define DISABLE_MS_WARNING_BEGIN(number)
	#define DISABLE_MS_WARNING_END
#endif
//#define DISALLOW_COPY_AND_ASSIGN(TypeName)           
//	TypeName            (TypeName const &) = delete; 
//	TypeName & operator=(TypeName const &) = delete;     

#define COPY_CTOR(type, method) \
	type(type const &) = method;

#define ASSIGN_CTOR(type, method) \
	type & operator=(type const &) = method;

#define MOVE_CTOR(type, method)  \
	type(type &&)             = method; \
	type & operator=(type &&) = method;

#define COPY_ASSIGN_CTOR(type, cmethod, amethod) \
	type(type const &) = cmethod;                              \
	type & operator=(type const &) = amethod;


#define COPY_ASSIGN_MOVE_CTOR(type, cmethod, amethod, mmethod) \
	type(type const &) = cmethod;                              \
	type & operator=(type const &) = amethod;                  \
	type(type &&)             = mmethod;                       \
	type & operator=(type &&) = mmethod;

#define MAKE_HASHABLE(type, ...)                          \
	namespace std {                                       \
		template<> struct hash<type> {                    \
			std::size_t operator()(const type &t) const { \
				std::size_t ret = 0;                      \
				hash_combine(ret, __VA_ARGS__);           \
				return ret;                               \
			}                                             \
		};                                                \
	}

#define MAKE_TIED(...) auto tied() const { return std::tie(__VA_ARGS__); }
#define ADD_EQUALITY_FROM_TIED(classname)                                             \
	bool operator==(const classname & other) const { return tied() == other.tied(); } \
	bool operator!=(const classname & other) const { return tied() != other.tied(); }

#if defined NO_MESSAGES
	#define MESSAGE(x) 
#else
	#if defined IS_MSVC
		#define MESSAGE(x) message(__FILE__ "(" STRINGIFY(__LINE__) ") : " x)
	#else
		#define MESSAGE(x) message(x)
	#endif
#endif

// Startup initialization macro.
#define AT_STARTUP(some_code)                                      \
	namespace {                                                    \
		static struct NV_STRING_JOIN2(AtStartup_, __LINE__)        \
		{                                                          \
			NV_STRING_JOIN2(AtStartup_, __LINE__)() { some_code; } \
		}                                                          \
		NV_STRING_JOIN3(AtStartup_, __LINE__, Instance); }



#if !defined DEBUG
	#define HandyDebugBreak()
	#define HandyDebugBreakOnce()
	#define HandyDebugAssert(x)
#else
	#if defined IS_MSVC
		#define HandyDebugBreak() __debugbreak()
	#elif defined IS_GNU || defined IS_CLANG
		#define HandyDebugBreak() __builtin_trap()
	#else
		#error HandyDebugBreak() not implemented for this compiler.
	#endif

	#define HandyDebugBreakOnce()                                \
		MULTI_LINE_MACRO_BEGIN                              \
		static bool firstTime = true;                       \
		if (firstTime) { firstTime = false; HandyDebugBreak(); } \
		MULTI_LINE_MACRO_END
	
	#define HandyDebugAssert(x)                   \
		MULTI_LINE_MACRO_BEGIN               \
		if (!(x)) { HandyDebugBreak(); assert((x)); } \
		MULTI_LINE_MACRO_END
#endif



/// This REQUIRES C++17 to use!!!!
///https://en.cppreference.com/w/cpp/language/if#If_Statements_with_Initializer
#define Scoped(...) if (__VA_ARGS__; true)
// 
/// Instead of:
//  {
//      RAIIThing raiiThing(42);
//      ...
//      // Scope ends here.
//  }
// 
/// --you can do:
//  Scoped(RAIIThing raiiThing(42))
//  {
//      ...
//      // Scope still ends here.
//  }
//
/// This makes it explicit what "thing" you are RAII'ing out of existance, and when.


template <typename T>
std::unique_ptr<T> handy_make_unique_ptr(T* ptr)
{
	return std::move(std::unique_ptr<T>(ptr));
}

#define Unique(IDENTIFIER, ...) if (auto IDENTIFIER = handy_make_unique_ptr(__VA_ARGS__); true)
// 
/// Instead of:
//  {
//      std::unique_ptr<SomeType> SomeName(SomePointer);
//      ...
//      // Scope ends here.
//  }
// 
/// --you can do:
//  Unique(SomeName, SomePointer)
//  {
//      ...
//      // Scope still ends here.
//  }


#define Locked(...) if (std::scoped_lock CurrentLock(__VA_ARGS__); true)
// 
/// Instead of:
//  {
//      std::lock_guard<std::mutex> CurrentLock(someMutex);
//      // or		
//      std::scoped_lock CurrentLock(someMutex);
//      ...
//      // Scope ends here.
//  }
// 
/// --you can do:
//  Locked(someMutex)
//  {
//      ...
//      // Scope still ends here.
//  }
//
/// This makes it explicit what the range of your locking is.

#define LockedN(IDENTIFIER, ...) if (std::scoped_lock IDENTIFIER(__VA_ARGS__); true)
// 
/// Instead of:
//  {
//      std::lock_guard<std::mutex> MyName(someMutex);
//      // or		
//      std::scoped_lock MyName(someMutex);
//      ...
//      // Scope ends here.
//  }
// 
/// --you can do:
//  Locked(MyName, someMutex)
//  {
//      ...
//      // Scope still ends here.
//  }
//
/// This makes it explicit what the range of your locking is.


///-----------------------------------------------
/// ITERATE_BY_FIELD
///
/// Inside of declaration add this to make your class range-iterable using 
/// one of the fieldnames from the class.
/// 1. The field must be declared ABOVE the use of this macro.
/// 2. Be sure to put it after a "public:" region to make it visible publicly.
#define ITERATE_BY_FIELD(x)                                               \
		typedef typename decltype(x)::iterator iterator;                  \
		typedef typename decltype(x)::const_iterator const_iterator;      \
																		  \
		FORCEINLINE iterator       begin()        { return x .begin();  } \
		FORCEINLINE iterator       end()          { return x .end();    } \
		FORCEINLINE const_iterator begin()  const { return x .begin();  } \
		FORCEINLINE const_iterator end()    const { return x .end();    } \
		FORCEINLINE const_iterator cbegin() const { return x .cbegin(); } \
		FORCEINLINE const_iterator cend()   const { return x .cend();   }

/// VS is too stoopid to decltype a templated class in some cases, if so, 
/// use t = { type of x }, "::" might be a problem though, not sure.
#define ITERATE_BY_FIELD_TYPE(x, t)                                       \
		typedef typename t ::iterator iterator;                           \
		typedef typename t ::const_iterator const_iterator;               \
																		  \
		FORCEINLINE iterator       begin()        { return x .begin();  } \
		FORCEINLINE iterator       end()          { return x .end();    } \
		FORCEINLINE const_iterator begin()  const { return x .begin();  } \
		FORCEINLINE const_iterator end()    const { return x .end();    } \
		FORCEINLINE const_iterator cbegin() const { return x .cbegin(); } \
		FORCEINLINE const_iterator cend()   const { return x .cend();   }      
///-----------------------------------------------



///-----------------------------------------------
/// Literal suffixes
///
// Respectively:
//
//	Adds uint8_t  literal suffix, ie 42_u8
//	Adds uint16_t literal suffix, ie 42_u16
//	Adds uint32_t literal suffix, ie 42_u32
//	Adds uint64_t literal suffix, ie 42_u64

//	Adds int8_t  literal suffix, ie -42_i8
//	Adds int16_t literal suffix, ie -42_i16
//	Adds int32_t literal suffix, ie -42_i32
//	Adds int64_t literal suffix, ie -42_i64

//  Adds size_t  literal suffix, ie  42_szt

//	Adds float  literal suffix, ie 1_f
//	Adds double literal suffix, ie 1_d

inline constexpr uint8_t  operator "" _u8 (unsigned long long value) { return static_cast<uint8_t> (value); }
inline constexpr uint16_t operator "" _u16(unsigned long long value) { return static_cast<uint16_t>(value); }
inline constexpr uint32_t operator "" _u32(unsigned long long value) { return static_cast<uint32_t>(value); }
inline constexpr uint64_t operator "" _u64(unsigned long long value) { return static_cast<uint64_t>(value); }

inline constexpr int8_t   operator "" _i8 (unsigned long long value) { return static_cast<int8_t>  (value); }
inline constexpr int16_t  operator "" _i16(unsigned long long value) { return static_cast<int16_t> (value); }
inline constexpr int32_t  operator "" _i32(unsigned long long value) { return static_cast<int32_t> (value); }
inline constexpr int64_t  operator "" _i64(unsigned long long value) { return static_cast<int64_t> (value); }

inline constexpr size_t    operator "" _szt(unsigned long long value) { return static_cast<size_t>   (value); }
inline constexpr ptrdiff_t operator "" _pdt(unsigned long long value) { return static_cast<ptrdiff_t>(value); }

inline constexpr float    operator "" _f  (unsigned long long value) { return static_cast<float>   (value); }
inline constexpr float    operator "" _f  (long double        value) { return static_cast<float>   (value); }
inline constexpr double   operator "" _d  (unsigned long long value) { return static_cast<double>  (value); }
inline constexpr double   operator "" _d  (long double        value) { return static_cast<double>  (value); }
//inline __float128       operator "" _q  (unsigned long long value) { return static_cast<__float128>(value); }
//inline __float128       operator "" _q  (__float128         value) { return static_cast<__float128>(value); }

inline constexpr std::byte operator "" _byte (unsigned long long value) { return static_cast<std::byte> (value); }

inline constexpr uint64_t operator "" _KiB(unsigned long long val) { return val * 1 << 10; }
inline constexpr uint64_t operator "" _MiB(unsigned long long val) { return val * 1 << 20; }
inline constexpr uint64_t operator "" _GiB(unsigned long long val) { return val * 1 << 30; }

inline constexpr uint64_t operator "" _KB(unsigned long long val) { return val *         1'000; }
inline constexpr uint64_t operator "" _MB(unsigned long long val) { return val *     1'000'000; }
inline constexpr uint64_t operator "" _GB(unsigned long long val) { return val * 1'000'000'000; }


/// Defined in HandyCompat.hpp:
//inline constexpr std::byte operator "" _byte (unsigned long long value) { return static_cast<std::byte> (value); }

///-----------------------------------------------


/// Get rid of "_t" on integer types, since they are annoying to type.
typedef uint8_t  uint8;
typedef int8_t   int8;

typedef uint16_t uint16;
typedef int16_t  int16;

typedef uint32_t uint32;
typedef int32_t  int32;

typedef uint64_t uint64;
typedef int64_t  int64;


/// Make unordered_set stop being a jackass about tuples.
/// (Makes std::tuple types work automagically in 
/// std::unordered_set)
namespace std { 
	namespace {
		// Code from boost
		// Reciprocal of the golden ratio helps spread entropy
		//     and handles duplicates.
		// See Mike Seymour in magic-numbers-in-boosthash-combine:
		//     http://stackoverflow.com/questions/4948780

		template <class T>
		inline void hash_combine(std::size_t& seed, T const& v)
		{
			seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
		}

		template <typename T, typename... Rest>
		inline void hash_combine(std::size_t& seed, const T& v, Rest... rest) {
			std::hash<T> hasher;
			seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			hash_combine(seed, rest...);
		}

		// Recursive template code derived from Matthieu M.
		template <class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
		struct HashValueImpl
		{
			static void apply(size_t& seed, Tuple const& tuple)
			{
				HashValueImpl<Tuple, Index-1>::apply(seed, tuple);
				hash_combine(seed, std::get<Index>(tuple));
			}
		};

		template <class Tuple>
		struct HashValueImpl<Tuple,0>
		{
			static void apply(size_t& seed, Tuple const& tuple)
			{
				hash_combine(seed, std::get<0>(tuple));
			}
		};
	}

	template <typename ... TT>
	struct hash<std::tuple<TT...>> 
	{
		size_t operator()(std::tuple<TT...> const& tt) const
		{                                              
			size_t seed = 0;                             
			HashValueImpl<std::tuple<TT...> >::apply(seed, tt);    
			return seed;                                 
		}                                              
	};
}
///-----------------------------------------------


/// If you would like the functions/classes in Handy to be in a different namespace, just define HANDY_NS as your desired namespace name.
#ifndef HANDY_NS
	#define HANDY_NS Handy
#endif

namespace HANDY_NS {

	/// Byte swap an integer, auto sized.
	template <typename T>
	FORCEINLINE static T bswapi(T t)
	{
		STATIC_ASSERT(std::is_integral<T>::value)

		if      constexpr (sizeof(T) == 1) return t;
		else if constexpr (sizeof(T) == 2) return bswap_16(static_cast<uint16_t>(t));
		else if constexpr (sizeof(T) == 4) return bswap_32(static_cast<uint32_t>(t));
		else if constexpr (sizeof(T) == 8) return bswap_64(static_cast<uint64_t>(t));
	}

	/// Byte swap an integer, only if this is running on a little endian architecture.
	template <typename T>
	FORCEINLINE static T bswapi_l(T t)
	{
		return
			#if defined IS_LITTLE_ENDIAN
				bswapi(t);
			#else
				t;
			#endif
	}

	/// Byte swap an integer, only if this is running on a big endian architecture.
	template <typename T>
	FORCEINLINE static T bswapi_b(T t)
	{
		return
			#if defined IS_LITTLE_ENDIAN
				t;
			#else
				bswapi(t);
			#endif
	}

	inline std::string BuildInfoString()
	{
		std::string InfoStr;
		
		InfoStr += "==========================     Build Info     ===========================\n";
		InfoStr += "Compiler:                " COMPILER_STRING "\n";
		InfoStr += "Language:                " LANGUAGE_STRING
		#ifdef IS_CLI
			" CLI"
		#endif
		                                                       "\n";
		InfoStr += "Target CPU Architecture: " CPU_STRING      "\n";
		InfoStr += "Target Operating System: " OS_STRING       "\n";
		InfoStr += "Target Endianness:       " ENDIAN_STRING   "\n";
		InfoStr += "Built:                   " __DATE__ " @ " __TIME__ "\n";
		InfoStr += "=========================================================================\n";
		return InfoStr;
	}


	///-----------------------------------------------
	/// Quick helpers for equality in pairs:
	template <typename T>
	FORCEINLINE bool IsAnyOrder(T const & t1, T const & t2, T const & tTest1, T const & tTest2) { return (t1 == tTest1 && t2 == tTest2) || (t2 == tTest1 && t1 == tTest2); }
	template <typename T>
	FORCEINLINE bool IsEither(T const & tTest, T const & t1, T const & t2) { return t1 == tTest || t2 == tTest; }
	template <typename T>
	FORCEINLINE bool EitherIs(T const & t1, T t2, T const & tTest) { return t1 == tTest || t2 == tTest; }
	template <typename T>
	FORCEINLINE bool IsNeither(T const & tTest, T const & t1, T const & t2) { return !(t1 == tTest || t2 == tTest); }
	template <typename T>
	FORCEINLINE bool NeitherIs(T const & t1, T const & t2, T const & tTest) { return !(t1 == tTest || t2 == tTest); }
	template <typename T>
	FORCEINLINE bool BothAre(T const & t1, T const & t2, T const & tTest) { return t1 == tTest && t2 == tTest; }

	/// ONLY WORKS AS PASS-BY-VALUE
	template <typename T>
	FORCEINLINE bool IsAnyOf(T tTest, std::initializer_list<T> ts)
	{
		for (T const & item : ts)
			if (item == tTest)
				return true;

		return false;
	}

	/// ONLY WORKS AS PASS-BY-VALUE
	template <typename T>
	FORCEINLINE bool AllAre(std::initializer_list<T> ts, T tTest)
	{
		for (T const & item : ts)
			if (item != tTest)
				return false;

		return true;
	}
	///-----------------------------------------------

	///-----------------------------------------------
	/// https://randomascii.wordpress.com/2013/11/24/stdmin-causing-three-times-slowdown-on-vc/
	/// Fast min/max for VC++, note that std::min/max are JUST FINE on Clang and GCC
	#define FASTMINMAXDEF(TYPE) \
		FORCEINLINE TYPE FastMin(TYPE a, TYPE b) { return a < b ? a : b; } \
		FORCEINLINE TYPE FastMax(TYPE a, TYPE b) { return a < b ? b : a; }

	FASTMINMAXDEF(int8_t)
	FASTMINMAXDEF(int16_t)
	FASTMINMAXDEF(int32_t)
	FASTMINMAXDEF(int64_t)
	FASTMINMAXDEF(uint8_t)
	FASTMINMAXDEF(uint16_t)
	FASTMINMAXDEF(uint32_t)
	FASTMINMAXDEF(uint64_t)
	FASTMINMAXDEF(float)
	FASTMINMAXDEF(double)
	///-----------------------------------------------

	///-----------------------------------------------
	/// Quick helpers for more than pair min/max:
	#define FASTMINMAX3DEF(TYPE) \
		FORCEINLINE TYPE FastMin(TYPE a, TYPE b, TYPE c)         { return FastMin(a, FastMin(b, c)); } \
		FORCEINLINE TYPE FastMax(TYPE a, TYPE b, TYPE c)         { return FastMax(a, FastMax(b, c)); }

	#define FASTMINMAX4DEF(TYPE) \
		FORCEINLINE TYPE FastMin(TYPE a, TYPE b, TYPE c, TYPE d) { return FastMin(a, FastMin(b, FastMin(c, d))); } \
		FORCEINLINE TYPE FastMax(TYPE a, TYPE b, TYPE c, TYPE d) { return FastMax(a, FastMax(b, FastMax(c, d))); }

	FASTMINMAX3DEF(int8_t)
	FASTMINMAX3DEF(int16_t)
	FASTMINMAX3DEF(int32_t)
	FASTMINMAX3DEF(int64_t)
	FASTMINMAX3DEF(uint8_t)
	FASTMINMAX3DEF(uint16_t)
	FASTMINMAX3DEF(uint32_t)
	FASTMINMAX3DEF(uint64_t)
	FASTMINMAX3DEF(float)
	FASTMINMAX3DEF(double)


	FASTMINMAX4DEF(int8_t)
	FASTMINMAX4DEF(int16_t)
	FASTMINMAX4DEF(int32_t)
	FASTMINMAX4DEF(int64_t)
	FASTMINMAX4DEF(uint8_t)
	FASTMINMAX4DEF(uint16_t)
	FASTMINMAX4DEF(uint32_t)
	FASTMINMAX4DEF(uint64_t)
	FASTMINMAX4DEF(float)
	FASTMINMAX4DEF(double)
	///-----------------------------------------------


	// ALL of this is just to support IsNan and WithinEpsilon below. 
	namespace internal
	{
		#if !defined(MAXULPS)
			#define MAXULPS 8
		#endif

		//constexpr size_t MaxUlps = size_t(MAXULPS);

		template <size_t size> struct TypeWithSize { typedef void UInt; };
		template <> struct TypeWithSize<4> { typedef int32_t Int; typedef uint32_t UInt; };
		template <> struct TypeWithSize<8> { typedef int64_t Int; typedef uint64_t UInt; };

		template <typename FT>
		struct private_Float
		{
			typedef typename TypeWithSize<sizeof(FT)>::UInt SIZED_UINT;

			static constexpr size_t       BitCount = sizeof(FT) * 8_szt;
			static constexpr size_t   FracBitCount = std::numeric_limits<FT>::digits - 1;
			static constexpr size_t    ExpBitCount = BitCount - 1 - FracBitCount;

			static constexpr SIZED_UINT SignBitMask =  SIZED_UINT(1) << (BitCount    - 1);
			static constexpr SIZED_UINT FracBitMask = ~SIZED_UINT(0) >> (ExpBitCount + 1);
			static constexpr SIZED_UINT  ExpBitMask = ~(SignBitMask | FracBitMask);

			static FORCEINLINE SIZED_UINT SignAndMagnitudeToBiased(SIZED_UINT sam)
			{
				if (SignBitMask & sam)
					return ~sam + 1;  // two's complement
				else
					return SignBitMask | sam;  // * 2
			}

			static FORCEINLINE SIZED_UINT ULP_diff(SIZED_UINT sam1, SIZED_UINT sam2)
			{
				SIZED_UINT const biased1 = SignAndMagnitudeToBiased(sam1);
				SIZED_UINT const biased2 = SignAndMagnitudeToBiased(sam2);

				return (biased1 >= biased2) ? (biased1 - biased2) : (biased2 - biased1);
			}

			static FORCEINLINE bool IsNanUF(SIZED_UINT f)
			{
				return ((ExpBitMask & f) == ExpBitMask) && ((FracBitMask & f) != 0);
			}
		};
	}
	// ---------------------------------------------------------------------------------------

	template <typename FT>  constexpr   FT   Pi         = FT(3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679);
	template <typename FT>  constexpr   FT   PiHalf     = Pi<FT> * FT(0.5);
	template <typename FT>  constexpr   FT   TwoPi      = FT(2) * Pi<FT>;
	template <typename FT>  constexpr   FT   PiQuarter  = Pi<FT> * FT(0.25);
	template <typename FT>  constexpr   FT   PiOver180  = Pi<FT> / FT(180);
	template <typename FT>  constexpr   FT   PiUnder180 = FT(180) / Pi<FT>;
	template <typename FT>  constexpr   FT   RPi        = FT(1) / Pi<FT>;
	template <typename FT>  constexpr   FT   RPiHalf    = FT(1) / PiHalf<FT>;
	template <typename FT>  constexpr   FT   RTwoPi     = FT(1) / TwoPi<FT>;
	template <typename FT>  constexpr   FT   RPiQuarter = FT(1) / PiQuarter<FT>;
	template <typename FT>  constexpr   FT   Tau        = FT(6.2831853071795864769252867665590057683943387987502116419498891846156328125724179972560696506842341359);
	template <typename FT>  constexpr   FT   E_Constant = FT(2.7182818284590452353602874713526624977572470936999595749669676277240766303535475945713821785251664274);
	template <typename FT>  constexpr   FT   Sqrt2      = FT(1.4142135623730950488016887242096980785696718753769480731766797379907324784621070388503875343276415727);
	template <typename FT>  constexpr   FT   Sqrt3      = FT(1.7320508075688772935274463415058723669428052538103806280558069794519330169088000370811461867572485756);
	template <typename FT>  constexpr   FT   SqrtHalf   = Sqrt2<FT> * FT(0.5);

	template <typename FT>  constexpr   FT   Nan = std::numeric_limits<FT>::quiet_NaN();
	template <typename FT>  constexpr   FT   Inf = std::numeric_limits<FT>::infinity();
	template <typename FT>  constexpr   FT   NInf = -Inf<FT>;

	template <typename IT>  constexpr   IT   Kibibyte = IT(        1'024);
	template <typename IT>  constexpr   IT   Mebibyte = IT(    1'048'576);
	template <typename IT>  constexpr   IT   Gibibyte = IT(1'073'741'824);

	template <typename FT>  constexpr   FT   Deg2Rad(FT degrees) { STATIC_ASSERT(std::is_floating_point<FT>::value); return degrees * PiOver180<FT>; }
	template <typename FT>  constexpr   FT   Rad2Deg(FT radians) { STATIC_ASSERT(std::is_floating_point<FT>::value); return radians * PiUnder180<FT>; }

	template <typename T>   constexpr   bool WithinRange_Open      (T x, T lowerBound, T upperBound) { return x >  lowerBound && x <  upperBound; }
	template <typename T>   constexpr   bool WithinRange_OpenClosed(T x, T lowerBound, T upperBound) { return x >  lowerBound && x <= upperBound; }
	template <typename T>   constexpr   bool WithinRange_ClosedOpen(T x, T lowerBound, T upperBound) { return x >  lowerBound && x <= upperBound; }
	template <typename T>   constexpr   bool WithinRange_Closed    (T x, T lowerBound, T upperBound) { return x >= lowerBound && x <= upperBound; }

	template <typename T>   constexpr   T    Clamp(T x, T minVal, T maxVal) { return Handy::FastMin   (Handy::FastMax   (x, minVal), maxVal); }

	template <typename FT>  constexpr   FT   Lerp      (FT edge0, FT edge1, FT t) { STATIC_ASSERT(std::is_floating_point<FT>::value); return edge1 * t + edge0 * (FT(1) - t); }
	template <typename FT>  constexpr   FT   SmoothStep(FT edge0, FT edge1, FT t) { STATIC_ASSERT(std::is_floating_point<FT>::value); return Lerp(edge0, edge1, t * t * (FT(3) - FT(2) * t)); }
	
	template <typename UIT> UIT  GCD_U(UIT u, UIT v)
	{
		STATIC_ASSERT(std::is_integral<UIT>::value && std::is_unsigned<UIT>::value);

		while (v != 0)
		{
			UIT r = u % v;
			u = v;
			v = r;
		}
		return u;
	}

	template <typename UIT> bool IsPowerOfTwo(UIT v)
	{
		STATIC_ASSERT(std::is_integral<UIT>::value && std::is_unsigned<UIT>::value);

		return v && !(v & (v - 1)); // https://graphics.stanford.edu/~seander/bithacks.html
	}

	FORCEINLINE uint32_t NextPowerOfTwo(uint32_t v)
	{
		// https://graphics.stanford.edu/~seander/bithacks.html
		v--;
		v |= v >> 1;
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;
		v++;

		return v;
	}

	FORCEINLINE uint64_t NextPowerOfTwo(uint64_t v)
	{
		// https://graphics.stanford.edu/~seander/bithacks.html
		v--;
		v |= v >> 1;
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;
		v |= v >> 32;
		v++;

		return v;
	}


	// Returns offset to most significant bit, -1 if zero
	// 1b      -> 0d
	// 100b    -> 2d
	// 101000b -> 5d
	FORCEINLINE int32_t FindMSB(uint32_t x)
	{
		int32_t p = -1;

		while (x) 
		{
			x >>= 1;
			++p;
		}

		return p;
	}

	// Returns offset to most significant bit, -1 if zero
	// 1b      -> 0d
	// 100b    -> 2d
	// 101000b -> 5d
	FORCEINLINE int32_t FindMSB(uint64_t x)
	{
		int32_t p = -1;

		while (x) 
		{
			x >>= 1;
			++p;
		}

		return p;
	}

	// Bit reverse an integer given a word of nb bits when nb is [0, 31]
	// examples:
	// 10b, 2      -> 01b
	// 10b, 4      -> 0100b
	// 101b, 3     -> 101b
	// 1011b, 4    -> 1101b
	// 0111001b, 7 -> 1001110b
	FORCEINLINE uint32_t BitReverse(uint32_t x, uint32_t nb)
	{
		x = ( x               << 16) | ( x               >> 16);
		x = ((x & 0x00FF00FF) <<  8) | ((x & 0xFF00FF00) >>  8);
		x = ((x & 0x0F0F0F0F) <<  4) | ((x & 0xF0F0F0F0) >>  4);
		x = ((x & 0x33333333) <<  2) | ((x & 0xCCCCCCCC) >>  2);
		x = ((x & 0x55555555) <<  1) | ((x & 0xAAAAAAAA) >>  1);

		return ((x >> (32 - nb)) & (0xFFFFFFFF >> (32 - nb)));
	}

	//FORCEINLINE float FastSqrtF(float number) // This likely isn't faster on modern CPUs, it's more here for historical purposes.
	//{
	//	long i;
	//	float x2, y;
	//	const float threehalfs = 1.5F;
	//
	//	x2 = number * 0.5F;
	//	y  = number;
	//	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	//	i  = 0x5f3759df - ( i >> 1 );               // what the?
	//	y  = * ( float * ) &i;
	//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
	//												//      y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
	//
	//	return 1.0f / y;
	//}
	//
	//FORCEINLINE float FastInvSqrtF(float number) // This likely isn't faster on modern CPUs, it's more here for historical purposes.
	//{
	//	long i;
	//	float x2, y;
	//	const float threehalfs = 1.5F;
	//
	//	x2 = number * 0.5F;
	//	y  = number;
	//	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	//	i  = 0x5f3759df - ( i >> 1 );               // what the?
	//	y  = * ( float * ) &i;
	//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
	//												//      y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
	//
	//	return y;
	//}

	enum class EasingFunction : uint32_t
	{

		//    .-- 
		//   / 
		SINE_INV_QUARTER = 0,

		//    .--. 
		//   /    \      .
		SINE_INV_HALF,

		//        /
		//   .---' 
		//  /
		SINE_INV_HALF_MID,

		//     /
		//  --'
		SINE_QUARTER,

		//      .--
		//     /
		//  --' 
		SINE_HALF,

		//      .--. 
		//     /    \        .
		//  _.'      '._
		SINE_FULL,

		//    .
		//   / \      .
		//  /   \     .
		// /     \    .
		RAMP_FULL,

		//   /
		//  / 
		// /  
		RAMP_INC,

		// Google it.
		SMOOTHSTEP,

		// Google it.
		SMOOTHSTEP_INC
	};


	template <typename FT> inline FT Easing(EasingFunction alpha, FT value)
	{
		STATIC_ASSERT(std::is_floating_point<FT>::value);

		float ret = 0;
		switch (alpha)
		{
			case EasingFunction::SINE_INV_QUARTER:
				ret = std::sin(value * Pi<FT> / 2.0f);
				break;
			case EasingFunction::SINE_INV_HALF:
				ret = std::sin(value * Pi<FT>);
				break;
			case EasingFunction::SINE_INV_HALF_MID:
				if (value < 0.5f)
					ret = std::sin(value * Pi<FT>) / 2;
				else
					ret = 0.5f + (1.0f - std::sin(value * Pi<FT>)) / 2.0f;
				break;
			case EasingFunction::SINE_QUARTER:
				ret = 1.0f - std::sin((0.5f - value * 0.5f) * Pi<FT>);
				break;
			case EasingFunction::SINE_HALF:
				ret = 1.0f - std::sin((0.5f - value) * Pi<FT>);
				break;

			case EasingFunction::SINE_FULL:
				ret = (std::sin(value * 2.0f * Pi<FT> + (6.0f * Pi<FT> / 4.0f)) + 1.0f) / 2.0f;
				break;

			case EasingFunction::SMOOTHSTEP:
				ret = value * 2.0f;
				if (ret > 1.0f)
					ret = 1.0f - (ret - 1.0f);
				ret = 1.0f - (2.0f * (ret * ret * ret) - 3 * (ret * ret) + 1.0f);
				break;

			case EasingFunction::SMOOTHSTEP_INC:
				ret = -2.0f * (value * value * value) + 3.0f * (value * value);
				break;

			case EasingFunction::RAMP_FULL:
				ret = value * 2.0f;
				if (ret > 1.0f)
					ret = 1.0f - (ret - 1.0f);
				break;

			case EasingFunction::RAMP_INC:    
			default:
				ret = value;
		} 

		return ret;
	}	



	template<typename FT> FORCEINLINE bool IsNan(FT f) 
	{ 
		STATIC_ASSERT(std::is_floating_point<FT>::value); 
		return internal::private_Float<FT>::IsNanUF(*(typename internal::private_Float<FT>::SIZED_UINT*)&f); 
	}


	/// NOTE: FOR FLOATS:
	///          Most precise, smallest possible difference between two floats:
	///               maxULPS = 1 or 2
	///          Approximately 7 digits of precision:
	///               maxULPs = Less than 4
	///          Approximately 6 digits of precision:
	///               maxULPs = Less than 42
	///          Approximately 5 digits of precision:
	///               maxULPs = Less than 419
	///          Approximately 4 digits of precision:
	///               maxULPs = Less than 4193
	///          Approximately 3 digits of precision:
	///               maxULPs = Less than 41943
	///          Approximately 2 digits of precision:
	///               maxULPs = Less than 419430
	///
	/// NOTE: FOR DOUBLES:
	///          Most precise, 16 digits precision:
	///               maxULPS = 1
	///          Approximately 15 digits of precision:
	///               maxULPs = Less than 23
	///          Approximately 14 digits of precision:
	///               maxULPs = Less than 225
	///          Approximately 13 digits of precision:
	///               maxULPs = Less than 2252
	///          Approximately 12 digits of precision:
	///               maxULPs = Less than 22518
	///          Approximately 11 digits of precision:
	///               maxULPs = Less than 225180
	///          Approximately 10 digits of precision:
	///               maxULPs = Less than 2251800
	template<typename FT> FORCEINLINE bool WithinEpsilon(FT lhs, FT rhs, size_t maxULPS = 8)
	{
		STATIC_ASSERT(std::is_floating_point<FT>::value);

		typename internal::private_Float<FT>::SIZED_UINT luf = *(typename internal::private_Float<FT>::SIZED_UINT*)&lhs;
		typename internal::private_Float<FT>::SIZED_UINT ruf = *(typename internal::private_Float<FT>::SIZED_UINT*)&rhs;

		#ifdef HANDY_PROPER_NAN_BEHAVIOR
		if (internal::private_Float<FT>::IsNanUF(luf) || internal::private_Float<FT>::IsNanUF(ruf)) 
			return false;
		#endif

		return internal::private_Float<FT>::ULP_diff(luf, ruf) <= maxULPS; //internal::MaxUlps;
	}

	FORCEINLINE float InchesToPixels(float inches, float DPI)
	{
		return std::roundf(DPI * inches);
	}

	FORCEINLINE float CentimetersToPixels(float centimeters, float DPI)
	{
		return std::roundf((DPI / 2.54f) * centimeters);
	}

	/// Returns the minimum number of segments needed to draw a circle approximated as an 
	/// inscribed equilateral polygon, given a maximum distance the polygon can diverge from 
	/// a perfect circle. Always returns at least 3. radius and maxError must both be >0.
	FORCEINLINE int32_t CircleSegsToApprox(float radius, float maxError)
	{
		const float r = radius;
		const float s = maxError;

		const float minN = 2_f * Pi<float> / std::acos((2_f*s*s - 4_f*r*s) / (r*r) + 1);

		return Handy::FastMax(3, (int)std::ceil(minN));
	}

} // HANDY_NS


///-----------------------------------------------
/// Bitmask (flags) Enablement Tools
///
///		- This should be used just below the definition of an enum class, in the same namespace. 
///     - CANNOT BE USED INSIDE A CLASS
#define HANDY_ENABLE_BITMASK_OPERATORS(ENUMTYPE) \
inline constexpr ENUMTYPE   operator |  (ENUMTYPE  a, ENUMTYPE b) throw() { return  ENUMTYPE   (((std::underlying_type_t<ENUMTYPE>  )a) |  ((std::underlying_type_t<ENUMTYPE>)b)); } \
inline           ENUMTYPE & operator |= (ENUMTYPE &a, ENUMTYPE b) throw() { return (ENUMTYPE &)(((std::underlying_type_t<ENUMTYPE> &)a) |= ((std::underlying_type_t<ENUMTYPE>)b)); } \
inline constexpr ENUMTYPE   operator &  (ENUMTYPE  a, ENUMTYPE b) throw() { return  ENUMTYPE   (((std::underlying_type_t<ENUMTYPE>  )a) &  ((std::underlying_type_t<ENUMTYPE>)b)); } \
inline           ENUMTYPE & operator &= (ENUMTYPE &a, ENUMTYPE b) throw() { return (ENUMTYPE &)(((std::underlying_type_t<ENUMTYPE> &)a) &= ((std::underlying_type_t<ENUMTYPE>)b)); } \
inline constexpr ENUMTYPE   operator ~  (ENUMTYPE  a)             throw() { return  ENUMTYPE  (~((std::underlying_type_t<ENUMTYPE>  )a)                                         ); } \
inline constexpr ENUMTYPE   operator ^  (ENUMTYPE  a, ENUMTYPE b) throw() { return  ENUMTYPE   (((std::underlying_type_t<ENUMTYPE>  )a) ^  ((std::underlying_type_t<ENUMTYPE>)b)); } \
inline           ENUMTYPE & operator ^= (ENUMTYPE &a, ENUMTYPE b) throw() { return (ENUMTYPE &)(((std::underlying_type_t<ENUMTYPE> &)a) ^= ((std::underlying_type_t<ENUMTYPE>)b)); }
/// Example of usage:
//enum class SomeFlags : uint64_t
//{
//	None = 0,
//
//	OptionA = 1,
//	OptionB = 1 << 1,
//	OptionC = 1 << 2
//
//}; HANDY_ENABLE_BITMASK_OPERATORS(SomeFlags)
//
//int blah()
//{
//	SomeFlags a1 = SomeFlags::OptionA | SomeFlags::OptionB;
//	SomeFlags a2 = a1 & SomeFlags::OptionB;
//	a2 ^= SomeFlags::OptionA;
//	SomeFlags a3 = ~a1;
//}
///-----------------------------------------------


///-----------------------------------------------
/// Build Info Macros
///-----------------------------------------------

// Example of __DATE__ string: "Jul 27 2012"
// Example of __TIME__ string: "21:06:19"

#define COMPUTE_BUILD_YEAR \
	( \
		(__DATE__[ 7] - '0') * 1000 + \
		(__DATE__[ 8] - '0') *  100 + \
		(__DATE__[ 9] - '0') *   10 + \
		(__DATE__[10] - '0') \
	)


#define COMPUTE_BUILD_DAY \
	( \
		((__DATE__[4] >= '0') ? (__DATE__[4] - '0') * 10 : 0) + \
		(__DATE__[5] - '0') \
	)


#define BUILD_MONTH_IS_JAN (__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_FEB (__DATE__[0] == 'F')
#define BUILD_MONTH_IS_MAR (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r')
#define BUILD_MONTH_IS_APR (__DATE__[0] == 'A' && __DATE__[1] == 'p')
#define BUILD_MONTH_IS_MAY (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y')
#define BUILD_MONTH_IS_JUN (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_JUL (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l')
#define BUILD_MONTH_IS_AUG (__DATE__[0] == 'A' && __DATE__[1] == 'u')
#define BUILD_MONTH_IS_SEP (__DATE__[0] == 'S')
#define BUILD_MONTH_IS_OCT (__DATE__[0] == 'O')
#define BUILD_MONTH_IS_NOV (__DATE__[0] == 'N')
#define BUILD_MONTH_IS_DEC (__DATE__[0] == 'D')


#define COMPUTE_BUILD_MONTH \
	( \
		(BUILD_MONTH_IS_JAN) ?  1 : \
		(BUILD_MONTH_IS_FEB) ?  2 : \
		(BUILD_MONTH_IS_MAR) ?  3 : \
		(BUILD_MONTH_IS_APR) ?  4 : \
		(BUILD_MONTH_IS_MAY) ?  5 : \
		(BUILD_MONTH_IS_JUN) ?  6 : \
		(BUILD_MONTH_IS_JUL) ?  7 : \
		(BUILD_MONTH_IS_AUG) ?  8 : \
		(BUILD_MONTH_IS_SEP) ?  9 : \
		(BUILD_MONTH_IS_OCT) ? 10 : \
		(BUILD_MONTH_IS_NOV) ? 11 : \
		(BUILD_MONTH_IS_DEC) ? 12 : \
		/* error default */  99 \
	)

#define COMPUTE_BUILD_HOUR ((__TIME__[0] - '0') * 10 + __TIME__[1] - '0')
#define COMPUTE_BUILD_MIN  ((__TIME__[3] - '0') * 10 + __TIME__[4] - '0')
#define COMPUTE_BUILD_SEC  ((__TIME__[6] - '0') * 10 + __TIME__[7] - '0')


#define BUILD_DATE_IS_BAD (__DATE__[0] == '?')

#define BUILD_YEAR  ((BUILD_DATE_IS_BAD) ? 99 : COMPUTE_BUILD_YEAR)
#define BUILD_MONTH ((BUILD_DATE_IS_BAD) ? 99 : COMPUTE_BUILD_MONTH)
#define BUILD_DAY   ((BUILD_DATE_IS_BAD) ? 99 : COMPUTE_BUILD_DAY)

#define BUILD_TIME_IS_BAD (__TIME__[0] == '?')

#define BUILD_HOUR  ((BUILD_TIME_IS_BAD) ? 99 :  COMPUTE_BUILD_HOUR)
#define BUILD_MIN   ((BUILD_TIME_IS_BAD) ? 99 :  COMPUTE_BUILD_MIN)
#define BUILD_SEC   ((BUILD_TIME_IS_BAD) ? 99 :  COMPUTE_BUILD_SEC)


#if defined IS_MSVC && defined GTEQ_VS2017 && _MSVC_LANG < 201703L
	#error Compiling with VS requires VS2017+ and requires the /std:c++17 or /std:c++latest switch enabled.
#endif
