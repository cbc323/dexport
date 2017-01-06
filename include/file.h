#ifndef __DEXPORT_FILE_H__
#define __DEXPORT_FILE_H__

#include <tsk/libtsk.h>
#include <string>
#include <memory>
#include <fstream>
#include <cstdint>

#include "temp_file.h"
#include "file_magic.h"


namespace dexport {
	class FileMeta;

	// abstract file class representing a file to be processed from some source
	class File {
		protected:
			std::string _parentDirectory;
			std::string _fileName;
			uint64_t _archiveDepth = 0;


		public:
			File(const char *parentDirectory, const char *fileName);

			std::string absPath();
			std::string fileName();
			std::string dirName();
			uint64_t archiveDepth();

			virtual int64_t size() = 0;
			virtual std::vector<uint8_t> bytes() = 0;
			virtual int64_t copy(std::ofstream& out) = 0;
			virtual FileMeta meta() const = 0;
	};

	// represents a file within an image being processed w/ tsk
	class TSKFile : public File {
		private:
			TSK_INUM_T	_inum;
			TSK_FS_FILE *_finfo;


		public:
			TSKFile(TSK_FS_INFO *fs, const char *parentDirectory, const char *fileName, TSK_INUM_T inum);
			TSK_INUM_T inum();
			TSK_FS_FILE * finfo();

			virtual int64_t size();
			virtual std::vector<uint8_t> bytes();
			virtual int64_t copy(std::ofstream& out);
			virtual FileMeta meta() const;
	};
}

#endif
