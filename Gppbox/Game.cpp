
#include <imgui.h>
#include <array>
#include <vector>

#include "C.hpp"
#include "Game.hpp"

#include <iostream>

#include "HotReloadShader.hpp"


int Game::Cols = 1280 / C::GRID_SIZE;
int Game::LastLine = 720 / C::GRID_SIZE - 1;

Game::Game(sf::RenderWindow * win) : gameManager(new GameManager(*this)) {
	this->win = win;
	bg = sf::RectangleShape(Vector2f((float)win->getSize().x, (float)win->getSize().y));

	/*bool isOk = tex.loadFromFile("res/bg.jpg");
	if (!isOk) {
		printf("ERR : LOAD FAILED\n");
	}*/
	bg.setTexture(&tex);
	bg.setSize(sf::Vector2f(1280, 720));

	bgShader = new HotReloadShader("res/bg.vert", "res/bg.frag");

	wallTexture.loadFromFile("res/wall.png");
	/*
	for (int i = 0; i < 1280 / C::GRID_SIZE; ++i) 
		walls.push_back( Vector2i(i, LastLine) );

	for (int loop = 1; loop <= LastLine; loop++)
	{
		walls.push_back(Vector2i(0, LastLine - loop));
		walls.push_back(Vector2i(Cols - 1, LastLine - loop));
	}

	walls.push_back(Vector2i(Cols >>2, LastLine - 2));
	walls.push_back(Vector2i(Cols >>2, LastLine - 3));
	walls.push_back(Vector2i(Cols >>2, LastLine - 4));
	walls.push_back(Vector2i((Cols >> 2) + 1, LastLine - 4));
	walls.push_back(Vector2i((Cols >> 2) + 2, LastLine - 4));
	walls.push_back(Vector2i((Cols >> 2) + 3, LastLine - 4));
	walls.push_back(Vector2i((Cols >> 2) - 1, LastLine - 4));
	walls.push_back(Vector2i((Cols >> 2) - 2, LastLine - 4));
	walls.push_back(Vector2i((Cols >> 2) - 3, LastLine - 4));
	cacheWalls();
	*/
}

Game::~Game()
{
	delete gameManager;
}

void Game::cacheWalls()
{
	wallSprites.clear();
	for (Vector2i & w : walls) {
		/*sf::RectangleShape rect(Vector2f(16,16));
		rect.setPosition((float)w.x * C::GRID_SIZE, (float)w.y * C::GRID_SIZE);
		rect.setFillColor(sf::Color(0x07ff07ff));*/
		Sprite wallSprite = Sprite(wallTexture);
		wallSprite.setPosition((float)w.x * C::GRID_SIZE, (float)w.y * C::GRID_SIZE);
		wallSprite.setScale(2,2);
		
		wallSprites.push_back(wallSprite);
	}
}

void Game::processInput(sf::Event ev) {
	if (ev.type == sf::Event::Closed) {
		win->close();
		closing = true;
		return;
	}
	if (ev.type == sf::Event::KeyReleased) {
		
	
	}
}


static double g_time = 0.0;
static double g_tickTimer = 0.0;


void Game::pollInput(double dt) {

	float lateralSpeed = 8.0;
	float maxSpeed = 40.0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
		gameManager->hero->MoveOnX(false);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		gameManager->hero->MoveOnX(true);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
		gameManager->hero->Jump();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)) {

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
		if (!wasPressed) {
			onSpacePressed();
			wasPressed = true;
		}
	}
	else {
		wasPressed = false;
	}

}

static sf::VertexArray va;
static RenderStates vaRs;
static std::vector<sf::RectangleShape> rects;

int blendModeIndex(sf::BlendMode bm) {
	if (bm == sf::BlendAlpha) return 0;
	if (bm == sf::BlendAdd) return 1;
	if (bm == sf::BlendNone) return 2;
	if (bm == sf::BlendMultiply) return 3;
	return 4;
};

void Game::update(double dt) {
	pollInput(dt);

	g_time += dt;
	if (bgShader) bgShader->update(dt);

	beforeParts.update(dt);
	afterParts.update(dt);

	gameManager->Update(dt);
}

 void Game::draw(sf::RenderWindow & win) {
	if (closing) return;

	sf::RenderStates states = sf::RenderStates::Default;
	sf::Shader * sh = &bgShader->sh;
	states.blendMode = sf::BlendAdd;
	states.shader = sh;
	states.texture = &tex;
	sh->setUniform("texture", tex);
	//sh->setUniform("time", g_time);
	win.draw(bg, states);

	beforeParts.draw(win);

	for (sf::Sprite r : wallSprites)
		win.draw(r);

	for (sf::RectangleShape& r : rects) 
		win.draw(r);
	

	afterParts.draw(win);

	gameManager->Draw();
}

void Game::AddWall(Vector2i wallPos)
{
	walls.push_back(wallPos);
}

void Game::RemoveWall(Vector2i wallPos)
{
	walls.erase(std::remove(walls.begin(), walls.end(), wallPos), walls.end());
}


void Game::onSpacePressed() {
	
}


bool Game::isWall(int cx, int cy)
{
	for (Vector2i & w : walls) {
		if (w.x == cx && w.y == cy)
			return true;
	}
	return false;
}

void Game::im()
{

}