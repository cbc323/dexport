#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <memory>

#include "file_processor.h"
#include "file_magic.h"
#include "temp_file.h"

using namespace std;
using namespace dexport;

FileProcessor::FileProcessor(shared_ptr<File> file) : _file(file) { }

void FileProcessor::operator()() {
	cout << "FileProcessor::operator called" << endl;
	FileMagic fm;

	if(_file->size() < 1024/* * 1024 * 500*/) {
		cout << "File is small enough to read into memory" << endl;
		vector<char> fbuff = _file->bytes();
		auto fmr = fm.test(fbuff, true);
		cout << "FileMagic thinks this file is: " << fmr.type() << endl;
		if(fmr.compression().empty() == false) {
			cout << "The file is compressed with encoding: " << fmr.compression() << endl;
		}
	} else { // this file will be written to the temp directory
		TempFile tmpf("/dev/shm/dextmp");

		cout << "Writing file out to temp directory to path: " << tmpf.name() << endl;
		//_file->copy(tmpf.ofstream());

		auto fmr = fm.test(tmpf.name(), true);
		cout << "FileMagic thinks this file is: " << fmr.type() << endl;
		if(fmr.compression().empty() == false) {
			cout << "The file is compressed with encoding: " << fmr.compression() << endl;
		}
	}
}
