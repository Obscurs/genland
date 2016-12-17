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
#include <algorithm>
class Game
{
public:
    Game();
    ~Game();
	void Start();
	void LoadData();
	bool true_exit;
	//static sf::RenderWindow& GetWindow();
	//const static sf::Input& GetInput();



private:
	bool IsExiting();
	void GameLoop(double delta);
	void ExitGame();

    void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory);
    void CreateNewGame(std::string path, std::string seed,std::string name);
    void DeleteGame(int index,std::string path);
	enum GameState { Uninitialized, NewGame, LoadGame, Paused,
					ShowingMenu, Playing, Exiting, Config };

	GameState _gameState;
	sf::RenderWindow window;
	Inputs inputs;
    RunningGame game;


};
