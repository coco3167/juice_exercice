#pragma once
#include "Entity.hpp"
class Game;

class GameManager
{
private:
	Game* game;

public:
	const float xSpeed = 0.01;
	const float gravity = 0.05;
	const float jumpForce = 0.8;
	Entity* hero;
	std::vector<Entity*> entities;
	GameManager(Game* game);
	void Update();
	void Draw();
	bool HasCollision(int x, int y);
};

