#include <stdio.h>


// TODO: reference additional headers your program requires here
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <map>
#include <iostream>
#include <cassert>


#include "Inputs.h"
#include "Inventory.h"



Inputs::Inputs()
{
	keys["mouseLeft"] = sf::Vector2i(0,0);
	keys["mouseRight"] = sf::Vector2i(0,0);
	keys["LShift"] = sf::Vector2i(0,0);
	keys["Q"] = sf::Vector2i(0,0);
	keys["A"] = sf::Vector2i(0,0);
	keys["D"] = sf::Vector2i(0,0);
	keys["Space"] = sf::Vector2i(0,0);
	keys["wheel"] = sf::Vector2i(0,0);
	wheelDelta = 0;
	for (unsigned int i = 1; i <= Inventory::TAB_SLOTS; ++i) {
		keys["number"+std::to_string(i)] = sf::Vector2i(0,0);
	}
}


Inputs::~Inputs()
{
	
}
void Inputs::UpdateKey(bool pressed, std::string key){
	if (pressed){
		keys[key].x=1;
		keys[key].y=0;
	} else {
		if(keys[key].x==1){
			keys[key].x=0;
			keys[key].y=1;
		} else{
			keys[key].y=0;
		}
	}
}


void Inputs::Update(){

	UpdateKey(sf::Mouse::isButtonPressed(sf::Mouse::Left), "mouseLeft");
	UpdateKey(sf::Mouse::isButtonPressed(sf::Mouse::Right), "mouseRight");
	UpdateKey(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift), "LShift");
	UpdateKey(sf::Keyboard::isKeyPressed(sf::Keyboard::Q), "Q");
	UpdateKey(sf::Keyboard::isKeyPressed(sf::Keyboard::A), "A");
	UpdateKey(sf::Keyboard::isKeyPressed(sf::Keyboard::D), "D");
	UpdateKey(sf::Keyboard::isKeyPressed(sf::Keyboard::Space), "Space");
	keys["wheel"] = sf::Vector2i(wheelDelta, 0);
	wheelDelta = 0;
	for (unsigned int i = 1; i <= Inventory::TAB_SLOTS; ++i) {
		UpdateKey(sf::Keyboard::isKeyPressed(sf::Keyboard::Key(int(sf::Keyboard::Num0) + i)), "number"+std::to_string(i));
	}
}

void Inputs::UpdateWheel(int delta) {
	wheelDelta = delta;
}

sf::Vector2i Inputs::getKey(std::string s){
	return keys[s];
}
