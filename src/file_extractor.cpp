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
#include "archive_identifier.h"

using namespace std;
using namespace dexport;


FileExtractor::FileExtractor(shared_ptr<File> file, Context& context) : _file(file), _context(context), started(false) {}


FileExtractor::~FileExtractor() {
	if(started) {
		cout << ">> extractor decrementing jobs" << endl;
		_context.workq().getInProgressCount().fetch_sub(1);
	}
}


void FileExtractor::operator()() {
	started = true;
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

	if(ArchiveIdentifier::isArchiveMime(fmagic.type())) {
		// this doesn't appear to be an archive it should be pushed through the processor
		_context.workq().async(ExtractedArchiveProcessor(ef, _context));
	} else {
		// this is an archive, unpack it and put those items on the async queue
		_context.workq().async(ExtractedFileProcessor(ef, _context));
	}
}
