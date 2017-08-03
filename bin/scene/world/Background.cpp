//
// Created by arnito on 8/09/16.
//

#include "Background.h"
#include <iostream>
#include <cmath>
Background::Background(std::string file, float dist, sf::Vector2f s){
    _distance = dist;
    _image.loadFromFile(file);
    _position.x=0;
    _position.y=0;
    _size.x =s.x;
    _size.y =s.y;
}
Background::~Background()
{
}

void Background::Draw(sf::RenderTexture &texture1)
{

    sf::Sprite s;
    s.setPosition(_position);
    s.setTexture(_image);

    sf::Vector2f new_scale(_size.x/s.getTexture()->getSize().x, _size.y/s.getTexture()->getSize().y);
    s.setScale(new_scale);

    sf::View currentView = texture1.getView();
    sf::Vector2f centerView = currentView.getCenter();
    sf::Vector2f sizeView = currentView.getSize();
    float x_view1 = centerView.x-sizeView.x/2;
    float x_view2 = centerView.x+sizeView.x/2;
    float dist = x_view1-_position.x;
    float first_x = (std::floor(dist/_size.x))*_size.x+_position.x;
    for(int i = first_x; i<x_view2; i= i+_size.x){
        s.setPosition(sf::Vector2f(i, _position.y-600*std::pow(_distance,-1)+150));
        texture1.draw(s);
    }

}

void Background::updatePosition(sf::Vector2f playerPos){
    _position.x = playerPos.x/_distance;
    _position.y = playerPos.y/_distance;

}
