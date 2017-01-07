#ifndef __DEXPORT_EXTRACTED_FILE_H__
#define __DEXPORT_EXTRACTED_FILE_H__

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <archive.h>

#include "file_meta.h"
#include "file_magic.h"
#include "temp_file.h"

namespace dexport {
	class ExtractedFile {
		protected:
			FileMeta _meta;
			FileMagic _magic;

		public:
			ExtractedFile(FileMeta &&meta) : _meta(std::move(meta)) {}

			virtual FileMagicResult fileMagic() = 0;
			virtual void setupReadArchive(struct archive *arc) = 0;

			const FileMeta& getMeta() const {
				return _meta;
			}
	};

	class MemoryExtractedFile : public ExtractedFile {
		private:
			std::vector<uint8_t> _bytes;

		public:
			MemoryExtractedFile(std::vector<uint8_t> &&bytes, FileMeta &&meta) : ExtractedFile(std::move(meta)), _bytes(std::move(bytes)) {}

			virtual FileMagicResult fileMagic() {
				return _magic.test(_bytes);
			}

			virtual void setupReadArchive(struct archive *arc) {
				int32_t r = archive_read_open_memory(arc, _bytes.data(), _bytes.size());
				if(r != ARCHIVE_OK) {
					throw std::runtime_error("Unable to open in memory archive for reading");
				}
			}
	};

	class TempExtractedFile : public ExtractedFile {
		private:
			TempFile _tf;

		public:
			TempExtractedFile(TempFile &&tf, FileMeta &&meta) : ExtractedFile(std::move(meta)), _tf(std::move(tf)) {}

			FileMagicResult fileMagic() {
				return _magic.test(_tf.name());
			}

			virtual void setupReadArchive(struct archive *arc) {
				int32_t r = archive_read_open_filename(arc, _tf.name().c_str(), 10240);
				if(r != ARCHIVE_OK) {
					throw std::runtime_error("Unable to open archive file for reading");
				}
			}
	};
}

#endif
