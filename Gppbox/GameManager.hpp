#pragma once
#include <fstream>

#include "Entity.hpp"
#include "Game.hpp"
class Game;



class GameManager
{
private:
	Game& game;
	//int levelArray[Game::Cols][Game::LastLine];
	
public:
	int Gravity = 700;
	bool EditMode = false;
	std::fstream inputFile;
	
	sf::View camera;
	Entity* hero;
	std::vector<Entity*> entities;
	
	GameManager(Game& game);
	~GameManager();
	void Update(float deltaTime);
	void Draw();
	bool HasCollision(int x, int y);

	void CreateLevel();
	void SaveLevel();
	void LoadLevel();

	Entity* AddEntity(const int& x, const int& y, const std::string& texture, bool isEnemy);
};

