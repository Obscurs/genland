//
// Created by arnito on 16/12/17.
//
#include <fstream>
#include "../../Resources.h"
#include "Table.h"
Table::Table(): Entity("table"){
    _positionCol = _position;
    _sizeCol = sf::Vector2i(16,16);

    _animationFrame = rand()%2;
    _sprite.setTextureRect(sf::IntRect(64*_animationFrame,640,64,64));

}
void Table::setPosition(int x, int y)
{
    _position.x=x;
    _position.y=y;
}
void Table::draw(sf::RenderTarget & renderTar) {
    if (!_removed) {
        sf::Texture *t = Resources::getTexture("entities");

        _sprite.setTexture(*t);
        _sprite.setPosition(getPosition().x, getPosition().y);


        renderTar.draw(_sprite);
    }
}

void Table::saveToFile(int chunk, std::ofstream &myfile){
    if(!_removed){
        myfile << "picture" << " ";
        myfile << chunk << " ";
        myfile << _position.x << " " << _position.y << " ";
        myfile << _positionCol.x << " " << _positionCol.y << " ";
        myfile << _sizeCol.x << " " << _sizeCol.y << " ";
    }

}
void Table::loadFromFile(std::ifstream &myfile){
    myfile >> _chunk;
    myfile >> _position.x;
    myfile >> _position.y;
    myfile >> _positionCol.x;
    myfile >> _positionCol.y;
    myfile >> _sizeCol.x;
    myfile >> _sizeCol.y;
    setPosition(_position.x, _position.y);

}