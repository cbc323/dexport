#ifndef __DEXPORT_EVIDENCE_H__
#define __DEXPORT_EVIDENCE_H__

#include <string>
#include <memory>
#include <functional>
#include <vector>

#include <tsk/libtsk.h>

namespace dexport {
	class Evidence {
		private:
			std::string _evidencePath;
			TSK_IMG_INFO *_imgInfo;

			// this holds all of the open filesystems
			std::vector<TSK_FS_INFO *> open_fs;


		public:
			Evidence(const std::string& path);
			virtual ~Evidence();

			void eachFs(std::function<void(TSK_FS_INFO *)> callback);
	};
}

#endif
