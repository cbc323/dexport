#ifndef __DEXPORT_HTTP_UPLOAD_H__
#define __DEXPORT_HTTP_UPLOAD_H__

#include <memory>
#include <string>
#include <curl/curl.h>

#include "extracted_file.h"

namespace dexport {
	class HTTPUpload {
		private:
			CURL *_curl;
			std::vector<std::string> _headers;
			std::string _url;

			void upload();

		public:
			HTTPUpload(const std::string& url);
			~HTTPUpload();

			void file(std::shared_ptr<MemoryExtractedFile> mef);
			void file(std::shared_ptr<TempExtractedFile> tef);
	};
}
#endif
