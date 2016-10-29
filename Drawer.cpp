//
// Created by arnito on 30/10/16.
//

#include "Drawer.h"
#include "Game.h"
Drawer::Drawer(){
    texture_plain_sprite.create(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT);
    texture_front = new sf::RenderTexture();
    texture_back = new sf::RenderTexture();
    texture_front->create(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT);
    texture_back->create(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT);

    black_texture.create(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT);
    sf::RectangleShape rectangle(sf::Vector2f(0, 0));
    rectangle.setSize(sf::Vector2f(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT));
    rectangle.setFillColor(sf::Color::Black);
    black_texture.draw(rectangle);
}


Drawer::~Drawer()
{

}
