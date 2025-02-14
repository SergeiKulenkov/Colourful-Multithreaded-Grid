#include "ThreadPool.h"

////////////////////

ThreadPool::ThreadPool()
{
	m_NumberOfThreads = std::thread::hardware_concurrency();
	m_Threads.resize(m_NumberOfThreads);

	for (int i = 0; i < m_Threads.size(); i++)
	{
		m_Threads[i] = std::thread([this]
			{
				std::unique_lock<std::mutex> lock(m_Mutex);
				while (!m_IsShuttingDown ||
						(m_IsShuttingDown && !m_Tasks.empty()))
				{
					m_ConditionVariable.wait(lock, [this] { return m_IsShuttingDown || !m_Tasks.empty(); });
					if (!m_Tasks.empty())
					{
						auto task = std::move(m_Tasks.front());
						m_Tasks.pop();
						lock.unlock();
						task();
						lock.lock();
					}
				}
			});
	}
}

ThreadPool::~ThreadPool()
{
	m_IsShuttingDown = true;
	m_ConditionVariable.notify_all();
	for (std::thread& thread : m_Threads)
	{
		thread.join();
	}
}

void ThreadPool::Stop()
{
	std::queue<std::function<void()>> emptyQueue;
	std::swap(m_Tasks, emptyQueue);
}
