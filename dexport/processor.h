#include <tsk/libtsk.h>
#include <memory>
#include <vector>

namespace dexport {
	class Processor : public TskAuto {
		public:
			Processor(const std::vector<const char *>& image);
			virtual TSK_RETVAL_ENUM processFile(TSK_FS_FILE * fs_file, const char * path);
	};
}
