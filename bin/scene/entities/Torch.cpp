//
// Created by arnito on 8/10/17.
//
#include <fstream>
#include "../../Resources.h"
#include "Torch.h"
Torch::Torch(): Entity("torch"),
                _light(sf::Vector2f(0,0),95.0,105.0,100.0, sf::Color::Green){
    _positionCol = _position;
    _sizeCol = sf::Vector2i(64,64);
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
        sf::Sprite s;
        s.setTexture(*t);
        s.setPosition(getPosition().x, getPosition().y);

        s.setTextureRect(sf::IntRect(64, 0, 64, 64));

        renderTar.draw(s);
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

}