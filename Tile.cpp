#include <stdio.h>


// TODO: reference additional headers your program requires here
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <map>
#include <iostream>
#include <cassert>

#include "Chunk.h"


Tile::Tile(int id_t, int l){
    id_temp = id_t;
    layer = l;

}


Tile::~Tile()
{
	
}


void Tile::Reload(std::string new_id)
{
	id = new_id;
	if(new_id == "0"){
		visible = false;
		weight = 0;
		max_tension = 0;
		rigid = false;
		reach_floor = false;
	}
	else if(new_id == "D"){
		visible = true;
		weight = 0;
		max_tension = 500;
		rigid = true;
		reach_floor = true;
	}
	else if(new_id == "d"){
		visible = true;
		weight = 20;
		max_tension = 200;
		rigid = false;
		reach_floor = false;
	}
	else if(new_id == "r"){
		visible = true;
		weight = 10;
		max_tension = 30;
		rigid = false;
		reach_floor = false;
	}
    else if(new_id == "C"){
        visible = true;
        weight = 10;
        max_tension = 30;
        rigid = false;
        reach_floor = false;
    }
    else if(new_id == "c"){
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


void Tile::Draw(sf::RenderWindow & renderWindow, TextureManager &t)
{

	if(layer==1){
		bool drawable=false;
		for(int i=0; i<8; i++){
			if(lights[i]){
				drawable = true;
				break;
			}
		}
		if(drawable) {
			//sf::Sprite s = t.generateSprite(id, position);
			//_sprite.setTexture(t._image);
			//_sprite.setPosition(position.x, position.y);
			sf::Sprite s;
			t.generateSprite(id, position, s, size);
			renderWindow.draw(s);
		}
		else {

			sf::RectangleShape rec(sf::Vector2f(Chunk::TILE_SIZE,Chunk::TILE_SIZE));
			rec.setPosition(GetPosition());
			rec.setFillColor(sf::Color::Black);
			renderWindow.draw(rec);

            //CONTORNO SOMBREADO
            sf::Sprite s;
            sf::Vector2f shadow_pos = GetPosition();
            shadow_pos.y -= GetHeight();
            t.generateSprite("S", shadow_pos, s, sf::Vector2f(GetWidth(),GetHeight()));
            //s.setRotation(-90);
            renderWindow.draw(s);

            shadow_pos = GetPosition();
            shadow_pos.x += GetWidth();
            t.generateSprite("S", shadow_pos, s, sf::Vector2f(GetWidth(),GetHeight()));
            s.setRotation(90);
            renderWindow.draw(s);

            shadow_pos = GetPosition();
            shadow_pos.y += GetHeight();
            shadow_pos.x -= GetWidth();
            t.generateSprite("S", shadow_pos, s, sf::Vector2f(GetWidth(),GetHeight()));
            s.setRotation(-90);
            renderWindow.draw(s);

            shadow_pos = GetPosition();
            shadow_pos.y += GetHeight()*3;
            shadow_pos.x += GetWidth();
            t.generateSprite("S", shadow_pos, s, sf::Vector2f(GetWidth(),GetHeight()));
            s.setRotation(180);
            renderWindow.draw(s);
		}
	}
	else {
		//sf::Sprite s = t.generateSprite(id, position);
		sf::Sprite s;

		t.generateSprite(id, position, s, size);
		renderWindow.draw(s);
	}
}


void Tile::Update(float elapsedTime)
{
}

void Tile::SetPosition(float x, float y)
{

	position.x=x;
	position.y=y;
}
void Tile::SetSize(float x)
{

	size.x=x;
	size.y=x;
}

sf::Vector2f Tile::GetPosition() const
{
	/*
	if(_isLoaded)
	{
		return _sprite.getPosition();
	}*/
	return position;
}

float Tile::GetHeight() const
{
	return size.y;
	//return _sprite.getTexture()->getSize().y*_sprite.getScale().y;
}

float Tile::GetWidth() const
{
	return size.x;
	//return _sprite.getTexture()->getSize().x*_sprite.getScale().x;
}



