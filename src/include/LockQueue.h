#pragma once

#include <queue>
#include <mutex>    // pthread_mutex_t
#include <condition_variable>   // pthread_condition_t

// 异步写日志的日志队列
template<class T>
class LockQueue
{
public:
    // 多个worker线程都会写日志queue 
    void push(const T &data)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _queue.push(data);
        _cond.notify_one();
    }

    // 一个线程读日志queue，写日志文件
    T pop()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        while(_queue.empty())
        {
            // 日志队列为空，线程进入wait状态
            _cond.wait(lock);
        }
        
        T data = _queue.front();
        _queue.pop();
        return data;
    }

private:
    std::queue<T> _queue;
    std::mutex _mutex;
    std::condition_variable _cond;
};