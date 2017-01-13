#ifndef __DEXPORT_HTTP_UPLOAD_H__
#define __DEXPORT_HTTP_UPLOAD_H__

#include <memory>
#include <string>
#include <boost/asio.hpp>

#include "extracted_file.h"

namespace dexport {
	class HTTPUpload {
		private:
			std::vector<std::string> _headers;
			std::string _hostname;
			std::string _port;
			std::string _path;

		public:
			HTTPUpload(const std::string& host, const std::string& port, const std::string path = "/");
			void upload(std::shared_ptr<ExtractedFile> ef);

			virtual ~HTTPUpload();
	};
}
#endif
