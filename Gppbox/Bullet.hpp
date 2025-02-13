#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Clock.hpp>

#include "IPoolable.hpp"

class Bullet : public IPoolable
{
    static constexpr float SPEED = 2000.f;
    static constexpr float BULLET_DURATION = 1.f;
    const sf::Vector2f RECTANGLE_SIZE = {20.f, 10.f};
    sf::Clock clock;
    bool moveLeft;
    
public:
    sf::RectangleShape rectangle;
    
    Bullet();
    Bullet& operator=(const Bullet& bullet);
    
    void Update(const float& dt);
    void Reset() override;
    void Init(sf::Vector2f position, bool moveLeft = true);
    bool IsBulletEnd() const;
};
