#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <memory>


#include "file_extractor.h"
#include "file_magic.h"
#include "temp_file.h"
#include "extracted_file.h"
#include "processors.h"

using namespace std;
using namespace dexport;

FileExtractor::FileExtractor(shared_ptr<File> file, Context& context) : _file(file), _context(context) { }

void FileExtractor::operator()() {
	shared_ptr<ExtractedFile> ef;
	if(_file->size() < 1024 * 1024 * 500) {
		ef = make_shared<MemoryExtractedFile>(_file->bytes(), _file->meta());
	} else { // this is a larger file
		TempFile tmpf = TempFile::fromPrefix("/dev/shm/dextmp-");
		tmpf.write([this](ofstream& tmpout) { _file->copy(tmpout); });
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
		_context.workq().async(ExtractedArchiveProcessor(ef, _context));
	} else {
		// this is an archive, unpack it and put those items on the async queue
		_context.workq().async(ExtractedFileProcessor(ef, _context));
	}
}
