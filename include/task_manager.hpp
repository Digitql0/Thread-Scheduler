#ifndef TASK_MANAGER_HPP
#define TASK_MANAGER_HPP

#include "thread_manager.hpp"
#include <thread>
#include <vector>

//Task Manager Class with abbreviation tsk
class tsk {
	tsk();
	std::vector<std::thread> threads;
};

#endif