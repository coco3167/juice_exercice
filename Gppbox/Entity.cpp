
#include "Entity.hpp"
#include "C.hpp"
#include "GameManager.hpp"
#include <iostream>

Entity::Entity(int x, int y, std::string texturePath, GameManager* gameManager)
{
	sf::Texture text;
	text.loadFromFile(texturePath);
	Sprite.setTexture(text);
	SetCoord(x, y);

	this->gameManager = gameManager;
}

void Entity::Update()
{
	XMovement = fmin(XMovement, 2);
	XMovement = fmax(XMovement, -2);

	XRatio += XMovement;
	YRatio += YMovement + gameManager->gravity;
	XMovement *= 0.96;
	YMovement *= 0.96;

	while (XRatio > 1)
	{
		if (HasCollision(XGrid + 1, YGrid) && XRatio >= 0.7)
		{
			XRatio = 0.7;
			XMovement = 0;
			break;
		}
		XRatio--;
		XGrid++;
	}
	while (XRatio < 0)
	{
		if (HasCollision(XGrid - 1, YGrid) && XRatio <= 0.3)
		{
			XRatio = 0.3;
			XMovement = 0;
			break;
		}
		XRatio++;
		XGrid--;
	}
	while (YRatio > 1)
	{
		if (HasCollision(XGrid, YGrid + 1) && YRatio >= 0.7)
		{
			YRatio = 0.7;
			YMovement = 0;
			break;
		}
		YRatio--;
		YGrid++;
	}
	while (YRatio < 0)
	{
		if (HasCollision(XGrid, YGrid - 1) && YRatio <= 0.3)
		{
			YRatio = 0.3;
			YMovement = 0;
			break;
		}
		YRatio++;
		YGrid--;
	}

	if (HasCollision(XGrid + 1, YGrid) && XRatio >= 0.7)
	{
		XRatio = 0.7;
		XMovement = 0;
	}
	if (HasCollision(XGrid - 1, YGrid) && XRatio <= 0.3)
	{
		XRatio = 0.3;
		XMovement = 0;
	}
	if (HasCollision(XGrid, YGrid + 1) && YRatio >= 0.7)
	{
		YRatio = 0.7;
		YMovement = 0;
	}
	if (HasCollision(XGrid, YGrid - 1) && YRatio <= 0.3)
	{
		YRatio = 0.3;
		YMovement = 0;
	}
	

	std::cout << XReal << std::endl;
	std::cout << YReal << std::endl;

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
