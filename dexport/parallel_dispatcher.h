#include <tsk/libtsk.h>
#include <queue>
#include <vector>
#include <thread>

#include "dispatcher.h"

namespace dexport {
	class ParallelDispatcher : public Dispatcher {
		private:
			struct fs_file_info {
				TSK_FS_FILE * fsFile;
				const char * fsPath;
			};

			std::queue<fs_file_info> tasks;
			std::vector<std::thread> threads;
			
		public:
			ParallelDispatcher(size_t num_threads);
			virtual void dispatchFile(TSK_FS_FILE * fsFile, const char * path);
	};
}
