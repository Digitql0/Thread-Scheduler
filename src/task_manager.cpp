#include "task_manager.hpp"

#include <algorithm>
#include <memory>

taskManager::taskManager() {
  std::string str = "Task Manager Initialized";
  safePrint(str);

  makeHardwareThreads();
}

void taskManager::makeHardwareThreads() {
  // Get Available Threads
  unsigned int threadCount = std::thread::hardware_concurrency();
  std::string str = std::to_string(threadCount) + " Threads Available.";
  safePrint(str);
  if (threadCount == 0) {
    threadCount = 4;  // fallback to 4 if undetectable
  }

  // Emplace a worker for every available Thread
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

//  FIXME: Fix Endless Loop Bug
void taskManager::distributeTasks() {
  if (tasks.empty()) {
    return;
  }

  std::string str;

  static int taskManagerLoops = 0;
  str = "Checked Threads " + std::to_string(taskManagerLoops++) + " times.";
  safePrint(str);

  str = "Tasks: " + std::to_string(tasks.size()) + " ";
  safePrint(str);

  str = "Left Tasks: \n";
  for (auto& t : tasks) {
    str += "[" + std::to_string(t.id) + "]: {";
    for (auto& d : t.dependencies) {
      str += std::to_string(d) + ", ";
    }
    str += "} \n";
  }
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

  dependencyIDs.clear();

  // Pick Next Task
  int task_index = findNextTask();
  if (task_index == -1) {
    return;
  }
  task t = tasks.at(task_index);

  bool assigned = false;

  for (auto& w : threads) {
    if (!w->isBusy() || w->current_task.done) {
      if (w->gotJob.load()) {
        dependencyIDs.push_back(w->current_task.id);
      }

      if (w->assignJob(t)) {
        tasks.erase(tasks.begin() + task_index);
      }

      assigned = true;

      if (tasks.empty()) {
        break;
      }

      // Pick next Task
      task_index = findNextTask();
      if (task_index == -1) {
        break;
      }
      t = tasks.at(task_index);
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
