#pragma once
#include <SFML/Graphics.hpp>
class GameManager;

class Entity
{
private:
	GameManager* gameManager;

public :
	sf::Sprite Sprite;

	// Base coordinates
	int XGrid;
	int YGrid;
	float XRatio;
	float YRatio;

	// Resulting coordinates
	float XReal;
	float YReal;

	// Movements
	float XMovement;
	float YMovement;

	bool OnGround;
	float XSpeed = 0.003f, XAirSpeed = 0.002f, MaxSpeed = 2.f, JumpForce = 0.5f, GroundFriction = 0.96f, AirFriction = 0.98f;


	Entity(int x, int y, std::string texturePath, GameManager* gameManager);

	void Update();

	bool HasCollision(int xGrid, int yGrid);
	void SetCoord(float x, float y);
	void MoveOnX(bool rightMovement);
	void Jump();
};

