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
#include "WorldBackground.h"
#include "Clock.h"
#include "MagicView.h"
class Drawer {
public:
    Drawer(Map *m,Player *p,WorldBackground *b, Clock *c);
    ~Drawer();

    void Draw(sf::RenderWindow &window);

private:
    Map *map_curr;
    Player *player;
    Clock *clock;
    WorldBackground *backgrounds;
    //sf::RenderWindow window; //TO DO

    TextureManager* texMan;

    sf::Shader tile_shader;
    sf::Shader sun_shader;
    sf::Shader sun_mix_shader;
    sf::Shader mix_back_terr_shader;
    sf::Shader sun_background_shader;

    sf::RenderTexture texture_plain_sprite;
    sf::RenderTexture *texture_front;
    sf::RenderTexture *texture_back;
    sf::RenderTexture black_texture;
    sf::RenderTexture texture_background;

    sf::View view_player;

    sf::Sprite get_plain_sprite(sf::RenderWindow& renderWindow,sf::VertexArray &render_array,sf::VertexArray &sky_array);
    void DrawLights(sf::VertexArray &render_array,sf::VertexArray &sky_array, sf::Sprite map_without_lights);
    void DrawMap(sf::RenderWindow& renderWindow);
    void DrawFrontItemsMap(sf::RenderWindow& renderWindow,sf::VertexArray &render_array);
};


#endif //GENLAND_DRAWER_H
