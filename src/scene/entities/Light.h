//
// Created by ant on 17/10/16.
//

#ifndef GENLAND_LIGHT_H
#define GENLAND_LIGHT_H

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "../../TextureManager.h"
class Light {
public:
    Light(sf::Vector2f pos, float rad_min, float rad_max, float exp, sf::Color c, bool isPlayer);
    ~Light();
    sf::Vector2f position;
    float radius_min;
    float radius_max;
    float expand;
    sf::Color color;
    float radius;
    bool player;
    void Update(float deltatime);
    void Draw(sf::Vector2f pos_light, sf::Sprite &map_without_lights, sf::RenderTexture *tex_front, sf::RenderTexture *tex_back,sf::RenderTexture &tex_sun);
};


#endif //GENLAND_LIGHT_H
