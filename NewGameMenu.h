//
// Created by ant on 06/11/16.
//

#ifndef GENLAND_NEWGAMEMENU_H
#define GENLAND_NEWGAMEMENU_H

#include "Button.h"
#include "TextInput.h"
#include "Inputs.h"
#include "MagicView.h"
class NewGameMenu {
public:
    static void Draw(sf::RenderWindow &window, sf::Font font);
    static void Update();
    static bool startClicked(Inputs &inputs);
    static bool backClicked(Inputs &inputs);
    static bool seedClicked(Inputs &inputs);
    static bool nameClicked(Inputs &inputs);
    static void disSelectAll();
    static Button start;
    static Button back;
    static MagicView view;
    static TextInput seed;
    static TextInput name;
};



#endif //GENLAND_NEWGAMEMENU_H
