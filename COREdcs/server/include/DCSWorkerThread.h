#ifndef DCS_WORKER_THREAD_H
#define DCS_WORKER_THREAD_H
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <iostream>

  // RAII worker thread ~ 1-thread thread pool
  // To be used as member of Device Controllers
class DCSWorkerThread {

public:
  // RAII worker thread ~ 1-thread thread pool
  // To be used as member of Device Controllers
  DCSWorkerThread() {
    worker = std::thread([this] { workerLoop(); });
  }
  ~DCSWorkerThread() {
    {
      std::unique_lock<std::mutex> lock(qMutex);
      terminate = true;
    }
    condition.notify_all();
    worker.join();
  }
  // Pushes jobs to thread queue and immediately wakes thread
  inline void pushJob(std::function<void()> command) {
    std::unique_lock<std::mutex> lock(qMutex);
    buffer.push(command);
    condition.notify_all();
  }
  // Binds jobs and pushes it to thread queue and immediately wakes thread
  template <class F, class... Args> void pushJob(F f, Args... args) {
    std::function<void()> command = std::bind(f, args...);
    pushJob(command);
  }

private:
    //thread-safe queue front+pop
  inline std::function<void()> dequeue() {
    std::unique_lock<std::mutex> lock(qMutex);
    auto command = buffer.front();
    buffer.pop();
    return command;
  }
  //thread-safe queue size
  inline int size() {
    std::unique_lock<std::mutex> lock(qMutex);
    return buffer.size();
  }
    
  inline bool empty() {
    std::unique_lock<std::mutex> lock(qMutex);
    return buffer.empty();
  }
    // thread infinite loop
  void workerLoop() {
    while (true) {
      std::function<void()> job;
      {
        std::unique_lock<std::mutex> lock(qMutex);
        condition.wait(lock, [this] { return !buffer.empty() || terminate; });
        if (terminate) {
          return;
        }
        job = buffer.front();
        buffer.pop();
      }
      job();
      std::cout<< "called from device thread"<<std::endl;
    }
  }
  std::queue<std::function<void()>> buffer;
  std::mutex qMutex;
  std::condition_variable condition;
  std::thread worker;
  bool terminate = false;
};

#endif // DCS_WORKER_THREAD_H
