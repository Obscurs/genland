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
		weight = 20;
		max_tension = 100;
		rigid = false;
		reach_floor = true;
	}
	else if(new_id == "d"){
		visible = true;
		weight = 20;
		max_tension = 100;
		rigid = false;
		reach_floor = false;
	}
	else if(new_id == "r"){
		visible = true;
		weight = 10;
		max_tension = 200;
		rigid = false;
		reach_floor = false;
	}
    else if(new_id == "C"){
        visible = true;
        weight = 20;
        max_tension = 500;
        rigid = false;
        reach_floor = false;
    }
    else if(new_id == "c"){
        visible = true;
        weight = 20;
        max_tension = 500;
        rigid = false;
        reach_floor = false;
    }
	else{
		Reload("0");
	}

	
}
bool Tile::drawable(){
	bool res =false;
    if(layer==1 && id=="0") return true;
	for(int i=0; i<8; i++){
		if(neighbors[i] !=nullptr && neighbors[i]->id=="0"){
			res=true;
			break;
		}
	}
	return res;
}
void Tile::DrawFadeOut(sf::RenderWindow & renderWindow, TextureManager &t){
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
void Tile::DrawAmbientOclusion(sf::RenderWindow & renderWindow, TextureManager &t){
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
void Tile::DrawOuts(sf::RenderWindow & renderWindow, TextureManager &t)
{
    Tile* t1 = neighbors[1];
    Tile* t3 = neighbors[3];
    Tile* t5 = neighbors[5];
    Tile* t7 = neighbors[7];
    bool valid_t1= (t1 != nullptr && (t1->id =="d" || t1->id =="D" || t1->id =="c" || t1->id =="C"));
    bool valid_t3= (t3 != nullptr && (t3->id =="d" || t3->id =="D" || t3->id =="c" || t3->id =="C"));
    bool valid_t5= (t5 != nullptr && (t5->id =="d" || t5->id =="D" || t5->id =="c" || t5->id =="C"));
    bool valid_t7 = (t7 != nullptr && (t7->id =="d" || t7->id =="D" || t7->id =="c" || t7->id =="C"));
    if(valid_t1 && valid_t3 && t1->id==t3->id){
        std::string id_mini = t1->id;
        id_mini.append("_out");
        sf::Sprite s;
        t.generateMiniSprite(id_mini, position, s, sf::Vector2f(GetWidth(),GetHeight()),1);
        renderWindow.draw(s);
    }
    if(valid_t1 && valid_t7 && t1->id==t7->id){
        std::string id_mini = t1->id;
        id_mini.append("_out");
        sf::Sprite s;
        t.generateMiniSprite(id_mini, position, s, sf::Vector2f(GetWidth(),GetHeight()),0);
        renderWindow.draw(s);
    }
    if(valid_t3 && valid_t5 && t3->id==t5->id){
        std::string id_mini = t3->id;
        id_mini.append("_out");
        sf::Sprite s;
        t.generateMiniSprite(id_mini, position, s, sf::Vector2f(GetWidth(),GetHeight()),3);
        renderWindow.draw(s);
    }
    if(valid_t7 && valid_t5 && t5->id==t7->id){
        std::string id_mini = t5->id;
        id_mini.append("_out");
        sf::Sprite s;
        t.generateMiniSprite(id_mini, position, s, sf::Vector2f(GetWidth(),GetHeight()),2);
        renderWindow.draw(s);
    }
}
void Tile::DrawIns(sf::RenderWindow & renderWindow, TextureManager &t){
    bool is_mini[4] ={0,0,0,0};
    if(neighbors[1] != nullptr && neighbors[1]->id=="0" && neighbors[2] != nullptr && neighbors[2]->id=="0" && neighbors[3] != nullptr && neighbors[3]->id=="0"){

        std::string id_mini = id;
        id_mini.append("_in");
        sf::Sprite s;
        t.generateMiniSprite(id_mini, position, s, sf::Vector2f(GetWidth(),GetHeight()),1);
        renderWindow.draw(s);
        is_mini[0]=true;
    }
    if(neighbors[3] != nullptr && neighbors[3]->id=="0" && neighbors[4] != nullptr && neighbors[4]->id=="0" && neighbors[5] != nullptr && neighbors[5]->id=="0"){

        std::string id_mini = id;
        id_mini.append("_in");
        sf::Sprite s;
        t.generateMiniSprite(id_mini, position, s, sf::Vector2f(GetWidth(),GetHeight()),3);
        renderWindow.draw(s);
        is_mini[1]=true;
    }
    if(neighbors[5] != nullptr && neighbors[5]->id=="0" && neighbors[6] != nullptr && neighbors[6]->id=="0" && neighbors[7] != nullptr && neighbors[7]->id=="0"){

        std::string id_mini = id;
        id_mini.append("_in");
        sf::Sprite s;
        t.generateMiniSprite(id_mini, position, s, sf::Vector2f(GetWidth(),GetHeight()),2);
        renderWindow.draw(s);
        is_mini[2]=true;
    }
    if(neighbors[7] != nullptr && neighbors[7]->id=="0" && neighbors[1] != nullptr && neighbors[1]->id=="0" && neighbors[0] != nullptr && neighbors[0]->id=="0"){

        std::string id_mini = id;
        id_mini.append("_in");
        sf::Sprite s;
        t.generateMiniSprite(id_mini, position, s, sf::Vector2f(GetWidth(),GetHeight()),0);
        renderWindow.draw(s);
        is_mini[3]=true;
    }
    if(!is_mini[0] && !is_mini[1] && !is_mini[2] && !is_mini[3]){
        sf::Sprite s;

        t.generateSprite(id, position, s, size);
        renderWindow.draw(s);
    } else{
        if(!is_mini[0]){
            sf::Sprite s;
            t.generateMiniSprite(id, position, s, sf::Vector2f(GetWidth(),GetHeight()),1);
            renderWindow.draw(s);
        }
        if(!is_mini[1]){
            sf::Sprite s;
            t.generateMiniSprite(id, position, s, sf::Vector2f(GetWidth(),GetHeight()),3);
            renderWindow.draw(s);
        }
        if(!is_mini[2]){
            sf::Sprite s;
            t.generateMiniSprite(id, position, s, sf::Vector2f(GetWidth(),GetHeight()),2);
            renderWindow.draw(s);
        }
        if(!is_mini[3]){
            sf::Sprite s;
            t.generateMiniSprite(id, position, s, sf::Vector2f(GetWidth(),GetHeight()),0);
            renderWindow.draw(s);
        }
    }
}

void Tile::DrawGrass(sf::RenderWindow & renderWindow, TextureManager &t){
    sf::Sprite s;
    sf::Vector2f pos_grass(position.x,position.y-GetHeight()/2);
    t.generateSprite("grass0", pos_grass, s, sf::Vector2f(GetWidth(),GetHeight()));
    renderWindow.draw(s);
    if(neighbors[3]!=nullptr && (neighbors[3]->id !="D" || (neighbors[2] !=nullptr && neighbors[2]->neighbors[8]->id !="0"))){
        sf::Sprite s2;
        sf::Vector2f pos_grass2(position.x+GetWidth(),position.y-GetHeight()/2);
        t.generateSprite("grass1", pos_grass2, s2, sf::Vector2f(GetWidth(),GetHeight()));
        renderWindow.draw(s2);
    }
    if(neighbors[7]!=nullptr && (neighbors[7]->id !="D"|| (neighbors[0] !=nullptr && neighbors[0]->neighbors[8]->id !="0"))){
        sf::Sprite s2;
        sf::Vector2f pos_grass2(position.x,position.y+GetHeight()/2+5);
        t.generateSprite("grass1", pos_grass2, s2, sf::Vector2f(GetWidth(),GetHeight()));
        s2.setRotation(180);
        renderWindow.draw(s2);
    }
}
void Tile::Draw(sf::RenderWindow & renderWindow, TextureManager &t)
{

	if(layer==1){
		if(drawable()) {
			//sf::Sprite s = t.generateSprite(id, position);
			//_sprite.setTexture(t._image);
			//_sprite.setPosition(position.x, position.y);
            if(neighbors[8] !=nullptr && neighbors[8]->id != "0") neighbors[8]->Draw(renderWindow,t);
            DrawIns(renderWindow, t);
            DrawFadeOut(renderWindow, t);

		}
		else {

			sf::RectangleShape rec(sf::Vector2f(Chunk::TILE_SIZE,Chunk::TILE_SIZE));
			rec.setPosition(GetPosition());
			rec.setFillColor(sf::Color::Black);
			renderWindow.draw(rec);

		}
	}
	else {

        DrawIns(renderWindow, t);
        DrawAmbientOclusion(renderWindow, t);


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



