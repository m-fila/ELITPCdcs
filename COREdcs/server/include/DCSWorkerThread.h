#ifndef DCS_WORKER_THREAD_H
#define DCS_WORKER_THREAD_H
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

// RAII worker thread ~ 1-thread thread pool
// To be used as member of Device Controllers
class DCSWorkerThread {

public:
  // RAII worker thread ~ 1-thread thread pool
  // To be used as member of Device Controllers
  DCSWorkerThread() {
    worker = std::thread([this]() { workerLoop(); });
  }
  ~DCSWorkerThread() {
    {
      std::unique_lock<std::mutex> lock(qMutex);
      terminate = true;
    }
    condition.notify_all();
    worker.join();
  }

  // thread-safe job scheduling for last position. Addinational arguments are
  // bound to f
  void push_back(std::function<void()> f) { return pushJob(false, f); }
  // thread-safe job scheduling for first position. Addinational arguments are
  // bound to f
  void push_front(std::function<void()> f) { return pushJob(true, f); }

private:
  // thread infinite loop
  void workerLoop() {
    while (true) {
      std::function<void()> job;
      {
        std::unique_lock<std::mutex> lock(qMutex);
        condition.wait(lock, [this] {
          return !buffer.empty() || !bufferPriority.empty() || terminate;
        });
        if (terminate) {
          return;
        } else if (!bufferPriority.empty()) {
          job = bufferPriority.front();
          bufferPriority.pop();
        } else {
          job = buffer.front();
          buffer.pop();
        }
      }
      job();
    }
  }
  // Binds jobs and pushes it to thread queue and immediately wakes thread
  void pushJob(bool frontQueue, std::function<void()> f) {
    std::function<void()> command = f;
    std::unique_lock<std::mutex> lock(qMutex);
    if (frontQueue) {
      bufferPriority.push(command);
    } else {
      buffer.push(command);
    }
    condition.notify_all();
  }

  std::queue<std::function<void()>> buffer;
  std::queue<std::function<void()>> bufferPriority;
  std::mutex qMutex;
  std::condition_variable condition;
  std::thread worker;
  bool terminate = false;
};

#endif // DCS_WORKER_THREAD_H
