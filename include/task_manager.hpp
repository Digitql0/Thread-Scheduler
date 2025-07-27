#ifndef TASK_MANAGER_HPP
#define TASK_MANAGER_HPP

#include "thread_manager.hpp"
#include <thread>
#include <vector>

//Task Manager Class with abbreviation tsk
class taskManager {
public:
	taskManager();
	bool makeTask();
	std::vector<std::thread> threads;
};

#endif
