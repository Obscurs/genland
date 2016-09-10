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
bool Tile::drawable(){
	bool res =false;
	for(int i=0; i<8; i++){
		if(neighbors[i] !=nullptr && neighbors[i]->id=="0"){
			res=true;
			break;
		}
	}
	return res;
}

void Tile::Draw(sf::RenderWindow & renderWindow, TextureManager &t)
{

	if(layer==1){
		if(drawable()) {
			//sf::Sprite s = t.generateSprite(id, position);
			//_sprite.setTexture(t._image);
			//_sprite.setPosition(position.x, position.y);
			sf::Sprite sp;
			t.generateSprite(id, position, sp, size);
			renderWindow.draw(sp);

			//CONTORNO SOMBREADO
			if(neighbors[0] != nullptr && !neighbors[0]->drawable() && neighbors[7]->drawable() && neighbors[1]->drawable()){
				sf::Sprite s;
				sf::Vector2f shadow_pos = GetPosition();
				shadow_pos.x += GetWidth();
				t.generateSprite("S2", shadow_pos, s, sf::Vector2f(GetWidth(),GetHeight()));
				s.setRotation(90);
				renderWindow.draw(s);
			}
			if(neighbors[1] != nullptr && !neighbors[1]->drawable()){
				sf::Sprite s;
				sf::Vector2f shadow_pos = GetPosition();
				shadow_pos.y += GetHeight();
				shadow_pos.x += GetWidth();
				t.generateSprite("S", shadow_pos, s, sf::Vector2f(GetWidth(),GetHeight()));
				s.setRotation(180);
				renderWindow.draw(s);
			}
			if(neighbors[2] != nullptr && !neighbors[2]->drawable() && neighbors[3]->drawable() && neighbors[1]->drawable()){
				sf::Sprite s;
				sf::Vector2f shadow_pos = GetPosition();
				shadow_pos.y += GetHeight();
				shadow_pos.x += GetWidth();
				t.generateSprite("S2", shadow_pos, s, sf::Vector2f(GetWidth(),GetHeight()));
				s.setRotation(180);
				renderWindow.draw(s);

			}
			if(neighbors[3] != nullptr && !neighbors[3]->drawable()){
				sf::Sprite s;
				sf::Vector2f shadow_pos = GetPosition();
				shadow_pos.y += GetHeight();
				t.generateSprite("S", shadow_pos, s, sf::Vector2f(GetWidth(),GetHeight()));
				s.setRotation(-90);
				renderWindow.draw(s);
			}
			if(neighbors[4] != nullptr && !neighbors[4]->drawable() && neighbors[3]->drawable() && neighbors[5]->drawable()){
				sf::Sprite s;
				sf::Vector2f shadow_pos = GetPosition();
				shadow_pos.y += GetHeight();
				t.generateSprite("S2", shadow_pos, s, sf::Vector2f(GetWidth(),GetHeight()));
				s.setRotation(-90);
				renderWindow.draw(s);
			}
			if(neighbors[5] != nullptr && !neighbors[5]->drawable()){
				sf::Sprite s;
				sf::Vector2f shadow_pos = GetPosition();
				t.generateSprite("S", shadow_pos, s, sf::Vector2f(GetWidth(),GetHeight()));
				renderWindow.draw(s);

			}
			if(neighbors[6] != nullptr && !neighbors[6]->drawable() && neighbors[5]->drawable() && neighbors[7]->drawable()){
				sf::Sprite s;
				sf::Vector2f shadow_pos = GetPosition();
				t.generateSprite("S2", shadow_pos, s, sf::Vector2f(GetWidth(),GetHeight()));
				renderWindow.draw(s);

			}
			if(neighbors[7] != nullptr && !neighbors[7]->drawable()){
				sf::Sprite s;
				sf::Vector2f shadow_pos = GetPosition();
				shadow_pos.x += GetWidth();
				t.generateSprite("S", shadow_pos, s, sf::Vector2f(GetWidth(),GetHeight()));
				s.setRotation(90);
				renderWindow.draw(s);
			}
		}
		else {

			sf::RectangleShape rec(sf::Vector2f(Chunk::TILE_SIZE,Chunk::TILE_SIZE));
			rec.setPosition(GetPosition());
			rec.setFillColor(sf::Color::Black);
			renderWindow.draw(rec);

		}
	}
	else {
		//sf::Sprite s = t.generateSprite(id, position);
		sf::Sprite sp;

		t.generateSprite(id, position, sp, size);
		renderWindow.draw(sp);

		//CONTORNO SOMBREADO
		if(neighbors[0] != nullptr && !neighbors[0]->drawable() && neighbors[7]->drawable() && neighbors[1]->drawable()){

		}
		if(neighbors[1] != nullptr && neighbors[1]->neighbors[8]->id!="0"){
			sf::Sprite s;
			sf::Vector2f shadow_pos = GetPosition();
			shadow_pos.y += GetHeight();
			shadow_pos.x += GetWidth();
			t.generateSprite("s", shadow_pos, s, sf::Vector2f(GetWidth(),GetHeight()));
			s.setRotation(180);
			renderWindow.draw(s);
		}
		if(neighbors[2] != nullptr && !neighbors[2]->drawable() && neighbors[3]->drawable() && neighbors[1]->drawable()){


		}
		if(neighbors[3] != nullptr && neighbors[3]->neighbors[8]->id!="0"){
			sf::Sprite s;
			sf::Vector2f shadow_pos = GetPosition();
			shadow_pos.y += GetHeight();
			t.generateSprite("s", shadow_pos, s, sf::Vector2f(GetWidth(),GetHeight()));
			s.setRotation(-90);
			renderWindow.draw(s);
		}
		if(neighbors[4] != nullptr && !neighbors[4]->drawable() && neighbors[3]->drawable() && neighbors[5]->drawable()){

		}
		if(neighbors[5] != nullptr && neighbors[5]->neighbors[8]->id!="0"){
			sf::Sprite s;
			sf::Vector2f shadow_pos = GetPosition();
			t.generateSprite("s", shadow_pos, s, sf::Vector2f(GetWidth(),GetHeight()));
			renderWindow.draw(s);

		}
		if(neighbors[6] != nullptr && !neighbors[6]->drawable() && neighbors[5]->drawable() && neighbors[7]->drawable()){


		}
		if(neighbors[7] != nullptr && neighbors[7]->neighbors[8]->id!="0"){
			sf::Sprite s;
			sf::Vector2f shadow_pos = GetPosition();
			shadow_pos.x += GetWidth();
			t.generateSprite("s", shadow_pos, s, sf::Vector2f(GetWidth(),GetHeight()));
			s.setRotation(90);
			renderWindow.draw(s);
		}
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



