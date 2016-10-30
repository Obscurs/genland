#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include "Drawer.h"
#include "Player.h"
#include "Map.h"
#include "WorldBackground.h"
#include "Clock.h"

class Game
{
public:
	static void Start();
	//static sf::RenderWindow& GetWindow();
	//const static sf::Input& GetInput();
	const static int SCREEN_WIDTH = 1024;
	const static int SCREEN_HEIGHT = 768;

private:
	static bool IsExiting();
	static void GameLoop(double delta);
	static void ExitGame();

	enum GameState { Uninitialized, Starting, Paused, 
					ShowingMenu, Playing, Exiting };

	static GameState _gameState;
	static sf::RenderWindow window;
	static Map map_curr;
	static Player player;
	static Inputs inputs;
	static Drawer drawer;
	static WorldBackground backgrounds;
	static Clock clock;




};