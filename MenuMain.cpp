//
// Created by ant on 03/11/16.
//

#include "MenuMain.h"
#include "Game.h"

void MenuMain::Draw(sf::RenderWindow &window, sf::Font font)
{
    new_game.Draw(window, font);
    load.Draw(window, font);
    config.Draw(window, font);
    exit.Draw(window, font);
}
void MenuMain::Update(sf::RenderWindow &window)
{
}
Button MenuMain::new_game(sf::Vector2f(Game::SCREEN_WIDTH/2,Game::SCREEN_HEIGHT/2),
                          sf::Vector2f(Game::SCREEN_WIDTH/10,Game::SCREEN_HEIGHT/20),
                          "NEW GAME",
                          0);
Button MenuMain::load(sf::Vector2f(Game::SCREEN_WIDTH/2,Game::SCREEN_HEIGHT/2+(Game::SCREEN_HEIGHT/18)),
                          sf::Vector2f(Game::SCREEN_WIDTH/10,Game::SCREEN_HEIGHT/20),
                          "LOAD",
                          0);
Button MenuMain::config(sf::Vector2f(Game::SCREEN_WIDTH/2,Game::SCREEN_HEIGHT/2+(Game::SCREEN_HEIGHT/18)*2),
                          sf::Vector2f(Game::SCREEN_WIDTH/10,Game::SCREEN_HEIGHT/20),
                          "CONFIG",
                          0);
Button MenuMain::exit(sf::Vector2f(Game::SCREEN_WIDTH/2,Game::SCREEN_HEIGHT/2+(Game::SCREEN_HEIGHT/18)*3),
                          sf::Vector2f(Game::SCREEN_WIDTH/10,Game::SCREEN_HEIGHT/20),
                          "EXIT",
                          0);
bool MenuMain::newGameClicked(Inputs &inputs,sf::RenderWindow &window){
    sf::Vector2i mouseLeft = inputs.getKey("mouseLeft");
    sf::Vector2i posMouse = sf::Mouse::getPosition(window);
    return (mouseLeft.y && new_game.isMouseOver(posMouse));
}
bool MenuMain::loadClicked(Inputs &inputs,sf::RenderWindow &window){
    sf::Vector2i mouseLeft = inputs.getKey("mouseLeft");
    sf::Vector2i posMouse = sf::Mouse::getPosition(window);
    return (mouseLeft.y && load.isMouseOver(posMouse));
}
bool MenuMain::configClicked(Inputs &inputs,sf::RenderWindow &window){
    sf::Vector2i mouseLeft = inputs.getKey("mouseLeft");
    sf::Vector2i posMouse = sf::Mouse::getPosition(window);
    return (mouseLeft.y && config.isMouseOver(posMouse));
}
bool MenuMain::exitClicked(Inputs &inputs,sf::RenderWindow &window){
    sf::Vector2i mouseLeft = inputs.getKey("mouseLeft");
    sf::Vector2i posMouse = sf::Mouse::getPosition(window);
    return (mouseLeft.y && exit.isMouseOver(posMouse));
}