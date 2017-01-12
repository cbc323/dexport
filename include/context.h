#ifndef __DEXPORT_CONTEXT_H__
#define __DEXPORT_CONTEXT_H__

#include <string>
#include <vector>
#include <atomic>
#include <cstdint>

#include "work_queue.h"
#include "file_magic.h"

namespace dexport {
	class Context {
		private:
			WorkQueue _workq;
			std::vector<std::string> _archiveMimes;

		public:
			Context(const std::string& mimesPath);
			WorkQueue& workq();
			const std::vector<std::string>& archiveMimes();
	};
}

#endif
