//
// Created by ant on 06/11/16.
//

#include "MenuLoadGame.h"
#include "../Game.h"
#include "../Inputs.h"

void MenuLoadGame::Draw(sf::RenderWindow &window, sf::Font font)
{
    const sf::View* oldView = &window.getView();
    window.setView(view);
    del.Draw(window, font);
    load.Draw(window, font);
    back.Draw(window, font);
    save_list.Draw(window, font);
    window.setView(*oldView);
}
void MenuLoadGame::Update()
{
    sf::Vector2f mousePos = view.getMouseCoord();
    load.update(mousePos);
    back.update(mousePos);
    del.update(mousePos);
    save_list.update(mousePos);


}


bool MenuLoadGame::delClicked(){
    return (Inputs::MouseBreak(Inputs::M_LEFT) && del.mouseOver);
}
bool MenuLoadGame::loadClicked(){
    return (Inputs::MouseBreak(Inputs::M_LEFT) && load.mouseOver);
}
bool MenuLoadGame::backClicked(){
    return (Inputs::MouseBreak(Inputs::M_LEFT) && back.mouseOver);
}
Button MenuLoadGame::back(sf::Vector2f(10,1800),
                          sf::Vector2f(300,90),
                          "BACK",
                          0);
Button MenuLoadGame::load(sf::Vector2f(1700,1800),
                         sf::Vector2f(300,90),
                         "LOAD",
                         0);
Button MenuLoadGame::del(sf::Vector2f(850,1800),
                          sf::Vector2f(300,90),
                          "DELETE",
                          0);
InterfaceList MenuLoadGame::save_list(sf::Vector2f(300,500),
                         sf::Vector2f(1500,90),
                         0,
                         10);

MagicView MenuLoadGame::view;