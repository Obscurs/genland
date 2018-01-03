//
// Created by arnito on 11/12/16.
//

#include "InterfaceList.h"
#include "../Inputs.h"
#include <iostream>
InterfaceList::InterfaceList(sf::Vector2f pos, sf::Vector2f s, int tp, int vis_slots, std::vector<std::string> &elems)
{
    position=pos;
    size = s;
    type = tp;
    visible_slots = vis_slots;
    selected_slot = 0;
    current_first_slot = 0;
    mouseOver =false;
    for(int i = 0; i< elems.size(); i++){
        elements.push_back(elems[i]);
    }
}
InterfaceList::InterfaceList(sf::Vector2f pos, sf::Vector2f s, int tp, int vis_slots)
{
    position=pos;
    size = s;
    type = tp;
    visible_slots = vis_slots;
    selected_slot = 0;
    current_first_slot = 0;
    mouseOver =false;
}

InterfaceList::~InterfaceList()

{
}
void InterfaceList::insertElement(std::string elem) {
    elements.push_back(elem);
}
void InterfaceList::Draw(sf::RenderWindow &window, sf::Font font){
    //if(!elements.empty()) {
        sf::RectangleShape rectangle(sf::Vector2f(0, 0));


        rectangle.setFillColor(sf::Color(255, 211, 124));
        rectangle.setOutlineThickness(GRID_THICKNESS);
        rectangle.setOutlineColor(sf::Color(147, 91, 0));
        rectangle.setSize(sf::Vector2f(size.x - GRID_THICKNESS, size.y - GRID_THICKNESS));

        for (int i = -1; i < visible_slots; i = ++i) {
            if(i == -1){
                rectangle.setFillColor(sf::Color(179, 145, 81));
            }
            else rectangle.setFillColor(sf::Color(255, 211, 124));

            rectangle.setPosition(sf::Vector2f(position.x, i * size.y + position.y));
            window.draw(rectangle);
            if(i != -1){
                rectangle.setFillColor(sf::Color(179, 145, 81));

                rectangle.setSize(sf::Vector2f(100, rectangle.getSize().y));
                window.draw(rectangle);
                rectangle.setSize(sf::Vector2f(size.x - GRID_THICKNESS, size.y - GRID_THICKNESS));

                sf::Text sftext;
                sftext.setCharacterSize(size.y / 2);
                sftext.setColor(sf::Color::Black);
                sftext.setPosition(sf::Vector2f(position.x+130, (i - current_first_slot) * size.y + position.y));
                sftext.setString(std::to_string(i+1));
                sftext.setFont(font); // font is a sf::Font


                sf::FloatRect textRect = sftext.getLocalBounds();
                sftext.setOrigin(textRect.left + textRect.width/2.0f,
                                 textRect.top  + textRect.height/2.0f);
                sftext.setPosition(sf::Vector2f(position.x+(100)/2.0f,(i - current_first_slot) * size.y + position.y+(size.y)/2.0f));

                window.draw(sftext);
            } else{
                sf::Text sftext;
                sftext.setCharacterSize(size.y / 2);
                sftext.setColor(sf::Color::Black);
                sftext.setPosition(sf::Vector2f(position.x+130, (i - current_first_slot) * size.y + position.y));
                sftext.setString("Saved games");
                sftext.setFont(font); // font is a sf::Font


                sf::FloatRect textRect = sftext.getLocalBounds();
                sftext.setOrigin(textRect.left + textRect.width/2.0f,
                                 textRect.top  + textRect.height/2.0f);
                sftext.setPosition(sf::Vector2f(position.x+(size.x)/2.0f,(i - current_first_slot) * size.y + position.y+(size.y)/2.0f));

                window.draw(sftext);
            }

        }

        for (int i = current_first_slot;
             i < std::min(visible_slots + current_first_slot, ((int) elements.size())); ++i) {

            sf::Text sftext;
            sftext.setCharacterSize(size.y / 2);
            sftext.setColor(sf::Color::Black);
            sftext.setPosition(sf::Vector2f(position.x+130, (i - current_first_slot) * size.y + position.y));
            sftext.setString(elements[i]);
            sftext.setFont(font); // font is a sf::Font


            sf::FloatRect textRect = sftext.getLocalBounds();
            sftext.setOrigin(textRect.left,
                             textRect.top  + textRect.height/2.0f);
            sftext.setPosition(sf::Vector2f(sftext.getPosition().x,(i - current_first_slot) * size.y + position.y+(size.y)/2.0f));

            window.draw(sftext);

        }
        sf::RectangleShape selected(rectangle);
        selected.setPosition(sf::Vector2f(position.x, (selected_slot - current_first_slot) * size.y + position.y));
        selected.setFillColor(sf::Color::Transparent);
        selected.setOutlineThickness(GRID_SELECTED_THICKNESS);
        selected.setOutlineColor(sf::Color(0, 0, 0));
        window.draw(selected);
    //}
}
void InterfaceList::update(sf::Vector2f mousePos){
    if(!elements.empty()) {
        sf::FloatRect buttonRect(
                position.x,
                position.y,
                size.x,
                size.y*elements.size());
        mouseOver = buttonRect.contains(mousePos.x, mousePos.y);

        mouseOver = mouseOver*((mousePos.y-size.y/2)/size.y);
        int tab_selection_delta = Inputs::GetWheel();
        int up_key = Inputs::KeyBreak(Inputs::UP);
        int down_key = Inputs::KeyBreak(Inputs::DOWN);
        if (tab_selection_delta != 0) {
            selected_slot = (selected_slot - tab_selection_delta);
        }
        else if(mouseOver && Inputs::MouseBreak(Inputs::M_LEFT)){
            selected_slot = mouseOver-=5;
        }
        else if (up_key == 1) selected_slot -= 1;
        else if (down_key == 1) selected_slot += 1;
        while (selected_slot < 0) {
            selected_slot += elements.size();
        }
        selected_slot %= elements.size();

        if (selected_slot == 0) current_first_slot = 0;
        else if (selected_slot >= current_first_slot + visible_slots) {
            current_first_slot += 1;
        }
        else if (selected_slot < current_first_slot) {
            current_first_slot -= 1;
        }
    }
}