#pragma once

class Tween
{
public:

    float* variableToTween;
    float endValue;
    float duration;
    int shouldIncrement = 1;
    bool isPlaying = false;

    Tween* nextTween;
    
    Tween(float* variableToTween, const float& endValue, const float& duration, Tween* nextTween = nullptr);
    void NextValue(const float& dt);
    void Update(const float& dt);
};
