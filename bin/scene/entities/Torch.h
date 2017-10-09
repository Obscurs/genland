//
// Created by arnito on 8/10/17.
//

#ifndef GENLAND_TORCH_H
#define GENLAND_TORCH_H


#include <SFML/Graphics/RenderTarget.hpp>
#include "Entity.h"
#include "Light.h"

class Torch : public Entity{
public:
    Torch();
    Light *getLight();
    void setPosition(int x, int y);
    void draw(sf::RenderTarget & renderTar);
    void saveToFile(int chunk, std::ofstream &myfile);
    void loadFromFile(std::ifstream &myfile);
private:
    Light _light;
    sf::Vector2f size;
};


#endif //GENLAND_TORCH_H
