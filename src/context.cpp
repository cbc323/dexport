#include <fstream>

#include "context.h"

using namespace std;
using namespace dexport;

Context::Context(const std::string& mimesPath, size_t numThreads) : tpool(numThreads) {
	ifstream fs(mimesPath);
	std::string line;
	while(getline(fs, line)) {
		_archiveMimes.push_back(line);
	}
}

ThreadPool& Context::getThreadPool() {
	return tpool;
}


const std::vector<std::string>& Context::archiveMimes() {
	return _archiveMimes;
}
