#pragma once
#include <SFML/Graphics.hpp>
class GameManager;

class Entity
{
private:
	GameManager* gameManager;

public :
	static float XSpeed, XAirSpeed, MaxSpeed, JumpForce, GroundFriction, AirFriction;
	
	sf::Sprite Sprite;
	sf::Texture texture;
	bool isLookingLeft;

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


	Entity(int x, int y, const std::string& texturePath, GameManager* gameManager);

	void Update();

	bool HasCollision(int xGrid, int yGrid);
	void SetCoord(float x, float y);
	void MoveOnX(bool rightMovement);
	void Jump();
};

