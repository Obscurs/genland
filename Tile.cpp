#include <stdio.h>


// TODO: reference additional headers your program requires here
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <map>
#include <iostream>
#include <cassert>

#include "Tile.h"


Tile::Tile() :
_isLoaded(false)
{

	colisionable = false;
}


Tile::~Tile()
{
	
}

void Tile::Load()
{
	if(id != "0"){
		_filename = id;
		if(_image.loadFromFile(_filename.append(".png")) == false)
		{
			_filename =  "error.png";
			_isLoaded = false;
		}
		else
		{
			//_filename = id.append(".png");
			_sprite.setTexture(_image);
			_isLoaded = true;
		}
	}
	else{
		_filename = "no_image";
		if(_image.loadFromFile("no_image.png") == false)
		{
			_filename =  "error.png";
			_isLoaded = false;
		}
		else
		{
			//_filename = id.append(".png");
			_sprite.setTexture(_image);
			_isLoaded = true;
		}
	}
}
void Tile::Reload(std::string new_id)
{
	id = new_id;
	Load();
	if(new_id == "0"){
		colisionable = false;
		visible = false;
		weight = 0;
		max_tension = 0;
		rigid = false;
		reach_floor = false;
	}
	else if(new_id == "bed"){
		colisionable = true;
		visible = true;
		weight = 0;
		max_tension = 500;
		rigid = true;
		reach_floor = true;
	}
	else if(new_id == "cob"){
		colisionable = false;
		visible = true;
		weight = 20;
		max_tension = 200;
		rigid = false;
		reach_floor = false;
	}
	else if(new_id == "red"){
		colisionable = true;
		visible = true;
		weight = 10;
		max_tension = 30;
		rigid = false;
		reach_floor = false;
	}
	else{
		Reload("0");
	}

	
}
void Tile::Remove(){
	Reload("0");
}


void Tile::Draw(sf::RenderWindow & renderWindow)
{
	if(_isLoaded)
	{
		renderWindow.draw(_sprite);
	}
}


void Tile::Update(float elapsedTime)
{
}

void Tile::SetPosition(float x, float y)
{
	if(_isLoaded)
	{
		_sprite.setPosition(x,y);
	}
}
void Tile::SetSize(float x)
{
	sf::Vector2f new_scale(x/_sprite.getTexture()->getSize().x, x/_sprite.getTexture()->getSize().y);
	_sprite.setScale(new_scale);
}

sf::Vector2f Tile::GetPosition() const
{
	if(_isLoaded)
	{
		return _sprite.getPosition();
	}
	return sf::Vector2f();
}

float Tile::GetHeight() const
{
	return _sprite.getTexture()->getSize().y*_sprite.getScale().y;
}

float Tile::GetWidth() const
{
	return _sprite.getTexture()->getSize().x*_sprite.getScale().x;
}

sf::Rect<float> Tile::GetBoundingRect() const
{
	sf::Vector2f size = _sprite.getScale();
	sf::Vector2f position = _sprite.getPosition();

	return sf::Rect<float>(
						position.x - size.x/2,
						position.y - size.y/2,
						position.x + size.x/2,
						position.y + size.y/2
						);
}

sf::Sprite& Tile::GetSprite()
{
	return _sprite;
}

bool Tile::IsLoaded() const
{
	return _isLoaded;
}