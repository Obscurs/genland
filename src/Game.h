#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"


#include "scene/Drawer.h"
#include "scene/Player.h"
#include "scene/world/Map.h"
#include "menu_interface/MenuMain.h"
#include "menu_interface/NewGameMenu.h"
#include "menu_interface/MenuLoadGame.h"
#include "menu_interface/MenuConfigGame.h"
#include "scene/world/WorldBackground.h"
#include "scene/Clock.h"
#include "scene/Scene.h"
#include "MagicView.h"
#include <algorithm>
class Game
{
public:
    Game();
    ~Game();
	void Start();
	void LoadData();
	bool _true_exit;
private:
	bool IsExiting();
	void GameLoop();
	void ExitGame();
	void Events();
    void CreateNewGame(std::string path, std::string seed,std::string name);
    void DeleteGame(int index,std::string path);
	enum GameState { Uninitialized, NewGame, LoadGame,
					ShowingMenu, Playing, Exiting, Config, Help, About };

	GameState _gameState;
	sf::RenderWindow _window;
	WorldBackground _backgrounds;
    sf::Font _font;
	bool _resize;
	float _timerMusic;
    sf::Clock _clock;
    float _counter;

};
