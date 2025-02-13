#include "Tween.hpp"

#include <iostream>


void Tween::Update(const float& dt)
{
    NextValue(dt);
    if((*variableToTween-endValue)*shouldIncrement >= 0)
    {
        *variableToTween = endValue;
        isPlaying = false;
        if(nextTween != nullptr)
            nextTween->isPlaying = true;
    }
}

void Tween::NextValue(const float& dt)
{
    *variableToTween += (dt/duration) * std::abs(endValue-*variableToTween)*shouldIncrement;
    // std::cout << *variableToTween << std::endl;
}

Tween::Tween(float* variableToTween, const float& endValue, const float& duration, Tween* nextTween):
    variableToTween(variableToTween), endValue(endValue), duration(duration), nextTween(nextTween)
{
    shouldIncrement = copysign(shouldIncrement, endValue-*variableToTween);
}
