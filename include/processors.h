#ifndef __DEXPORT_FILE_PROCESSOR_H__
#define __DEXPORT_FILE_PROCESSOR_H__

#include <memory>

#include "extracted_file.h"
#include "context.h"

namespace dexport {
	class ExtractedObjectProcessor {
		protected:
			std::shared_ptr<ExtractedFile> _extractedFile;
			bool started;
			Context& _context;

		public:
			ExtractedObjectProcessor(std::shared_ptr<ExtractedFile> extractedFile, Context& context);
			virtual ~ExtractedObjectProcessor();

			virtual void operator()() = 0;
	};

	class ExtractedFileProcessor : public ExtractedObjectProcessor {
		public:
			ExtractedFileProcessor(std::shared_ptr<ExtractedFile> extractedFile, Context& context);
			void operator()();
	};

	class ExtractedArchiveProcessor : public ExtractedObjectProcessor {
		public:
			ExtractedArchiveProcessor(std::shared_ptr<ExtractedFile> extractedFile, Context& context);
			void operator()();
	};


	class ExtractedMetaFileProcessor : public ExtractedObjectProcessor {
		public:
			ExtractedMetaFileProcessor(std::shared_ptr<ExtractedFile> extractedFile, Context& context);
			void operator()();
	};
}

#endif
