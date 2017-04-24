//
// Created by ant on 06/11/16.
//

#ifndef GENLAND_MENULOADGAME_H
#define GENLAND_MENULOADGAME_H

#include "Button.h"
#include "../Inputs.h"
#include "../MagicView.h"
#include "InterfaceList.h"
class MenuLoadGame {
public:
    static void Draw(sf::RenderWindow &window, sf::Font font);
    static void Update();
    static bool loadClicked();
    static bool backClicked();
    static bool delClicked();
    static MagicView view;
    static Button load;
    static Button back;
    static Button del;
    static InterfaceList save_list;
};


#endif //GENLAND_MENULOADGAME_H
