#include "Bullet.hpp"

#include <SFML/Graphics/CircleShape.hpp>

Bullet::Bullet(): moveLeft(false)
{
    rectangle = sf::RectangleShape(RECTANGLE_SIZE);
    circle = sf::CircleShape(RECTANGLE_SIZE.y);
}

Bullet& Bullet::operator=(const Bullet& bullet)
{
    rectangle.setPosition(bullet.rectangle.getPosition());
    moveLeft = bullet.moveLeft;
    return *this;
}

void Bullet::Update(const float& dt)
{
    rectangle.setPosition(rectangle.getPosition() + sf::Vector2f(SPEED * dt * (moveLeft ? -1 : 1),0));
}

void Bullet::Draw(sf::RenderWindow* win)
{
    win->draw(rectangle);
    if(startCircleFrames < 10)
    {
        win->draw(circle);
        startCircleFrames++;
    }
}

void Bullet::Reset()
{
    IPoolable::Reset();
    clock.restart();
}

void Bullet::Init(sf::Vector2f position, bool moveLeft)
{
    rectangle.setPosition(position + sf::Vector2f(0,-RECTANGLE_SIZE.y));
    circle.setPosition(position + sf::Vector2f(0,-RECTANGLE_SIZE.y));
    this->moveLeft = moveLeft;
    startCircleFrames = 0;
}

bool Bullet::IsBulletEnd() const
{
    return clock.getElapsedTime().asSeconds()>=BULLET_DURATION;
}

