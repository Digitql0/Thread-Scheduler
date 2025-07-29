#ifndef THREAD_MANAGER_HPP
#define THREAD_MANAGER_HPP

#include "task.hpp"
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <thread>

class worker {
public:
	worker(std::condition_variable& c, std::mutex& m);
	~worker();
	void idle();
	void assignJob(const task& tsk);
	bool isBusy();

	std::thread t;
	std::condition_variable& cv;
	std::mutex& mtx;
	std::atomic<bool> shutdown;
	task current_task;
	int id;
	bool isIdle;
	std::atomic<bool> gotJob;
};

#endif
