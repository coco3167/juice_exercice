#pragma once

class Tween
{
private:
    float* variableToTween;
    float endValue;
    float duration;
    int shouldIncrement = 1;
    float difference;

    Tween* nextTween;

public:
    bool isPlaying = false;
    
    Tween(float* variableToTween, const float& endValue, const float& duration, Tween* nextTween = nullptr);
    void NextValue(const float& dt);
    void Update(const float& dt);
    void Start();
    void Stop();
    void Display();
};
