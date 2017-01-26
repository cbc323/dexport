#ifndef __DEXPORT_THREAD_POOL_H__
#define __DEXPORT_THREAD_POOL_H__

#include <queue>
#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <stdexcept>

namespace dexport {
	class ThreadPool;

	class Worker {
		friend class ThreadPool;
		private:
			std::thread _thread;
			ThreadPool& _parent;
			void _popAndRun(std::unique_lock<std::mutex>& lk);

		public:
			Worker(ThreadPool& pool);
			void operator()();

	};

	class ThreadPool {
		friend class Worker;
		private:
			std::vector<Worker> _threads;

			std::mutex _workMutex;
			std::condition_variable _workEvent;
			std::queue<std::function<void()>> _work;

			bool _running = false;

		public:
			ThreadPool(size_t poolSize);

			template<typename CallableType>
			void putWork(CallableType&& callable) {
				if(_threads.empty()) {
					throw std::runtime_error("Thread pool is empty");
				}

				if(_running == false) {
					throw std::runtime_error("Thread pool is not running");
				}

				std::lock_guard<std::mutex> lk(_workMutex);
				_work.push(std::move(callable));
				_workEvent.notify_one();
			}

			bool isRunning();
			void shutdown();
			void join();
	};
}


#endif
