#include <iostream>
#include <algorithm>

#include "evidence.h"


using namespace dexport;
using namespace std;


Evidence::Evidence(const std::string& path) :
	_evidencePath(path) {

	_imgInfo = tsk_img_open_sing(path.c_str(), TSK_IMG_TYPE_DETECT, 0);
	if(_imgInfo == nullptr) {
		throw -1;
	}
}


Evidence::~Evidence() {
	for_each(open_fs.begin(), open_fs.end(), [](TSK_FS_INFO * fs) {
		tsk_fs_close(fs);
	});

	open_fs.clear();
}


void Evidence::eachFs(function<void (TSK_FS_INFO *)> callback) {
	cout << "Trying to open volume system..." << endl;
	auto vs = tsk_vs_open(_imgInfo, 0, TSK_VS_TYPE_DETECT);
	if(vs == nullptr) {
		cout << "Failed to open volume system" << endl;
		auto fs = tsk_fs_open_img(_imgInfo, 0, TSK_FS_TYPE_DETECT);
		if(fs == nullptr) {
			cout << "Unable to open file system" << endl;
			return; // no vol, no fs, maybe could carve
		}

		open_fs.push_back(fs);
		callback(fs);
		return;
	}

	for(size_t i = 0; i < vs->part_count; ++i) {
		auto fs = tsk_fs_open_vol(&vs->part_list[i], TSK_FS_TYPE_DETECT);
		if(fs == nullptr) {
			cout << "Cannot open partition with index: " << i << endl;
			continue;
		}

		open_fs.push_back(fs);
		callback(fs);
	}
}
