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
