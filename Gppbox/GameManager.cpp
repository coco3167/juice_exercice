#include "GameManager.hpp"

#include <iostream>

#include "C.hpp"
#include "Game.hpp"

GameManager::GameManager(Game& game): game(game)
{
	AddEntity(6, 6, "res/Perso.png", false);
	AddEntity(12,6, "res/Perso.png", true);

	windowSize = game.win->getSize();
	windowSize.x /= C::GRID_SIZE;
	windowSize.y /= C::GRID_SIZE;

	CreateLevel();
}

GameManager::~GameManager()
{
	for (std::vector<Entity*>::iterator iterator = entities.begin(); iterator != entities.end(); ++iterator)
	{
		delete *iterator;
	}
	entities.clear();
}

void GameManager::Update(float deltaTime)
{
	for (std::vector<Entity*>::iterator iterator = entities.begin(); iterator != entities.end(); ++iterator)
	{
		Entity* entity = *iterator;
		entity->Update(deltaTime);
		int x = entity->XGrid, y = entity->YGrid;
		if(x < 0 || y < 0 || x > windowSize.x || entity->YGrid > windowSize.y)
		{
			iterator = --entities.erase(iterator);
			delete entity;
		}
	}
	std::cout << entities.size() << std::endl;
}

void GameManager::Draw()
{
	for (std::vector<Entity*>::iterator iterator = entities.begin(); iterator != entities.end(); ++iterator)
	{
		game.win->draw((*iterator)->Sprite);
	}
}

bool GameManager::HasCollision(int x, int y)
{
	return game.isWall(x, y);
}

void GameManager::CreateLevel()
{
	inputFile.open("Level.txt", std::fstream::in);
	if (inputFile.fail())
	{
		inputFile.open("Level.txt", std::fstream::out);
		for (int rows = 0; rows < Game::LastLine + 1; rows++)
		{
			for (int columns = 0; columns < Game::Cols; columns++)
			{
				inputFile << "0";
			}
			inputFile << "\n";
		}
		inputFile.flush();
		inputFile.close();
		CreateLevel();
	}
	else
	{
		inputFile.close();
		LoadLevel();
	}
}


void GameManager::SaveLevel()
{
	inputFile.open("Level.txt", std::fstream::out);
	for (int rows = 0; rows < Game::LastLine + 1; rows++)
	{
		for (int columns = 0; columns < Game::Cols; columns++)
		{
			if (game.isWall(columns, rows))
				inputFile << "X";
			else
				inputFile << "0";
		}
		inputFile << "\n";
	}
	inputFile.close();
}

void GameManager::LoadLevel()
{
	game.walls.clear();
	inputFile.open("Level.txt", std::fstream::in);
	std::string line;
	int row = 0;
	while (inputFile >> line)
	{
		for (int columns = 0; columns < static_cast<int>(line.size()); columns++)
		{
			if (line[columns] == 'X')
				game.AddWall(Vector2i(columns, row));
		}
		++row;
	}
	game.cacheWalls();
	inputFile.close();
}

void GameManager::AddEntity(const int& x, const int& y, const std::string& texture, bool isEnemy)
{
	entities.emplace_back(new Entity(x, y, texture, *this, isEnemy));
}

bool GameManager::RemoveEntityByPos(const int& x, const int& y, bool isEnemy)
{
	auto iterator = GetEntityByPos(x, y);
	if(iterator == entities.end())
		return false;

	if((*iterator)->isEnemy != isEnemy)
		return false;
	
	delete *iterator;
	entities.erase(iterator);
	return true;
}

bool GameManager::IsEntity(const int& x, const int& y, bool isEnemy)
{
	auto iterator = GetEntityByPos(x, y);
	if(iterator == entities.end())
		return false;
	return (*iterator)->isEnemy == isEnemy;
}

std::vector<Entity*>::iterator GameManager::GetEntityByPos(const int& x, const int& y)
{
	for (std::vector<Entity*>::iterator iterator = entities.begin(); iterator<entities.end(); ++iterator)
	{
		Entity* entity = *iterator;
		if(entity->XGrid == x && entity->YGrid == y)
		{
			return iterator;
		}
	}
	return entities.end();
}
