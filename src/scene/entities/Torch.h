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
    const static int SPRITE_MAX_TIME = 1;
    const static int SPRITE_SIZE = 16;
    Torch(int type);
    Light *getLight();
    void update(float delta);
    void setPosition(int x, int y);
    void draw(sf::RenderTarget & renderTar);
    void saveToFile(int chunk, std::ofstream &myfile);
    void loadFromFile(std::ifstream &myfile);
    int _typeTorx;
private:
    sf::Sprite _sprite;
    Light _light;
    sf::Vector2f size;
    float _spriteTime;
    int _animationFrame;
    int _numFramesAnimation;

    int _animationId;
};


#endif //GENLAND_TORCH_H
