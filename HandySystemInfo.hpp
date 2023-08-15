
/// =============================================================================
///	SystemInfo Library
/// https://github.com/poli0048/SystemInfo
///
/// LICENSE: BSD 3-Clause License
/// 
/// Copyright (c) 2017 Sentek Systems, LLC.
///
/// This software is provided 'as-is', without any express or implied
/// warranty.  In no event will the authors be held liable for any damages
/// arising from the use of this software.
/// 
/// Permission is granted to anyone to use this software for any purpose,
/// including commercial applications, and to alter it and redistribute it
/// freely, subject to the following restrictions:
/// 
/// 1. The origin of this software must not be misrepresented; you must not
///    claim that you wrote the original software. If you use this software
///    in a product, an acknowledgment in the product documentation would be
///    appreciated but is not required.
/// 2. Altered source versions must be plainly marked as such, and must not be
///    misrepresented as being the original software.
/// 3. This notice may not be removed or altered from any source distribution.
/// =============================================================================

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "HandyBase.hpp"
#include "HandyCompat.hpp"
#include "HandyString.hpp"

#if defined IS_NIX || defined IS_APPLE || defined IS_WINDOWS || defined IS_EMSCRIPTEN
#else
	#error "Unable to define SystemInfo for unknown platform"
#endif

namespace HANDY_NS {
	
	bool IsDebuggerAttached();

	class SingleInstanceLock 
	{
		//The SingleInstanceLock class creates a lock file and holds locked write-access to the file for the lifetime of the object.
		//If write-access cannot be secured, it is an indication that another instance of the program is already running and holding
		//the lock. On destruction the lock file is deleted. If a lock file is present during construction but we are able to
		//get write-access it is an indication that the program did not close properly (it crashed), but it will not be confused
		//with the existance of another instance of the application. By depending on write-access instead of the existance of a lock
		//file we avoid the problem of a program crash preventing future instances of the program from running.

	public:
		enum class ResultType 
		{
			Lock_Succeeded,
			Lock_Succeeded_ButPreviousInstanceCrashed,
			Lock_Failed
		};

		// Fields  **************************************************************************
		bool lockActive = false;
		std::filesystem::path lockFilePath;
		#ifdef __unix
			int instanceLockFileHandle;
		#else
			void* instanceLockFileHandle;
		#endif

		// Methods  *************************************************************************
		SingleInstanceLock() = delete;
		SingleInstanceLock(std::filesystem::path LockFilePath) : lockFilePath(LockFilePath) { }
		~SingleInstanceLock();

		ResultType GetLock();
	};

	struct CPUCapabilities
	{
		std::string Vendor;
		std::string Brand;

		// Old SIMD Extentions
		bool MMX;    //(Deployed around 1997)
		bool MMXEXT; //Extended MMX (Deployed around 1999)

						// SSE SIMD extensions common to both Intel and AMD
		bool SSE;
		bool SSE2;
		bool SSE3;
		bool SSSE3;
		bool SSE41;
		bool SSE42;

		// Non-Standard Extentions Introduced around the same time as later-revision SSE4
		bool POPCNT;  // Population count
		bool LZCNT;   // Leading Zero Count

						// Mainstream AVX - Advanced Vector Extentions (Follow-on to SSE)
		bool AVX;
		bool AVX2;

		// Specialty AVX - Generally supported only on modern high-end and specialized CPUs
		bool AVX512F;  // AVX-512 Foundation
		bool AVX512PF; // AVX-512 Pre-Fetch Instructions
		bool AVX512ER; // AVX-512 Exponential and Reciprocal Instructions
		bool AVX512CD; // AVX-512 Conflict Detection Instructions

						// AMD-only SSE extensions (Introduced in AMD Barcelona Architecture)
		bool SSE4a;

		// AMD-only 3DNow Extentions (Competed with original SSE - never supported by Intel and dropped by AMD in new processors)
		bool _3DNOW;    // (Deployed around 1998)
		bool _3DNOWEXT;

		// Additional Extentions:
		bool PCLMULQDQ;  // Carry-less Multiplication (Deployed around 2010)
		bool FMA;        // Fused Multiply-Add (Probably refers to FMA3) (Deployed around 2013)
		bool CMPXCHG16B; // Compare and Exchange - 16 Bytes (Deployed around 2003)
		bool AES;        // Advanced Encryption Standard Instruction Set (Deployed around 2011)
		bool SHA;        // Secure Hash Algorithm Extentions (Deployed around 2016)

		bool MONITOR;    bool MOVBE;       bool XSAVE;    bool OSXSAVE;
		bool F16C;       bool RDRAND;      bool MSR;      bool CX8;
		bool SEP;        bool CMOV;        bool CLFSH;    bool FXSR;
		bool FSGSBASE;   bool BMI1;        bool HLE;      bool BMI2;
		bool ERMS;       bool INVPCID;     bool RTM;      bool RDSEED;
		bool ADX;        bool PREFETCHWT1; bool LAHF;     bool ABM;
		bool XOP;        bool TBM;         bool SYSCALL;  bool RDTSCP;
	};

	uint64_t        InstalledMemorySizeBytes(); // Get the size of installed RAM available to the OS, in bytes.
	CPUCapabilities CPUInfo();                  // Get a structure describing the capabilities/intrinsics of the CPU.
	std::string     SystemInfoString();         // Get a string with formatted, human-readable text showing info on the running system.

	namespace Paths {

		std::filesystem::path CacheDirectory   (std::string appName); // Uses cfgpath to get storage locations for cache data.
		std::filesystem::path UserDataDirectory(std::string appName); // Uses cfgpath to get storage locations for program data
		std::filesystem::path LibraryDirectory (std::string appName); // Returns a good location for a default library.

		// A platform-independent function for getting the full path to the running executable.
		// This is based on code from the PathFind GitHub project by Brian Kloppenborg.
		// This is only tested on Linux and Windows so far and I had to make some modifications to get it 
		// working there so don't expect perfection on other platforms without testing.
		std::filesystem::path ThisExecutableFile();
		std::filesystem::path ThisExecutableDirectory();
	}
}
