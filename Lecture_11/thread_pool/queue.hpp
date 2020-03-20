#pragma once

#include <shared_mutex>
#include <queue>

template <typename T>
class ConcurentQueue
{
public:
    bool empty()
    {
        std::shared_lock<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    int size()
    {
        std::shared_lock<std::mutex> lock(m_mutex);
        return m_queue.size();
    }

    void push(T t)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push(t);
    }

      bool pop(T& t) {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_queue.empty()) {
          return false;
        }
        t = std::move(m_queue.front());
        m_queue.pop();
        return true;
      }

    bool try_pop(T& t)
    {
       std::unique_lock<std::mutex> lock(m_mutex, std::defer_lock);
       if (!lock.try_lock()) {
         return false; 
       }
       if (m_queue.empty()) {
         return false;
       }
        t = std::move(m_queue.front());
        m_queue.pop();
        return true;
    }

private:
    std::queue<T> m_queue;
    std::shared_mutex m_mutex;
};