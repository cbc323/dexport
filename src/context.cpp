#include <fstream>

#include "context.h"

using namespace std;
using namespace dexport;

Context::Context(const std::string& mimesPath) {
	ifstream fs(mimesPath);
	std::string line;
	while(getline(fs, line)) {
		_archiveMimes.push_back(line);
	}
}

WorkQueue& Context::workq() {
	return _workq;
}

const std::vector<std::string>& Context::archiveMimes() {
	return _archiveMimes;
}
