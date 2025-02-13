#include "Pool.hpp"
#include "Bullet.hpp"

// Template shenanigans
template Pool<Bullet>::Pool(size_t size);
template Pool<Bullet>::~Pool();
template Bullet* Pool<Bullet>::Get();


template <Poolable T>
Pool<T>::Pool(size_t size)
{
    for (int loop = 0; loop < size; loop++)
        queue.push(new T());
}

template <Poolable T>
Pool<T>::~Pool()
{
    while (!queue.empty())
    {
        T* p = queue.front();
        queue.pop();
        delete p;
    }
}

template <Poolable T>
T* Pool<T>::Get()
{
    T* ret = queue.front();
    queue.pop();
    ret->Reset();
    queue.push(ret);
    return ret;
}
