#include "GameManager.hpp"

#include <iostream>

#include "Game.hpp"

GameManager::GameManager(Game& game): game(game)
{
	hero = new Entity(100, 100, "res/Perso.png", *this);
	entities.push_back(hero);

	CreateLevel();
}

GameManager::~GameManager()
{
	for (int loop = 0; loop < entities.size(); loop++)
	{
		delete entities[loop];
	}
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
		for (int columns = 0; columns < Game::Cols; columns++)
		{
			for (int rows = 0; rows < Game::LastLine; rows++)
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
		LoadLevel();
	}
}


void GameManager::SaveLevel()
{
	std::cout << "Saving level" << std::endl;
}

void GameManager::LoadLevel()
{
	for (int columns = 0; columns < Game::Cols; columns++)
	{
		std::string line;
		inputFile >> line;
		for (int rows = 0; rows < Game::LastLine; rows++)
		{
			if (line[rows] == 'X')
				game.AddWall(Vector2i(rows, columns));
		}
	}
	game.cacheWalls();
}
