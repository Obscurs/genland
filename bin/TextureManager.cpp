//
// Created by arnito on 8/09/16.
//
#include "TextureManager.h"
#include <iostream>


TextureManager::TextureManager(std::string file, int size_sprite_x, int size_sprite_y){
    file_path=file;
    size_sprite = sf::Vector2i(size_sprite_x, size_sprite_y);
    _image.loadFromFile(file_path);
}
TextureManager::~TextureManager()
{
}

void TextureManager::insert_map_value(std::string k, sf::Vector2i v){
    texture_map[k] = v;
}
void TextureManager::insert_block_all_values(std::string k,std::string k2, sf::Vector2i v, int size){

    insert_map_value(k,sf::Vector2i(v.x,v.y));
    insert_map_value(k2,sf::Vector2i(v.x+3*size,v.y));

    insert_map_value((k+"_in"),sf::Vector2i(v.x+size,v.y));
    insert_map_value((k2+"_in"),sf::Vector2i(v.x+size*4,v.y));
    insert_map_value((k+"_out"),sf::Vector2i(v.x+size*2,v.y));
    insert_map_value((k2+"_out"),sf::Vector2i(v.x+size*5,v.y));
}
void TextureManager::insert_leaves_all_values(std::string k,std::string k2, sf::Vector2i v, int size){
    insert_map_value(k+"_1",sf::Vector2i(v.x,v.y));
    insert_map_value(k2+"_1",sf::Vector2i(v.x+4*size,v.y));
    insert_map_value(k+"_2",sf::Vector2i(v.x+size,v.y));
    insert_map_value(k2+"_2",sf::Vector2i(v.x+5*size,v.y));
    insert_map_value(k+"_3",sf::Vector2i(v.x+size*2,v.y));
    insert_map_value(k2+"_3",sf::Vector2i(v.x+6*size,v.y));
    insert_map_value(k+"_4",sf::Vector2i(v.x+size*3,v.y));
    insert_map_value(k2+"_4",sf::Vector2i(v.x+7*size,v.y));
}
sf::Vector2u TextureManager::getSizeTex(){
    return _image.getSize();
}
sf::Vector2i TextureManager::getPositionSprite(std::string key){
    return texture_map[key];
}

void TextureManager::generateSprite(std::string key, sf::Vector2f sprite_pos, sf::Sprite &s, sf::Vector2f size){
    sf::Vector2i tex_sprite_pos = texture_map[key];
    sf::IntRect r(tex_sprite_pos.x, tex_sprite_pos.y, size_sprite.x, size_sprite.y);
    //sf::Sprite sprite(_image,r);
    s.setTextureRect(r);
    s.setTexture(_image);
    s.setPosition(sprite_pos.x, sprite_pos.y);
    sf::Vector2f new_scale(size.x/size_sprite.x, size.y/size_sprite.y);
    s.setScale(new_scale);
}
const sf::Texture* TextureManager::getTexture(){
    return &_image;
}
void TextureManager::generateMiniSprite(std::string key, sf::Vector2f sprite_pos, sf::Sprite &s, sf::Vector2f size, int mini_pos){
    sf::Vector2i tex_sprite_pos = texture_map[key];
    //std::cout << tex_sprite_pos.x << " " << tex_sprite_pos.y << std::endl;
    if(mini_pos==1) {
        tex_sprite_pos.x+=size_sprite.x/2;
        sprite_pos.x+=size.x/2;
    }
    else if(mini_pos==2) {
        tex_sprite_pos.y+=size_sprite.y/2;
        sprite_pos.y+=size.y/2;
    }
    else if(mini_pos==3){
        tex_sprite_pos.x+=size_sprite.x/2;
        tex_sprite_pos.y+=size_sprite.y/2;
        sprite_pos.x+=size.x/2;
        sprite_pos.y+=size.y/2;
        //std::cout << tex_sprite_pos.x << " " << tex_sprite_pos.y << std::endl;
    }
    sf::IntRect r(tex_sprite_pos.x, tex_sprite_pos.y, size_sprite.x/2, size_sprite.y/2);
    //sf::Sprite sprite(_image,r);
    s.setTextureRect(r);
    s.setTexture(_image);
    s.setPosition(sprite_pos.x, sprite_pos.y);
    sf::Vector2f new_scale(size.x/size_sprite.x, size.y/size_sprite.y);
    s.setScale(new_scale);
}