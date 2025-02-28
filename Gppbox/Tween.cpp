#include "Tween.hpp"

#include <iostream>


void Tween::Update(const float& dt)
{
    if (!isPlaying)
        return;
    NextValue(dt);
    //std::cout << (*variableToTween-endValue)*shouldIncrement << std::endl;
    if((*variableToTween-endValue)*shouldIncrement >= std::numeric_limits<float>::epsilon())
    {
        *variableToTween = endValue;
        isPlaying = false;
        std::cout << "Tween Ended\n" << std::endl;
        if(nextTween != nullptr)
        {
            nextTween->Start();
        }
    }
}

void Tween::Start()
{
    isPlaying = true;
    difference = endValue - *variableToTween;
    shouldIncrement = copysign(shouldIncrement, difference);

    if (nextTween != nullptr)
        nextTween->Stop();

    Display();
}

void Tween::Stop()
{
    isPlaying = false;
}

void Tween::Display()
{
    std::cout << "Var : " << *variableToTween << "\n";
    std::cout << "Duration : " << duration << "\n";
    std::cout << "EndValue : " << endValue << "\n";
    std::cout << "Difference : " << difference << "\n" << std::endl;
}

void Tween::NextValue(const float& dt)
{
    *variableToTween += dt/duration * difference;
    std::cout << *variableToTween << " | ";
}

Tween::Tween(float* variableToTween, const float& endValue, const float& duration, Tween* nextTween):
    variableToTween(variableToTween), endValue(endValue), duration(duration), nextTween(nextTween)
{
}
