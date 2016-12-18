#include <stdio.h>


// TODO: reference additional headers your program requires here
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <map>
#include <iostream>
#include <cassert>


#include "Item.h"


Item::Item(std::string id_item)
{
	//std::cout << id_item << std::endl;
	id = id_item;
	max_stack_amount= 1;
	id_set0="-1";
	id_set1="-1";
	Reload(id_item);
	amount = 0;

	/*
	if (!font.loadFromFile("resources/font1.ttf"))
	{
		    std::cout << "font error" << std::endl;
	}
	text.setCharacterSize(18); 
	text.setColor(sf::Color::Red);
	text.setStyle(sf::Text::Bold);
	text.setFont(font); // font is a sf::Font */
}


Item::~Item()
{
	
}
Item* Item::getItemCopy(){
	Item* new_it = new Item(id);
	sf::Vector2f pos = GetPosition();
	new_it->SetPosition(pos.x, pos.y);
	new_it->SetSize(GetWidth());
	return new_it;
}


void Item::Draw(sf::RenderWindow& renderWindow, TextureManager &t, sf::Text &text)
{

		sf::Sprite s;
		sf::Vector2f item_pos = GetPosition();
		t.generateSprite(id, item_pos, s, sf::Vector2f(GetWidth(),GetHeight()));
		renderWindow.draw(s);



	sf::Vector2f test_pos = GetPosition();
	text.setPosition(test_pos.x, test_pos.y);
	text.setScale(sf::Vector2f(1.5,1.5));
	renderWindow.draw(text);
		//renderWindow.draw(_sprite);
		/*if(amount >0){
			char c[10];
			sprintf(c, "%i", amount);
			std::string string(c);
			sf::String str(string);
			text.setString(str);
			text.setPosition(GetPosition().x, GetPosition().y);
			renderWindow.draw(text);	
		}*/

}




void Item::decrement(int amount_dec){
	amount -= amount_dec;
	//delete this;
}

void Item::SetPosition(float x, float y)
{
	position.x=x;
	position.y=y;
}
void Item::SetSize(float x)
{

	size.x=x;
	size.y=x;
}

sf::Vector2f Item::GetPosition() const
{
		return position;
}

float Item::GetHeight() const
{
	return size.y;
	//return _sprite.getTexture()->getSize().y*_sprite.getScale().y;
}

float Item::GetWidth() const
{
	return size.x;
	//return _sprite.getTexture()->getSize().x*_sprite.getScale().x;
}


void Item::Reload(std::string new_id)
{
	id = new_id;
	if(new_id == "0"){
		max_stack_amount= 10;
		amount=0;
		id_set0="0";
		id_set1="0";

	}
	else if(new_id == "D"){
		max_stack_amount= 10;
		amount=0;
		id_set0="d";
		id_set1="D";
		craft_cost["r"] = 2;
		craft_cost["d"] = 3;
	}
	else if(new_id == "d"){
		max_stack_amount= 10;
		amount = 0;
		id_set0="d";
		id_set1="D";
	}
	else if(new_id == "c"){
		max_stack_amount= 10;
		amount = 0;
		id_set0="c";
		id_set1="C";
	}
	else if(new_id == "C"){
		max_stack_amount= 5;
		amount = 0;
		id_set0="c";
		id_set1="C";
	}
	else if(new_id == "r"){
		max_stack_amount= 5;
		amount = 0;
		craft_cost["d"] = 5;
		id_set0="r";
		id_set1="R";

	}
	else{
		Reload("0");
	}

	
}