#ifndef __DEXPORT_WORK_QUEUE_H__
#define __DEXPORT_WORK_QUEUE_H__

#include <thread>
#include <future>
#include <vector>
#include <algorithm>
#include <iostream>
#include <mutex>
#include <atomic>

namespace dexport {
	class WorkQueue {
		private:
			std::vector<std::future<void>> _futures;

			// because we don't know what thread we will be running in, and async procedures push work
			// onto this queue, we need to support recursive locks to this mutex
			std::recursive_mutex _futuresMutex;

			size_t _gcThreshold;
			bool _parallel = true;
			std::atomic<uint64_t> _inProgress;

		public:
			WorkQueue(size_t gcThreshold = 1) : _gcThreshold(gcThreshold), _inProgress(0) {}
			WorkQueue& operator=(const WorkQueue) = delete;
			WorkQueue(const WorkQueue&) = delete;


			bool hasWork() {
				return _inProgress.load() > 0;
			}


			std::atomic<uint64_t>& getInProgressCount() {
				return _inProgress;
			}


			void parallel(bool para) {
				_parallel = para;
			}


			template <typename Function>
			void async(Function &&func) {
				if(!_parallel) {
					func();
					return;
				}

				std::unique_lock<std::recursive_mutex> lck(_futuresMutex);
				_inProgress.fetch_add(1);
				_futures.push_back(std::async(std::launch::async, func));

				// clean up some of the futures that are complete
				if(_futures.size() >= _gcThreshold) {
					_futures.erase(
							std::remove_if(_futures.begin(), _futures.end(), [](const std::future<void>& fut) {
									if(!fut.valid()) { return true; }
									return fut.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
								}),
							_futures.end()
						);
				}
			}

			//TODO: Add a destructor that ensure all of the futures return

			void join() {
				std::unique_lock<std::recursive_mutex> lck(_futuresMutex);
				_futures.erase(
						std::remove_if(_futures.begin(), _futures.end(), [](const std::future<void>& fut) {
								if(!fut.valid()) { return true; }
								return fut.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
							}),
						_futures.end()
						);
			}
	};
}
#endif
