#include "IPoolable.hpp"

#include <iostream>
#include <ostream>

void IPoolable::Reset()
{
    std::cout << "reset" << std::endl;
}
