#ifndef __DEXPORT_CONCURRENT_TASK_H__
#define __DEXPORT_CONCURRENT_TASK_H__

#include <atomic>

namespace dexport {
	class ConcurrentTaskTracker {
		private:
			std::atomic<uint64_t> _tasksOut;

		public:
			ConcurrentTaskTracker();
			uint64_t& operator++(int add);
			uint64_t operator++(uint64_t add, int);


	};


	class ConcurrentTask {
		public:
			virtual void run() = 0;
	};
}
#endif
