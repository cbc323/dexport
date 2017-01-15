#include <stdexcept>
#include <cstdio>
#include <algorithm>

#include "http_upload.h"

using namespace dexport;
using namespace std;

using boost::asio::ip::tcp;


HTTPUpload::HTTPUpload(const std::string& host, const std::string& port, const std::string path)
	: _hostname(host), _port(port), _path(path) { }


void HTTPUpload::upload(shared_ptr<ExtractedFile> ef) {
	tcp::iostream st(_hostname, _port);
	st << "POST " << _path << " HTTP/1.1\r\n";
	st << "Host: " << _hostname << "\r\n";
	st << "Content-Type: application/octet-stream\r\n";
	st << "Content-Length: " << ef->size() << "\r\n";

	auto meta = ef->getMeta().toMap();
	for(auto curMeta : meta) {
		st << "X-DEXPORT-" << curMeta.first << ": " << curMeta.second << "\r\n";
	}


	st << "\r\n"; // end of headers, start content
	//ef->write(st);
}


HTTPUpload::~HTTPUpload() { }

