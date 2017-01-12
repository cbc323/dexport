#ifndef __DEXPORT_FILE_EXTRACTOR_H__
#define __DEXPORT_FILE_EXTRACTOR_H__

#include <tsk/libtsk.h>
#include <memory>

#include "context.h"
#include "file.h"

namespace dexport {
	class FileExtractor {
		private:
			std::shared_ptr<File> _file;
			Context& _context;
			bool started;

		public:
			FileExtractor(std::shared_ptr<File> file, Context& context);
			virtual ~FileExtractor();

			void operator()();
	};
}

#endif
