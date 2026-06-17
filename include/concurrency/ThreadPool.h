#ifndef THREAD_POOL
#define THREAD_POOL

#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>

class ThreadPool {
public:
	ThreadPool(size_t numThreads = std::thread::hardware_concurrency());
	~ThreadPool();
	void enqueue(std::function<void()> task);
	size_t getThreadCount() const { return threads.size(); }
private:
	std::vector<std::thread> threads;
	std::queue<std::function<void()>> tasks;
	std::mutex mutex;
	std::condition_variable condition;
	bool stop = false;
};

#endif