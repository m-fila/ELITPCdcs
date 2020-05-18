#ifndef DCS_WORKER_THREAD_H
#define DCS_WORKER_THREAD_H
#include <condition_variable>
#include <functional>
#include <mutex>
#include <deque>
#include <thread>
#include <iostream>

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
 
  // thread-safe job scheduling for last position. Addinational arguments are bound to f
  void push_back(std::function<void()>f) {
    return pushJob(false,f);
  }
  // thread-safe job scheduling for first position. Addinational arguments are bound to f
void push_front(std::function<void()>f) {
    return pushJob(true,f);
  }

private:

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
        buffer.pop_front();
      }
      job();
      std::cout<< "called from device thread"<<std::endl;
    }
  }
    // Binds jobs and pushes it to thread queue and immediately wakes thread
void pushJob(bool frontQueue, std::function<void()>f) {
    std::function<void()> command=f;
        std::unique_lock<std::mutex> lock(qMutex);
    if(frontQueue){buffer.push_front(command);}
    else{
    buffer.push_back(command);
    }condition.notify_all();
  }

  std::deque<std::function<void()>> buffer;
  std::mutex qMutex;
  std::condition_variable condition;
  std::thread worker;
  bool terminate = false;
};

#endif // DCS_WORKER_THREAD_H
