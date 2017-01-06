#ifndef __DEXPORT_FILE_PROCESSOR_H__
#define __DEXPORT_FILE_PROCESSOR_H__

#include <memory>

#include "extracted_file.h"
#include "context.h"

namespace dexport {
	class FileProcessor {
		private:
			std::shared_ptr<ExtractedFile> _extractedFile;
			Context& _context;

		public:
			FileProcessor(std::shared_ptr<ExtractedFile> extractedFile, Context& context);
			void operator()();
	};
}

#endif
