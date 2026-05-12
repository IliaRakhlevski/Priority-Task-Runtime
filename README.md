# Priority-Task-Runtime

Modern C++ concurrency project featuring a priority-based thread pool,
futures, condition variables, mutexes, and atomic synchronization.

---

## Features

* Priority-based task scheduling
* Multi-threaded worker pool
* Blocking task queue
* High / Normal / Low task priorities
* Future-based asynchronous results
* Variadic template task submission
* Graceful shutdown
* Thread-safe synchronization using:

  * std::mutex
  * std::condition_variable
  * std::future
  * std::packaged_task
  * std::atomic

---

## Architecture

submit(task)
↓
priority queue
↓
worker threads
↓
task execution
↓
future result

---

## Example

```cpp
ThreadPool pool(4);

auto future = pool.submit(
    Priority::High,
    [](int a, int b)
    {
        return a + b;
    },
    2,
    3
);

std::cout << future.get() << std::endl;
```

---

## Example Output

```text
HIGH 0
HIGH 1
NORMAL 0
LOW 0
LOW 1
```

---

## Technologies

* C++20
* STL concurrency primitives
* Variadic templates
* Futures / packaged_task
* Multi-threading

---

## Future Improvements

* Starvation prevention
* Bounded blocking queues
* Dynamic worker scaling
* Work stealing
* Metrics and monitoring

```
```
