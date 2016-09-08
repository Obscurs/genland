#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

class TextureManager {
    public:
        TextureManager(std::string file, int size_sprite_x, int size_sprite_y);
        ~TextureManager();
        void insert_map_value(std::string k, sf::Vector2i v);
        void generateSprite(std::string key, sf::Vector2f sprite_pos, sf::Sprite &s);


    std::string file_path;
    sf::Vector2i size_sprite;
    std::map<std::string, sf::Vector2i> texture_map;
    sf::Texture _image;
    private:

};


