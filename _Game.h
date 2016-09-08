#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "Player.h"
#include "Map.h"

class Game
{
public:
	Game();
	~Game();
	void Start();
	//static sf::RenderWindow& GetWindow();
	//const static sf::Input& GetInput();
	const static int SCREEN_WIDTH = 1024;
	const static int SCREEN_HEIGHT = 768;

private:
	bool IsExiting();
	void GameLoop(double delta);
	void ExitGame();

	enum GameState { Uninitialized, Starting, Paused, 
					ShowingMenu, Playing, Exiting };

	GameState _gameState;
	sf::RenderWindow window;
	Map* map_curr;
	Player player;
	Inputs inputs;


};

