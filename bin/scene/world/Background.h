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
    Background(float distance, sf::Vector2f size, float speed);
    Background(std::string file, float distance, sf::Vector2f size, float speed);
    ~Background();

    void Draw(sf::RenderTarget &texture1);
    void updatePosition(sf::Vector2f playerPos);
    sf::RenderTexture* getTexture();
private:
    sf::Texture _image;
    sf::RenderTexture _renderTexture;
    float _distance; //factor de parallax
    sf::Vector2f _position;
    sf::Vector2f _size;
    float _speed;
};


#endif //GENLAND_BACKGROUND_H
