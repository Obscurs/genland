//
// Created by ant on 03/11/16.
//

#include "MenuMain.h"
#include "../Game.h"
#include "../Inputs.h"
#include "../Resources.h"

void MenuMain::Draw(sf::RenderWindow &window, const sf::Font& font)
{


    const sf::View* oldView = &window.getView();
    window.setView(view);
    sf::Sprite sprite(*Resources::getTexture("title2"));
    window.draw(sprite);
    new_game.Draw(window, font);
    load.Draw(window, font);
    //config.Draw(window, font);
    help.Draw(window, font);
    about.Draw(window, font);
    exit.Draw(window, font);
    window.setView(*oldView);
}
void MenuMain::Update()
{
    sf::Vector2f mousePos = view.getMouseCoord();
    new_game.update(mousePos);
    load.update(mousePos);
    //config.update(mousePos);
    help.update(mousePos);
    about.update(mousePos);
    exit.update(mousePos);
}
Button MenuMain::new_game(sf::Vector2f(850,800),
                          sf::Vector2f(300,90),
                          "NEW GAME",
                          0);
Button MenuMain::load(sf::Vector2f(850,900),
                      sf::Vector2f(300,90),
                          "LOAD",
                          0);
Button MenuMain::config(sf::Vector2f(850,1000),
                        sf::Vector2f(300,90),
                          "CONFIG",
                          0);
Button MenuMain::help(sf::Vector2f(850,1000),
                        sf::Vector2f(300,90),
                        "HELP",
                        0);
Button MenuMain::about(sf::Vector2f(850,1100),
                        sf::Vector2f(300,90),
                        "ABOUT",
                        0);
Button MenuMain::exit(sf::Vector2f(850,1200),
                      sf::Vector2f(300,90),
                          "EXIT",
                          0);
bool MenuMain::newGameClicked(){
    return (Inputs::MouseBreak(Inputs::M_LEFT) && new_game.mouseOver);
}
bool MenuMain::loadClicked(){
    return (Inputs::MouseBreak(Inputs::M_LEFT) && load.mouseOver);
}
bool MenuMain::configClicked(){
    return (Inputs::MouseBreak(Inputs::M_LEFT) && config.mouseOver);
}
bool MenuMain::exitClicked(){
    return (Inputs::MouseBreak(Inputs::M_LEFT) && exit.mouseOver);
}
bool MenuMain::aboutClicked(){
    return (Inputs::MouseBreak(Inputs::M_LEFT) && about.mouseOver);
}
bool MenuMain::helpClicked(){
    return (Inputs::MouseBreak(Inputs::M_LEFT) && help.mouseOver);
}
MagicView MenuMain::view;
