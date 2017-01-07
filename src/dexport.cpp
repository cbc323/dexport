#include <iostream>
#include <memory>
#include <thread>
#include <future>
#include <stdexcept>

#include "evidence.h"
#include "work_queue.h"
#include "file.h"
#include "file_extractor.h"
#include "context.h"
#include "archive_identifier.h"

using namespace std;
using namespace dexport;

std::vector<std::string> ArchiveIdentifier::_archiveMimes = std::vector<std::string>();

TSK_WALK_RET_ENUM dirwalk_callback(TSK_FS_FILE *fsFile, const char *path, void *context) {
	if(context == nullptr) {
		cout << "context is nullptr" << endl;
	}

	if(fsFile->meta->type == TSK_FS_META_TYPE_VIRT) {
		return TSK_WALK_CONT;
	}

	if(fsFile->meta->type == TSK_FS_META_TYPE_DIR) {
		return TSK_WALK_CONT;
	}

	//((Context *) context)->workq().parallel(false);

	try {
		((Context *) context)->workq().async(FileExtractor(make_shared<TSKFile>(fsFile->fs_info, path, fsFile->name->name, fsFile->meta->addr), *((Context *)context)));
	} catch(const runtime_error& re) {
		cout << "Caught exception while attempting to add a file to the workq" << endl;
		cout << re.what() << endl;
	} catch(...) {
		cout << "Caught exception while attempting to add a file to the workq" << endl;
	}

	return TSK_WALK_CONT;
}


int main(int argc, const char **argv) {
	if(argc == 1) {
		cerr << "Invalid arguments" << endl;
		return 1;
	}

	Context ctx(argv[2]);
	ctx.workq().parallel(false);

	// setup the mimes for the ArchiveIdentifier class
	ArchiveIdentifier::setMimes(ctx.archiveMimes());

	Evidence e(argv[1]);
	e.eachFs([&ctx](TSK_FS_INFO *fs) {
		tsk_fs_dir_walk(
			fs,
			fs->root_inum,
			(TSK_FS_DIR_WALK_FLAG_ENUM)(TSK_FS_DIR_WALK_FLAG_UNALLOC | TSK_FS_DIR_WALK_FLAG_ALLOC | TSK_FS_DIR_WALK_FLAG_RECURSE),
			dirwalk_callback,
			&ctx
		);
	});

	cout << "Finished walking the evidence" << endl;
	ctx.workq().join();
	cout << "All futures returned" << endl;
}
