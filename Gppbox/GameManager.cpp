#include "GameManager.hpp"
#include "Game.hpp"

int GameManager::Gravity = 700;
bool GameManager::EditMode = false;

GameManager::GameManager(Game* game)
{
	this->game = game;
	hero = new Entity(100, 100, "res/Perso.png", this);
	entities.push_back(hero);
}

void GameManager::Update(float deltaTime)
{
	for (std::vector<Entity*>::iterator iterator = entities.begin(); iterator != entities.end(); ++iterator)
	{
		(*iterator)->Update(deltaTime);
	}
}

void GameManager::Draw()
{
	for (std::vector<Entity*>::iterator iterator = entities.begin(); iterator != entities.end(); ++iterator)
	{
		game->win->draw((*iterator)->Sprite);
	}
}

bool GameManager::HasCollision(int x, int y)
{
	return game->isWall(x, y);
}
