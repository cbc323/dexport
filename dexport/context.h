#include "work_queue.h"

namespace dexport {
	class Context {
		private:
			WorkQueue _workq;

		public:
			WorkQueue& workq();
	};
}
