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
}

Button::~Button()

{
}
bool Button::isMouseOver(sf::Vector2i mousePos) {
    sf::FloatRect buttonRect(
            position.x,
            position.y,
            size.x,
            size.y);
    return buttonRect.contains(mousePos.x, mousePos.y);
}
void Button::Draw(sf::RenderWindow &window, sf::Font font){
    sf::View currentView = window.getView();
    sf::Vector2f centerView = currentView.getCenter();
    sf::Vector2f sizeView = currentView.getSize();


    sf::Text sftext;
    sftext.setCharacterSize(size.y/2);
    sftext.setColor(sf::Color::Black);
    sftext.setPosition(sf::Vector2f(position.x+centerView.x-sizeView.x/2+10, position.y+centerView.y-sizeView.y/2+5));
    sftext.setString(text);
    sftext.setFont(font); // font is a sf::Font


    sf::RectangleShape rectangle(sf::Vector2f(0,0));
    rectangle.setPosition(sf::Vector2f(position.x+centerView.x-sizeView.x/2, position.y+centerView.y-sizeView.y/2));
    rectangle.setSize(sf::Vector2f(size.x, size.y));
    sf::Vector2i posMouse = sf::Mouse::getPosition(window);
    if(isMouseOver(posMouse))rectangle.setFillColor(sf::Color(200, 200, 200));
    else rectangle.setFillColor(sf::Color(100, 100, 100));
    window.draw(rectangle);
    window.draw(sftext);
}

