#pragma once
#include <functional>
#include <vector>
#include <thread>
#include <future>
#include "BlockingPriorityQueue.h"


class ThreadPool
{
public:

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

    explicit ThreadPool(size_t threadCount);

    ~ThreadPool();

	template<typename Func, typename... Args>
	auto submit(Priority priority, Func&& func, Args&&... args)
	{
		using ReturnType = std::invoke_result_t<Func, Args...>;

		auto task = std::make_shared<std::packaged_task<ReturnType()>>(
			[func = std::forward<Func>(func),
			... args = std::forward<Args>(args)]() mutable {
				return std::invoke(func, args...);
			}
		);

		std::future<ReturnType> result = task->get_future();

		queue_.push([task]() {
			(*task)();
			}, priority);

		return result;
	}

private:

    void workerLoop();

    BlockingPriorityQueue queue_;

    std::vector<std::thread> workers_;
};


