#ifndef __DEXPORT_TEMP_FILE_H__
#define __DEXPORT_TEMP_FILE_H__
#include <string>
#include <fstream>
#include <random>
#include <memory>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <unistd.h>

namespace dexport {
	class TempFile {
		private:
			std::string _fileName;
			//std::unique_ptr<std::ofstream> _stream;
			std::fstream _fstream;


		public:
			TempFile(TempFile&& other) : _fileName(std::move(other._fileName)), _fstream(_fileName) {}
			TempFile(const std::string& fileName) : _fileName(fileName), _fstream(_fileName){}


			static std::string generateTempName(const std::string& prefix) {
				std::random_device rd;
				std::stringstream fn;
				fn << prefix << std::hex << rd();

				return fn.str();
			}


			static TempFile fromPrefix(const std::string& prefix) {
				auto tname = generateTempName(prefix);
				return TempFile(tname);
			}

			virtual ~TempFile() {
				std::remove(_fileName.c_str());
			}

			const std::string& name() const {
				return _fileName;
			}
	};
}

#endif
