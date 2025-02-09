#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Clock.hpp>

class Bullet
{
    static constexpr float SPEED = 2000.f;
    static constexpr float BULLET_DURATION = 1.f;
    const sf::Vector2f RECTANGLE_SIZE = {20.f, 10.f};
    sf::Clock clock;
    bool moveLeft;
    
public:
    sf::RectangleShape rectangle;
    
    Bullet(sf::Vector2f position, bool moveLeft = true);
    Bullet& operator=(const Bullet& bullet);
    
    void Update(const float& dt);
    bool IsBulletEnd() const;
};
