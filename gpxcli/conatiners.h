#pragma once

template<typename DataType>
class Container
{
public:
    bool empty() const
    {
        return _deque.empty();
    }
    DataType& front()
    {
        return _deque.front();
    }
    void pop_front()
    {
        _deque.pop_front();
    }
    void push_back(DataType& data)
    {
        _deque.push_back(data);
    }
    size_t size() const
    {
        return _deque.size();
    }
private:
    std::deque<DataType> _deque;
};