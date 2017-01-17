#include <stdexcept>
#include <cstdio>
#include <algorithm>

#include "http_upload.h"

using namespace dexport;
using namespace std;

using boost::asio::ip::tcp;


HTTPUpload::HTTPUpload(const std::string& host, const std::string& port, const std::string& path)
	: _hostname(host), _port(port), _path(path), _connection(_hostname, _port) {
	_headers["Host"] = host;
}


void HTTPUpload::_sendHeaders() {
	_connection << "POST " << _path << " HTTP/1.1\r\n";

	for(auto& header : _headers) {
		_connection << header.first << ": " << header.second << "\r\n";
	}

	_connection << "\r\n";
	_headers.clear();
}


void HTTPUpload::_setMetaHeaders(const FileMeta& fm) {
	auto meta = fm.toMap();
	for(auto curMeta : meta) {
		_headers["X-DEXPORT-" + curMeta.first] = curMeta.second;
	}
}


void HTTPUpload::upload(const FileMeta& fm) {
	_setMetaHeaders(fm);
	setHeader("Content-Length", 0);

	_sendHeaders();
}


void HTTPUpload::upload(shared_ptr<ExtractedFile> ef) {
	_setMetaHeaders(ef->getMeta());
	setHeader("Content-Length", ef->getMeta().fileSize);
	_sendHeaders();

	//ef->write(_connection);
}

