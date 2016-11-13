//
// Created by ant on 06/11/16.
//

#ifndef GENLAND_RUNNINGGAME_H
#define GENLAND_RUNNINGGAME_H
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include "Drawer.h"
#include "Player.h"
#include "Map.h"
#include "MenuMain.h"
#include "NewGameMenu.h"
#include "WorldBackground.h"
#include "Clock.h"
#include "MagicView.h"

class RunningGame {
public:
    void restart(std::string path,sf::RenderWindow &window);
    RunningGame(std::string path,sf::RenderWindow &window);
    ~RunningGame();
    Map map_curr;
    Player player;
    Inputs inputs;
    Drawer drawer;
    WorldBackground backgrounds;
    Clock clock;
    std::string pathGame;
    void update(sf::RenderWindow &window, MagicView &game_view,float delta,Inputs &inputs);
    void saveGame();
    void draw(sf::RenderWindow &window,MagicView &mv);

};


#endif //GENLAND_RUNNINGGAME_H
