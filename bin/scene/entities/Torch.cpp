//
// Created by arnito on 8/10/17.
//
#include <fstream>
#include "../../Resources.h"
#include "Torch.h"
Torch::Torch(): Entity("torch"),
                _light(sf::Vector2f(0,0),95.0,105.0,100.0, sf::Color::Green, false){
    _positionCol = _position;
    _sizeCol = sf::Vector2i(16,16);
    _numFramesAnimation = 3;
    _animationFrame = 0;
    _spriteTime = 0;
}
void Torch::setPosition(int x, int y)
{
    _position.x=x;
    _position.y=y;
    _light.position.x = x;
    _light.position.y = y;
}
Light *Torch::getLight(){
    return &_light;
}
void Torch::draw(sf::RenderTarget & renderTar) {
    if (!_removed) {
        sf::Texture *t = Resources::getTexture("entities");

        _sprite.setTexture(*t);
        _sprite.setPosition(getPosition().x, getPosition().y);


        renderTar.draw(_sprite);
    }
}

void Torch::saveToFile(int chunk, std::ofstream &myfile){
    if(!_removed){
        myfile << "torch" << " ";
        myfile << chunk << " ";
        myfile << _position.x << " " << _position.y << " ";
        myfile << _positionCol.x << " " << _positionCol.y << " ";
        myfile << _sizeCol.x << " " << _sizeCol.y << " ";
    }

}
void Torch::loadFromFile(std::ifstream &myfile){
    myfile >> _chunk;
    myfile >> _position.x;
    myfile >> _position.y;
    myfile >> _positionCol.x;
    myfile >> _positionCol.y;
    myfile >> _sizeCol.x;
    myfile >> _sizeCol.y;
    setPosition(_position.x, _position.y);

}
void Torch::update(float delta){
    _spriteTime += delta;
    float maxtime = float(SPRITE_MAX_TIME)/10.0f;
    if(_spriteTime > maxtime){
        _spriteTime -= maxtime;
        _animationFrame +=1;
        if(_animationFrame >=_numFramesAnimation){
            _animationFrame = 0;
        }
    }
    _sprite.setTextureRect(sf::IntRect(64*_animationFrame,64,64,64));
}