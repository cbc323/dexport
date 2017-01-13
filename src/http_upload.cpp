#include <stdexcept>
#include <cstdio>

#include "http_upload.h"

using namespace dexport;
using namespace std;

HTTPUpload::HTTPUpload(const std::string& url) : _url(url) {
	_curl = curl_easy_init();
	if(_curl == nullptr) {
		throw runtime_error("Unable to initialize libcurl");
	}

	curl_easy_setopt(_curl, CURLOPT_URL, _url.c_str());
	curl_easy_setopt(_curl, CURLOPT_POST, 1L);
}


HTTPUpload::~HTTPUpload() {
	curl_easy_cleanup(_curl);
}


void HTTPUpload::upload() {
	struct curl_slist *hdrs = nullptr;
	if(!_headers.empty()) {
		for(auto& header : _headers) {
			// curl_slist_append DOES copy the string
			hdrs = curl_slist_append(hdrs, header.c_str());
			if(hdrs == nullptr) {
				throw runtime_error("Unable to add a header to the header list");
			}
		}

		curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, hdrs);
	}

	auto res = curl_easy_perform(_curl);
	if(hdrs != nullptr) {
		curl_slist_free_all(hdrs);
	}

	if(res != CURLE_OK) {
		throw runtime_error("There was an error uploading a file via HTTP");
	}
}


void HTTPUpload::file(shared_ptr<MemoryExtractedFile> mef) {
	curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE, mef->size());
	curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, mef->bytes().data());
	upload();
}


void HTTPUpload::file(shared_ptr<TempExtractedFile> tef) {
	_headers.push_back("Transfer-Encoding: chunked");
	_headers.push_back("Expect:");

	FILE *upfile = fopen(tef->name().c_str(), "rb");
	curl_easy_setopt(_curl, CURLOPT_READDATA, (void *) upfile);
	upload();
}

