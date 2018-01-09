//
// Created by arnito on 8/10/17.
//

#include <fstream>
#include "Stairs.h"
#include "../../Resources.h"

Stairs::Stairs(): Entity("stairs"){
    _positionCol = _position;
    _sizeCol = sf::Vector2i(32,32);
    _numFramesAnimation = 1;
    _animationFrame = 0;
}
void Stairs::draw(sf::RenderTarget & renderTar) {
    if (!_removed) {
        sf::Texture *t = Resources::getTexture("entities");
        sf::Sprite s;
        s.setTexture(*t);
        s.setPosition(getPosition().x, getPosition().y);

        s.setTextureRect(sf::IntRect(0, 0, 64, 64));

        renderTar.draw(s);
    }
}

void Stairs::saveToFile(int chunk, std::ofstream &myfile){
    if(!_removed){
        myfile << "stairs" << " ";
        myfile << chunk << " ";
        myfile << _position.x << " " << _position.y << " ";
        myfile << _positionCol.x << " " << _positionCol.y << " ";
        myfile << _sizeCol.x << " " << _sizeCol.y << " ";
    }

}
void Stairs::loadFromFile(std::ifstream &myfile){
    myfile >> _chunk;
    myfile >> _position.x;
    myfile >> _position.y;
    myfile >> _positionCol.x;
    myfile >> _positionCol.y;
    myfile >> _sizeCol.x;
    myfile >> _sizeCol.y;

}