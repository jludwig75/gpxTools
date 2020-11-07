#pragma once

#include <condition_variable>
#include <deque>
#include <mutex>

template<typename DataType>
class Container
{
public:
    bool empty() const
    {
        std::unique_lock lock(_lock);
        return _deque.empty();
    }
    // This is only thread safe if only one thread calls front() and pop_front().
    DataType& front()
    {
        std::unique_lock lock(_lock);
        _cond.wait(lock, [this] { return !_deque.empty(); } );
        
        return _deque.front();
    }
    // This is only thread safe if only one thread calls front() and pop_front().
    void pop_front()
    {
        std::unique_lock lock(_lock);
        _cond.wait(lock, [this] { return !_deque.empty(); } );

        _deque.pop_front();
    }
    void push_back(DataType& data)
    {
        std::unique_lock lock(_lock);
        _deque.push_back(data);
        _cond.notify_one();
    }
    size_t size() const
    {
        std::unique_lock lock(_lock);
        return _deque.size();
    }
private:
    std::deque<DataType> _deque;
    mutable std::mutex _lock;
    std::condition_variable _cond;
};