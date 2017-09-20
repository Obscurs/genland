//
// Created by arnito on 20/09/17.
//

#ifndef GENLAND_NOISEGENERATOR_H
#define GENLAND_NOISEGENERATOR_H
#include <iostream>
#include <random>
#include <map>
#include "world/Simplex2d.h"

class NoiseGenerator {
public:

    static void init(std::string seed);
    static Simplex2d* getNoise(const std::string& key);
private:
    static std::map<std::string, Simplex2d*> _noises;
};


#endif //GENLAND_NOISEGENERATOR_H
