#pragma once
#include "Entity.hpp"
class Game;



class GameManager
{
private:
	Game* game;

public:
	static float Gravity;
	
	sf::View camera;
	Entity* hero;
	std::vector<Entity*> entities;
	GameManager(Game* game);
	void Update();
	void Draw();
	bool HasCollision(int x, int y);
};

