
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

#include <vector>
#include <deque>
#include <chrono>

#include "HandyBase.hpp"

//#if !defined __cplusplus_cli
#ifndef IS_CLI

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace HANDY_NS {

	enum class ThreadPriority : int32_t
	{
		Level0_Idle     = -3, /// Special meaning in win32, otherwise same as Level1_Lowest
		Level1_Lowest   = -2,
		Level2_Low      = -1,
		Level3_Normal   =  0,
		Level4_High     =  1,
		Level5_Highest  =  2,
		Level6_RealTime =  3  /// Special meaning in win32, otherwise same as Level5_Highest
	};

	void NameThread(std::thread & t, char const * name);
	void AssignThreadPriority(std::thread & t, ThreadPriority tp);
	void AssignCurrentThreadPriority(ThreadPriority tp);

	///-----------------------------------------------
	/// Threadpool Implementation. 
	/// Simple ThreadPool that creates `threadCount` threads upon its creation,
	/// and pulls from a collection to get new jobs. The default is std::thread::hardware_concurrency() threads.
	/// https://github.com/stfx/ThreadPool2
	class ThreadPool final
	{
		size_t const                      m_threadCount;

		std::vector<std::thread>          m_threads;
		std::deque<std::tuple<std::function<void()>, uint64_t>> m_queue; //Each item is of the form <Function, token>. Protected by m_queueMutex
		uint64_t nextToken = 0U; //Protected by m_queueMutex

		int                     m_jobsLeft;
		bool                    m_bailout;
		std::condition_variable m_jobAvailableVar;
		std::condition_variable m_waitVar;
		std::mutex              m_jobsLeftMutex; //Is there a reason m_jobsLeft isn't just an atomic<int>?
		std::mutex              m_queueMutex;

		// Take the next job in the queue and run it. Notify the main thread that a job has completed.
		void Task()
		{
			while (true)
			{
				std::tuple<std::function<void()>, uint64_t> job;

				{
					std::unique_lock<std::mutex> lock(m_queueMutex);

					// Wait for a job if we don't have any.
					m_jobAvailableVar.wait(lock,
						[this] { return !m_queue.empty() || m_bailout; });

					if (m_bailout)
						return;

					job = m_queue.front(); // Get job from the queue
					m_queue.pop_front();
				}

				std::get<0>(job)(); //Execute the job

				{
					std::lock_guard<std::mutex> lock(m_jobsLeftMutex);
					--m_jobsLeft;
				}

				m_waitVar.notify_all();
			}
		}

	public:

		void SetThreadNames(std::string name)
		{
			for (size_t index = 0; index < m_threadCount; ++index)
			{
				auto & thread = m_threads.at(index);
				std::string tName = name + " "s + std::to_string(index);
				NameThread(thread, tName.c_str());
			}
		}

		void SetThreadPriorities(Handy::ThreadPriority priority)
		{
			for (size_t index = 0; index < m_threadCount; ++index)
				AssignThreadPriority(m_threads.at(index), priority);
		}

		explicit ThreadPool(size_t threadCount = size_t(std::thread::hardware_concurrency()))
			: m_threadCount(threadCount)
			, m_jobsLeft(0)
			, m_bailout(false)
		{
			m_threads.reserve(m_threadCount);
			for (size_t index = 0; index < m_threadCount; ++index)
				m_threads.emplace_back([this] { this->Task(); });

			SetThreadNames("ThreadPool");
		}

		~ThreadPool()
		{
			Join();
		}

		// Get the number of threads in this pool
		size_t Size() const
		{
			return m_threadCount;
		}

		// Add a new job to the pool. If there are no jobs in the queue, a thread is woken up to take the job. 
		// If all threads are busy, the job is added to the end of the queue.
		// Returns a token that can be used to refer to this job in the future (e.g. to remove it from the queue)
		uint64_t AddJob(const std::function<void()> & job)
		{
			uint64_t newJobToken = 0U;
			{
				std::lock_guard<std::mutex> lock(m_queueMutex);
				newJobToken = nextToken;
				m_queue.emplace_back(std::make_tuple(job, nextToken++));
			}
			{
				std::lock_guard<std::mutex> lock(m_jobsLeftMutex);
				++m_jobsLeft;
			}
			m_jobAvailableVar.notify_one();
			return newJobToken;
		}

		//functionally equivalent to WaitJob(AddJob(job))
		void AddWaitJob(const std::function<void()> & job)
		{
			WaitJob(AddJob(job));


















			/// FIXME: WaitJob does not work as expected. This is a dumb hack, and will not have ideal performance.
			Wait();
		}

		// Add a new job to the pool. If there are no jobs in the queue, a thread is woken up to take the job. 
		// If all threads are busy, the job is added to the front of the queue.
		// Returns a token that can be used to refer to this job in the future (e.g. to remove it from the queue)
		uint64_t AddJobToFront(const std::function<void()> & job)
		{
			uint64_t newJobToken = 0U;
			{
				std::lock_guard<std::mutex> lock(m_queueMutex);
				newJobToken = nextToken;
				m_queue.emplace_front(std::make_tuple(job, nextToken++));
			}
			{
				std::lock_guard<std::mutex> lock(m_jobsLeftMutex);
				++m_jobsLeft;
			}
			m_jobAvailableVar.notify_one();
			return newJobToken;
		}

		//functionally equivalent to WaitJob(AddJobToFront(job))
		void AddWaitJobToFront(const std::function<void()> & job)
		{
			WaitJob(AddJobToFront(job));
		}

		//Try to remove a job with the given token from the job queue. If the job is not found (including if it is currently executing), this does nothing.
		//Returns true if the job was found and removed and false otherwise.
		bool RemoveJob(uint64_t JobToken)
		{
			bool JobRemoved = false;
			{
				std::lock_guard<std::mutex> lock(m_queueMutex);
				for (auto iter = m_queue.begin(); iter != m_queue.end(); iter++)
				{
					if (std::get<1>(*iter) == JobToken) {
						//We found the job - remove it
						m_queue.erase(iter); //iter is now invalid
						JobRemoved = true;
						break;
					}
				}
			}
			if (JobRemoved)
			{
				std::lock_guard<std::mutex> lock(m_jobsLeftMutex);
				m_jobsLeft--;
			}
			//No need to notify threads since this never adds work; it can only remove a job
			return JobRemoved;
		}

		//Remove all jobs currently enqueued. This does not affect any jobs that are already underway.
		void ClearJobQueue()
		{
			//One subtlety: Worker threads can remove a job from the queue and decrement m_jobsLeft later (they are locked with different mutexes).
			//Thus, if we just set m_jobsLeft to 0, we could bring it out of sync with the state of the pool. Instead, we decrement by the number of jobs removed.
			std::lock_guard<std::mutex> lock1(m_queueMutex);
			std::lock_guard<std::mutex> lock2(m_jobsLeftMutex);
			int jobsRemoved = int(m_queue.size());
			m_queue.clear();
			m_jobsLeft -= jobsRemoved; //Don't just set to 0
		}

		// Join with all threads. Block until all threads have completed. The queue may be filled after this call, but the threads will
		// be done. After invoking `ThreadPool::Join`, the pool can no longer be used. This method IS NOT safe to call at the same time as AddJob*(). 
		void Join()
		{
			Wait();
			{
				std::lock_guard<std::mutex> lock(m_queueMutex);
				if (m_bailout)
					return;
				m_bailout = true;
			}

			m_jobAvailableVar.notify_all(); // note that we're done, and wake up any thread that's waiting for a new job

			for (std::thread & thread : m_threads)
				if (thread.joinable())
					thread.join();
		}

		// Wait for the pool to empty before continuing. This does not call `std::thread::join`, it only waits until
		// all jobs have finished executing.
		void Wait()
		{
			std::unique_lock<std::mutex> lock(m_jobsLeftMutex);
			if (m_jobsLeft > 0)
			{
				m_waitVar.wait(lock, [this]
				{
					return m_jobsLeft == 0;
				});
			}
		}

		// Wait for the pool to empty (up to number of seconds) before continuing. This does not call `std::thread::join`, it only waits until
		// all jobs have finished executing or the time has passed. Returns false if timed out.
		template<class Tr, class Tp>
		bool WaitFor(std::chrono::duration<Tr, Tp> const & duration)
		{
			std::unique_lock<std::mutex> lock(m_jobsLeftMutex);
			if (m_jobsLeft > 0)
			{
				m_waitVar.wait_for(lock, duration, [this]
				{
					return m_jobsLeft == 0;
				});
			}

			return m_jobsLeft == 0;
		}

		// Wait for the pool to empty (up to the given timepoint) before continuing. This does not call `std::thread::join`, it only waits until
		// all jobs have finished executing or the timepoint has been hit. Returns false if timed out.
		template<class Tc, class Td>
		bool WaitUntil(std::chrono::time_point<Tc, Td> const & timePoint)
		{
			std::unique_lock<std::mutex> lock(m_jobsLeftMutex);
			if (m_jobsLeft > 0)
			{
				m_waitVar.wait_until(lock, timePoint, [this]
				{
					return m_jobsLeft == 0;
				});
			}

			return m_jobsLeft == 0;
		}

		//Wait for a job to finish, don't call this if you have a huge number of jobs. It just does a linear search for your ID every time a job is completed.
		void WaitJob(uint64_t JobToken)
		{
			std::unique_lock<std::mutex> lock(m_queueMutex);
			if (m_jobsLeft > 0)
			{
				m_waitVar.wait(lock, [this, JobToken]
				{
					for (auto iter = m_queue.begin(); iter != m_queue.end(); iter++)
						if (std::get<1>(*iter) == JobToken)
							return false; //We found the job

					return true;
				});
			}
		}

		// Get the number of jobs left in the queue.
		size_t JobsRemaining()
		{
			std::lock_guard<std::mutex> lock(m_jobsLeftMutex);

			return m_jobsLeft;
		}

		// Get the vector of threads themselves, in order to set the affinity, or anything else you might want to do
		std::vector<std::thread> & GetThreads()
		{
			return m_threads;
		}
	};

	template <typename T>
	class WorkPool final
	{
		size_t const                        m_threadCount;

		std::vector<std::thread>            m_threads;
		std::function<void(T task)>         m_task;

		htd::deque<std::tuple<T, uint64_t>> m_queue; //Each item is of the form <Function, token>. Protected by m_queueMutex
		uint64_t nextToken = 0U; //Protected by m_queueMutex

		int                     m_jobsLeft;
		bool                    m_bailout;
		std::condition_variable m_jobAvailableVar;
		std::condition_variable m_waitVar;
		std::mutex              m_jobsLeftMutex; //Is there a reason m_jobsLeft isn't just an atomic<int>?
		std::mutex              m_queueMutex;

		// Take the next job in the queue and run it. Notify the main thread that a job has completed.
		void Task()
		{
			while (true)
			{
				std::tuple<T, uint64_t> job;

				{
					std::unique_lock<std::mutex> lock(m_queueMutex);

					// Wait for a job if we don't have any.
					m_jobAvailableVar.wait(lock,
										   [this] { return !m_queue.empty() || m_bailout; });

					if (m_bailout)
						return;

					job = m_queue.front(); // Get job from the queue
					m_queue.pop_front();
				}

				m_task(std::get<0>(job)); //Execute the job

				{
					std::lock_guard<std::mutex> lock(m_jobsLeftMutex);
					--m_jobsLeft;
				}

				m_waitVar.notify_all();
			}
		}

	public:

		void SetThreadNames(std::string name)
		{
			for (size_t index = 0; index < m_threadCount; ++index)
			{
				auto & thread = m_threads.at(index);
				std::string tName = name + " "s + std::to_string(index);
				NameThread(thread, tName.c_str());
			}
		}

		void SetThreadPriorities(Handy::ThreadPriority priority)
		{
			for (size_t index = 0; index < m_threadCount; ++index)
				AssignThreadPriority(m_threads.at(index), priority);
		}

		explicit WorkPool(size_t threadCount = size_t(std::thread::hardware_concurrency()))
			: m_threadCount(threadCount)
			, m_task([] (T t) {})
			, m_jobsLeft(0)
			, m_bailout(false)
		{
			m_threads.reserve(m_threadCount);
			for (size_t index = 0; index < m_threadCount; ++index)
				m_threads.emplace_back([this] { this->Task(); });

			SetThreadNames("WorkPool");
		}

		~WorkPool()
		{
			Join();
		}

		/// Queue MUST be empty before calling this function. If any jobs are queued or running, the result is
		/// undefined.
		void SetTask(std::function<void(T task)> task)
		{
			m_task = task;
		}

		// Get the number of threads in this pool
		size_t Size() const
		{
			return m_threadCount;
		}

		// Add a new job to the pool. If there are no jobs in the queue, a thread is woken up to take the job. 
		// If all threads are busy, the job is added to the end of the queue.
		// Returns a token that can be used to refer to this job in the future (e.g. to remove it from the queue)
		uint64_t AddJob(T job)
		{
			uint64_t newJobToken = 0U;
			{
				std::lock_guard<std::mutex> lock(m_queueMutex);
				newJobToken = nextToken;
				m_queue.emplace_back(std::make_tuple(job, nextToken++));
			}
			{
				std::lock_guard<std::mutex> lock(m_jobsLeftMutex);
				++m_jobsLeft;
			}
			m_jobAvailableVar.notify_one();
			return newJobToken;
		}
		
		//functionally equivalent to WaitJob(AddJob(job))
		void AddWaitJob(T job)
		{
			WaitJob(AddJob(job));
		}

		// Add a new job to the pool. If there are no jobs in the queue, a thread is woken up to take the job. 
		// If all threads are busy, the job is added to the front of the queue.
		// Returns a token that can be used to refer to this job in the future (e.g. to remove it from the queue)
		uint64_t AddJobToFront(T job)
		{
			uint64_t newJobToken = 0U;
			{
				std::lock_guard<std::mutex> lock(m_queueMutex);
				newJobToken = nextToken;
				m_queue.emplace_front(std::make_tuple(job, nextToken++));
			}
			{
				std::lock_guard<std::mutex> lock(m_jobsLeftMutex);
				++m_jobsLeft;
			}
			m_jobAvailableVar.notify_one();
			return newJobToken;
		}

		//functionally equivalent to WaitJob(AddJobToFront(job))
		void AddWaitJobToFront(T job)
		{
			WaitJob(AddJobToFront(job));
		}

		//Try to remove a job with the given token from the job queue. If the job is not found (including if it is currently executing), this does nothing.
		//Returns true if the job was found and removed and false otherwise.
		bool RemoveJob(uint64_t JobToken)
		{
			bool JobRemoved = false;
			{
				std::lock_guard<std::mutex> lock(m_queueMutex);
				for (auto iter = m_queue.begin(); iter != m_queue.end(); iter++) {
					if (std::get<1>(*iter) == JobToken) {
						//We found the job - remove it
						m_queue.erase(iter); //iter is now invalid
						JobRemoved = true;
						break;
					}
				}
			}
			if (JobRemoved)
			{
				std::lock_guard<std::mutex> lock(m_jobsLeftMutex);
				m_jobsLeft--;
			}
			//No need to notify threads since this never adds work; it can only remove a job
			return JobRemoved;
		}

		//Try to remove a job with the given token from the job queue. If the job is not found (including if it is currently executing), this does nothing.
		//Returns true if the job was found and removed and false otherwise.
		bool RemoveJob(T job)
		{
			bool JobRemoved = false;
			{
				std::lock_guard<std::mutex> lock(m_queueMutex);
				for (auto iter = m_queue.begin(); iter != m_queue.end(); iter++) {
					if (std::get<0>(*iter) == job) {
						//We found the job - remove it
						m_queue.erase(iter); //iter is now invalid
						JobRemoved = true;
						break;
					}
				}
			}
			if (JobRemoved)
			{
				std::lock_guard<std::mutex> lock(m_jobsLeftMutex);
				m_jobsLeft--;
			}
			//No need to notify threads since this never adds work; it can only remove a job
			return JobRemoved;
		}

		//Remove all jobs currently enqueued. This does not effect any jobs that are already underway.
		void ClearJobQueue()
		{
			//One subtlety: Worker threads can remove a job from the queue and decrement m_jobsLeft later (they are locked with different mutexes).
			//Thus, if we just set m_jobsLeft to 0, we could bring it out of sync with the state of the pool. Instead, we decrement by the number of jobs removed.
			std::lock_guard<std::mutex> lock1(m_queueMutex);
			std::lock_guard<std::mutex> lock2(m_jobsLeftMutex);
			int jobsRemoved = int(m_queue.size());
			m_queue.clear();
			m_jobsLeft -= jobsRemoved; //Don't just set to 0
		}

		// Join with all threads. Block until all threads have completed. The queue may be filled after this call, but the threads will
		// be done. After invoking `ThreadPool::Join`, the pool can no longer be used. This method IS NOT safe to call at the same time as AddJob*(). 
		void Join()
		{
			Wait();
			{
				std::lock_guard<std::mutex> lock(m_queueMutex);
				if (m_bailout)
					return;
				m_bailout = true;
			}

			m_jobAvailableVar.notify_all(); // note that we're done, and wake up any thread that's waiting for a new job

			for (std::thread & thread : m_threads)
				if (thread.joinable())
					thread.join();
		}

		// Wait for the pool to empty before continuing. This does not call `std::thread::join`, it only waits until
		// all jobs have finished executing.
		void Wait()
		{
			std::unique_lock<std::mutex> lock(m_jobsLeftMutex);
			if (m_jobsLeft > 0)
			{
				m_waitVar.wait(lock, [this]
				{
					return m_jobsLeft == 0;
				});
			}
		}

		// Wait for the pool to empty (up to number of seconds) before continuing. This does not call `std::thread::join`, it only waits until
		// all jobs have finished executing or the time has passed. Returns false if timed out.
		template<class Tr, class Tp>
		bool WaitFor(std::chrono::duration<Tr, Tp> const & duration)
		{
			std::unique_lock<std::mutex> lock(m_jobsLeftMutex);
			if (m_jobsLeft > 0)
			{
				m_waitVar.wait_for(lock, duration, [this]
				{
					return m_jobsLeft == 0;
				});
			}

			return m_jobsLeft == 0;
		}

		// Wait for the pool to empty (up to the given timepoint) before continuing. This does not call `std::thread::join`, it only waits until
		// all jobs have finished executing or the timepoint has been hit. Returns false if timed out.
		template<class Tc, class Td>
		bool WaitUntil(std::chrono::time_point<Tc, Td> const & timePoint)
		{
			std::unique_lock<std::mutex> lock(m_jobsLeftMutex);
			if (m_jobsLeft > 0)
			{
				m_waitVar.wait_until(lock, timePoint, [this]
				{
					return m_jobsLeft == 0;
				});
			}

			return m_jobsLeft == 0;
		}

		//Wait for a job to finish, don't call this if you have a huge number of jobs. It just does a linear search for your ID every time a job is completed.
		void WaitJob(uint64_t JobToken)
		{
			std::unique_lock<std::mutex> lock(m_queueMutex);
			if (m_jobsLeft > 0)
			{
				m_waitVar.wait(lock, [this, JobToken]
				{
					for (auto iter = m_queue.begin(); iter != m_queue.end(); iter++)
						if (std::get<1>(*iter) == JobToken)
							return false; //We found the job

					return true;
				});
			}
		}

		// Get the number of jobs left in the queue.
		size_t JobsRemaining()
		{
			std::lock_guard<std::mutex> lock(m_jobsLeftMutex);

			return m_jobsLeft;
		}

		// Get the vector of threads themselves, in order to set the affinity, or anything else you might want to do
		std::vector<std::thread> & GetThreads()
		{
			return m_threads;
		}
	};
	///-----------------------------------------------

	/// From: https://stackoverflow.com/questions/29775153/stopping-long-sleep-threads
	struct InterruptibleSleep
	{
		// returns false if killed:
		template<class R, class P>
		bool sleep_for(std::chrono::duration<R,P> const & time) const
		{
			std::unique_lock<std::mutex> lock(m);
			wake = false;
			return !cv.wait_for(lock, time, [&] { return wake; });
		}

		void interrupt() const
		{
			std::unique_lock<std::mutex> lock(m);
			wake = true; // should be modified inside mutex lock
			cv.notify_all(); // it is safe, and *sometimes* optimal, to do this outside the lock
		}
		
		InterruptibleSleep()                                       = default; 
		InterruptibleSleep(InterruptibleSleep &&)                  = delete;
		InterruptibleSleep(InterruptibleSleep const &)             = delete;
		InterruptibleSleep & operator=(InterruptibleSleep &&)      = delete;
		InterruptibleSleep & operator=(InterruptibleSleep const &) = delete;
	private:
		mutable std::condition_variable cv;
		mutable std::mutex m;
		mutable bool wake = false;
	};

} // HANDY_NS


#endif
