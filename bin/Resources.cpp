//
// Created by arnito on 8/07/17.
//

#include <iostream>
#include "Resources.h"
#include "Settings.h"

#define SHADERPATH "res/Shaders/"
#define TEXTURETPATH "res/textures/"
#define ANIMATIONPATH "res/Textures/Animations/"

std::map<std::string, sf::Font> Resources::fontsMap;
std::map<std::string, sf::Shader> Resources::shadersMap;
std::map<std::string, sf::Texture> Resources::texturesMap;
std::map<std::string, TextureManager> Resources::textureManagerMap;
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
    //FONTS
    addFont( "debugFont", "resources/font1.ttf");
    addFont( "font", "resources/font2.otf");
    //SHADERS
    addShader("sun_background_shader","resources/sun_background.frag");
    addShader("tile_shader","resources/light.frag");
    addShader("sun_shader","resources/sun.frag");
    addShader("sun_mix_shader","resources/sun_mix.frag");
    addShader("mix_back_terr_shader","resources/mix_background_terrain.frag");
    addShader("rain_shader","resources/rain.frag");
    //TEXTURES
    addTexture("cursors", "resources/cursors.png");
    addTexture("playerSprite", "resources/player.png");
    addTexture("entities", "resources/entities.png");
    addTexture("mobsBodies", "resources/mobsBodies.png");
    addTexture("mobsHeads", "resources/mobsHeads.png");
    addTexture("mobsHands", "resources/mobsHands.png");
    addTexture("mobsComplements", "resources/mobsComplements.png");
    addTexture("mobsLegs", "resources/mobsLegs.png");

    addTexture("clock_background", "resources/rellotge_background.png");
    addTexture("clock_clock", "resources/rellotge.png");
    addTexture("clock_circle", "resources/aro_rellotge.png");
    addTexture("clock_neddle_hour", "resources/agulla_hora.png");
    addTexture("clock_neddle_season", "resources/agulla_season.png");
    addTexture("clock_neddle_temp", "resources/agulla_temp.png");
    addTextureManager("tileMap","resources/tiles5.png", 16, 16);
    TextureManager *texMan = getTextureManager("tileMap");
    texMan->insert_block_all_values("D", "d", sf::Vector2i(0,0),16);     //dirt
    texMan->insert_block_all_values("C", "c", sf::Vector2i(0,16),16);    //cobblestone
    texMan->insert_block_all_values("K", "k", sf::Vector2i(0,32),16);    //cuper
    texMan->insert_block_all_values("G", "g", sf::Vector2i(0,48),16);    //gold
    texMan->insert_block_all_values("I", "i", sf::Vector2i(0,64),16);    //iron
    texMan->insert_block_all_values("L", "l", sf::Vector2i(0,80),16);    //coal
    texMan->insert_block_all_values("Y", "y", sf::Vector2i(0,96),16);    //diamond
    texMan->insert_block_all_values("B", "b", sf::Vector2i(0,112),16);    //bedrock
    texMan->insert_block_all_values("N", "n", sf::Vector2i(0,128),16);    //sand
    texMan->insert_block_all_values("W", "w", sf::Vector2i(0,144),16);    //snow
    texMan->insert_block_all_values("R", "r", sf::Vector2i(0,160),16);    //ice rock
    texMan->insert_block_all_values("J", "j", sf::Vector2i(0,176),16);    //jungle dirt



    texMan->insert_block_all_values("F", "f", sf::Vector2i(0,192),16);      //fulles
    texMan->insert_block_all_values("T", "t", sf::Vector2i(0,208),16);      //arbre
    texMan->insert_block_all_values("A", "a", sf::Vector2i(0,224),16);      //arbre

    texMan->insert_block_all_values("M", "m", sf::Vector2i(0,240),16);    //briks
    texMan->insert_block_all_values("P", "p", sf::Vector2i(0,256),16);    //Stone briks

    texMan->insert_map_value("0",sf::Vector2i(64,32));
    texMan->insert_map_value("s",sf::Vector2i(96,32));
    texMan->insert_map_value("S",sf::Vector2i(112,32));
    texMan->insert_map_value("S2",sf::Vector2i(128,32));

    texMan->insert_map_value("Sfull",sf::Vector2i(96,112));
    texMan->insert_map_value("Swall",sf::Vector2i(96,96));
    texMan->insert_map_value("Scorner",sf::Vector2i(112,96));

    texMan->insert_map_value("grass0",sf::Vector2i(96,48));
    texMan->insert_map_value("grass1",sf::Vector2i(112,48));
    texMan->insert_map_value("grassIce0",sf::Vector2i(96,64));
    texMan->insert_map_value("grassIce1",sf::Vector2i(112,64));
    texMan->insert_map_value("grassJungle0",sf::Vector2i(96,80));
    texMan->insert_map_value("grassJungle1",sf::Vector2i(112,80));

    texMan->insert_leaves_all_values("L1","l1",sf::Vector2i(128,48),16);
    texMan->insert_leaves_all_values("L2","l2",sf::Vector2i(128,64),16);
    texMan->insert_leaves_all_values("L3","l3",sf::Vector2i(128,80),16);

    texMan->insert_map_value("armor",sf::Vector2i(144,0));
    texMan->insert_map_value("armor1",sf::Vector2i(160,0));
    texMan->insert_map_value("armor2",sf::Vector2i(176,0));
    texMan->insert_map_value("armor3",sf::Vector2i(192,0));
    texMan->insert_map_value("tool",sf::Vector2i(144,16));
    texMan->insert_map_value("sword1",sf::Vector2i(160,16));
    texMan->insert_map_value("pickaxe1",sf::Vector2i(176,16));
    texMan->insert_map_value("pickaxe2",sf::Vector2i(192,16));
    texMan->insert_map_value("hammer",sf::Vector2i(208,16));
    texMan->insert_map_value("stairs",sf::Vector2i(144,32));
    texMan->insert_map_value("torch",sf::Vector2i(160,32));
    texMan->insert_map_value("food",sf::Vector2i(176,32));
    texMan->insert_map_value("heart",sf::Vector2i(192,32));
    texMan->insert_map_value("logs",sf::Vector2i(208,32));
    texMan->insert_map_value("fireplace",sf::Vector2i(224,32));
    texMan->insert_map_value("fireplaceCamp",sf::Vector2i(240,32));
    texMan->insert_map_value("picture",sf::Vector2i(224,16));
    texMan->insert_map_value("table",sf::Vector2i(224,0));
    texMan->insert_map_value("chair",sf::Vector2i(240,0));
    std::cout << " Resources Loaded " << std::endl;
}

void Resources::addTexture(const std::string& key, const std::string& path) {
    if(! texturesMap[key].loadFromFile(path)) printError(path);
}
void Resources::addTextureManager(const std::string& key, const std::string& path, int sizeX, int sizeY) {
    textureManagerMap.insert(std::pair<std::string, TextureManager>(key,TextureManager(path,sizeX, sizeY)));
    //textureManagerMap[key] = TextureManager(path,sizeX, sizeY);
}
void Resources::addAnimation(const std::string& key, const std::string& path, const std::string& name, int animationFrames, const std::string& extension = ".png") {
    for(int i = 0; i < animationFrames; ++i) {
        std::string currentPath = ANIMATIONPATH+path+name+std::to_string(i)+extension;
        if(! animationsMap[key][i].loadFromFile(currentPath)) printError(currentPath);
    }
}

void Resources::addShader(const std::string& key, const std::string& path) {
    if(sf::Shader::isAvailable()) {
        if (!shadersMap[key].loadFromFile(path, sf::Shader::Fragment)) {
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
TextureManager* Resources::getTextureManager(const std::string& key) {

    return &textureManagerMap.find(key)->second;
}

const std::vector<sf::Texture>& Resources::getAnimation(const std::string& key) {
    return animationsMap[key];
}

