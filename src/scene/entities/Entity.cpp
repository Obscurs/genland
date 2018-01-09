//
// Created by arnito on 9/09/17.
//

#include <iostream>
#include "Entity.h"
#include "../world/Ecosystem.h"

Entity::Entity(std::string type){
    _typeEntity = type;
    _position = sf::Vector2f(0,0);
    _positionCol = sf::Vector2f(0,0);
    _sizeCol = sf::Vector2i(0,0);
    _removed = false;
    _ecosystemIndex = -1;
}
void Entity::setEcosystemIndex(int ind) {
    _ecosystemIndex = ind;
}
int Entity::getEcosystemIndex() {
    return _ecosystemIndex;
}
void Entity::saveToFile(int chunk, std::ofstream &myfile){
    std::cout << "mala señal 1" << std::endl;
}
void Entity::loadFromFile(std::ifstream &myfile){
    std::cout << "mala señal 2" << std::endl;
}
void Entity::draw(sf::RenderTarget & renderTar){
    std::cout << "mala señal 3" << std::endl;
}
bool Entity::pointHitsEntity(sf::FloatRect pos){
    sf::FloatRect boundingBox(_positionCol.x, _positionCol.y,_sizeCol.x,_sizeCol.y);
    return boundingBox.intersects(pos);
}
void Entity::setPosition(int x, int y)
{
    _position.x=x;
    _position.y=y;
}
void Entity::setPositionCol(int x, int y)
{
    _positionCol.x=x;
    _positionCol.y=y;
}
sf::Vector2f Entity::getPosition(){
    return _position;
}

sf::Vector2f Entity::getPositionCol(){
    return _positionCol;
}
sf::FloatRect Entity::getBoundingBox(){
    return sf::FloatRect(_positionCol.x,_positionCol.y,_positionCol.x+_sizeCol.x+10,_positionCol.y+_sizeCol.y+10);
}