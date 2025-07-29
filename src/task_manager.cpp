#include <algorithm>
#include <memory>

#include "task_manager.hpp"

taskManager::taskManager() {
  std::string str = "Task Manager Initialized";
  safePrint(str);

  unsigned int threadCount = std::thread::hardware_concurrency();

  str = std::to_string(threadCount) + " Threads Available.";
  safePrint(str);

  if (threadCount == 0) {
    threadCount = 4;  // fallback to 4 if undetectable
  }

  for (int i = 0; i < threadCount; i++) {
    threads.emplace_back(std::unique_ptr<worker>(
        new worker(cv, mtx)));  // Custom Make_unique due to C++11
  }
}

void taskManager::addTask(task t) {
  t.done = false;
  tasks.push_back(t);
}

// Assign to each non-Busy Thread the task per FIFO algorithm, provided its
// dependencies are done

//TODO: Fix Endless Loop Bug
void taskManager::distributeTasks() {
  if (tasks.empty()) {
    return;
  }

  std::string str;

  static int taskManagerLoops = 0;
  str = "Checked Threads " + std::to_string(taskManagerLoops++) + " times.";
  safePrint(str);

  str = "Tasks: " + std::to_string(tasks.size());
  safePrint(str);

  std::vector<int> dependencyIDs;

  for (auto& w : threads) {
    if (w->gotJob.load() == true) {
      dependencyIDs.push_back(w->current_task.id);
    }
  }

  for (int i = 0; i < dependencyIDs.size(); i++) {
    for (auto& t : tasks) {
      auto found = std::find(t.dependencies.begin(), t.dependencies.end(),
                             dependencyIDs.at(i));
      if (found != t.dependencies.end()) {
        std::lock_guard<std::mutex> lock(mtx);
        t.dependenciesDone++;
        safePrint(str);
      }
    }
  }

  int task_index = findNextTask();
  if (task_index == -1) return;
  task t = tasks.at(task_index);
  tasks.erase(tasks.begin() + task_index);

  bool assigned = false;

  for (auto& w : threads) {
    if (!w->isBusy()) {
      w->assignJob(t);
      assigned = true;

      if (tasks.empty()) {
        break;
      }

      task_index = findNextTask();
      if (task_index == -1) break;
      t = tasks.at(task_index);
      tasks.erase(tasks.begin() + task_index);
    }
  }

  // After Task assigned, wake up all Threads
  if (assigned) {
    cv.notify_all();
  }
}

int taskManager::findNextTask() {
  for (int i = 0; i < tasks.size(); i++) {
    std::lock_guard<std::mutex> lock(mtx);
    if (tasks.at(i).dependenciesDone == tasks.at(i).dependencies.size()) {
      return i;
    }
  }

  return -1;
}
