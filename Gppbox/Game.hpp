#pragma once

#include <vector>

#include "GameManager.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include "sys.hpp"

#include "Particle.hpp"
#include "ParticleMan.hpp"


class GameManager;
class HotReloadShader;
class Game {
public:
	static int Cols, LastLine;
	
	sf::RenderWindow*				win = nullptr;

	sf::RectangleShape				bg;
	HotReloadShader *				bgShader = nullptr;

	sf::Texture						tex;

	bool							closing = false;
	
	std::vector<sf::Vector2i>		walls;
	std::vector<sf::Sprite> wallSprites;
	sf::Texture	wallTexture;

	ParticleMan beforeParts;
	ParticleMan afterParts;

	GameManager* gameManager;

	Game(sf::RenderWindow * win);
	~Game();

	void cacheWalls();

	void processInput(sf::Event ev);
	bool wasPressed = false;
	void pollInput(double dt);
	void onSpacePressed();

	void update(double dt);

	void draw(sf::RenderWindow& win);

	void AddWall(Vector2i wallPos);
	void RemoveWall(Vector2i wallPos);
	bool isWall(int cx, int cy);
	void im();
};