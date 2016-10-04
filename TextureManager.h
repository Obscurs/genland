#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

class TextureManager {
    public:
        TextureManager(std::string file, int size_sprite_x, int size_sprite_y);
        ~TextureManager();
        void insert_map_value(std::string k, sf::Vector2i v);
        void generateSprite(std::string key, sf::Vector2f sprite_pos, sf::Sprite &s, sf::Vector2f size);
        void generateMiniSprite(std::string key, sf::Vector2f sprite_pos, sf::Sprite &s, sf::Vector2f size, int mini_pos);
    sf::Vector2i getPositionSprite(std::string key);
    sf::Vector2u getSizeTex();
    //sf::Shader* getTileShader();
    std::string file_path;
    sf::Vector2i size_sprite;
    std::map<std::string, sf::Vector2i> texture_map;
    sf::Texture _image;
    //sf::Shader tile_shader;
    private:

};


