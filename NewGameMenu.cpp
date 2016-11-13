//
// Created by ant on 03/11/16.
//

#include "NewGameMenu.h"
#include "Game.h"

void NewGameMenu::Draw(sf::RenderWindow &window, sf::Font font)
{

    view.setRenderTarget(&window);
    view.setViewport({0,0,1,1});
    view.setResolution(sf::Vector2i(2000,2000));
    view.setMode(MagicView::crop);
    sf::View oldView = window.getView();
    window.setView(view);
    start.Draw(window, font);
    back.Draw(window, font);
    window.setView(oldView);
}
void NewGameMenu::Update()
{
    sf::Vector2f mousePos = view.getMouseCoord();
    start.update(mousePos);
    back.update(mousePos);
}

bool NewGameMenu::startClicked(Inputs &inputs){
    sf::Vector2i mouseLeft = inputs.getKey("mouseLeft");
    return (mouseLeft.y && start.mouseOver);
}
bool NewGameMenu::backClicked(Inputs &inputs){
    sf::Vector2i mouseLeft = inputs.getKey("mouseLeft");
    return (mouseLeft.y && back.mouseOver);
}
Button NewGameMenu::start(sf::Vector2f(10,1900),
                          sf::Vector2f(300,90),
                          "START",
                          0);
Button NewGameMenu::back(sf::Vector2f(1700,1900),
                         sf::Vector2f(300,90),
                          "BACK",
                          0);
MagicView NewGameMenu::view;