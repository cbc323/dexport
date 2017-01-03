#include <magic.h>
#include <vector>
#include <string>
#include <regex>
#include <iostream>

namespace dexport {
	class FileMagicResult {
		private:
			std::string _magicString;
			std::string _fileMime;
			std::string _encodingMime;

		public:
			FileMagicResult(std::string magicString):_magicString(magicString) {
				std::cerr << "FileMagicResult: " << magicString << std::endl;
				_fileMime = _magicString.substr(0, _magicString.find(";"));
				std::cerr << "_fileMime: " << _fileMime << std::endl;
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
			static const int _DEFAULT_FLAGS = MAGIC_CONTINUE | MAGIC_MIME_TYPE | MAGIC_MIME_ENCODING;
			magic_t _magic;

			void setFlags(bool decompress) {
				if(decompress) {
					std::cerr << "Magic will decompress" << std::endl;
				}

				magic_setflags(_magic, _DEFAULT_FLAGS);/* | (decompress == false) ? 0:MAGIC_COMPRESS);*/
			}

		public:
			FileMagic() {
				// the flags are set on a per-call basis
				_magic = magic_open(_DEFAULT_FLAGS);
				magic_load(_magic, nullptr);
			}

			FileMagicResult test(const std::vector<char> &contents, bool decompress=false) {
				setFlags(decompress);
				std::cerr << "FileMagic checking " << contents.size() << " byte buffer" << std::endl;
				return FileMagicResult(magic_buffer(_magic, contents.data(), contents.size()));
			}

			FileMagicResult test(const std::string &filename, bool decompress=false) {
				setFlags(decompress);
				return FileMagicResult(magic_file(_magic, filename.c_str()));
			}
	};
}
