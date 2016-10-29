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
#include "Map.h"

class Drawer {
public:
    Drawer();
    ~Drawer();

    void Draw(Player &player, Map &map,sf::RenderWindow &window);

private:
    //sf::RenderWindow window; //TO DO

    TextureManager* texMan;

    sf::Shader tile_shader;
    sf::Shader sun_shader;
    sf::Shader sun_mix_shader;

    sf::RenderTexture texture_plain_sprite;
    sf::RenderTexture *texture_front;
    sf::RenderTexture *texture_back;
    sf::RenderTexture black_texture;
};


#endif //GENLAND_DRAWER_H
