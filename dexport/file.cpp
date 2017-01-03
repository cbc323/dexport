#include <tsk/libtsk.h>
#include "file.h"

using namespace dexport;
using namespace std;


File::File(const char * parentDirectory, const char * fileName) :
	_parentDirectory(parentDirectory), _fileName(fileName) {}


string File::absPath() {
	return _parentDirectory + "/" + _fileName;
}


string File::fileName() {
	return _fileName;
}


string File::dirName() {
	return _parentDirectory;
}


uint64_t File::archiveDepth() {
	return _archiveDepth;
}


TSKFile::TSKFile(TSK_FS_INFO * fs, const char * parentDirectory, const char * fileName, TSK_INUM_T inum) :
	File(parentDirectory, fileName),
	_inum(inum)
{
	_finfo = tsk_fs_file_open_meta(fs, nullptr, inum);

	if(_finfo == nullptr) {
		//TODO: put a real exception in here
		throw -1;
	}
}


TSK_INUM_T TSKFile::inum() {
	return _inum;
}


int64_t TSKFile::size() {
	return _finfo->meta->size;
}

vector<char> TSKFile::bytes() {
	vector<char> fbuff(size());
	int64_t bytes_read = tsk_fs_file_read(_finfo, 0, fbuff.data(), size(), TSK_FS_FILE_READ_FLAG_NONE);
	if(bytes_read != size()) {
		throw -1;
	}

	return fbuff;
}

int64_t TSKFile::copy(ofstream& out) {
	int64_t chunk_size = 1024 * 1024 * 500;
	vector<char> buffer(chunk_size);
	int64_t read_off = 0;

	if(chunk_size > _finfo->meta->size - read_off) {
		chunk_size = _finfo->meta->size - read_off;
	}

	while(read_off < _finfo->meta->size) {
		auto bytes_read = tsk_fs_file_read(
				_finfo,
				read_off,
				buffer.data(),
				chunk_size,
				TSK_FS_FILE_READ_FLAG_NONE
			);

		if(bytes_read == -1) {
			throw -1;
		}

		read_off += bytes_read;
		out.write(buffer.data(), bytes_read);
	}

	return read_off;
}


TSK_FS_FILE * TSKFile::finfo() {
	return _finfo;
}
