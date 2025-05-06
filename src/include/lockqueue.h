#pragma once
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable> // 条件变量

// 异步写日志的日志队列
template <typename T>
class LockQueue
{
public:
    // 入队
    void Push(const T &value);

    // 出队
    T& Pop();

    // 判断队列是否为空
    bool Empty()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }
private:
    std::queue<T> m_queue; // 日志队列
    std::mutex m_mutex; // 互斥锁
    std::condition_variable m_condvariable; // 条件变量
};