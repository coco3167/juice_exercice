#pragma once
#include <fstream>

#include "Entity.hpp"
class Game;



class GameManager
{
private:
	Game& game;
	sf::Vector2u windowSize;
	
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

	void AddEntity(const int& x, const int& y, const std::string& texture, bool isEnemy);
	bool RemoveEntityByPos(const int& x, const int& y, bool isEnemy);
	bool IsEntity(const int& x, const int& y, bool isEnemy);

	std::vector<Entity*>::iterator GetEntityByPos(const int& x, const int& y);
};

