//
// Created by ant on 03/11/16.
//

#include "NewGameMenu.h"
#include "Game.h"

void NewGameMenu::Draw(sf::RenderWindow &window, sf::Font font)
{
    start.Draw(window, font);
    back.Draw(window, font);
}
void NewGameMenu::Update(sf::RenderWindow &window)
{
}

bool NewGameMenu::startClicked(Inputs &inputs,sf::RenderWindow &window){
    sf::Vector2i mouseLeft = inputs.getKey("mouseLeft");
    sf::Vector2i posMouse = sf::Mouse::getPosition(window);
    return (mouseLeft.y && start.isMouseOver(posMouse));
}
bool NewGameMenu::backClicked(Inputs &inputs,sf::RenderWindow &window){
    sf::Vector2i mouseLeft = inputs.getKey("mouseLeft");
    sf::Vector2i posMouse = sf::Mouse::getPosition(window);
    return (mouseLeft.y && back.isMouseOver(posMouse));
}
Button NewGameMenu::start(sf::Vector2f(0+Game::SCREEN_WIDTH/20,Game::SCREEN_HEIGHT-Game::SCREEN_HEIGHT/20),
                          sf::Vector2f(Game::SCREEN_WIDTH/10,Game::SCREEN_HEIGHT/20),
                          "START",
                          0);
Button NewGameMenu::back(sf::Vector2f(Game::SCREEN_WIDTH-Game::SCREEN_WIDTH/20,Game::SCREEN_HEIGHT-Game::SCREEN_HEIGHT/20),
                          sf::Vector2f(Game::SCREEN_WIDTH/10,Game::SCREEN_HEIGHT/20),
                          "BACK",
                          0);