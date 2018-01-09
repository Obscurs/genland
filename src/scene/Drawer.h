//
// Created by arnito on 30/10/16.
//

#ifndef GENLAND_DRAWER_H
#define GENLAND_DRAWER_H

#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "Player.h"
#include "world/Map.h"
#include "world/WorldBackground.h"
#include "Clock.h"
#include "../MagicView.h"
class Drawer {
public:
    Drawer(Map *m,Player *p,WorldBackground *b, Clock *c);
    ~Drawer();
    void addLight(Light *l);
    void Draw(sf::RenderWindow &window);
    void debugMap(const std::string keyDebug);
private:
    sf::Clock timer;

    sf::RenderTexture texture_scene;
    sf::RenderTexture texture_sun;
    sf::RenderTexture *texture_front;
    sf::RenderTexture *texture_back;
    sf::RenderTexture black_texture;
    sf::RenderTexture texture_background;
    std::vector<Light*> _lights;
    sf::View view_player;
    sf::Vector2f GetPosSprite();
    void DrawSceneTex();
    void DrawBackground();
    void DrawRain();
    void DrawLights();
    void DrawMap(sf::RenderWindow& renderWindow);
};


#endif //GENLAND_DRAWER_H
