#include <iostream>
#include "task_manager.hpp"

std::mutex mtx;
std::condition_variable cv;

void saySomething(void* data) {
    const char* str = (char*) data;
    std::cout << str << std::endl;
}

void generateTask(taskManager* tsk, void (*task)(void* data), void* data) {
}

int main() {
    std::string str = "Program Started.";
    safePrint(str);

    taskManager tsk;

    task t = makeTask();
    t.task = saySomething;
    t.data = (void*)("This should come first");
    int firstID = t.id;

    task ta = makeTask();
    ta.task = saySomething;
    ta.data = (void*)("This should come second");
    ta.dependencies = {firstID};
    int secondID = ta.id;

    task tas = makeTask();
    tas.task = saySomething;
    tas.data = (void*)("This should come third");
    tas.dependencies = {firstID, secondID};
    int thirdID = tas.id;

    task task = makeTask();
    task.task = saySomething;
    task.data = (void*)("This should come fourth");
    task.dependencies = {firstID, secondID, thirdID};

    tsk.addTask(t);
    tsk.addTask(ta);
    tsk.addTask(tas);
    tsk.addTask(task);

    int incrementer = 0;

    while(!tsk.tasks.empty() || incrementer++ > 10000) {
        tsk.distributeTasks();
    }

    str = "Program Ended.";
    safePrint(str);

    return 0;
}
