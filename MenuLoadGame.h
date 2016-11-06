//
// Created by ant on 06/11/16.
//

#ifndef GENLAND_MENULOADGAME_H
#define GENLAND_MENULOADGAME_H

#include "Button.h"
#include "Inputs.h"
class MenuLoadGame {
public:
    static void Draw(sf::RenderWindow &window, sf::Font font);
    static void Update(sf::RenderWindow &window);
    static bool loadClicked(Inputs &inputs,sf::RenderWindow &window);
    static bool backClicked(Inputs &inputs,sf::RenderWindow &window);
    static bool delClicked(Inputs &inputs,sf::RenderWindow &window);
    static Button load;
    static Button back;
    static Button del;
};


#endif //GENLAND_MENULOADGAME_H
