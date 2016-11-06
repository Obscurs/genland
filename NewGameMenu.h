//
// Created by ant on 06/11/16.
//

#ifndef GENLAND_NEWGAMEMENU_H
#define GENLAND_NEWGAMEMENU_H

#include "Button.h"
#include "Inputs.h"
class NewGameMenu {
public:
    static void Draw(sf::RenderWindow &window, sf::Font font);
    static void Update(sf::RenderWindow &window);
    static bool startClicked(Inputs &inputs,sf::RenderWindow &window);
    static bool backClicked(Inputs &inputs,sf::RenderWindow &window);
    static Button start;
    static Button back;
};


#endif //GENLAND_NEWGAMEMENU_H
