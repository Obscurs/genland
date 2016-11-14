#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include "Drawer.h"
#include "Player.h"
#include "Map.h"
#include "MenuMain.h"
#include "NewGameMenu.h"
#include "MenuLoadGame.h"
#include "MenuConfigGame.h"
#include "WorldBackground.h"
#include "Clock.h"
#include "RunningGame.h"
#include "MagicView.h"
class Game
{
public:
	static void Start();
	//static sf::RenderWindow& GetWindow();
	//const static sf::Input& GetInput();
	const static int SCREEN_WIDTH = 1000;
	const static int SCREEN_HEIGHT = 700;

private:
	static bool IsExiting();
	static void GameLoop(double delta);
	static void ExitGame();
    static void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory);
    static void CreateNewGame(std::string path);
    static void DeleteGame(int index,std::string path);
	enum GameState { Uninitialized, NewGame, LoadGame, Paused,
					ShowingMenu, Playing, Exiting };

	static GameState _gameState;
	static sf::RenderWindow window;
	static Inputs inputs;
    static RunningGame game;


};
