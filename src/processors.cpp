#include <archive.h>
#include <archive_entry.h>

#include "processors.h"

using namespace std;
using namespace dexport;


ExtractedObjectProcessor::ExtractedObjectProcessor(shared_ptr<ExtractedFile> extractedFile, Context& context)
	: _extractedFile(extractedFile), _context(context) {}

bool ExtractedObjectProcessor::isArchive() {
	auto fmagic = _extractedFile->fileMagic();
	auto mime = find_if(
		_context.archiveMimes().begin(),
		_context.archiveMimes().end(),
		[&fmagic](const std::string& cur){
			return (cur == fmagic.type());
	});

	return (mime != _context.archiveMimes().end());
}

ExtractedFileProcessor::ExtractedFileProcessor(shared_ptr<ExtractedFile> extractedFile, Context& context)
	: ExtractedObjectProcessor(extractedFile, context) {}


void ExtractedFileProcessor::operator()() {
	// run this through all of the export plugins
}


ExtractedArchiveProcessor::ExtractedArchiveProcessor(shared_ptr<ExtractedFile> extractedFile, Context& context)
	: ExtractedObjectProcessor(extractedFile, context) {}


void ExtractedArchiveProcessor::operator()() {
	cout << "\tarchive" << endl;

	// TODO: push out a meta file that will represent this archive (but without the contents)

	struct archive *a = archive_read_new();
	archive_read_support_filter_all(a);
	archive_read_support_format_raw(a);

	_extractedFile->setupReadArchive(a);

	struct archive_entry *ae;
	//size_t total_headers = 0;
	while(archive_read_next_header(a, &ae) == ARCHIVE_OK) {
		const char * entryPath = archive_entry_pathname(ae);
		int64_t entry_size = archive_entry_size(ae);

		FileMeta childMeta(_extractedFile->getMeta());
		childMeta.makeChild(entryPath);
		shared_ptr<ExtractedFile> childEf;
		if(entry_size > 0 && entry_size < 1024 * 1024 * 500) {
			// extract this file into memory in one shot
			vector<uint8_t> bytes (entry_size);
			if(archive_read_data(a, bytes.data(), bytes.size()) != ARCHIVE_OK) {
				cout << "Unable to read data from archive" << endl;
			}

			childEf = make_shared<MemoryExtractedFile>(std::move(bytes), std::move(childMeta));
		} else { // if we don't know the size, or the size is > 500MB, dump it to disk
			TempFile tmpf = TempFile::fromPrefix("/dev/shm/dextmp-");
			childEf = make_shared<TempExtractedFile>(std::move(tmpf), std::move(childMeta));
		}

		if(childEf != nullptr) {
			// queue this up to be processed
			//_context.workq().async(FileProcessor(childEf, _context));
		}
	}
}
