#include <vector>
#include <string>
#include <algorithm>

#include <iostream>

namespace dexport {
	class ArchiveIdentifier {
		private:
			static std::vector<std::string> _archiveMimes;

		public:
			static void setMimes(std::vector<std::string> mimes) {
				_archiveMimes = mimes;
			}

			static bool isArchiveMime(const std::string& mime) {
				auto found = find_if(
					_archiveMimes.begin(),
					_archiveMimes.end(),
					[&mime](const std::string& cur){
						return (cur == mime);
				});

				return (found != _archiveMimes.end());
			}
	};
}
