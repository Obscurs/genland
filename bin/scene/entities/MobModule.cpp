//
// Created by arnito on 25/10/17.
//

#include <iostream>
#include "MobModule.h"
#include "../../Resources.h"
#include <fstream>
#include <sstream>
MobModule::MobModule(std::string type, int id, sf::Vector2f offset, float localScale){
    sf::Texture *t;

    _idModule = id;
    _keyframe = 0;
    _offset = offset;
    _typeModule = type;
    if(_typeModule == "head"){
        t = Resources::getTexture("mobsHeads");
    } else if(_typeModule == "hand"){
        t = Resources::getTexture("mobsHands");
    } else if(_typeModule == "body"){
        t = Resources::getTexture("mobsBodies");
    } else if(_typeModule == "complement"){
        t = Resources::getTexture("mobsComplements");
    } else if(_typeModule == "leg" || _typeModule == "legBack" ){
        t = Resources::getTexture("mobsLegs");
    }
    _sprite.setTexture(*t);
    _localScale = localScale;
    sf::Vector2f _position(0,0);
    sf::Vector2f _sizeCol(MOB_SPRITE_SIZE,MOB_SPRITE_SIZE);
    sf::Vector2f _positionCol(0,0);
    _currentAnimation = nullptr;
    _sprite.setPosition(_position.x,_position.y);
    _spriteTime = 0;
    _initialized = false;
    _y_floor_die = 0;
    _die_anim_percent = 0;
    setAnimations();
}
MobModule::~MobModule(){

}
bool MobModule::die(float delta, float positionFloor){
    if(_positionCol.y+_sizeCol.y< positionFloor) _positionCol.y+=delta;
    else return true;
    return true;
}
void MobModule::addAnimation(std::string name, int frameIni, int numFrames){
    Animation *a = new Animation;
    a->a_name = name;
    a->a_numFrames = numFrames;
    a->a_frameIni = frameIni;
    _animations.push_back(a);
}
void MobModule::draw(sf::RenderTarget & renderTar){
    if(_initialized){
        _sprite.setTextureRect(sf::IntRect(MOB_SPRITE_SIZE*_keyframe,MOB_SPRITE_SIZE*_idModule,MOB_SPRITE_SIZE,MOB_SPRITE_SIZE));
        sf::Vector2f pos = _sprite.getPosition();
        float y_low = pos.y+_sizeCol.y;
        pos.y += std::abs((_y_floor_die-y_low)*_die_anim_percent);
        _sprite.setPosition(pos);
        renderTar.draw(_sprite);
    }

}
int MobModule::getIdModule() {
    return _idModule;
}
sf::FloatRect MobModule::getBoundingBox(sf::Vector2f position, float scale,int direction){
    float totalScale = scale*_localScale;
    float left;
    if(direction==0) {
        left = _offset.x*totalScale+position.x;
    }
    else {
        left = position.x-_offset.x*totalScale+(1-totalScale)*MOB_SPRITE_SIZE;
    }
    float top = _offset.y*totalScale+position.y;
    float width = MOB_SPRITE_SIZE*totalScale+left;
    float height = MOB_SPRITE_SIZE*totalScale+top;
    return sf::FloatRect(left,top,width,height);
}
bool MobModule::hasAnimation(std::string name){
    for(int i=0; i<_animations.size(); i++){
        if(_animations[i]->a_name==name) return true;
    }
    return false;
}
void MobModule::saveToFile(std::ofstream &myfile){
    myfile << _typeModule << " " << _idModule << " " << _offset.x << " " << _offset.y << " " <<_localScale << " ";
}
void MobModule::setAnimations(){

    if(_typeModule == "head"){
        addAnimation("idle",0,1);
        addAnimation("attacking",1,2);
    } else if(_typeModule == "hand"){
        addAnimation("idle",0,1);
        addAnimation("walking",1,3);
    } else if(_typeModule == "body"){
        addAnimation("idle",0,1);
    } else if(_typeModule == "complement"){
        addAnimation("idle",0,1);
    } else if(_typeModule == "leg"){
        addAnimation("idle",0,1);
        addAnimation("walking",1,3);
    } else if(_typeModule == "legBack"){
        addAnimation("idle",4,1);
        addAnimation("walking",5,3);
    }


}
sf::Vector2f MobModule::getOffset() {
    return _offset;
}
float MobModule::getScale() {
    return _localScale;
}
void MobModule::update(float delta, sf::Vector2f mobPosition, float scale, std::string currentAnimation, int direction){
    float final_scale = scale*_localScale;
    _spriteTime += delta;
    _initialized = true;
    if(direction==0) {
        _position = mobPosition+_offset*final_scale;
        _positionCol = mobPosition+_offset*final_scale;
        _sprite.setScale(sf::Vector2f(final_scale,final_scale));
        _sprite.setPosition(_position.x,_position.y);
        //_sprite.setPosition(x-(PLAYER_SPRITE_SIZE-PLAYER_WIDTH-PLAYER_WIDTH/2),y-(PLAYER_SPRITE_SIZE-PLAYER_HEIGHT));
    }
    else {
        _position = sf::Vector2f(mobPosition.x-final_scale*_offset.x+(1-final_scale)*MOB_SPRITE_SIZE,mobPosition.y+_offset.y*final_scale);
        _positionCol = sf::Vector2f(mobPosition.x-_offset.x*final_scale,mobPosition.y+_offset.y*final_scale);
        _sprite.setScale(sf::Vector2f(-final_scale,final_scale));
        _sprite.setPosition(_position.x+MOB_SPRITE_SIZE*final_scale,_position.y);
        //_sprite.setPosition(x-(PLAYER_SPRITE_SIZE-PLAYER_WIDTH-PLAYER_WIDTH/2)+64,y-(PLAYER_SPRITE_SIZE-PLAYER_HEIGHT));
    }

    _sizeCol = sf::Vector2i(MOB_SPRITE_SIZE*final_scale,MOB_SPRITE_SIZE*final_scale);
    if(_currentAnimation== nullptr || currentAnimation != _currentAnimation->a_name){
        bool changed = false;
        for(int i=0; i<_animations.size(); i++){
            if(_animations[i]->a_name == currentAnimation){
                _currentAnimation = _animations[i];
                _keyframe = _currentAnimation->a_frameIni;
                changed = true;
            }
        }
        if(!changed) std::cout << "wrowng animation" << std::endl;
    }
    if(_currentAnimation != nullptr){
        float maxtime = float(MODULE_SPRITE_MAX_TIME)/10.0f;
        if(_spriteTime > maxtime){
            _spriteTime -= maxtime;
            {
                _keyframe +=1;
                if(_keyframe >_currentAnimation->a_numFrames+_currentAnimation->a_frameIni-1){
                    _keyframe = _currentAnimation->a_frameIni;
                }
            }

        }
    } else std::cout << "null animation" << std::endl;

}