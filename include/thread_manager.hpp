#ifndef THREAD_MANAGER_HPP
#define THREAD_MANAGER_HPP

#include "task.hpp"
#include <atomic>
#include <condition_variable>
#include <thread>

class worker {
public:
	worker(std::condition_variable& c);
	~worker();
	void idle();
	void assignJob(task t);
	void isBusy();

	std::thread t;
	std::condition_variable& cv;
	std::mutex mtx;
	std::atomic<bool> shutdown;
	task current_task;
	bool isIdle = true;
};

#endif
