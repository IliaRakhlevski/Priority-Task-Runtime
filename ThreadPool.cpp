#include "ThreadPool.h"
using namespace std;


ThreadPool::ThreadPool(size_t threadCount)
{
	for (size_t i = 0; i < threadCount; i++)
	{
		workers_.emplace_back([this]
			{
				workerLoop();
			});
	}
}


void ThreadPool::workerLoop()
{
	Task task;

	while (queue_.pop(task))
	{
		task();
	}
}


ThreadPool::~ThreadPool()
{
	queue_.shutdown();

	for (thread& t : workers_)
	{
		if (t.joinable())
			t.join();
	}
}

