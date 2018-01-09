//
// Created by ant on 03/11/16.
//

#include "Button.h"
#include "../SoundManager.hpp"
#include <iostream>

Button::Button(sf::Vector2f pos, sf::Vector2f s, std::string txt, int t)
{
    position=pos;
    size = s;
    text = txt;
    type = t;
    mouseOver =false;
}

Button::~Button()

{
}

void Button::Draw(sf::RenderWindow &window, sf::Font font){


    sf::Text sftext;
    sftext.setCharacterSize(size.y/2);
    sftext.setColor(sf::Color::Black);

    sftext.setFont(font); // font is a sf::Font
    sftext.setString(text);
    sftext.setPosition(sf::Vector2f(position.x, position.y));
    sf::FloatRect textRect = sftext.getLocalBounds();
    sftext.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top  + textRect.height/2.0f);
    sftext.setPosition(sf::Vector2f(position.x+(size.x)/2.0f,position.y+(size.y)/2.0f));


    //




    sf::RectangleShape rectangle(sf::Vector2f(0,0));
    rectangle.setPosition(sf::Vector2f(position.x, position.y));
    rectangle.setSize(sf::Vector2f(size.x, size.y));
    rectangle.setOutlineColor(sf::Color::Black);
    rectangle.setOutlineThickness(4.0);
    if(mouseOver) rectangle.setFillColor(sf::Color(200, 200, 200));
    else rectangle.setFillColor(sf::Color(100, 100, 100));
    window.draw(rectangle);
    window.draw(sftext);
}
void Button::update(sf::Vector2f mousePos){
    sf::FloatRect buttonRect(
            position.x,
            position.y,
            size.x,
            size.y);
    bool lastOver = mouseOver;
    mouseOver = buttonRect.contains(mousePos.x, mousePos.y);
    if(!lastOver && mouseOver) SoundManager::playSound("click");
}
