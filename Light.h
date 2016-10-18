//
// Created by ant on 17/10/16.
//

#ifndef GENLAND_LIGHT_H
#define GENLAND_LIGHT_H

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

class Light {
public:
    Light(sf::Vector2f pos, float rad_min, float rad_max, float exp, sf::Color c);
    ~Light();
    sf::Vector2f position;
    float radius_min;
    float radius_max;
    float expand;
    sf::Color color;
    float radius;
    void Update(float deltatime);

private:

};


#endif //GENLAND_LIGHT_H
