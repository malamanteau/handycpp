
/// =============================================================================
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
/// =============================================================================

/// https://docs.microsoft.com/en-us/visualstudio/debugger/how-to-set-a-thread-name-in-native-code?view=vs-2019
/// https://ofekshilon.com/2009/04/10/naming-threads/

#ifdef _WIN32
	#include <windows.h>
	const DWORD MS_VC_EXCEPTION = 0x406D1388;

	#pragma pack(push,8)
	typedef struct tagTHREADNAME_INFO
	{
		DWORD dwType; // Must be 0x1000.
		LPCSTR szName; // Pointer to name (in user addr space).
		DWORD dwThreadID; // Thread ID (-1=caller thread).
		DWORD dwFlags; // Reserved for future use, must be zero.
	} THREADNAME_INFO;
	#pragma pack(pop)


	void SetThreadName(std::thread & thread, const char * threadName)
	{
		#if defined HANDY_WINDOWS_10_ONLY
		DWORD dwVersion = 0; 
		DWORD dwMajorVersion = 0;
		DWORD dwMinorVersion = 0; 
		DWORD dwBuild = 0;

		dwVersion = GetVersion();

		// Get the Windows version.

		dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
		dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

		// Get the build number.

		if (dwVersion < 0x80000000)              
			dwBuild = (DWORD)(HIWORD(dwVersion));

		if (dwMajorVersion > 10 || (dwMajorVersion == 10 && dwBuild >= 1607))
		{
			/// This only works for VS2017+ on Windows 10+
			SetThreadDescription(static_cast<HANDLE>(thread.native_handle()), HANDY_NS::ToWString(std::string(threadName)).c_str());
		}
		else
		#endif
		{
			/// This works on all versions of VS and Windows, but only works if the debugger is attached for the call.
			DWORD threadId = ::GetThreadId(static_cast<HANDLE>(thread.native_handle()));
			THREADNAME_INFO info;
			info.dwType     = 0x1000;
			info.szName     = threadName;
			info.dwThreadID = threadId;
			info.dwFlags    = 0;
			#pragma warning(push)
			#pragma warning(disable: 6320 6322)
			__try
			{
				RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{

			}
			#pragma warning(pop)
		}
	}

#else
	void SetThreadName(std::thread & thread, const char * threadName)
	{
		auto handle = thread.native_handle();
		pthread_setname_np(handle, threadName);
	}

	#include <sys/prctl.h>
	void SetThreadName(const char* threadName)
	{
		prctl(PR_SET_NAME, threadName, 0, 0, 0);
	}

#endif

void HANDY_NS::NameThread(std::thread & t, char const * name)
{
	SetThreadName(t, name);
}

/// From: https://gist.github.com/yni3/307207dc3732b59b89e6ec63bbcfb621
/// License : Public Domain
void HANDY_NS::AssignThreadPriority(std::thread & t, ThreadPriority tp)
{
	#if defined(IS_WINDOWS)

		switch (tp)
		{
			case ThreadPriority::Level0_Idle:     SetThreadPriority(t.native_handle(), THREAD_PRIORITY_IDLE);          break;
			case ThreadPriority::Level1_Lowest:   SetThreadPriority(t.native_handle(), THREAD_PRIORITY_LOWEST);        break;
			case ThreadPriority::Level2_Low:      SetThreadPriority(t.native_handle(), THREAD_PRIORITY_BELOW_NORMAL);  break;
			case ThreadPriority::Level3_Normal:   SetThreadPriority(t.native_handle(), THREAD_PRIORITY_NORMAL);        break;
			case ThreadPriority::Level4_High:     SetThreadPriority(t.native_handle(), THREAD_PRIORITY_ABOVE_NORMAL);  break;
			case ThreadPriority::Level5_Highest:  SetThreadPriority(t.native_handle(), THREAD_PRIORITY_HIGHEST);       break;
			case ThreadPriority::Level6_RealTime: SetThreadPriority(t.native_handle(), THREAD_PRIORITY_TIME_CRITICAL); break;
		}

	#else

		int min = sched_get_priority_min(SCHED_OTHER);
		int max = sched_get_priority_max(SCHED_OTHER);
		int normal = (max    + min) / 2;
		int low    = (normal + min) / 2;
		int high   = (normal + max) / 2;

		sched_param sch_params;

		switch (tp)
		{
			case ThreadPriority::Level0_Idle:    
			case ThreadPriority::Level1_Lowest:   sch_params.sched_priority = min;    break;
			case ThreadPriority::Level2_Low:      sch_params.sched_priority = low;    break;
			case ThreadPriority::Level3_Normal:   sch_params.sched_priority = normal; break;
			case ThreadPriority::Level4_High:     sch_params.sched_priority = high;   break;
			case ThreadPriority::Level5_Highest:  
			case ThreadPriority::Level6_RealTime: sch_params.sched_priority = max;    break;
		}
		if (pthread_setschedparam(t.native_handle(), SCHED_OTHER, &sch_params))
			throw std::runtime_error("Failed to AssignThreadPriority");

	#endif
}

void HANDY_NS::AssignCurrentThreadPriority(ThreadPriority tp)
{
	#if defined(IS_WINDOWS)

		switch (tp)
		{
			case ThreadPriority::Level0_Idle:     SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_IDLE);          break;
			case ThreadPriority::Level1_Lowest:   SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_LOWEST);        break;
			case ThreadPriority::Level2_Low:      SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_BELOW_NORMAL);  break;
			case ThreadPriority::Level3_Normal:   SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_NORMAL);        break;
			case ThreadPriority::Level4_High:     SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);  break;
			case ThreadPriority::Level5_Highest:  SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_HIGHEST);       break;
			case ThreadPriority::Level6_RealTime: SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL); break;
		}

	#else

		int min = sched_get_priority_min(SCHED_OTHER);
		int max = sched_get_priority_max(SCHED_OTHER);
		int normal = (max    + min) / 2;
		int low    = (normal + min) / 2;
		int high   = (normal + max) / 2;

		sched_param sch_params;

		switch (tp)
		{
			case ThreadPriority::Level0_Idle:    
			case ThreadPriority::Level1_Lowest:   sch_params.sched_priority = min;    break;
			case ThreadPriority::Level2_Low:      sch_params.sched_priority = low;    break;
			case ThreadPriority::Level3_Normal:   sch_params.sched_priority = normal; break;
			case ThreadPriority::Level4_High:     sch_params.sched_priority = high;   break;
			case ThreadPriority::Level5_Highest:  
			case ThreadPriority::Level6_RealTime: sch_params.sched_priority = max;    break;
		}
		if (pthread_setschedparam(pthread_self(), SCHED_OTHER, &sch_params))
			throw std::runtime_error("Failed to AssignThreadPriority");

	#endif
}