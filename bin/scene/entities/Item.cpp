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
#include "../../Resources.h"


Item::Item(std::string id_item)
{
	//std::cout << id_item << std::endl;
	id = id_item;
	max_stack_amount= 1;
	id_set0="-1";
	id_set1="-1";
    _isEntity = false;
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


void Item::Draw(sf::RenderWindow& renderWindow, sf::Text &text)
{
	TextureManager *t = Resources::getTextureManager("tileMap");
		sf::Sprite s;
		sf::Vector2f item_pos = GetPosition();
		t->generateSprite(id, item_pos, s, sf::Vector2f(GetWidth(),GetHeight()));
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
    type = COLLECTABLE;
	id = new_id;
    _isEntity = false;
	if(new_id == "0"){
		max_stack_amount= 10;
		amount=0;
		id_set0="0";
		id_set1="0";

	}
	else if(new_id == "D" || new_id == "d"){
		max_stack_amount= 10;
		amount=0;
		id_set0="d";
		id_set1="D";
		craft_cost["r"] = 2;
		craft_cost["d"] = 3;
	}
	else if(new_id == "c" || new_id == "C"){
		max_stack_amount= 10;
		amount = 0;
		id_set0="c";
		id_set1="C";
	}
	else if(new_id == "r" || new_id == "R"){
		max_stack_amount= 5;
		amount = 0;
		craft_cost["d"] = 5;
		id_set0="r";
		id_set1="R";

	}
	else if(new_id == "n" || new_id == "N"){
		max_stack_amount= 5;
		amount = 0;
		craft_cost["n"] = 2;
		id_set0="n";
		id_set1="N";

	}
	else if(new_id == "T" || new_id == "t"){
		max_stack_amount= 20;
		amount = 0;
		craft_cost["n"] = 2;
		id_set0="-1";
		id_set1="-1";
	}
    else if(new_id == "F" || new_id == "f"){
        max_stack_amount= 20;
        amount = 0;
        craft_cost["n"] = 2;
        id_set0="-1";
        id_set1="-1";
    }
    else if(new_id == "K" || new_id == "k"){
        max_stack_amount= 20;
        amount = 0;
        craft_cost["n"] = 2;
        id_set0="k";
        id_set1="K";
    }
    else if(new_id == "G" || new_id == "g"){
        max_stack_amount= 20;
        amount = 0;
        craft_cost["n"] = 2;
        id_set0="g";
        id_set1="G";
    }
    else if(new_id == "I" || new_id == "i"){
        max_stack_amount= 20;
        amount = 0;
        craft_cost["n"] = 2;
        id_set0="i";
        id_set1="I";
    }
    else if(new_id == "L" || new_id == "l"){
        max_stack_amount= 20;
        amount = 0;
        craft_cost["n"] = 2;
        id_set0="i";
        id_set1="I";
    }
    else if(new_id == "Y" || new_id == "y"){
        max_stack_amount= 20;
        amount = 0;
        craft_cost["n"] = 2;
        id_set0="y";
        id_set1="Y";
    }
    else if(new_id == "B" || new_id == "b"){
        max_stack_amount= 20;
        amount = 0;
        craft_cost["n"] = 2;
        id_set0="b";
        id_set1="B";
    }
    else if(new_id == "W" || new_id == "w"){
        max_stack_amount= 20;
        amount = 0;
        craft_cost["n"] = 2;
        id_set0="w";
        id_set1="W";
    }
    else if(new_id == "J" || new_id == "j"){
        max_stack_amount= 20;
        amount = 0;
        craft_cost["n"] = 2;
        id_set0="j";
        id_set1="J";
    }
    else if(new_id == "A" || new_id == "a"){
        max_stack_amount= 20;
        amount = 0;
        craft_cost["T"] = 5;
        id_set0="a";
        id_set1="A";
    }
    else if(new_id == "sword1"){
        max_stack_amount= 1;
        amount = 0;
        craft_cost["T"] = 2;
        craft_cost["F"] = 5;
        id_set0="-1";
        id_set1="-1";
        type = TOOL;
    }
    else if(new_id == "pickaxe1"){
        max_stack_amount= 1;
        amount = 0;
        craft_cost["T"] = 2;
        craft_cost["F"] = 5;
        id_set0="-1";
        id_set1="-1";
        type = TOOL;
    }
    else if(new_id == "armor3"){ //iron
        max_stack_amount= 1;
        amount = 0;
        craft_cost["I"] = 100;
        craft_cost["Y"] = 1;
        craft_cost["K"] = 50;
        craft_cost["G"] = 10;
        id_set0="-1";
        id_set1="-1";
        type = ARMOR;
        _isEntity = true;
    }else if(new_id == "armor2"){ //summer
        max_stack_amount= 1;
        amount = 0;
        craft_cost["F"] = 50;
        id_set0="-1";
        id_set1="-1";
        type = ARMOR;
        _isEntity = true;
    }else if(new_id == "armor1"){ //winter
        max_stack_amount= 1;
        amount = 0;
        craft_cost["F"] = 100;
        craft_cost["K"] = 10;
        craft_cost["I"] = 5;
        id_set0="-1";
        id_set1="-1";
        type = ARMOR;
        _isEntity = true;
    }else if(new_id == "stairs"){
        max_stack_amount= 10;
        amount = 0;
        craft_cost["F"] = 10;
        craft_cost["T"] = 4;
        id_set0="-1";
        id_set1="stairs";
        _isEntity = true;
    }else if(new_id == "torch"){
        max_stack_amount= 10;
        amount = 0;
        craft_cost["F"] = 5;
        craft_cost["L"] = 3;
        id_set0="-1";
        id_set1="torch";
        _isEntity = true;
    } else if(new_id == "food"){
        max_stack_amount= 10;
        amount = 0;
        craft_cost["F"] = 10;
        id_set0="-1";
        id_set1="-1";
        _isEntity = true;
    }
	else{
        std::cout << "item " << new_id << " dont exist" << std::endl;
		Reload("0");
	}

	
}