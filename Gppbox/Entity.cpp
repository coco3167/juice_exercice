#include <iostream>

#include "C.hpp"
#include "GameManager.hpp"
#include "Entity.hpp"


int Entity::XSpeed = 10, Entity::XAirSpeed = 2, Entity::MaxSpeed = 50, Entity::JumpForce = 150;
float Entity::GroundFriction = 0.1f, Entity::AirFriction = 0.5f;

Entity::Entity(int x, int y, const std::string &texturePath, GameManager& gameManager, bool isEnemy) : m_gameManager(&gameManager), isEnemy(isEnemy), deathTween(&isAlive, 1.f, .05f)
{
	texture.loadFromFile(texturePath);
	Sprite.setTexture(texture);
	Sprite.setScale(2,2);
	Sprite.setOrigin(Sprite.getTexture()->getSize().x / 2, Sprite.getTexture()->getSize().y / 2);
	SetCoord(x*C::GRID_SIZE, y*C::GRID_SIZE);

	// Randomize starting movement direction
	moveLeft = std::rand()%2 == 0;
}

Entity& Entity::operator=(const Entity& entity)
{
	m_gameManager = entity.m_gameManager;
	isEnemy = entity.isEnemy;
	texture = entity.texture;
	Sprite.setTexture(texture);
	Sprite.setScale(entity.Sprite.getScale());
	Sprite.setOrigin(entity.Sprite.getOrigin());
	SetCoord(entity.XReal, entity.YReal);
	moveLeft = entity.moveLeft;
	return *this;
}

void Entity::Update(float deltaTime)
{
	if(deathTween.isPlaying)
		deathTween.Update(deltaTime);
	if(isAlive > 0)
		return;
	
	if (isEnemy)
		MoveOnX(moveLeft);
	
	XMovement = std::clamp(XMovement, -float(MaxSpeed), float(MaxSpeed));
	YMovement += m_gameManager->Gravity*deltaTime;
	
	XRatio += (XMovement + recoil)*deltaTime;
	YRatio += YMovement*deltaTime;

	recoil = 0;

	// Sprite direction
	sf::IntRect textureRect = sf::IntRect(Sprite.getTextureRect());
	if (XMovement < 0 && !isLookingLeft)
	{
		textureRect.left = textureRect.width;
		textureRect.width = -textureRect.width;
		Sprite.setTextureRect(textureRect);
		isLookingLeft = true;
	}
	else if (XMovement > 0 && isLookingLeft)
	{
		textureRect.left = 0;
		textureRect.width = -textureRect.width;
		Sprite.setTextureRect(textureRect);
		isLookingLeft = false;
	}

	// Dragging
	XMovement *= OnGround ? std::pow(GroundFriction, 10*deltaTime) : std::pow(AirFriction, 10*deltaTime);

	// Apply movement and check for collision
	while (XRatio > 1)
	{
		if (HasCollision(XGrid + 2, YGrid) || HasCollision(XGrid + 2, YGrid + 1))
		{
			XRatio = 1;
			XMovement = 0;
			moveLeft = false;
			break;
		}
		XRatio--;
		XGrid++;
	}
	while (XRatio < 0)
	{
		if (HasCollision(XGrid - 2, YGrid) || HasCollision(XGrid - 2, YGrid + 1))
		{
			XRatio = 0;
			XMovement = 0;
			moveLeft = true;
			break;
		}
		XRatio++;
		XGrid--;
	}
	while (YRatio > 1)
	{
		if (HasCollision(XGrid, YGrid + 2) || HasCollision(XGrid - 1, YGrid + 2))
		{
			YRatio = 1;
			YMovement = 0;
			OnGround = true;
			break;
		}
		YRatio--;
		YGrid++;
	}
	while (YRatio < 0)
	{
		if (HasCollision(XGrid, YGrid - 2) || HasCollision(XGrid - 1, YGrid - 2))
		{
			YRatio = 0;
			YMovement = 0.f;
			break;
		}
		YRatio++;
		YGrid--;
	}

	//Sprite position
	XReal = (XGrid + XRatio) * C::GRID_SIZE;
	YReal = (YGrid + YRatio) * C::GRID_SIZE;
	Sprite.setPosition(XReal, YReal);

	// Bullets handling
	for(std::vector<Bullet*>::iterator iterator = bullets.begin(); iterator < bullets.end();)
	{
		Bullet* bullet(*iterator);
		bullet->Update(deltaTime);
		
		if(bullet->IsBulletEnd())
			iterator = bullets.erase(iterator);
		else
			++iterator;
	}
}

bool Entity::HasCollision(int xGrid, int yGrid)
{
	return m_gameManager->HasCollision(xGrid, yGrid);
}

void Entity::SetCoord(float x, float y)
{
	XReal = x;
	YReal = y;
	XGrid = int(x / C::GRID_SIZE);
	YGrid = int(y / C::GRID_SIZE);
	XRatio = (x - XGrid * C::GRID_SIZE)/C::GRID_SIZE;
	XRatio = (x - XGrid * C::GRID_SIZE)/C::GRID_SIZE;
}

void Entity::MoveOnX(bool rightMovement)
{
	XMovement += (rightMovement ? 1 : -1) * (OnGround ? XSpeed : XAirSpeed);
}

void Entity::Jump()
{
	if (!OnGround)
		return;

	YMovement -= JumpForce;
	OnGround = false;
}

void Entity::Shoot()
{
	if(m_shootTime.getElapsedTime().asSeconds() >= SHOOT_INTERVAL)
	{
		m_gameManager->ShakeScreen(10);
		bullets.emplace_back(m_gameManager->bulletPool.Get());
		bullets.back()->Init({XReal, YReal - 5}, isLookingLeft);

		recoil = (isLookingLeft ? 1 : -1)*100;
		
		m_shootTime.restart();
	}
	
}

void Entity::Hurt()
{
	Sprite.setColor(sf::Color::Red);
	deathTween.isPlaying = true;
}


