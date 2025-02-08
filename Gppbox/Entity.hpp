#pragma once
#include <SFML/Graphics.hpp>
class GameManager;

class Entity
{
private:
	GameManager& gameManager;

public :
	static int XSpeed, XAirSpeed, MaxSpeed, JumpForce;
	static float GroundFriction, AirFriction;
	
	sf::Sprite Sprite;
	sf::Texture texture;
	bool isLookingLeft;
	bool isEnemy;
	bool moveLeft;

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


	Entity(int x, int y, const std::string& texturePath, GameManager& gameManager, bool isEnemy);

	void Update(float deltaTime);

	bool HasCollision(int xGrid, int yGrid);
	void SetCoord(float x, float y);
	void MoveOnX(bool rightMovement = false);
	void Jump();
};

