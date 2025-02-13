#pragma once

class IPoolable
{
public:
    virtual ~IPoolable() = default;
    virtual void Reset();
};
