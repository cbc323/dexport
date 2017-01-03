#ifndef __DEXPORT_WORK_QUEUE_H__
#define __DEXPORT_WORK_QUEUE_H__
#include <thread>
#include <future>
#include <vector>
#include <algorithm>

namespace dexport {
	class WorkQueue {
		private:
			std::vector<std::future<void>> _futures;
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

			//TODO: Add a function that can be called to wait for all futures to complete
			//TODO: Add a destructor that ensure all of the futures return
	};
}
#endif
