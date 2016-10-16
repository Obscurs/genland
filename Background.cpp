//
// Created by arnito on 8/09/16.
//

#include "Background.h"
#include <iostream>
#include <cmath>
Background::Background(std::string file, float dist, sf::Vector2f s){
    file_path=file;
    distance = dist;
    _image.loadFromFile(file_path);
    position.x=0;
    position.y=0;
    size.x =s.x;
    size.y =s.y;
}
Background::~Background()
{
}

void Background::Draw(sf::RenderTexture &texture1)
{

    sf::Sprite s;
    s.setPosition(position);
    s.setTexture(_image);

    sf::Vector2f new_scale(size.x/s.getTexture()->getSize().x, size.y/s.getTexture()->getSize().y);
    s.setScale(new_scale);

    sf::View currentView = texture1.getView();
    sf::Vector2f centerView = currentView.getCenter();
    sf::Vector2f sizeView = currentView.getSize();
    float x_view1 = centerView.x-sizeView.x/2;
    float x_view2 = centerView.x+sizeView.x/2;
    float dist = x_view1-position.x;
    float first_x = (std::floor(dist/size.x))*size.x+position.x;
    for(int i = first_x; i<x_view2; i= i+size.x){
        s.setPosition(sf::Vector2f(i, position.y-600*std::pow(distance,-1)));
        texture1.draw(s);
    }
    //std::cout << distance << " " << x_view1 << " "<< first_x << " " << size.x << " "<<  ceilf(distance/size.x)<< std::endl;




}

void Background::updatePosition(sf::Vector2f playerPos){
    position.x = playerPos.x/distance;
    position.y = playerPos.y/distance;
}
