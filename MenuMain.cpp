//
// Created by ant on 03/11/16.
//

#include "MenuMain.h"
#include "Game.h"

void MenuMain::Draw(sf::RenderWindow &window, sf::Font font)
{
    new_game.Draw(window, font);
}
void MenuMain::Update(sf::RenderWindow &window)
{
}
Button MenuMain::new_game(sf::Vector2f(Game::SCREEN_WIDTH/2,Game::SCREEN_HEIGHT/2),
                          sf::Vector2f(Game::SCREEN_WIDTH/10,Game::SCREEN_HEIGHT/20),
                          "NEW GAME",
                          0);
bool MenuMain::newGameClicked(Inputs &inputs,sf::RenderWindow &window){
    sf::Vector2i mouseLeft = inputs.getKey("mouseLeft");
    sf::Vector2i posMouse = sf::Mouse::getPosition(window);
    return (mouseLeft.y && new_game.isMouseOver(posMouse));
}