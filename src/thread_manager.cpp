#include "thread_manager.hpp"
#include <condition_variable>
#include <thread>

worker::worker(std::condition_variable& c, std::mutex& m) : cv(c), mtx(m), isIdle(false) {
	t = std::thread(&worker::idle, this);
	shutdown.store(false);
}

worker::~worker() {
	if (t.joinable()) t.join();
}

void worker::idle() {
{	std::unique_lock<std::mutex> lock(mtx);
	bool predicate = !current_task.done.load() || shutdown;
	isIdle.store(true);
	cv.wait(lock, predicate);
	isIdle.store(false);
	if (shutdown) return;
}
	current_task.task(current_task.data);
}

void assignJob(task t) {
	std::unique_lock<std::mutex> lock(mtx);
	if (current_task.done.load()) {
		current_task=t;
		isIdle=false;
	}
}

void isBusy() {
	return isIdle;
}
