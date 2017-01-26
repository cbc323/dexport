#include "concurrent_task.h"

using namespace dexport;


ConcurrentTaskTracker::ConcurrentTaskTracker() : _tasksOut(0) {}


void ConcurrentTaskTracker::increment() {
	_tasksOut++;
}
