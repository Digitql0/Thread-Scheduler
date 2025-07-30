#include "thread_manager.hpp"

#include <condition_variable>
#include <mutex>
#include <thread>

worker::worker(std::condition_variable& c, std::mutex& m)
    : cv(c), mtx(m), isIdle(true) {
  shutdown.store(false);
  gotJob.store(false);

  static int worker_count = 1;
  id = worker_count;
  std::string str =
      "Worker ID " + std::to_string(worker_count++) + " initialized";
  safePrint(str);

  current_task.done = true;

  t = std::thread(&worker::idle, this);
}

worker::~worker() {
  std::string str = "Worker " + std::to_string(id) + " shutting down.";
  safePrint(str);
  shutdown.store(true);        // Shutdown signal this Thread
  cv.notify_all();             // Wake up all Threads
  if (t.joinable()) t.join();  // Shutdown this Thread
}

void worker::idle() {
  while (!shutdown.load()) {
    std::unique_lock<std::mutex> lock(mtx);

    // Wait until a new task is assigned or shutdown is signaled
    cv.wait(lock, [this] { return (!current_task.done) || shutdown.load(); });

    if (shutdown.load()) return;

    std::string str = "Worker " + std::to_string(id) + " executes its task.";
    safePrint(str);

    if (current_task.task != nullptr) {
      current_task.task(current_task.data);
    } else {
      str = "Worker " + std::to_string(id) + " has a nullptr task.";
      safePrint(str);
    }

    current_task.done = true;
    isIdle = true;
  }
}

bool worker::assignJob(const task& tsk) {
  std::unique_lock<std::mutex> lock(mtx);
  if (isIdle) {
    std::string str = "Worker " + std::to_string(id) + " was assigned job id " +
                      std::to_string(tsk.id);
    safePrint(str);

    current_task = tsk;

    isIdle = false;

    if (gotJob.load() == false) gotJob.store(true);

    cv.notify_all();

    return true;
  } else {
    return false;
  }
}

bool worker::isBusy() { return !isIdle; }
