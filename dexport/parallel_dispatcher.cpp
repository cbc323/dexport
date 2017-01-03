#include <tsk/libtsk.h>
#include "parallel_dispatcher.h"

using namespace dexport;
using namespace std;

void ParallelDispatcher::dispatchFile(TSK_FS_FILE *fsFile, const char * path) {
	tasks.emplace(fs_file_info{fsFile, path});
}
