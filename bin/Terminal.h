//
// Created by arnito on 6/10/17.
//

#ifndef GENLAND_TERMINAL_H
#define GENLAND_TERMINAL_H
#include <iostream>
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include "menu_interface/TextInput.h"

class Terminal {
public:
    Terminal();
    ~Terminal();
    void Update(float delta);
    void draw(sf::RenderWindow &_window, sf::Text &text);
    bool isActive();
    void setText(std::string s);
    void sendTerminalInstruction();
    void disable();
    std::string getText();
private:
    std::vector<std::string> _commands;
    bool _active;
    TextInput _instructionBox;
};


#endif //GENLAND_TERMINAL_H
