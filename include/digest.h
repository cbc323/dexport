#ifndef __DEXPORT_HASHES_H__
#define __DEXPORT_HASHES_H__

#include <map>
#include <vector>
#include <cstdint>
#include <string>
#include <mbedtls/md.h>

namespace dexport {
	class Digester {
		private:
			static const std::vector<mbedtls_md_type_t> HASH_TYPES;
			std::vector<mbedtls_md_context_t> contexts;

		public:
			Digester();

			void start();

			void update(const std::vector<uint8_t>& data);
			void update(const uint8_t * data, size_t dataSize);

			std::map<std::string, std::vector<uint8_t>> finish();

			std::map<std::string, std::vector<uint8_t>> digest(const std::vector<uint8_t>& data);
			std::map<std::string, std::vector<uint8_t>> digest(const uint8_t * data, size_t dataSize);
	};
};

#endif
