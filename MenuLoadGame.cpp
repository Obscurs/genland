//
// Created by ant on 06/11/16.
//

#include "MenuLoadGame.h"
#include "Game.h"

void MenuLoadGame::Draw(sf::RenderWindow &window, sf::Font font)
{
    del.Draw(window, font);
    load.Draw(window, font);
    back.Draw(window, font);
}
void MenuLoadGame::Update(sf::RenderWindow &window)
{
}

bool MenuLoadGame::loadClicked(Inputs &inputs,sf::RenderWindow &window){
    sf::Vector2i mouseLeft = inputs.getKey("mouseLeft");
    sf::Vector2i posMouse = sf::Mouse::getPosition(window);
    return (mouseLeft.y && load.mouseOver);
}
bool MenuLoadGame::delClicked(Inputs &inputs,sf::RenderWindow &window){
    sf::Vector2i mouseLeft = inputs.getKey("mouseLeft");
    sf::Vector2i posMouse = sf::Mouse::getPosition(window);
    return (mouseLeft.y && del.mouseOver);
}
bool MenuLoadGame::backClicked(Inputs &inputs,sf::RenderWindow &window){
    sf::Vector2i mouseLeft = inputs.getKey("mouseLeft");
    sf::Vector2i posMouse = sf::Mouse::getPosition(window);
    return (mouseLeft.y && back.mouseOver);
}
Button MenuLoadGame::load(sf::Vector2f(0+Game::SCREEN_WIDTH/20,Game::SCREEN_HEIGHT-Game::SCREEN_HEIGHT/20),
                          sf::Vector2f(Game::SCREEN_WIDTH/10,Game::SCREEN_HEIGHT/20),
                          "LOAD",
                          0);
Button MenuLoadGame::del(sf::Vector2f(0+Game::SCREEN_WIDTH/10,Game::SCREEN_HEIGHT-Game::SCREEN_HEIGHT/20),
                          sf::Vector2f(Game::SCREEN_WIDTH/10,Game::SCREEN_HEIGHT/20),
                          "DELETE",
                          0);
Button MenuLoadGame::back(sf::Vector2f(Game::SCREEN_WIDTH-Game::SCREEN_WIDTH/20,Game::SCREEN_HEIGHT-Game::SCREEN_HEIGHT/20),
                         sf::Vector2f(Game::SCREEN_WIDTH/10,Game::SCREEN_HEIGHT/20),
                         "BACK",
                         0);