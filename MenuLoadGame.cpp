//
// Created by ant on 06/11/16.
//

#include "MenuLoadGame.h"
#include "Game.h"

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
void MenuLoadGame::Update(Inputs &inputs)
{
    sf::Vector2f mousePos = view.getMouseCoord();
    load.update(mousePos);
    back.update(mousePos);
    del.update(mousePos);
    save_list.update(mousePos, inputs);


}


bool MenuLoadGame::delClicked(Inputs &inputs){
    sf::Vector2i mouseLeft = inputs.getKey("mouseLeft");
    return (mouseLeft.y && del.mouseOver);
}
bool MenuLoadGame::loadClicked(Inputs &inputs){
    sf::Vector2i mouseLeft = inputs.getKey("mouseLeft");
    return (mouseLeft.y && load.mouseOver);
}
bool MenuLoadGame::backClicked(Inputs &inputs){
    sf::Vector2i mouseLeft = inputs.getKey("mouseLeft");
    return (mouseLeft.y && back.mouseOver);
}
Button MenuLoadGame::load(sf::Vector2f(10,1900),
                          sf::Vector2f(300,90),
                          "LOAD",
                          0);
Button MenuLoadGame::back(sf::Vector2f(1700,1900),
                         sf::Vector2f(300,90),
                         "BACK",
                         0);
Button MenuLoadGame::del(sf::Vector2f(850,1900),
                          sf::Vector2f(300,90),
                          "DELETE",
                          0);
InterfaceList MenuLoadGame::save_list(sf::Vector2f(850,500),
                         sf::Vector2f(300,90),
                         0,
                         5);

MagicView MenuLoadGame::view;