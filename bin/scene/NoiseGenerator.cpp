//
// Created by arnito on 20/09/17.
//

#include "NoiseGenerator.h"

std::map<std::string, Simplex2d*> NoiseGenerator::_noises;
void NoiseGenerator::init(std::string sseed){
    _noises.clear();
    std::mt19937 generator;
    int seed = std::stoi(sseed);
    generator.seed(seed);
    Simplex2d* rainSimplex = new Simplex2d(&generator, 150.0f, 0.0f, 1.0f);
    Simplex2d* escarp = new Simplex2d(&generator, 1000.0f, 0.0f, 0.1f);
    generator.seed(seed+1);
    Simplex2d* altitud = new Simplex2d(&generator, 20000.0f, 0.2f, 0.5f);
    generator.seed(seed+2);
    Simplex2d* escarp_factor = new Simplex2d(&generator, 6000.0f, 0.0f, 1.0f);
    Simplex2d* noise_stone_to_dirt = new Simplex2d(&generator, 50.0f, -0.01f, 0.01f);
    Simplex2d* noise_stone_to_dirt2 = new Simplex2d(&generator, 500.0f, -0.02f, 0.02f);
    Simplex2d* noise_transition_materials3 = new Simplex2d(&generator, 200.0f, -1.0f, 1.0f);
    generator.seed(seed+3);
    Simplex2d* mount_factor = new Simplex2d(&generator, 10000.0f, -1.2f, 1.0f);
    generator.seed(seed+4);
    Simplex2d* mountains = new Simplex2d(&generator, 500.0f, 0.4f, 0.5f);
    generator.seed(seed+5);
    Simplex2d* base_noise_temperature = new Simplex2d(&generator, 25000.0f, -20, 30);
    generator.seed(seed+6);
    Simplex2d* noise_humidity = new Simplex2d(&generator, 25000.0f, 0.0f, 100.0f);
    generator.seed(seed+7);
    Simplex2d* noiseCave = new Simplex2d(&generator, 300.0f, 0.0f, 1.0f);
    generator.seed(seed+8);
    Simplex2d* caveFactor_x = new Simplex2d(&generator, 10000.0f, 0.5f, 1.5f);
    Simplex2d* caveFactor_y = new Simplex2d(&generator, 1000.0f, 0.5f, 1.5f);
    generator.seed(seed+9);
    Simplex2d* caveHeight = new Simplex2d(&generator, 1000.0f, -0.05f, 0.05f);
    generator.seed(seed+10);
    Simplex2d* noiseGold = new Simplex2d(&generator, 400.0f, 0.0f, 1.0f);
    generator.seed(seed+11);
    Simplex2d* noiseCoal = new Simplex2d(&generator, 300.0f, 0.0f, 1.0f);
    generator.seed(seed+12);
    Simplex2d* noiseIron = new Simplex2d(&generator, 200.0f, 0.0f, 1.0f);
    generator.seed(seed+13);
    Simplex2d* noiseCuper = new Simplex2d(&generator, 250.0f, 0.0f, 1.0f);
    generator.seed(seed+14);
    Simplex2d* noiseDiamond = new Simplex2d(&generator, 500.0f, 0.0f, 1.0f);
    _noises["rainSimplex"] = rainSimplex;
    _noises["escarp"] = escarp;
    _noises["altitud"] = altitud;
    _noises["escarp_factor"] = escarp_factor;
    _noises["noise_stone_to_dirt"] = noise_stone_to_dirt;
    _noises["noise_stone_to_dirt2"] = noise_stone_to_dirt2;
    _noises["noise_transition_materials3"] = noise_transition_materials3;
    _noises["mount_factor"] = mount_factor;
    _noises["mountains"] = mountains;
    _noises["base_noise_temperature"] = base_noise_temperature;
    _noises["noise_humidity"] = noise_humidity;
    _noises["noiseCave"] = noiseCave;
    _noises["caveFactor_x"] = caveFactor_x;
    _noises["caveFactor_y"] = caveFactor_y;
    _noises["caveHeight"] = caveHeight;
    _noises["noiseGold"] = noiseGold;
    _noises["noiseCoal"] = noiseCoal;
    _noises["noiseIron"] = noiseIron;
    _noises["noiseCuper"] = noiseCuper;
    _noises["noiseDiamond"] = noiseDiamond;

}
Simplex2d* NoiseGenerator::getNoise(const std::string& key) {
    return _noises[key];
}
