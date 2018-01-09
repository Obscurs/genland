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
#include "../world/Map.h"
#include "../../Settings.h"
#include "../../Resources.h"
#include "../Scene.h"


AnimatedTile::AnimatedTile(std::string new_id,std::string id_pick): Entity("fallingTile"){
    id = new_id;
    _id_pick = id_pick;
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
bool AnimatedTile::collideWithRectangle(sf::FloatRect rect){
    sf::FloatRect tile(_position.x-_sizeCol.x,_position.y-_sizeCol.y,_sizeCol.x*4,_sizeCol.y*4);
    return rect.intersects(tile);
}
void AnimatedTile::Draw(sf::RenderTarget & renderWindow)
{
    TextureManager *t = Resources::getTextureManager("tileMap");
    if(deleted!=1){
        sf::Sprite s;
        s.setOrigin(_sizeCol.x/2-t->size_sprite.y/2, _sizeCol.y/2-t->size_sprite.y/2);
        s.setRotation(rotation);
        sf::Vector2f pos_grass(_position.x,_position.y-GetHeight()/2);
        t->generateSprite(id, pos_grass, s, sf::Vector2f(GetWidth(),GetHeight()));
        //
        s.setPosition(s.getPosition().x+_sizeCol.x/2, s.getPosition().y+_sizeCol.y/2);
        //s.setPosition(__position.x-_size.x/2, __position.y-_size.y/2);
        renderWindow.draw(s);
    }
    else{

    }

}

bool AnimatedTile::ColideWorld(){
    Scene *s = Scene::getScene();
    Map *m = s->getMap();
    sf::Vector2f center_pos(_positionCol.x+_sizeCol.x/2, _positionCol.y+_sizeCol.y/2);
        if(center_pos.y<0) center_pos.y = 0;
        Tile* t = m->getTile(center_pos.x, center_pos.y, 1);
        if(t != nullptr && t->id !="0"){
            return true;

    }
}
void AnimatedTile::Update(float elapsedTime)
{
    if(deleted==0){
        vx =disp_factor/10;
        vy = 1*elapsedTime*100 + vy;
        rotation += elapsedTime*disp_factor;
        float x0 = _position.x;
        float y0 = _position.y;

        float x = x0+vx*elapsedTime;
        float y = y0+vy*elapsedTime;
        if(!ColideWorld()) {
            //std::cout << "YAY" << std::endl;
            SetPosition(x, y);
            deleted = 2;
        }
    } else if(deleted==1) _removed = true;
    else if(deleted == 2 && !ColideWorld()) deleted = 0;

}

void AnimatedTile::SetPosition(float x, float y)
{

    _position.x=x;
    _position.y=y;
    _positionCol.x=x;
    _positionCol.y=y;
}
void AnimatedTile::SetSize(float x)
{

    _sizeCol.x=x;
    _sizeCol.y=x;
}

sf::Vector2f AnimatedTile::GetPosition() const
{
    /*
    if(_isLoaded)
    {
        return _sprite.getPosition();
    }*/
    return _position;
}

float AnimatedTile::GetHeight() const
{
    return _sizeCol.y;
    //return _sprite.getTexture()->getSize().y*_sprite.getScale().y;
}

float AnimatedTile::GetWidth() const
{
    return _sizeCol.x;
    //return _sprite.getTexture()->getSize().x*_sprite.getScale().x;
}