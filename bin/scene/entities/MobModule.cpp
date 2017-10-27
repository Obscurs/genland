//
// Created by arnito on 25/10/17.
//

#include <iostream>
#include "MobModule.h"
#include "../../Resources.h"

MobModule::MobModule(std::string type, int id, sf::Vector2f offset, float localScale){
    sf::Texture *t = Resources::getTexture("mobs");
    _sprite.setTexture(*t);
    _idModule = id;
    _keyframe = 0;
    _offset = offset;
    _typeModule = type;
    _localScale = localScale;
    sf::Vector2f _position(0,0);
    sf::Vector2f _sizeCol(0,0);
    sf::Vector2f _positionCol(0,0);
    _currentAnimation = nullptr;
    _sprite.setPosition(_position.x,_position.y);
    _spriteTime = 0;
    _initialized = false;
}
MobModule::~MobModule(){

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
        renderTar.draw(_sprite);
    }

}
sf::FloatRect MobModule::getBoundingBox(sf::Vector2f position, float scale){
    float totalScale = scale*_localScale;
    float left = _offset.x*totalScale+position.x;
    float width = MOB_SPRITE_SIZE*totalScale+left;
    float top = _offset.y*totalScale+position.y;
    float height = MOB_SPRITE_SIZE*totalScale+top;
    return sf::FloatRect(left,top,width,height);
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