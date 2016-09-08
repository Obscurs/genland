//
// Created by arnito on 8/09/16.
//
#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#ifndef GENLAND_BACKGROUND_H
#define GENLAND_BACKGROUND_H


class Background {
public:
    Background(std::string file, float distance, sf::Vector2f size);
    ~Background();
    sf::Texture _image;
    void Draw(sf::RenderWindow & renderWindow);
    void updatePosition(sf::Vector2f playerPos);
private:
    std::string file_path;
    float distance;
    sf::Vector2f position;
    sf::Vector2f size;
};


#endif //GENLAND_BACKGROUND_H
