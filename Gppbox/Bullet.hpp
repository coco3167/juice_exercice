#pragma once
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>

#include "IPoolable.hpp"

class Bullet : public IPoolable
{
    static constexpr float SPEED = 2000.f;
    static constexpr float BULLET_DURATION = 1.f;
    const sf::Vector2f RECTANGLE_SIZE = {20.f, 10.f};
    sf::Clock clock;
    bool moveLeft;

    int startCircleFrames;
    
public:
    sf::RectangleShape rectangle;
    sf::CircleShape circle;
    
    Bullet();
    Bullet& operator=(const Bullet& bullet);
    
    void Update(const float& dt);
    void Draw(sf::RenderWindow* win);
    void Reset() override;
    void Init(sf::Vector2f position, bool moveLeft = true);
    bool IsBulletEnd() const;
};
