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


AnimatedTile::AnimatedTile(){

	colisionable = false;
    vx=0;
    vy=0;
}


AnimatedTile::~AnimatedTile()
{
	
}


void AnimatedTile::Reload(std::string new_id)
{
	id = new_id;
	if(new_id == "0"){
		colisionable = false;
		
	}
	else if(new_id == "d"){
		colisionable = true;
		
	}
	else if(new_id == "D"){
		colisionable = false;
		
	}
	else if(new_id == "C"){
		colisionable = true;

	}
	else if(new_id == "c"){
		colisionable = false;

	}
	else if(new_id == "r"){
		colisionable = false;

	}
	else if(new_id == "red"){
		colisionable = true;
		
	}
	else{
		Reload("0");
	}

	
}

void AnimatedTile::Draw(sf::RenderWindow & renderWindow, TextureManager &t)
{
    sf::Sprite s;
    sf::Vector2f pos_grass(position.x,position.y-GetHeight()/2);
    t.generateSprite(id, pos_grass, s, sf::Vector2f(GetWidth(),GetHeight()));
    renderWindow.draw(s);
}


void AnimatedTile::Update(float elapsedTime)
{
	vy = 0.5*elapsedTime*100 + vy;
	float x0 = position.x;
	float y0 = position.y;

	float y = y0+vy*elapsedTime;
	SetPosition(x0, y);
}

void AnimatedTile::SetPosition(float x, float y)
{

    position.x=x;
    position.y=y;
}
void AnimatedTile::SetSize(float x)
{

    size.x=x;
    size.y=x;
}

sf::Vector2f AnimatedTile::GetPosition() const
{
    /*
    if(_isLoaded)
    {
        return _sprite.getPosition();
    }*/
    return position;
}

float AnimatedTile::GetHeight() const
{
    return size.y;
    //return _sprite.getTexture()->getSize().y*_sprite.getScale().y;
}

float AnimatedTile::GetWidth() const
{
    return size.x;
    //return _sprite.getTexture()->getSize().x*_sprite.getScale().x;
}