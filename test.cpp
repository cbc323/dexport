#include <tsk/libtsk.h>
#include <iostream>
#include <memory>

using namespace std;

TSK_WALK_RET_ENUM cpp_meta_callback(TskFsFile *fsFile, void *context) {
	if(context == nullptr) {
		cout << "context is nullptr" << endl;
	}

	cout << "meta_walk_callback invoked with inum: " << fsFile->getMeta()->getAddr() << endl;
	auto f = make_shared<TskFsFile>();
	cout << "attempting to open file" << endl;

	if(f->open((TskFsInfo *)context, f.get(), fsFile->getMeta()->getAddr())) {
		cout << "Unable to open file via inum" << endl;
	} else {
		cout << "Successfully opened file via inum" << endl;
		cout << "\tFile size: " << f->getMeta()->getSize() << endl;
		cout << "\tFile atime: " << to_string(f->getMeta()->getATime()) << endl;
		cout << "\tFile ctime: " << f->getMeta()->getCTime() << endl;
		cout << "\tFile mtime: " << f->getMeta()->getMTime() << endl;

		char buf[1024] = {0};
		auto rs = f->read(0, buf, 1024, TSK_FS_FILE_READ_FLAG_NONE);
		cout << "Read " << rs << " bytes from the file!" << endl;

		f->close();
	}
	return TSK_WALK_CONT;
}


int main(int argc, char** argv) {
	if(argc != 2) {
		cerr << "Image path required!" << endl;
		return 1;
	}
	
	auto info = make_shared<TskImgInfo>();
	if(info->open(argv[1], TSK_IMG_TYPE_DETECT,0) == 1) {
		cout << "There was an error opening the image" << endl;
		return 1;
	}

	cout << "Image opened sucessfully!" << endl;

	auto vsinfo = make_shared<TskVsInfo>();
	if(vsinfo->open(info.get(), 0, TSK_VS_TYPE_DETECT)) {
		cout << "Unable to find volume system, trying to open as FS" << endl;
		auto fsinfo = make_shared<TskFsInfo>();
		if(fsinfo->open(info.get(), 0, TSK_FS_TYPE_DETECT)) {
			cout << "Unable to find a filesystem either" << endl;
		} else {
			cout << "Found a filesystem without a volume!" << endl;

			fsinfo->metaWalk(
					fsinfo->getFirstINum(),
					fsinfo->getLastINum(),
					TSK_FS_META_FLAG_ALLOC,
					cpp_meta_callback,
					fsinfo.get()
				);


			return 0;
		}
		
		return 1;
	}

	auto pcount = vsinfo->getPartCount();

	cout << "Found " << pcount << "partitions" << endl;

	return 0;

}
