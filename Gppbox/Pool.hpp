#pragma once
#include <queue>

#include "IPoolable.hpp"

template<typename T>
concept Poolable = requires(T t) {std::is_base_of_v<IPoolable, T>;};

template<Poolable T>
class Pool
{
private:
    std::queue<T*> queue;
public:
    Pool(size_t size);
    ~Pool();
    T* Get();
};
