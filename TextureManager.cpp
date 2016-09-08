//
// Created by arnito on 8/09/16.
//
#include "TextureManager.h"

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
void TextureManager::generateSprite(std::string key, sf::Vector2f sprite_pos, sf::Sprite &s){
    sf::Vector2i tex_sprite_pos = texture_map[key];
    sf::IntRect r(tex_sprite_pos.x, tex_sprite_pos.y, size_sprite.x, size_sprite.y);
    //sf::Sprite sprite(_image,r);
    s.setTextureRect(r);
    s.setTexture(_image);
    s.setPosition(sprite_pos.x, sprite_pos.y);
}