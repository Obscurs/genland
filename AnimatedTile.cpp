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
#include "Map.h"
#include "Settings.h"


AnimatedTile::AnimatedTile(){

	colisionable = false;
    vx=0;
    vy=0;
    disp_factor = 0;
    rotation = 0;
    wall_left = false;
    wall_right = false;
    deleted=0;
}


AnimatedTile::~AnimatedTile()
{
	
}
void AnimatedTile::setFactor(float dist_x, float dist_y){
    if(wall_left && wall_right){
        if(dist_x>0) disp_factor = -(dist_x*5 + dist_y)/8;
        else disp_factor = -(dist_x*5-dist_y)/8;
    } else if(wall_left){
        if(dist_x>0) disp_factor = (dist_y)/8;
        else disp_factor = (dist_y)/8;
    } else if(wall_right){
        if(dist_x>0) disp_factor = (- dist_y)/8;
        else disp_factor = (-dist_y)/8;
    }
    else{
        if(dist_x>0) disp_factor = (dist_x*5 + dist_y)/8;
        else disp_factor = (dist_x*5-dist_y)/8;
    }


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
    if(deleted==0){
        sf::Sprite s;
        s.setOrigin(size.x/2-t.size_sprite.y/2, size.y/2-t.size_sprite.y/2);
        s.setRotation(rotation);
        sf::Vector2f pos_grass(position.x,position.y-GetHeight()/2);
        t.generateSprite(id, pos_grass, s, sf::Vector2f(GetWidth(),GetHeight()));
        //
        s.setPosition(s.getPosition().x+size.x/2, s.getPosition().y+size.y/2);
        //s.setPosition(position.x-size.x/2, position.y-size.y/2);
        renderWindow.draw(s);
    }
    else{

    }

}

bool AnimatedTile::ColideWorld(Chunk* c1,Chunk* c2,Chunk* c3, int posMap){
    sf::Vector2f center_pos(position.x+size.x/2, position.y+size.y/2);

        if(center_pos.y<0) center_pos.y = 0;
        int size_chunk_x = Chunk::N_TILES_X*Settings::TILE_SIZE;
        int chunk_x = (center_pos.x-size_chunk_x*posMap)/size_chunk_x;


        Chunk* c;
        if(chunk_x==0) c = c1;
        else if(chunk_x==1) c = c2;
        else c = c3;


        Tile* t =c->getTile(center_pos.x, center_pos.y, 1);
        if(t != nullptr && t->id !="0"){
            return true;

    }
}
void AnimatedTile::Update(float elapsedTime, Chunk* c1,Chunk* c2,Chunk* c3, int posMap)
{
    if(deleted==0){
        vx =disp_factor/10;
        vy = 1*elapsedTime*100 + vy;
        rotation += elapsedTime*disp_factor;
        float x0 = position.x;
        float y0 = position.y;

        float x = x0+vx*elapsedTime;
        float y = y0+vy*elapsedTime;
        SetPosition(x, y);
        if(ColideWorld(c1,c2,c3, posMap)){
            //std::cout << "YAY" << std::endl;
            deleted = 1;
        }
    }

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