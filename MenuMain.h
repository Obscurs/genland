//
// Created by ant on 03/11/16.
//

#ifndef GENLAND_MENUMAIN_H
#define GENLAND_MENUMAIN_H

#include "Button.h"
#include "Inputs.h"
class MenuMain {
public:
    static void Draw(sf::RenderWindow &window, sf::Font font);
    static void Update(sf::RenderWindow &window);
    static bool newGameClicked(Inputs &inputs,sf::RenderWindow &window);
    static bool loadClicked(Inputs &inputs,sf::RenderWindow &window);
    static bool configClicked(Inputs &inputs,sf::RenderWindow &window);
    static bool exitClicked(Inputs &inputs,sf::RenderWindow &window);
    static Button new_game;
    static Button load;
    static Button config;
    static Button exit;
};


#endif //GENLAND_MENUMAIN_H
