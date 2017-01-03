#include <iostream>
#include <memory>
#include <thread>
#include <future>

#include "evidence.h"
#include "work_queue.h"
#include "file.h"
#include "file_processor.h"
#include "context.h"

using namespace std;
using namespace dexport;

TSK_WALK_RET_ENUM dirwalk_callback(TSK_FS_FILE *fsFile, const char *path, void *context) {
	if(context == nullptr) {
		cout << "context is nullptr" << endl;
	}

	if(fsFile->meta->type == TSK_FS_META_TYPE_VIRT) {
		cout << "Skipping TSK Virtual file" << endl;
		return TSK_WALK_CONT;
	}

	if(fsFile->meta->type == TSK_FS_META_TYPE_DIR) {
		cout << "Skipping Directory" << endl;
		return TSK_WALK_CONT;
	}

	((Context *) context)->workq().async(FileProcessor(make_shared<TSKFile>(fsFile->fs_info, path, fsFile->name->name, fsFile->meta->addr)));

	return TSK_WALK_CONT;
}

int main(int argc, const char **argv) {
	if(argc == 1) {
		cerr << "Invalid arguments" << endl;
		return 1;
	}

	Context ctx;
	//ctx.workq().parallel(false);

	Evidence e(argv[1]);
	e.eachFs([&ctx](TSK_FS_INFO *fs) {
		cout << "Received a FS with id: " << fs->fs_id << endl;
		tsk_fs_dir_walk(
			fs,
			fs->root_inum,
			(TSK_FS_DIR_WALK_FLAG_ENUM)(TSK_FS_DIR_WALK_FLAG_UNALLOC | TSK_FS_DIR_WALK_FLAG_ALLOC | TSK_FS_DIR_WALK_FLAG_RECURSE),
			dirwalk_callback,
			&ctx
		);
	});

	cout << "Finished walking the evidence" << endl;
}
