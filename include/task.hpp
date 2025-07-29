#ifndef TASK_HPP
#define TASK_HPP

#include <iostream>
#include <mutex>
#include <string>
#include <vector>

typedef struct {
	void (*task) (void* data);
	void* data;
	std::vector<int> dependencies;
	int dependenciesDone;
	bool done;
	int id;
} task;

inline task makeTask() {
    static int task_id = 0;
    task t;
    t.task = nullptr;
    t.data = nullptr;
    t.dependencies.clear();
    t.dependenciesDone = 0;
    t.done = false;
    t.id = task_id++;
    return t;
}

inline void safePrint(const std::string& msg) {
    static std::mutex printMutex;  // static so it's shared everywhere
    std::lock_guard<std::mutex> lock(printMutex);
    std::cout << msg << std::endl;
}


#endif
