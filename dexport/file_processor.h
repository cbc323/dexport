#ifndef __DEXPORT_FILE_PROCESSOR_H__
#define __DEXPORT_FILE_PROCESSOR_H__

#include <tsk/libtsk.h>
#include <memory>

#include "file.h"

namespace dexport {
	class FileProcessor {
		private:
			std::shared_ptr<File> _file;

		public:
			FileProcessor(std::shared_ptr<File> file);

			void operator()();
	};
}

#endif
