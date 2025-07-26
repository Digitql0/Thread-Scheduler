#ifndef TASK_HPP
#define TASK_HPP

typedef struct {
	void (*task) (void* data);
	void* data;
} task;

#endif