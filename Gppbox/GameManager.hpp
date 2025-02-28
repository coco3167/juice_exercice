#pragma once
#include <fstream>

#include "Entity.hpp"
#include "Pool.hpp"
#include "Tween.hpp"
#include "Bullet.hpp"

class Game;

class GameManager
{
private:
	Game& game;
	sf::Vector2u windowSize;
	
public:
	int Gravity = 700;
	bool EditMode = false;
	std::fstream inputFile;
	
	sf::View view;
	Entity* hero;
	std::vector<Entity*> entities;
	Pool<Bullet> bulletPool;

	float viewZoom, viewZoomLaser;
	Tween screenShakeTween, unScreenShakeTween;
	Tween screenShakeLaserTween, unScreenShakeLaserTween;
	
	GameManager(Game& game);
	~GameManager();
	void Update(float deltaTime);
	void Draw();
	bool HasCollision(int x, int y);

	void CreateLevel();
	void SaveLevel();
	void LoadLevel();

	void AddEntity(const int& x, const int& y, const std::string& texture, bool isEnemy);
	bool RemoveEntityByPos(const int& x, const int& y, bool isEnemy);
	bool IsEntity(const int& x, const int& y, bool isEnemy);

	void ShakeScreen();
	void ShakeScreenLaser();

	std::vector<Entity*>::iterator GetEntityByPos(const int& x, const int& y);
};

