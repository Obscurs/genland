//
// Created by ant on 06/11/16.
//

#ifndef GENLAND_MENUCONFIG_H
#define GENLAND_MENUCONFIG_H

#include "Button.h"
#include "../MagicView.h"
#include "InterfaceList.h"
class MenuConfigGame {
public:
    static void Draw(sf::RenderWindow &window, sf::Font font);
    static void Update();
    static bool backClicked();
    static bool resClicked();
    static MagicView view;
    static Button back;
    static Button res;
    static InterfaceList resolution_list;
    static std::vector<int> res_keys;
    static bool resolution_visible;
};


#endif //GENLAND_MENUCONFIG_H
