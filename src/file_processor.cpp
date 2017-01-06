#include "file_processor.h"

using namespace std;
using namespace dexport;

FileProcessor::FileProcessor(shared_ptr<ExtractedFile> extractedFile, Context& context)
	: _extractedFile(extractedFile), _context(context) {}


void FileProcessor::operator()() {
	auto fmagic = _extractedFile->fileMagic();
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
	} else {
		// this is an archive, unpack it and put those items on the async queue
	}

}
