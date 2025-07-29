#ifndef TASK_MANAGER_HPP
#define TASK_MANAGER_HPP

#include "thread_manager.hpp"
#include <memory>
#include <vector>

//Task Manager Class with abbreviation tsk
class taskManager {
public:
	taskManager();
	void addTask(task t);
	void distributeTasks();
	int findNextTask();

	std::mutex mtx;
	std::condition_variable cv;
	std::vector<std::unique_ptr<worker>> threads;
	std::vector<task> tasks;
};

#endif
