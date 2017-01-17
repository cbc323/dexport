#ifndef __DEXPORT_HTTP_UPLOAD_H__
#define __DEXPORT_HTTP_UPLOAD_H__

#include <memory>
#include <string>
#include <map>
#include <boost/asio.hpp>

#include "extracted_file.h"

namespace dexport {
	class HTTPUpload {
		private:
			std::string _hostname;
			std::string _port;
			std::string _path;
		
			boost::asio::ip::tcp::iostream _connection;
			std::map<std::string, std::string> _headers;

			void _sendHeaders();
			void _setMetaHeaders(const FileMeta& meta);
			void _uploadContent(std::shared_ptr<ExtractedFile> ef, std::ostream& stream);

		public:
			HTTPUpload(const std::string& host, const std::string& port, const std::string& path = "/");

			template <typename ValueType>
			void setHeader(const std::string& headerName, const ValueType& headerValue) {
				_headers[headerName] = std::to_string(headerValue);
			}

			void upload(std::shared_ptr<ExtractedFile> ef);
			void upload(const FileMeta& meta);
	};
}
#endif
