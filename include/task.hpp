#ifndef TASK_HPP
#define TASK_HPP

#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <sstream>


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
    static std::mutex printMutex;
    std::lock_guard<std::mutex> lock(printMutex);

    // Build the entire message inside the lock
    std::ostringstream oss;
    oss << "[Thread " << std::this_thread::get_id() << "] " << msg;

    // Avoid std::endl because it flushes — do it manually
    std::cout << oss.str() << '\n' << std::flush;
}

#endif
