#pragma once
#include "Entity.hpp"
class Game;

class GameManager
{
private:
	Game* game;

public:
	const float gravity = 0.003;
	Entity* hero;
	std::vector<Entity*> entities;
	GameManager(Game* game);
	void Update();
	void Draw();
	bool HasCollision(int x, int y);
};

