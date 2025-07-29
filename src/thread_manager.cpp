#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

#include "thread_manager.hpp"

worker::worker(std::condition_variable& c, std::mutex& m)
    : cv(c), mtx(m), isIdle(false) {
  t = std::thread(&worker::idle, this);
  shutdown.store(false);
  gotJob.store(false);
  static int worker_count = 1;
  id = worker_count;
  std::string str =
      "Worker ID " + std::to_string(worker_count++) + " initialized";
  safePrint(str);
}

worker::~worker() {
  std::string str = "Worker " + std::to_string(id) + " shutting down.";
  safePrint(str);
  shutdown.store(true);        // Shutdown signal this Threads
  cv.notify_all();             // Wake up all Threads
  if (t.joinable()) t.join();  // Shutdown this Thread
}

void worker::idle() {
  while (!shutdown.load()) {
    std::unique_lock<std::mutex> lock(mtx);
    // Wait until a new task is assigned or shutdown is signaled
    cv.wait(lock, [this] { return !isIdle || shutdown.load(); });

    if (shutdown.load()) return;

    if (current_task.task) {
      std::string str = "Worker " + std::to_string(id) + " executes its task.";
      safePrint(str);
      current_task.task(current_task.data);
      current_task.done = true;
    }

    isIdle = true;
  }
}

void worker::assignJob(const task& tsk) {
  std::unique_lock<std::mutex> lock(mtx);
  if (isIdle) {
    std::string str = "Worker " + std::to_string(id) + " was assigned a job.";
    safePrint(str);
    current_task = tsk;
    isIdle = false;
    std::lock_guard<std::mutex> lock(mtx);
    if (gotJob.load() == false) gotJob.store(true);
    cv.notify_all();
  }
}

bool worker::isBusy() { return !isIdle; }
