#ifndef THREAD_MANAGER_HPP
#define THREAD_MANAGER_HPP

#include "task.hpp"
#include <thread>

class worker {
public:
	worker();
	~worker();
	void idle();
	void assignJob(task t);
	void isBusy();

	std::thread t;
};

#endif