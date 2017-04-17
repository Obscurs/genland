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
#include "world/Map.h"
#include "../menu_interface/MenuMain.h"
#include "../menu_interface/NewGameMenu.h"
#include "world/WorldBackground.h"
#include "Clock.h"
#include "../MagicView.h"

class Scene {
public:
    void restart(std::string path,sf::RenderWindow &window, std::string seed);
    Scene(sf::RenderWindow &window);
    ~Scene();
    Map map_curr;
    Player player;
    Inputs inputs;
    Drawer drawer;
    WorldBackground backgrounds;
    Clock clock;
    std::string pathGame;
    MagicView view_game;
    void update(sf::RenderWindow &window,float delta,Inputs &inputs);
    void saveGame();
    void draw(sf::RenderWindow &window);
    float zoom;
};


#endif //GENLAND_RUNNINGGAME_H
