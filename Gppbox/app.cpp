#include <iostream> 
#include <array>
#include <numeric>
#include <algorithm>
#include <functional>

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Audio.hpp>

#include "Bloom.hpp"
#include "Dice.hpp"
#include "Lib.hpp"
#include "Game.hpp"
#include "GameManager.hpp"
#include "Interp.hpp"
#include "HotReloadShader.hpp"
#include "app.hpp"


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "C.hpp"

extern "C" {
	// Force the use of the NVidia-graphics card on notebooks with both an IGP and a GPU
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	// AMD equivalent to the above
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

static HotReloadShader * bloomShader = nullptr;
static HotReloadShader * blurShader = nullptr;

static std::array<double, 60> dts;
static int curDts = 0;

int main()
{
    cout << "Hello World!\n";
	
    //sf::RenderWindow window(sf::VideoMode(1920, 1080,32), "SFML works!");
    //sf::RenderWindow window(sf::VideoMode(800, 600,32), "SFML works!");
    sf::RenderWindow window(sf::VideoMode(1280, 720,32), "Corentin Project", Style::Titlebar | Style::Close);
	window.setVerticalSyncEnabled(false);
    Font font;

    if (!font.loadFromFile("res/MAIAN.TTF")) {
        cout << "ERROR NO FONT" << endl;
        return 1;
    }

	if (!sf::Shader::isAvailable())
	{
		cout << "ERROR NO SHADER SYSTEM" << endl;
		return 1;
	}

	ImGui::SFML::Init(window);

    Game g(&window);
	GameManager& gameManager = *g.gameManager;

	Vector2i winPos;

	View v = window.getDefaultView();
	Vector2f viewCenter = v.getCenter();

	sf::Clock timer;

	sf::Text fpsCounter;
	fpsCounter.setFont(font);
	fpsCounter.setString("FPS:");

	double frameStart = 0.0;
	double frameEnd = 0.0;

	sf::Texture winTex;
	winTex.create(window.getSize().x, window.getSize().y);

	bloomShader = new HotReloadShader("res/simple.vert", "res/bloom.frag");
	blurShader = new HotReloadShader("res/simple.vert", "res/blur.frag");
	sf::RenderTexture* destX = new sf::RenderTexture();
	destX->create(window.getSize().x, window.getSize().y);
	destX->clear(sf::Color(0, 0, 0, 0));

	sf::RenderTexture* destFinal = new sf::RenderTexture();
	destFinal->create(window.getSize().x, window.getSize().y);
	destFinal->clear(sf::Color(0, 0, 0, 0));	

	float bloomWidth = 12;
	sf::Glsl::Vec4 bloomMul(1,1,1,0.8f);

    while (window.isOpen())
    {
		double dt = frameEnd - frameStart;
		frameStart = Lib::getTimeStamp();

		if (dt < 0.00000001) {
			dt = 0.00000001;
		}

        sf::Event event;
		while (window.pollEvent(event))//sort un evenement de la liste pour le traiter
		{
			ImGui::SFML::ProcessEvent(event);
			g.processInput(event);

			if (event.type == sf::Event::Resized) {
				auto nsz = window.getSize();
				winTex.create(window.getSize().x, window.getSize().y);

				destX->create(window.getSize().x, window.getSize().y);
				destX->clear(sf::Color(0, 0, 0, 0));

				destFinal->create(window.getSize().x, window.getSize().y);
				destFinal->clear(sf::Color(0, 0, 0, 0));

				v = sf::View(Vector2f(nsz.x * 0.5f, nsz.y * 0.5f), Vector2f((float)nsz.x, (float)nsz.y));
				viewCenter = v.getCenter();
			}
		}

		//don't use imgui before this;
		ImGui::SFML::Update(window, sf::seconds((float)dt));

        g.update(dt);
		
		if (ImGui::CollapsingHeader("View")) {
			auto sz = v.getSize();
			ImGui::Value("size x", sz.x);
			ImGui::Value("size y", sz.y);
		}
		if (ImGui::CollapsingHeader("App Stats")) {
			//double df = (Lib::getTimeStamp() - frameStart);

			double mdt = std::accumulate(dts.begin(),dts.end(),0.0) / dts.size();
			ImGui::LabelText("Update Time", "%0.6f", dt);
			ImGui::LabelText("FPS", "%0.6f", 1.0 / dt);

			static double captureMdt = 0.0;
			if (curDts == 0) {
				captureMdt = mdt;
			}
			ImGui::LabelText("Avg Update Time", "%0.6f", captureMdt);
			ImGui::LabelText("Avg FPS", "%0.6f", 1.0 / captureMdt);
		}
        window.clear();

		window.setView(v);//keep view up to date in case we want to do something with like... you know what.

		if (ImGui::CollapsingHeader("Bloom Control")) {
			ImGui::SliderFloat("bloomWidth", &bloomWidth, 0, 55);//55 is max acceptable kernel size for constants, otherwise we should use a texture
			ImGui::ColorEdit4("bloomMul", &bloomMul.x);
		}

    	if (ImGui::CollapsingHeader("Movement Control"))
    	{
    		ImGui::DragInt("Gravity", &gameManager.Gravity, 0);
    		ImGui::DragInt("X Speed", &Entity::XSpeed, 0);
    		ImGui::DragInt("X Air Speed", &Entity::XAirSpeed, 0);
    		ImGui::DragInt("Max Speed", &Entity::MaxSpeed, .5f, 5);
    		ImGui::DragInt("Jump Force", &Entity::JumpForce, 0, 5);
    		ImGui::DragFloat("Ground Friction", &Entity::GroundFriction, 0, 1);
    		ImGui::DragFloat("Air Friction", &Entity::AirFriction, 0, 1);
    	}

    	// Level editor used to iterate fast. User can add wall and enemies to the level
    	if (ImGui::CollapsingHeader("Level Editor", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
    	{
    		ImGui::Checkbox("Edit Mode", &gameManager.EditMode);
    		if (gameManager.EditMode)
    		{
    			ImGui::Text("Left Click to Add");
    			ImGui::Text("Right Click to Remove");
    			
    			ImGui::Combo("Spawning", &currentSpawnableIndex, "Wall\0Enemy\0\0");

    			// Maybe add a way to save enemies positioning
    			if (ImGui::Button("Save Level"))
    				gameManager.SaveLevel();
    			if (ImGui::Button("Load Level"))
    				gameManager.LoadLevel();
			    if (gameManager.EditMode)
				    HandleLevelPainting(window, g);
    		}
    	}

    	ImGui::Text("QD for movement"
				 "\nSpace to Jump"
				 "\nEnter to shoot bullets"
				 "\nP to use Laser");
		g.im();

        g.draw(window);

		window.draw(fpsCounter);

		if (blurShader) blurShader->update(dt);
		if (bloomShader) bloomShader->update(dt);

		if (bloomWidth)
			Bloom::render(window,winTex,destX,destFinal,&blurShader->sh,&bloomShader->sh, bloomWidth, bloomMul);

		ImGui::SFML::Render(window);
        window.display();
		

		frameEnd = Lib::getTimeStamp();
		
		fpsCounter.setString("FPS: "+std::to_string(1.0 / dt));
		
		ImGui::EndFrame();
		
		curDts++;
		if (curDts >= dts.size()) {
			curDts = 0;
		}
		dts[curDts] = dt;
    }
	ImGui::SFML::Shutdown();

    return 0;
}

void HandleLevelPainting(const Window& window, Game& g)
{
	// If input is for ImGui don't use it to create level edit
	if (ImGui::GetIO().WantCaptureMouse)
		return;
	
	Vector2i mousePosition = Mouse::getPosition(window);
	if (mousePosition.x >= 0 && mousePosition.y >= 0 && mousePosition.x < window.getSize().x && mousePosition.y < window.getSize().y)
	{
		const Vector2i inGameMousePos = mousePosition/C::GRID_SIZE;
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if(currentSpawnableIndex == 0)
			{
				if(!g.isWall(inGameMousePos.x, inGameMousePos.y))
				{
					g.AddWall(inGameMousePos);
					g.cacheWalls();
				}
			}
			else
			{
				// TODO rajouter une limite pour pas spawn trop d'ennemies
				if(!g.isEnemy(inGameMousePos.x, inGameMousePos.y))
					g.AddEnemy(inGameMousePos);
			}
		}
		else if (Mouse::isButtonPressed(Mouse::Right))
		{
			if(currentSpawnableIndex == 0)
			{
				g.RemoveWall(inGameMousePos);
				g.cacheWalls();
			}
			else
			{
				g.RemoveEnemy(inGameMousePos);
			}
		}
	}
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
