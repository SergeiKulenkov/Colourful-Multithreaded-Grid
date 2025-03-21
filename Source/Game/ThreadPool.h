#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <atomic>

////////////////////

class ThreadPool
{
public:
	ThreadPool();
	~ThreadPool();

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool &operator =(const ThreadPool&) = delete;

	uint8_t GetNumberOfThreads() { return m_NumberOfThreads; }

	template<typename F>
	void AddTask(F&& task)
	{
		std::unique_lock<std::mutex> tasksLock(m_Mutex);
		m_Tasks.emplace(std::forward<F>(task));
		tasksLock.unlock();
		m_ConditionVariable.notify_one();
	}

	void Stop();

private:
	uint8_t m_NumberOfThreads;
	std::atomic<bool> m_IsShuttingDown = false;

	std::vector<std::thread> m_Threads;
	std::queue<std::function<void()>> m_Tasks;
	std::mutex m_Mutex;
	std::condition_variable m_ConditionVariable;
};