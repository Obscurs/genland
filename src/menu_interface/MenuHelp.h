//
// Created by arnito on 8/01/18.
//

#ifndef GENLAND_MENUHELP_H
#define GENLAND_MENUHELP_H

#include "Button.h"
#include "../MagicView.h"
class MenuHelp {
public:
    static void Draw(sf::RenderWindow &window, sf::Font font);
    static void Update();
    static void nextImage();
    static bool backClicked();
    static bool nextClicked();
    static Button back;
    static Button next;
    static MagicView view;
    static int currentHelp;
};


#endif //GENLAND_MENUHELP_H
