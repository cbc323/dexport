#include <archive.h>
#include <archive_entry.h>
#include <stdexcept>
#include <algorithm>
#include <iomanip>
#include <sstream>

#include "processors.h"
#include "archive_identifier.h"
#include "http_upload.h"

using namespace std;
using namespace dexport;


ExtractedObjectProcessor::ExtractedObjectProcessor(shared_ptr<ExtractedFile> extractedFile, Context& context)
	: _extractedFile(extractedFile), started(false), _context(context){ }

ExtractedObjectProcessor::~ExtractedObjectProcessor() {
	if(started) {
		_context.workq().getInProgressCount().fetch_sub(1);
	}
}

ExtractedFileProcessor::ExtractedFileProcessor(shared_ptr<ExtractedFile> extractedFile, Context& context)
	: ExtractedObjectProcessor(extractedFile, context) {}


void ExtractedFileProcessor::operator()() {
	started = true;

	// run this through all of the export plugins
	HTTPUpload up("127.0.0.1", "80");
	up.upload(_extractedFile);
}


ExtractedArchiveProcessor::ExtractedArchiveProcessor(shared_ptr<ExtractedFile> extractedFile, Context& context)
	: ExtractedObjectProcessor(extractedFile, context) {}


void ExtractedArchiveProcessor::operator()() {
	started = true;

	// push out a meta file that will represent this archive
	_context.workq().async(ExtractedMetaFileProcessor(_extractedFile, _context));

	struct archive *a = archive_read_new();
	if(archive_read_support_filter_all(a) != ARCHIVE_OK) {
		cout << "There was an error adding all filters" << endl;
	}

	if(archive_read_support_format_all(a) != ARCHIVE_OK) {
		cout << "There was an error adding all of the formats" << endl;
	}

	_extractedFile->setupReadArchive(a);

	struct archive_entry *ae;

	while(archive_read_next_header(a, &ae) == ARCHIVE_OK) {
		const char * entryPath = archive_entry_pathname(ae);
		int64_t entrySize = archive_entry_size(ae);
		if(entrySize < 0) {
			throw runtime_error("Invalid entry size for archive!");
		}

		cout << "archive_entry: " << entryPath << " " << entrySize << " bytes" << endl;
		cout << "archive_format: " << archive_format_name(a) << " archive_filter: " << archive_filter_name(a, 0) << endl;

		FileMeta childMeta(_extractedFile->getMeta());
		childMeta.makeChild(entryPath);
		shared_ptr<ExtractedFile> childEf;

		if(entrySize > 0 && entrySize < 1024 * 1024 * 500) {
			// extract this file into memory in one shot
			vector<uint8_t> bytes (entrySize);
			size_t bytesRead = archive_read_data(a, bytes.data(), bytes.size());
			if(bytesRead != (size_t) entrySize) {
				throw runtime_error("Number of bytes read is not equal to number of bytes expected!");
			}

			childEf = make_shared<MemoryExtractedFile>(std::move(bytes), std::move(childMeta));
		} else { // if we don't know the size, or the size is > 500MB, dump it to disk
			TempFile tmpf = TempFile::fromPrefix("/dev/shm/dextmp-");
			vector<uint8_t> tmp(1024 * 1024 * 500);
			tmpf.write([a, &tmp](std::ofstream& ofs){
					size_t totalRead = 0;
					while(true) {
						int32_t bytesRead = archive_read_data(a, tmp.data(), tmp.size());
						if(bytesRead == ARCHIVE_FATAL) {
							// deal with this
							throw runtime_error("Unable to read data from archive!");
						}

						if(bytesRead == 0) {
							// this is the end of the archive
							break;
						}

						totalRead += bytesRead;
						ofs.write((const char *) tmp.data(), bytesRead);
					}

				});

			childEf = make_shared<TempExtractedFile>(std::move(tmpf), std::move(childMeta));
		}

		// queue this up to be processed, if its an archive, send it back through
		// this is necessary to tackle nested archives
		auto fmagic = childEf->fileMagic();

		if(ArchiveIdentifier::isArchiveMime(fmagic.type())) {
			_context.workq().async(ExtractedArchiveProcessor(childEf, _context));
		} else {
			_context.workq().async(ExtractedFileProcessor(childEf, _context));
		}
	}

	archive_read_free(a);
}


ExtractedMetaFileProcessor::ExtractedMetaFileProcessor(shared_ptr<ExtractedFile> extractedFile, Context& context):
	ExtractedObjectProcessor(extractedFile, context) { }


void ExtractedMetaFileProcessor::operator()() {
	started = true;

	auto digests = _extractedFile->digest();
	stringstream outstr;
	for_each(
			digests.begin(),
			digests.end(),
			[&outstr, this](auto& dig) {
				std::stringstream hashstr;
				hashstr << std::hex << std::setfill('0');
				for_each(
						dig.second.begin(),
						dig.second.end(),
						[&hashstr](const uint8_t& cur) {
							hashstr << std::setw(2) << static_cast<int>(cur);
						}
					);

				outstr << dig.first << ": " << hashstr.str() << endl;
			});

	cout << outstr.str();
}

