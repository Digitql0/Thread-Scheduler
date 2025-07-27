#ifndef TASK_HPP
#define TASK_HPP

#include <atomic>

typedef struct {
	void (*task) (void* data);
	void* data;
	std::atomic<bool> done;
} task;

#endif
