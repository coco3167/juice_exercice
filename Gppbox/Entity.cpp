
#include "Entity.hpp"
#include "C.hpp"
#include "GameManager.hpp"
#include <iostream>

float Entity::XSpeed = 0.003f, Entity::XAirSpeed = 0.002f, Entity::MaxSpeed = 2.f, Entity::JumpForce = 0.5f, Entity::GroundFriction = 0.96f, Entity::AirFriction = 0.98f;

Entity::Entity(int x, int y, const std::string &texturePath, GameManager* gameManager)
{
	texture.loadFromFile(texturePath);
	Sprite.setTexture(texture);
	Sprite.setScale(2,2);
	Sprite.setOrigin(Sprite.getTexture()->getSize().x / 2, Sprite.getTexture()->getSize().y / 2);
	SetCoord(x, y);

	this->gameManager = gameManager;
}

void Entity::Update()
{
	XMovement = std::clamp(XMovement, -MaxSpeed, MaxSpeed);
	YMovement += GameManager::Gravity;

	XRatio += XMovement;
	YRatio += YMovement;

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

	XMovement *= OnGround ? GroundFriction : AirFriction;
	YMovement *= GroundFriction;

	while (XRatio > 1)
	{
		if (HasCollision(XGrid + 2, YGrid) || HasCollision(XGrid + 2, YGrid + 1))
		{
			XRatio = 1;
			XMovement = 0;
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

	// End Update
	XReal = int((XGrid + XRatio) * C::GRID_SIZE);
	YReal = int((YGrid + YRatio) * C::GRID_SIZE);
	Sprite.setPosition(XReal, YReal);
}

bool Entity::HasCollision(int xGrid, int yGrid)
{
	return gameManager->HasCollision(xGrid, yGrid);
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