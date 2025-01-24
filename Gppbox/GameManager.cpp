#include "GameManager.hpp"
#include "Game.hpp"

GameManager::GameManager(Game* game)
{
	this->game = game;
	hero = new Entity(15, 50, "res/Perso.png", this);
	entities.push_back(hero);
}

void GameManager::Update()
{
	for (std::vector<Entity*>::iterator iterator = entities.begin(); iterator != entities.end(); iterator++)
	{
		(*iterator)->Update();
	}
}

void GameManager::Draw()
{
	for (std::vector<Entity*>::iterator iterator = entities.begin(); iterator != entities.end(); iterator++)
	{
		game->win->draw((*iterator)->Sprite);
	}
}

bool GameManager::HasCollision(int x, int y)
{
	return game->isWall(x, y);
}
