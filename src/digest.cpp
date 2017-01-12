#include <algorithm>
#include <mbedtls/md_internal.h>

#include "digest.h"


using namespace dexport;
using namespace std;

const vector<mbedtls_md_type_t> Digester::HASH_TYPES = { MBEDTLS_MD_MD5, MBEDTLS_MD_SHA1, MBEDTLS_MD_SHA224, MBEDTLS_MD_SHA256, MBEDTLS_MD_SHA384, MBEDTLS_MD_SHA512};

Digester::Digester() : contexts(HASH_TYPES.size()) {
	for(size_t i = 0; i < HASH_TYPES.size(); ++i) {
		mbedtls_md_init(&contexts[i]);
		int32_t error = mbedtls_md_setup(&contexts[i], mbedtls_md_info_from_type(HASH_TYPES[i]), 0);
		if(error) {
			// do something
		}
	}
}


void Digester::start() {
	for_each(contexts.begin(), contexts.end(), [](auto& cur){
				mbedtls_md_starts(&cur);
			});
}


void Digester::update(const std::vector<uint8_t>& data) {
	update(data.data(), data.size());
}


void Digester::update(const uint8_t * data, size_t dataSize) {
	for_each(contexts.begin(), contexts.end(), [data, dataSize](auto& cur) {
				mbedtls_md_update(&cur, data, dataSize);
			});
}


map<string, vector<uint8_t>> Digester::finish() {
	map<string, vector<uint8_t>> digests;
	for_each(contexts.begin(), contexts.end(), [&digests](auto& cur) {
				vector<uint8_t> digest(cur.md_info->size);
				mbedtls_md_finish(&cur, digest.data());
				digests[cur.md_info->name] = digest;
			});

	return digests;
}


map<string, vector<uint8_t>> Digester::digest(const uint8_t *data, size_t dataSize) {
	map<string, vector<uint8_t>> digests;
	for_each(HASH_TYPES.begin(), HASH_TYPES.end(), [&digests, data, dataSize](auto& cur) {
				auto info = mbedtls_md_info_from_type(cur);
				vector<uint8_t> digest(info->size);
				mbedtls_md(info, data, dataSize, digest.data());
				digests[info->name] = digest;
			});

	return digests;
}


map<string, vector<uint8_t>> Digester::digest(const vector<uint8_t>& data) {
	return digest(data.data(), data.size());
}

