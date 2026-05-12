#include "BlockingPriorityQueue.h"
using namespace std;


void BlockingPriorityQueue::push(Task task, Priority priority)
{
	unique_lock<mutex> lock(mutex_);

	if (stopped_)
		throw std::runtime_error("Queue stopped");

	switch (priority)
	{
		case Priority::Low:
			low_.push(move(task));
			break;

		case Priority::Normal:
			normal_.push(move(task));
			break;

		case Priority::High:
			high_.push(move(task));
			break;
	}

	lock.unlock();
	cv_.notify_one();
}


bool BlockingPriorityQueue::pop(Task& task)
{
	unique_lock<mutex> lock(mutex_);

	cv_.wait(lock, [&]() { 
		return stopped_ || !low_.empty() || !normal_.empty() || !high_.empty();
	});

	if(stopped_ && low_.empty() && normal_.empty() && high_.empty())
		return false;

	if (!high_.empty())
	{
		task = move(high_.front());
		high_.pop();
	}
	else if (!normal_.empty())
	{
		task = move(normal_.front());
		normal_.pop();
	}
	else if (!low_.empty())
	{
		task = move(low_.front());
		low_.pop();
	}

	return true;
}


void BlockingPriorityQueue::shutdown()
{
	{
		unique_lock<mutex> lock(mutex_);
		stopped_ = true;
	}
	cv_.notify_all();
}