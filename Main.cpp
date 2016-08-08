
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



sf::Clock deltaClock;
sf::Time dt;
int main(int argc, char** argv)
{
  Game::Start();
  
  return 0;
}
