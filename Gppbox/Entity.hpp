#pragma once
#include <SFML/Graphics.hpp>
#include "Bullet.hpp"
#include "Tween.hpp"

class GameManager;

class Entity
{
private:
	GameManager* m_gameManager;

	// Shoot parameters
	static constexpr float SHOOT_INTERVAL = .1f;
	sf::Clock m_shootTime;

	static constexpr float LASER_WIDTH = 2.f;

public:
	static int XSpeed, XAirSpeed, MaxSpeed, JumpForce;
	static float GroundFriction, AirFriction;
	
	sf::Sprite Sprite;
	sf::Texture texture;
	
	sf::RectangleShape laser;
	bool isLasering;
	
	bool isLookingLeft;
	bool isEnemy;
	bool moveLeft;

	// Death
	Tween deathTween;
	float isAlive = 0.f;

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

	std::vector<Bullet*> bullets;
	float recoil;

	


	Entity(int x, int y, const std::string& texturePath, GameManager& gameManager, bool isEnemy);
	Entity& operator=(const Entity& entity);

	void Update(float deltaTime);
	void Draw(sf::RenderWindow* win);

	bool HasCollision(int xGrid, int yGrid);
	void SetCoord(float x, float y);
	void MoveOnX(bool rightMovement = false);
	void Jump();
	void Shoot();
	void StartLasering();
	void StopLasering();
	void Hurt();
};

