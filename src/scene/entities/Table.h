//
// Created by arnito on 16/12/17.
//

#ifndef GENLAND_TABLE_HPP
#define GENLAND_TABLE_HPP


#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Entity.h"

class Table : public Entity{
public:
    const static int SPRITE_MAX_TIME = 1;
    const static int SPRITE_SIZE = 16;
    Table();
    void setPosition(int x, int y);
    void draw(sf::RenderTarget & renderTar);
    void saveToFile(int chunk, std::ofstream &myfile);
    void loadFromFile(std::ifstream &myfile);
private:
    sf::Sprite _sprite;

    sf::Vector2f size;
    int _animationFrame;
};


#endif //GENLAND_TABLE_HPP
