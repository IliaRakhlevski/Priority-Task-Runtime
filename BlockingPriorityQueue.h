#pragma once
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <stdexcept>


enum class Priority
{
    Low,
    Normal,
    High
};


using Task = std::function<void()>;

class BlockingPriorityQueue
{

public:

    BlockingPriorityQueue() = default;
    BlockingPriorityQueue(const BlockingPriorityQueue&) = delete;
    BlockingPriorityQueue& operator=(const BlockingPriorityQueue&) = delete;

    void push(Task task, Priority priority);
    bool pop(Task& task);
    void shutdown();

private:
    std::queue<Task> high_;
    std::queue<Task> normal_;
    std::queue<Task> low_;

    std::mutex mutex_;
    std::condition_variable cv_;
    bool stopped_ = false;

};

