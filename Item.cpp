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


Item::Item(std::string id_item) :
_isLoaded(false)
{
	//std::cout << id_item << std::endl;
	id = id_item;
	max_stack_amount= 1;
	Reload(id_item);
	amount = 0;
	if (!font.loadFromFile("font1.ttf"))
	{
		    std::cout << "font error" << std::endl;
	}
	text.setCharacterSize(18); 
	text.setColor(sf::Color::Red);
	text.setStyle(sf::Text::Bold);
	text.setFont(font); // font is a sf::Font
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


void Item::Draw(sf::RenderWindow& renderWindow)
{
if(_isLoaded)
	{
		renderWindow.draw(_sprite);
		if(amount >0){
			char c[10];
			sprintf(c, "%i", amount);
			std::string string(c);
			sf::String str(string);
			text.setString(str);
			text.setPosition(GetPosition().x, GetPosition().y);
			renderWindow.draw(text);	
		}
		
	}


}




void Item::decrement(int amount_dec){
	amount -= amount_dec;
	//delete this;
}
void Item::SetPosition(float x, float y)
{
	if(_isLoaded)
	{
		_sprite.setPosition(x,y);
	}
}
void Item::SetSize(float x)
{
	sf::Vector2f new_scale(x/_sprite.getTexture()->getSize().x, x/_sprite.getTexture()->getSize().y);
	_sprite.setScale(new_scale);
}

sf::Vector2f Item::GetPosition() const
{
	if(_isLoaded)
	{
		return _sprite.getPosition();
	}
	return sf::Vector2f();
}

float Item::GetHeight() const
{
	return _sprite.getTexture()->getSize().y*_sprite.getScale().y;
}

float Item::GetWidth() const
{
	return _sprite.getTexture()->getSize().x*_sprite.getScale().x;
}

sf::Sprite& Item::GetSprite()
{
	return _sprite;
}

bool Item::IsLoaded() const
{
	return _isLoaded;
}
void Item::Load()
{
	_filename = id;
		if(_image.loadFromFile(_filename.append(".png")) == false)
		{
			_filename =  "error.png";
			_isLoaded = false;
		}
		else
		{
			//_filename = _filename.append(".png");
			//std::cout << _filename << " a" << std::endl;
			_sprite.setTexture(_image);
			_isLoaded = true;
		}
	
}
void Item::Reload(std::string new_id)
{
	id = new_id;
	Load();
	if(new_id == "0"){
		max_stack_amount= 10;
		amount=0;

	}
	else if(new_id == "bed"){
		max_stack_amount= 10;
		amount=0;
		craft_cost["red"] = 2;
		craft_cost["cob"] = 3;
	}
	else if(new_id == "cob"){
		max_stack_amount= 10;
		amount = 0;
	}
	else if(new_id == "red"){
		max_stack_amount= 5;
		amount = 0;
		craft_cost["cob"] = 5;

	}
	else{
		Reload("0");
	}

	
}