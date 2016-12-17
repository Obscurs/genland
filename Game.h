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
#include "ConfigStarter.h"
#include <algorithm>
class Game
{
public:
	static void Start();
	static void LoadData();
	//static sf::RenderWindow& GetWindow();
	//const static sf::Input& GetInput();



private:
	static bool IsExiting();
	static void GameLoop(double delta);
	static void ExitGame();

    static void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory);
    static void CreateNewGame(std::string path, std::string seed,std::string name);
    static void DeleteGame(int index,std::string path);
	enum GameState { Uninitialized, NewGame, LoadGame, Paused,
					ShowingMenu, Playing, Exiting, Config };

	static GameState _gameState;
	static sf::RenderWindow window;
	static Inputs inputs;
    static RunningGame game;
	//static ConfigStarter cfg;


};
