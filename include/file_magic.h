#ifndef __DEXPORT_FILE_MAGIC_H__
#define __DEXPORT_FILE_MAGIC_H__

#include <magic.h>
#include <vector>
#include <string>
#include <regex>
#include <iostream>
#include <stdexcept>

namespace dexport {
	class FileMagicResult {
		private:
			std::string _magicString;
			std::string _fileMime;
			std::string _encodingMime;

		public:
			FileMagicResult(std::string magicString):_magicString(magicString) {
				std::smatch matches;
				std::regex e("(^[a-zA-Z0-9._-]+/[a-zA-Z0-9._-]+)");
				auto hits = std::regex_search(magicString, matches, e);
				if(hits && matches.size() > 0) {
					_fileMime = matches[1];
				} else {
					throw std::runtime_error("Unable to extract mime type from magic string: " + magicString);
				}
			}

			const std::string& type() {
				return _fileMime;
			}

			const std::string& compression() {
				return _encodingMime;
			}

			const std::string& magic() {
				return _magicString;
			}
	};

	class FileMagic {
		private:
			static const int _DEFAULT_FLAGS = MAGIC_COMPRESS | MAGIC_MIME_TYPE | MAGIC_MIME_ENCODING;
			magic_t _magic;

			void setFlags(bool decompress) {
				magic_setflags(_magic, _DEFAULT_FLAGS | ((decompress == false) ? 0:MAGIC_COMPRESS));
			}

		public:
			FileMagic() {
				// the flags are set on a per-call basis
				_magic = magic_open(_DEFAULT_FLAGS);
				magic_load(_magic, nullptr);
			}

			FileMagicResult test(const std::vector<uint8_t> &contents) {
				return FileMagicResult(magic_buffer(_magic, contents.data(), contents.size()));
			}

			FileMagicResult test(const std::string &filename) {
				return FileMagicResult(magic_file(_magic, filename.c_str()));
			}
	};
}

#endif
