#ifndef __DEXPORT_FILE_META_H__
#define __DEXPORT_FILE_META_H__

#include <string>
#include <cstdint>
#include <tsk/libtsk.h>

namespace dexport {
	class FileMeta {
		public:
			std::string parentPath;
			std::string fileName;

			uint64_t inum;

			uint64_t fileSize;

			time_t atime;
			uint32_t atimeNano;

			time_t crtime;
			uint32_t crtimeNano;

			time_t mtime;
			uint32_t mtimeNano;

			time_t mftAccess;
			time_t mftAccessNano;

			time_t mftCreate;
			time_t mftCreateNano;

			time_t mftModify;
			time_t mftModifyNano;

			time_t extDelete;
			time_t extDeleteNano;

			uint32_t gid;
			uint32_t uid;

			FileMeta(const TSK_FS_META *tskMeta, const std::string &parPath, const std::string &fname)
				: parentPath(parPath), fileName(fname) {

					inum = tskMeta->addr;
					fileSize = tskMeta->size;

					gid = tskMeta->gid;
					uid = tskMeta->uid;

					atime = tskMeta->atime;
					atimeNano = tskMeta->atime_nano;

					crtime = tskMeta->crtime;
					crtimeNano = tskMeta->crtime_nano;

					mtime = tskMeta->mtime;
					mtimeNano = tskMeta->mtime_nano;

					mftAccess = tskMeta->time2.ntfs.fn_atime;
					mftAccessNano = tskMeta->time2.ntfs.fn_atime_nano;

					mftCreate = tskMeta->time2.ntfs.fn_crtime;
					mftCreateNano = tskMeta->time2.ntfs.fn_crtime_nano;

					mftModify = tskMeta->time2.ntfs.fn_ctime;
					mftModifyNano = tskMeta->time2.ntfs.fn_ctime_nano;

					extDelete = tskMeta->time2.ext2.dtime;
					extDeleteNano = tskMeta->time2.ext2.dtime_nano;
			}
	};
}

#endif
