//
// Created by arnito on 8/10/17.
//

#ifndef GENLAND_STAIRS_H
#define GENLAND_STAIRS_H


#include "Entity.h"

class Stairs : public Entity{
public:
    Stairs();
    void draw(sf::RenderTarget & renderTar);
    void saveToFile(int chunk, std::ofstream &myfile);
    void loadFromFile(std::ifstream &myfile);
private:
    sf::Vector2f size;
};


#endif //GENLAND_STAIRS_H
