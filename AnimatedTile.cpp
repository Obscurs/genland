#include <stdio.h>


// TODO: reference additional headers your program requires here
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <map>
#include <iostream>
#include <cassert>

#include "AnimatedTile.h"


AnimatedTile::AnimatedTile() :
_isLoaded(false)
{

	colisionable = false;
}


AnimatedTile::~AnimatedTile()
{
	
}

void AnimatedTile::Load()
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
}
void AnimatedTile::Reload(std::string new_id)
{
	id = new_id;
	Load();
	if(new_id == "0"){
		colisionable = false;
		
	}
	else if(new_id == "bed"){
		colisionable = true;
		
	}
	else if(new_id == "cob"){
		colisionable = false;
		
	}
	else if(new_id == "red"){
		colisionable = true;
		
	}
	else{
		Reload("0");
	}

	
}
void AnimatedTile::Remove(){
	Reload("0");
}


void AnimatedTile::Draw(sf::RenderWindow & renderWindow)
{
	if(_isLoaded)
	{
		renderWindow.draw(_sprite);
	}
}


void AnimatedTile::Update(float elapsedTime)
{
	vy = 9.8*elapsedTime*100 + vy;
	float x0 = _sprite.getPosition().x;
	float y0 = _sprite.getPosition().y;

	float y = y0+vy*elapsedTime;
	SetPosition(x0, y);
}

void AnimatedTile::SetPosition(float x, float y)
{
	if(_isLoaded)
	{
		_sprite.setPosition(x,y);
	}
}
void AnimatedTile::SetSize(float x)
{
	sf::Vector2f new_scale(x/_sprite.getTexture()->getSize().x, x/_sprite.getTexture()->getSize().y);
	_sprite.setScale(new_scale);
}

sf::Vector2f AnimatedTile::GetPosition() const
{
	if(_isLoaded)
	{
		return _sprite.getPosition();
	}
	return sf::Vector2f();
}

float AnimatedTile::GetHeight() const
{
	return _sprite.getTexture()->getSize().y*_sprite.getScale().y;
}

float AnimatedTile::GetWidth() const
{
	return _sprite.getTexture()->getSize().x*_sprite.getScale().x;
}


sf::Sprite& AnimatedTile::GetSprite()
{
	return _sprite;
}

bool AnimatedTile::IsLoaded() const
{
	return _isLoaded;
}