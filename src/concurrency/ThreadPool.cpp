#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t numThreads) {
	for (size_t i = 0; i < numThreads; i++) {
		threads.emplace_back([this] {
			while (true) {
				std::function<void()> task;
				{
					std::unique_lock<std::mutex> lock(mutex);
					condition.wait(lock, [this] { return stop || !tasks.empty(); });
					if (stop && tasks.empty()) return;
					task = std::move(tasks.front());
					tasks.pop();
				}
				task();
			}
		});
	}
}

ThreadPool::~ThreadPool() {
	{
		std::unique_lock<std::mutex> lock(mutex);
		stop = true;
	}
	condition.notify_all();
	for (std::thread& thread : threads) {
		thread.join();
	}
}

void ThreadPool::enqueue(std::function<void()> task) {
	{
		std::unique_lock<std::mutex> lock(mutex);
		tasks.emplace(std::move(task));
	}
	condition.notify_one();
}