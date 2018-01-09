//
// Created by arnito on 8/01/18.
//

#ifndef GENLAND_MENUABOUT_H
#define GENLAND_MENUABOUT_H

#include "Button.h"
#include "../MagicView.h"
class MenuAbout {
public:
    static void Draw(sf::RenderWindow &window, sf::Font font);
    static void Update();
    static bool backClicked();
    static Button back;
    static MagicView view;
};



#endif //GENLAND_MENUABOUT_H
