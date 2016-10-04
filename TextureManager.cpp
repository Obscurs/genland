//
// Created by arnito on 8/09/16.
//
#include "TextureManager.h"
#include <iostream>
TextureManager::TextureManager(std::string file, int size_sprite_x, int size_sprite_y){
    file_path=file;
    size_sprite = sf::Vector2i(size_sprite_x, size_sprite_y);
    _image.loadFromFile(file_path);
    //if (!tile_shader.loadFromFile("resources/blur.frag", sf::Shader::Fragment)) std::cout<< "el shader no va" << std::endl;
}
TextureManager::~TextureManager()
{
}
void TextureManager::insert_map_value(std::string k, sf::Vector2i v){
    texture_map[k] = v;
}
sf::Vector2u TextureManager::getSizeTex(){
    return _image.getSize();
}
sf::Vector2i TextureManager::getPositionSprite(std::string key){
    return texture_map[key];
}
/*
sf::Shader* TextureManager::getTileShader(){
    return &tile_shader;
}
*/
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