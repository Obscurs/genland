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
    if(!elements.empty()) {
        sf::RectangleShape rectangle(sf::Vector2f(0, 0));


        rectangle.setFillColor(sf::Color(255, 211, 124));
        rectangle.setOutlineThickness(GRID_THICKNESS);
        rectangle.setOutlineColor(sf::Color(147, 91, 0));
        rectangle.setSize(sf::Vector2f(size.x - GRID_THICKNESS, size.y - GRID_THICKNESS));

        for (int i = 0; i < visible_slots; i = ++i) {
            rectangle.setPosition(sf::Vector2f(position.x, i * size.y + position.y));
            window.draw(rectangle);
        }

        for (int i = current_first_slot;
             i < std::min(visible_slots + current_first_slot, ((int) elements.size())); ++i) {

            sf::Text sftext;
            sftext.setCharacterSize(size.y / 2);
            sftext.setColor(sf::Color::Black);
            sftext.setPosition(sf::Vector2f(position.x, (i - current_first_slot) * size.y + position.y));
            sftext.setString(elements[i]);
            sftext.setFont(font); // font is a sf::Font

            window.draw(sftext);

        }
        sf::RectangleShape selected(rectangle);
        selected.setPosition(sf::Vector2f(position.x, (selected_slot - current_first_slot) * size.y + position.y));
        selected.setFillColor(sf::Color::Transparent);
        selected.setOutlineThickness(GRID_SELECTED_THICKNESS);
        selected.setOutlineColor(sf::Color(210, 160, 70));
        window.draw(selected);
    }
}
void InterfaceList::update(sf::Vector2f mousePos){
    if(!elements.empty()) {
        sf::FloatRect buttonRect(
                position.x,
                position.y,
                size.x,
                size.y);
        mouseOver = buttonRect.contains(mousePos.x, mousePos.y);

        int tab_selection_delta = Inputs::GetWheel();
        int up_key = Inputs::KeyBreak(Inputs::UP);
        int down_key = Inputs::KeyBreak(Inputs::DOWN);
        if (tab_selection_delta != 0) {
            selected_slot = (selected_slot - tab_selection_delta);
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