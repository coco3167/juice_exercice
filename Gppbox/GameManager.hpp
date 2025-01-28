#pragma once
#include "Entity.hpp"
class Game;



class GameManager
{
private:
	Game* game;

public:
	static int Gravity;
	static bool EditMode;
	
	sf::View camera;
	Entity* hero;
	std::vector<Entity*> entities;
	
	GameManager(Game* game);
	void Update(float deltaTime);
	void Draw();
	bool HasCollision(int x, int y);
};

