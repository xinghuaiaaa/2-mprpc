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
    void Push(const T &value)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(value);
        m_condvariable.notify_one(); // 通知一个等待线程
    }

    // 出队
    T Pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_queue.empty())
        {
            m_condvariable.wait(lock); // 等待条件变量
        }
        T value = m_queue.front();
        m_queue.pop();
        return value;
    }

private:
    std::queue<T> m_queue; // 日志队列
    std::mutex m_mutex; // 互斥锁
    std::condition_variable m_condvariable; // 条件变量
};