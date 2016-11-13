//
// Created by ant on 06/11/16.
//

#ifndef GENLAND_NEWGAMEMENU_H
#define GENLAND_NEWGAMEMENU_H

#include "Button.h"
#include "Inputs.h"
#include "MagicView.h"
class NewGameMenu {
public:
    static void Draw(sf::RenderWindow &window, sf::Font font);
    static void Update();
    static bool startClicked(Inputs &inputs);
    static bool backClicked(Inputs &inputs);
    static Button start;
    static Button back;
    static MagicView view;
};



#endif //GENLAND_NEWGAMEMENU_H
