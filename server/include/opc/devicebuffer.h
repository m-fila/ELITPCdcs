#ifndef DEVICEBUFFER
#define DEVICEBUFFER
#include <queue>
#include <mutex>
template <class T>
class DeviceBuffer{
//simple thread safe wrapper around queue
public:
    void push(T command){
        std::lock_guard<std::mutex> lock(qMutex);
        buffer.push(command );
    }
    T dequeue(){
        std::lock_guard<std::mutex> lock(qMutex);
        T command=buffer.front();
        buffer.pop();
        return command;
    }
    int size(){
     std::lock_guard<std::mutex> lock(qMutex);
     return buffer.size();
    }

private:
    std::queue<T> buffer;
    std::mutex qMutex;
};

#endif // DEVICEBUFFER

