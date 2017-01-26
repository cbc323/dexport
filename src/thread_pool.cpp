#include "thread_pool.h"

using namespace dexport;
using namespace std;


Worker::Worker(ThreadPool& pool) : _thread(this), _parent(pool) { }


void Worker::_popAndRun(unique_lock<mutex>& lk) {
		auto work = _parent._work.front();
		_parent._work.pop();
		lk.unlock();

		try {
			work();
		} catch(...) {
			// problem w/ the work.
		}
}


void Worker::operator()() {
	// even if the pool gets shut down, we still finish the work that remains
	while(_parent.isRunning() || _parent._work.empty() == false) {
		unique_lock<mutex> lk(_parent._workMutex);
		if(_parent._work.empty()) {
			// wait for notification that more work is available
			_parent._workEvent.wait(
					lk, // this lock will be unlocked while we wait for the condition variable
					[this]{ return (_parent._work.empty() == false); }
				);
		}

		// we now have the lock, and there is work
		// _popAndRun will release the lock before running the work
		_popAndRun(lk);
	}
}


ThreadPool::ThreadPool(size_t poolSize) {
	for(size_t i = 0; i < poolSize; ++i) {
		_threads.emplace_back(*this);
	}

	_running = true;
}


void ThreadPool::shutdown() {
	_running = false;
}


void ThreadPool::join() {
	condition_variable completed;
	unique_lock<mutex> lk(_workMutex);
	completed.wait(
			lk,
			[this]{ return (_running == false && _work.empty()); }
		);
}
