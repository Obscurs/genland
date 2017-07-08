//
// Created by arnito on 8/07/17.
//

#include <iostream>
#include "Resources.h"

#define SHADERPATH "res/Shaders/"
#define TEXTURETPATH "res/textures/"
#define ANIMATIONPATH "res/Textures/Animations/"

std::map<std::string, sf::Font> Resources::fontsMap;
std::map<std::string, sf::Shader> Resources::shadersMap;
std::map<std::string, sf::Texture> Resources::texturesMap;
std::map<std::string, std::vector<sf::Texture> > Resources::animationsMap;

void printError(const std::string& s) {
    std::cerr << "Resources:: Error loading... " << s << std::endl;
}

void Resources::load() {

    static bool firstTimeLoadingResources = true;
    if(! firstTimeLoadingResources) {
        std::cout << "Already loaded, aborting load" << std::endl;
        return;
    }
    firstTimeLoadingResources = false;


    // addTexture( "keyTexture" , "key.png" );
    //addTexture("clockTable", "spritesheets/clock.png");
    //addTexture("bgMap", "map0_room.png");
    //addTexture("player", "player.png");

    // addAnimation( "jumpAnimation", "jump/", "jump_", 5);
    // addAnimation( "jumpAnimation", "jump/", "jump_", 5, ".jpg"); //if != .png


    addFont( "debugFont", "resources/font1.ttf");


    const std::string fragmentShader = \
    "uniform sampler2D texture;" \
    "void main()" \
    "{"  \
        "vec4 originalColor = texture2D(texture, gl_TexCoord[0].xy) * gl_Color;" \
        "gl_FragColor = vec4(originalColor.r+0.7,originalColor.g,originalColor.b,originalColor.a);"  \
    "}";
    addShader("BlinkShader", fragmentShader);

    std::cout << " Resources Loaded " << std::endl;
}

void Resources::addTexture(const std::string& key, const std::string& path) {
    if(! texturesMap[key].loadFromFile(TEXTURETPATH+path)) printError(path);
}

void Resources::addAnimation(const std::string& key, const std::string& path, const std::string& name, int animationFrames, const std::string& extension = ".png") {
    for(int i = 0; i < animationFrames; ++i) {
        std::string currentPath = ANIMATIONPATH+path+name+std::to_string(i)+extension;
        if(! animationsMap[key][i].loadFromFile(currentPath)) printError(currentPath);
    }
}

void Resources::addShader(const std::string& key, const std::string& shader) {
    if(sf::Shader::isAvailable()) {
        if (!shadersMap[key].loadFromMemory(shader, sf::Shader::Fragment)) {
            std::cout << "error loading shader " << key << std::endl;
        }
    }
}

void Resources::addFont(const std::string& key, const std::string& path) {
    if (!fontsMap[key].loadFromFile(path)) printError(path);
}

sf::Font* Resources::getFont(const std::string& key) {
    return &fontsMap[key];
}

sf::Shader* Resources::getShader(const std::string& key) {
    return &shadersMap[key];
}

sf::Texture* Resources::getTexture(const std::string& key) {
    return &texturesMap[key];
}

const std::vector<sf::Texture>& Resources::getAnimation(const std::string& key) {
    return animationsMap[key];
}

