#ifndef __DEXPORT_EXTRACTED_FILE_H__
#define __DEXPORT_EXTRACTED_FILE_H__

#include <map>
#include <vector>
#include <cstdint>
#include <stdexcept>

#include <archive.h>
#include <mbedtls/md5.h>
#include <boost/interprocess/streams/bufferstream.hpp>

#include "file_meta.h"
#include "file_magic.h"
#include "temp_file.h"
#include "digest.h"

namespace dexport {
	class ExtractedFile {
		protected:
			FileMeta _meta;
			FileMagic _magic;

		public:
			ExtractedFile(FileMeta &&meta) : _meta(std::move(meta)) {}

			virtual FileMagicResult fileMagic() = 0;
			virtual void setupReadArchive(struct archive *arc) = 0;
			virtual std::vector<uint8_t> md5sum() = 0;
			virtual std::map<std::string, std::vector<uint8_t>> digest() = 0;
			virtual void write(std::ostream& out) = 0;

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


			virtual std::vector<uint8_t> md5sum() {
				std::vector<uint8_t> output(16);
				mbedtls_md5(_bytes.data(), _bytes.size(), output.data());
				return output;
			}

			virtual std::map<std::string, std::vector<uint8_t>> digest() {
				Digester d;
				return d.digest(_bytes);
			}

			virtual void write(std::ostream& out) {
				out.write((const char *) _bytes.data(), _bytes.size());
			}


			const std::vector<uint8_t>& bytes() const {
				return _bytes;
			}

			size_t size() const {
				return _bytes.size();
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


			virtual std::vector<uint8_t> md5sum() {
				mbedtls_md5_context ctx;
				mbedtls_md5_starts(&ctx);

				std::vector<uint8_t> temp(1024 * 1024 * 200);
				std::ifstream rd(_tf.name());
				while(rd.read((char *) temp.data(), temp.size())) {
					std::streamsize s = rd.gcount();
					if(s > 0) {
						mbedtls_md5_update(&ctx, temp.data(), (size_t) s);
					} else {
						// this should never happen, but it makes casting streamsize to a size_t less risky
						throw std::runtime_error("Unable to compute hash for temp file (streamsize was negative)");
					}
				}

				std::vector<uint8_t> output;
				mbedtls_md5_finish(&ctx, output.data());
				mbedtls_md5_free(&ctx);

				return output;
			}


			virtual std::map<std::string, std::vector<uint8_t>> digest() {
				std::vector<uint8_t> temp(1024 * 1024 * 200);
				std::ifstream rd(_tf.name());

				Digester d;
				d.start();

				while(rd.read((char *) temp.data(), temp.size())) {
					std::streamsize s = rd.gcount();
					d.update(temp.data(), s);
				}

				return d.finish();
			}


			virtual void write(std::ostream& out) {
				std::ifstream in(_tf.name());
				out << in.rdbuf();
			}


			std::string name() {
				return _tf.name();
			}
	};
}

#endif
