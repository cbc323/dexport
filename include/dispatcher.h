#include<tsk/libtsk.h>

namespace dexport {
	class Dispatcher {
		public:
			virtual void dispatchFile(TSK_FS_FILE * fsFile, const char * path) = 0;
	};
}
