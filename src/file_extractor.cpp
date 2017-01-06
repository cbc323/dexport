#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <memory>

#include <archive.h>
#include <archive_entry.h>

#include "file_extractor.h"
#include "file_magic.h"
#include "temp_file.h"
#include "extracted_file.h"
#include "file_processor.h"

using namespace std;
using namespace dexport;

FileExtractor::FileExtractor(shared_ptr<File> file, Context& context) : _file(file), _context(context) { }

void FileExtractor::operator()() {
	shared_ptr<ExtractedFile> ef;
	if(_file->size() < 1024 * 1024 * 500) {
		//_context.workq().async(FileProcessor(make_shared<MemoryExtractedFile>(_file->bytes(), _file->meta()), _context));
		ef = make_shared<MemoryExtractedFile>(_file->bytes(), _file->meta());
	} else { // this is a larger file
		TempFile tmpf = TempFile::fromPrefix("/dev/shm/dextmp-");
		tmpf.write([this](ofstream& tmpout) { _file->copy(tmpout); });
		//_context.workq().async(FileProcessor(make_shared<TempExtractedFile>(std::move(tmpf), _file->meta()), _context));
		ef = make_shared<TempExtractedFile>(std::move(tmpf), _file->meta());
	}

	auto fmagic = ef->fileMagic();
	cout << "FileMagic thinks this file is: " << fmagic.type() << endl;

	auto mime = find_if(
		_context.archiveMimes().begin(),
		_context.archiveMimes().end(),
		[&fmagic](const std::string& cur){
			return (cur == fmagic.type());
		}
	);

	if(mime == _context.archiveMimes().end()) {
		// this doesn't appear to be an archive it should be pushed through the processor
		cout << "\tNot an archive" << endl;
	} else {
		// this is an archive, unpack it and put those items on the async queue
		cout << "\tarchive" << endl;
	}
}
/*
	auto fmagic = ef->fileMagic();
	cout << "FileMagic thinks this file is: " << fmagic.type();

	auto mime = find_if(
		_context.archiveMimes().begin(),
		_context.archiveMimes().end(),
		[&fmagic](const std::string& cur){
			return (cur == fmagic.type());
		}
	);

	if(mime == _context.archiveMimes().end()) {
		cout << "type: " << fmagic.type() << " was not in the list of archive types" << endl;
		// this doesn't appear to be an archive
	} else {
		cout << "type: " << fmagic.type() << " was found in the list of archive types" << endl;
	}

	FileMagic fm;

	if(_file->size() == 0) {
		cout << "Found an empty file!" << endl;
	} else if(_file->size() < 1024 * 1024 * 500) {
		vector<uint8_t> fbuff = _file->bytes();

		auto fmr = fm.test(fbuff);
		cout << "FileMagic thinks this file is: " << fmr.type() << "(" << fmr.magic() << ")" << endl;
		cout << "\t" << _file->absPath() << endl;
		if(fmr.compression().empty() == false) {
			cout << "The file is compressed with encoding: " << fmr.compression() << endl;
		}

		auto mime = find_if(
			_context.archiveMimes().begin(),
			_context.archiveMimes().end(),
			[&fmr](const std::string& cur){
				return (cur == fmr.type());
			}
		);

		if(mime == _context.archiveMimes().end()) {
			cout << "type: " << fmr.type() << " was not in the list of archive types" << endl;
			// this doesn't appear to be an archive
		} else {
			cout << "type: " << fmr.type() << " was found in the list of archive types" << endl;

			// try and unpack this archive
			struct archive *a = archive_read_new();
			archive_read_support_filter_all(a);
			archive_read_support_format_raw(a);

			int32_t r = archive_read_open_memory(a, fbuff.data(), fbuff.size());
			if(r == ARCHIVE_OK) {
				struct archive_entry *ae;
				size_t total_headers = 0;
				while(archive_read_next_header(a, &ae) == ARCHIVE_OK) {
					cout << "\tpath: " << archive_entry_pathname(ae) << endl;
					cout << "\tformat: (" << archive_format(a) << ") " << archive_format_name(a) << endl;
					cout << "\tfilter: " << archive_filter_name(a,0) << endl;
					total_headers++;
				}

				cout << "Archive has " << total_headers << " headers" << endl;
			}
		}
	} else { // this file will be written to the temp directory
		TempFile tmpf = TempFile::fromPrefix("/dev/shm/dextmp-");

		// dump this file to a temp file
		tmpf.write([this](ofstream& tmpout) { _file->copy(tmpout); });
		auto fmr = fm.test(tmpf.name());

		cout << "FileMagic thinks this file is: " << fmr.type() << "(" << fmr.magic() << ")" << endl;
		cout << "\t" << _file->absPath();
		if(fmr.compression().empty() == false) {
			cout << "The file is compressed with encoding: " << fmr.compression() << endl;
		}
	}*/
