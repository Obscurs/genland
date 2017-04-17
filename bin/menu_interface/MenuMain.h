//
// Created by ant on 03/11/16.
//

#ifndef GENLAND_MENUMAIN_H
#define GENLAND_MENUMAIN_H

#include "Button.h"
#include "../Inputs.h"
#include "../MagicView.h"

class MenuMain {
public:
    static void Draw(sf::RenderWindow &window, const sf::Font& font);
    static void Update();
    static bool newGameClicked(Inputs &inputs);
    static bool loadClicked(Inputs &inputs);
    static bool configClicked(Inputs &inputs);
    static bool exitClicked(Inputs &inputs);
    static Button new_game;
    static Button load;
    static Button config;
    static Button exit;
    static MagicView view;
};


#endif //GENLAND_MENUMAIN_H
