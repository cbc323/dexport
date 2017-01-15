#ifndef __DEXPORT_FILE_META_H__
#define __DEXPORT_FILE_META_H__

#include <string>
#include <cstdint>
#include <map>
#include <tsk/libtsk.h>

namespace dexport {
	class FileMeta {
		public:
			std::string parentPath;
			std::string fileName;

			uint64_t archiveDepth = 0;

			uint64_t inum;

			uint64_t fileSize;

			time_t atime;
			uint32_t atimeNano;

			time_t ctime;
			uint32_t ctimeNano;

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

					ctime = tskMeta->ctime;
					ctimeNano = tskMeta->ctime_nano;

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


			void makeChild(const std::string& childPath) {
				archiveDepth++;
				parentPath = parentPath + "/" + fileName;
				fileName = childPath;
			}


			std::map<std::string, std::string> toMap() const {
				std::map<std::string, std::string> out;
				out["INUM"] = std::to_string(inum);
				out["SIZE"] = std::to_string(fileSize);

				out["FILE_GID"] = std::to_string(gid);
				out["FILE_UID"] = std::to_string(uid);

				out["ATIME"] = std::to_string(atime);
				out["ATIME_NANO"] = std::to_string(atimeNano);

				out["CTIME"] = std::to_string(ctime);
				out["CTIME_NANO"] = std::to_string(ctimeNano);

				out["CRTIME"] = std::to_string(crtime);
				out["CRTIME_NANO"] = std::to_string(crtimeNano);

				out["MTIME"] = std::to_string(mtime);
				out["MTIME_NANO"] = std::to_string(mtimeNano);

				out["MFT_ATIME"] = std::to_string(mftAccess);
				out["MFT_ATIME_NANO"] = std::to_string(mftAccessNano);

				out["MFT_CRTIME"] = std::to_string(mftCreate);
				out["MFT_CRTIME_NANO"] = std::to_string(mftCreateNano);

				out["MFT_MTIME"] = std::to_string(mftModify);
				out["MFT_MTIME_NANO"] = std::to_string(mftModifyNano);

				out["DTIME"] = std::to_string(extDelete);
				out["DTIME_NANO"] = std::to_string(extDeleteNano);

				return out;
			}
	};
}

#endif
