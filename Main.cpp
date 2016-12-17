
#include <stdio.h>


// TODO: reference additional headers your program requires here
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <map>
#include <iostream>
#include <cassert>
#include "Game.h"
#include "Settings.h"


sf::Clock deltaClock;
sf::Time dt;
int Settings::SCREEN_WIDTH = 0;
int Settings::SCREEN_HEIGHT = 0;
int Settings::TILE_SIZE = 0;
int Settings::GAME_WIDTH = 0;
int Settings::GAME_HEIGHT = 0;
int main(int argc, char** argv)
{
    bool true_exit = false;
    while(!true_exit) {
        std::ifstream myfile("config/config");
        myfile >> Settings::SCREEN_WIDTH >> Settings::SCREEN_HEIGHT;
        myfile.close();
        if (Settings::SCREEN_WIDTH > 1200) {
            Settings::TILE_SIZE = Settings::TILE_SIZE_HIGH;
            Settings::GAME_WIDTH = Settings::GAME_WIDTH_HIGH;
            Settings::GAME_HEIGHT = Settings::GAME_HEIGHT_HIGH;
        } else if (Settings::SCREEN_WIDTH > 1000) {
            Settings::TILE_SIZE = Settings::TILE_SIZE_MED;
            Settings::GAME_WIDTH = Settings::GAME_WIDTH_MED;
            Settings::GAME_HEIGHT = Settings::GAME_HEIGHT_MED;
        } else {
            Settings::TILE_SIZE = Settings::TILE_SIZE_LOW;
            Settings::GAME_WIDTH = Settings::GAME_WIDTH_LOW;
            Settings::GAME_HEIGHT = Settings::GAME_HEIGHT_LOW;
        }
        Game *g = new Game();
        g->Start();
        true_exit = g->true_exit;
    }
  return 0;
}