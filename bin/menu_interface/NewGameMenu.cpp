//
// Created by ant on 03/11/16.
//

#include "NewGameMenu.h"
#include "../Game.h"

void NewGameMenu::Draw(sf::RenderWindow &window, sf::Font font)
{
    const sf::View* oldView = &window.getView();
    window.setView(view);
    start.Draw(window, font);
    back.Draw(window, font);
    seed.Draw(window, font);
    name.Draw(window, font);
    window.setView(*oldView);
}
void NewGameMenu::Update()
{
    sf::Vector2f mousePos = view.getMouseCoord();
    start.update(mousePos);
    back.update(mousePos);
    seed.update(mousePos);
    name.update(mousePos);
}

bool NewGameMenu::startClicked(){
    return (Inputs::MouseBreak(Inputs::M_LEFT) && start.mouseOver);
}
bool NewGameMenu::backClicked(){
    return (Inputs::MouseBreak(Inputs::M_LEFT) && back.mouseOver);
}
bool NewGameMenu::seedClicked(){
    return (Inputs::MouseBreak(Inputs::M_LEFT) && seed.mouseOver);
}
bool NewGameMenu::nameClicked(){
    return (Inputs::MouseBreak(Inputs::M_LEFT) && name.mouseOver);
}
void NewGameMenu::disSelectAll(){
    seed.selected = false;
    name.selected = false;
}
Button NewGameMenu::start(sf::Vector2f(10,1900),
                          sf::Vector2f(300,90),
                          "START",
                          0);
Button NewGameMenu::back(sf::Vector2f(1700,1900),
                         sf::Vector2f(300,90),
                          "BACK",
                          0);
TextInput NewGameMenu::seed(sf::Vector2f(850,500),
                         sf::Vector2f(300,90),
                         0,
                         "", "seed");
TextInput NewGameMenu::name(sf::Vector2f(850,700),
                            sf::Vector2f(300,90),
                            0,
                            "", "Name");
MagicView NewGameMenu::view;