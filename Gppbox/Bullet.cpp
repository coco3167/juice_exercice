#include "Bullet.hpp"

Bullet::Bullet(): moveLeft(false)
{
    rectangle = sf::RectangleShape(RECTANGLE_SIZE);
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

void Bullet::Reset()
{
    IPoolable::Reset();
    clock.restart();
}

void Bullet::Init(sf::Vector2f position, bool moveLeft)
{
    rectangle.setPosition(position);
    this->moveLeft = moveLeft;
}

bool Bullet::IsBulletEnd() const
{
    return clock.getElapsedTime().asSeconds()>=BULLET_DURATION;
}

