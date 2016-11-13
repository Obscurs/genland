//
// Created by ant on 03/11/16.
//

#include "Button.h"
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
    sftext.setPosition(sf::Vector2f(position.x, position.y));
    sftext.setString(text);
    sftext.setFont(font); // font is a sf::Font


    sf::RectangleShape rectangle(sf::Vector2f(0,0));
    rectangle.setPosition(sf::Vector2f(position.x, position.y));
    rectangle.setSize(sf::Vector2f(size.x, size.y));
    if(mouseOver)rectangle.setFillColor(sf::Color(200, 200, 200));
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
    mouseOver = buttonRect.contains(mousePos.x, mousePos.y);
}
