//
// Created by arnito on 11/12/16.
//

#include "TextInput.h"
#include <iostream>
TextInput::TextInput(sf::Vector2f pos, sf::Vector2f s, int tp, std::string text_default, std::string lab)
{
    position=pos;
    size = s;
    text = text_default;
    type = tp;
    mouseOver =false;
    label = lab;
    selected = false;
}

TextInput::~TextInput()
{
}
std::string TextInput::getText(){
    return text;
}
void TextInput::setText(std::string new_text){
    text = new_text;
}

void TextInput::Draw(sf::RenderWindow &window, sf::Font font){

    if(type==0){
        std::string final_text = label;
        final_text.append(": ");
        final_text.append(text);
        sf::Text sftext;
        sftext.setCharacterSize(size.y/2);
        sftext.setColor(sf::Color::Black);
        sftext.setPosition(sf::Vector2f(position.x, position.y));
        sftext.setString(final_text);
        sftext.setFont(font); // font is a sf::Font


        sf::RectangleShape rectangle(sf::Vector2f(0,0));
        rectangle.setPosition(sf::Vector2f(position.x, position.y));
        rectangle.setSize(sf::Vector2f(size.x, size.y));
        rectangle.setOutlineColor(sf::Color::Black);
        rectangle.setOutlineThickness(4.0);
        if(selected)rectangle.setFillColor(sf::Color(200, 200, 200));
        else rectangle.setFillColor(sf::Color(100, 100, 100));
        window.draw(rectangle);
        window.draw(sftext);
    }

}
void TextInput::update(sf::Vector2f mousePos){
    sf::FloatRect buttonRect(
            position.x,
            position.y,
            size.x,
            size.y);
    mouseOver = buttonRect.contains(mousePos.x, mousePos.y);
}
