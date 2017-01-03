#include <cstdint>
#include <stdexcept>
#include <iostream>

#include "processor.h"

using namespace std;
using namespace dexport;

Processor::Processor(const vector<const char*>& paths) {
	auto status = openImage(paths.size(), paths.data(), TSK_IMG_TYPE_DETECT, 0);
	if (status) {
		throw runtime_error("Unable to open image");
	}
}

TSK_RETVAL_ENUM Processor::processFile(TSK_FS_FILE *fs_file, const char *path) {
	string s(path);
	if(s.length() > 0) {
		s.append("/");
	}

	s.append(fs_file->name->name);
	cout << "Got a " << fs_file->meta->size << " byte file @ " << s << endl;
	TSK_FS_FILE * lfile =  tsk_fs_file_open_meta(fs_file->fs_info, NULL, fs_file->meta->addr);
	cout << "Successfully opened file " << lfile->meta->addr << endl;
	cout << "\tFile atime: " << lfile->meta->atime << endl;
	cout << "\tFile create: " << lfile->meta->crtime << endl;
	cout << "\tFile mod: " << lfile->meta->mtime << endl;

	tsk_fs_file_close(lfile);
	lfile = nullptr;

	return TSK_OK;
}
