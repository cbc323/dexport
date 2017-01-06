#ifndef __DEXPORT_WORK_QUEUE_H__
#define __DEXPORT_WORK_QUEUE_H__

#include <thread>
#include <future>
#include <vector>
#include <algorithm>
#include <iostream>
#include <mutex>

namespace dexport {
	class WorkQueue {
		private:
			std::vector<std::future<void>> _futures;

			// because we don't know what thread we will be running in, and async procedures push work
			// onto this queue, we need to support recursive locks to this mutex
			std::recursive_mutex _futuresMutex;

			size_t _gcThreshold;
			bool _parallel = true;

		public:
			WorkQueue(size_t gcThreshold = 1000000) : _gcThreshold(gcThreshold) {}
			WorkQueue& operator=(const WorkQueue) = delete;
			WorkQueue(const WorkQueue&) = delete;

			void parallel(bool para) {
				_parallel = para;
			}

			template <typename Function>
			void async(Function func) {
				if(!_parallel) {
					func();
					return;
				}

				std::unique_lock<std::recursive_mutex> lck(_futuresMutex);
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
				std::for_each(_futures.begin(), _futures.end(), [](const std::future<void>& fut) {
					if(fut.valid()) {
						try {
							fut.wait();
						} catch(...) {
							std::cout << "exception in join" << std::endl;
						}
					}
				});
			}
	};
}
#endif
