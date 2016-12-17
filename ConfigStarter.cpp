//
// Created by arnito on 17/12/16.
//

#include "ConfigStarter.h"
#include <iostream>

#include <dirent.h>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <string.h>
#include <algorithm>
#include "Settings.h"

inline bool exists_file (const std::string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}
ConfigStarter::ConfigStarter(){
    if(exists_file("config/config")) {
        std::ifstream myfile("config/config");
        myfile >> SCREEN_WIDTH >> SCREEN_HEIGHT;
        myfile.close();
        if(SCREEN_WIDTH > 1200){
            TILE_SIZE = TILE_SIZE_HIGH;
            GAME_WIDTH = GAME_WIDTH_HIGH;
            GAME_HEIGHT = GAME_HEIGHT_HIGH;
        } else if(SCREEN_WIDTH > 1000){
            TILE_SIZE = TILE_SIZE_MED;
            GAME_WIDTH = GAME_WIDTH_MED;
            GAME_HEIGHT = GAME_HEIGHT_MED;
        } else {
            TILE_SIZE = TILE_SIZE_LOW;
            GAME_WIDTH = GAME_WIDTH_LOW;
            GAME_HEIGHT = GAME_HEIGHT_LOW;
        }
    }
}
ConfigStarter::~ConfigStarter(){

}
