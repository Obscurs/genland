//
// Created by arnito on 11/12/16.
//

#ifndef GENLAND_INTERFACELIST_H
#define GENLAND_INTERFACELIST_H
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include "../Inputs.h"
class InterfaceList {
public:
    const static int GRID_THICKNESS = 2;
    const static int GRID_SELECTED_THICKNESS = 4;
    InterfaceList(sf::Vector2f pos, sf::Vector2f s, int tp, int vis_slots, std::vector<std::string> &elems);
    InterfaceList(sf::Vector2f pos, sf::Vector2f s, int tp, int vis_slots);
    ~InterfaceList();
    sf::Vector2f position;
    sf::Vector2f size;
    int visible_slots;
    int selected_slot;
    int current_first_slot;
    int type;
    int mouseOver;
    void Draw(sf::RenderWindow &window,sf::Font font);
    void update(sf::Vector2f mousePos,Inputs &inputs);
    std::vector<std::string> elements;
    void insertElement(std::string elem);
};


#endif //GENLAND_INTERFACELIST_H
