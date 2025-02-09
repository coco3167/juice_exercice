#include "Bullet.h"

Bullet::Bullet(sf::Vector2f position, bool moveLeft) : moveLeft(moveLeft)
{
    clock.restart();
    rectangle = sf::RectangleShape(RECTANGLE_SIZE);
    rectangle.setPosition(position);
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

bool Bullet::IsBulletEnd() const
{
    return clock.getElapsedTime().asSeconds()>=BULLET_DURATION;
}

