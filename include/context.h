#ifndef __DEXPORT_CONTEXT_H__
#define __DEXPORT_CONTEXT_H__

#include <string>
#include <vector>
#include <atomic>
#include <cstdint>

#include "file_magic.h"
#include "thread_pool.h"

namespace dexport {
	class Context {
		private:
			std::vector<std::string> _archiveMimes;
			ThreadPool tpool;

		public:
			Context(const std::string& mimesPath, size_t numThreads);
			const std::vector<std::string>& archiveMimes();
			ThreadPool& getThreadPool();
	};
}

#endif
