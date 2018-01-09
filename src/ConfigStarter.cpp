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

}
ConfigStarter::~ConfigStarter(){

}
