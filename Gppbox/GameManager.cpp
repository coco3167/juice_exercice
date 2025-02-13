#include "GameManager.hpp"

#include <iostream>

#include "C.hpp"
#include "Game.hpp"
#include "Tween.hpp"

GameManager::GameManager(Game& game): 
	game(game),
	screenShakeTween(&viewZoom, .98f, .5f, &unScreenShakeTween),
	unScreenShakeTween(&viewZoom, 1, 1.f),
	bulletPool(Pool<Bullet>(20))
{
	AddEntity(6, 6, "res/Perso.png", false);
	hero = entities.front();
	AddEntity(12,6, "res/Enemy.png", true);

	windowSize = game.win->getSize();
	view.reset(FloatRect(0,0, windowSize.x, windowSize.y));
	view.setCenter(windowSize.x/2, windowSize.y/2);
	viewZoom = 1;
	
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
		Entity& entity = **iterator;
		entity.Update(deltaTime);
		
		// Delete entities if going outside the defined world
		int x = entity.XGrid, y = entity.YGrid;
		if(x < 0 || y < 0 || x > windowSize.x || entity.YGrid > windowSize.y)
		{
			iterator = --entities.erase(iterator);
		}
	}

	for(std::vector<Bullet*>::iterator bulletIterator = hero->bullets.begin(); bulletIterator < hero->bullets.end();)
	{
		Bullet* bullet = *bulletIterator;
		bool bulletErased = false;
		for (std::vector<Entity*>::iterator entityIterator = ++entities.begin(); entityIterator < entities.end();)
		{
			Entity& entity = **entityIterator;
			if(bullet->rectangle.getGlobalBounds().intersects(entity.Sprite.getGlobalBounds()))
			{
				bulletIterator = hero->bullets.erase(bulletIterator);
				entityIterator = entities.erase(entityIterator);
				bulletErased = true;
				break;
			}
			++entityIterator;
		}
		if(!bulletErased)
			++bulletIterator;
	}

	if(screenShakeTween.isPlaying)
	{
		screenShakeTween.Update(deltaTime);
		float x = windowSize.x*C::GRID_SIZE * *screenShakeTween.variableToTween;
		float y = windowSize.y*C::GRID_SIZE * *screenShakeTween.variableToTween;
		view.setSize(x, y);
	}
	else if(unScreenShakeTween.isPlaying)
	{
		unScreenShakeTween.Update(deltaTime);
		float x = windowSize.x*C::GRID_SIZE * *unScreenShakeTween.variableToTween;
		float y = windowSize.y*C::GRID_SIZE * *unScreenShakeTween.variableToTween;
		view.setSize(x, y);
	}
}

void GameManager::Draw()
{
	for (std::vector<Entity*>::iterator iterator = entities.begin(); iterator != entities.end(); ++iterator)
	{
		Entity& entity = **iterator;
		game.win->draw(entity.Sprite);
	}
	for (Bullet* bullet : hero->bullets)
		game.win->draw(bullet->rectangle);
}

bool GameManager::HasCollision(int x, int y)
{
	return game.isWall(x, y);
}

void GameManager::CreateLevel()
{
	// If no file exists creates one and calls back itself
	inputFile.open("res/Level.txt", std::fstream::in);
	if (inputFile.fail())
	{
		inputFile.open("res/Level.txt", std::fstream::out);
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
	inputFile.open("res/Level.txt", std::fstream::out);
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
	inputFile.open("res/Level.txt", std::fstream::in);
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
	std::vector<Entity*>::iterator iterator = GetEntityByPos(x, y);
	if(iterator == entities.end())
		return false;

	if((*iterator)->isEnemy != isEnemy)
		return false;
	
	entities.erase(iterator);
	return true;
}

bool GameManager::IsEntity(const int& x, const int& y, bool isEnemy)
{
	std::vector<Entity*>::iterator iterator = GetEntityByPos(x, y);
	if(iterator == entities.end())
		return false;
	return (*iterator)->isEnemy == isEnemy;
}

std::vector<Entity*>::iterator GameManager::GetEntityByPos(const int& x, const int& y)
{
	for (std::vector<Entity*>::iterator iterator = entities.begin(); iterator<entities.end(); ++iterator)
	{
		Entity& entity = **iterator;
		if(entity.XGrid == x && entity.YGrid == y)
		{
			return iterator;
		}
	}
	return entities.end();
}

void GameManager::ShakeScreen(int weight)
{
	screenShakeTween.isPlaying = true;
}