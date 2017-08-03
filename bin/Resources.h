//
// Created by arnito on 8/07/17.
//

#ifndef GENLAND_RESOURCES_H
#define GENLAND_RESOURCES_H
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

class Resources {
public:
    static void load();

    static sf::Font* getFont(const std::string& key);
    static sf::Shader* getShader(const std::string& key);
    static sf::Texture* getTexture(const std::string& key);
    static TextureManager* getTextureManager(const std::string& key);
    static const std::vector <sf::Texture>& getAnimation(const std::string& key);

private:

    static std::map<std::string, sf::Font> fontsMap;
    static std::map<std::string, sf::Shader> shadersMap;
    static std::map<std::string, sf::Texture> texturesMap;
    static std::map<std::string, std::vector<sf::Texture> > animationsMap;
    static std::map<std::string, TextureManager> textureManagerMap;
    static void addFont(const std::string& key, const std::string& path);
    static void addTexture(const std::string& key, const std::string& path);
    static void addTextureManager(const std::string& key, const std::string& path, int sizeX, int sizeY);
    static void addShader(const std::string& key, const std::string& shader);
    static void addAnimation(const std::string& key, const std::string& path, const std::string& name, int animationFrames, const std::string& extension);
};



#endif //GENLAND_RESOURCES_H
