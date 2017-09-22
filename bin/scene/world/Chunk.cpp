#include <stdio.h>


// TODO: reference additional headers your program requires here
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>
#include <queue>
#include <cstring>
#include "Chunk.h"
#include "../../Settings.h"
#include "../../Debuger.h"
#include "../NoiseGenerator.h"


void Chunk::saveToFile(std::string path){
    std::string filename = path;

    filename.append("/map/");
    //std::string filename = "map/";
    filename.append(std::to_string(_chunk_id));
    filename.append(".txt");
    std::cout << filename << std::endl;
    //std::ofstream myfile(filename, std::ios::trunc);
    std::ofstream myfile;
    myfile.open (filename);


    for(int i = 0; i<N_TILES_Y; ++i){
        for(int j = 0; j<N_TILES_X; ++j){
            Tile* t0 = tile_mat[i][j][0];
            Tile* t1 = tile_mat[i][j][1];
            //std::cout << t0->id << std::endl;
            if(!t0->_isTree && t0->_leaveDensity =="") myfile << t0->id;
            else myfile << "0";
            if(!t1->_isTree && t1->_leaveDensity =="") myfile << t1->id;
            else myfile << "0";
        }
    }

    myfile.close();

}
void Chunk::setTileNeighbors(int index_x, int index_y){
    Tile* t0 = tile_mat[index_y][index_x][0];
    Tile* t1 = tile_mat[index_y][index_x][1];

    t0->neighbors[8] = t1;
    t1->neighbors[8] = t0;
    if(index_x-1<0){
        //ESQUERRA
        if(neighbors[0] != nullptr){
            if(index_y-1<0){
                t0->neighbors[0] = nullptr;
                t1->neighbors[0] = nullptr;
            } else {
                t0->neighbors[0] = neighbors[0]->tile_mat[index_y-1][Chunk::N_TILES_X-1][0];
                t1->neighbors[0] = neighbors[0]->tile_mat[index_y-1][Chunk::N_TILES_X-1][1];
            }
            if(index_y+1>=Chunk::N_TILES_Y){
                t0->neighbors[6] = nullptr;
                t1->neighbors[6] = nullptr;
            }
            else{
                t0->neighbors[6] = neighbors[0]->tile_mat[index_y+1][Chunk::N_TILES_X-1][0];
                t1->neighbors[6] = neighbors[0]->tile_mat[index_y+1][Chunk::N_TILES_X-1][1];
            }
            t0->neighbors[7] = neighbors[0]->tile_mat[index_y][Chunk::N_TILES_X-1][0];
            t1->neighbors[7] = neighbors[0]->tile_mat[index_y][Chunk::N_TILES_X-1][1];
        } else{
            t0->neighbors[0] = nullptr;
            t0->neighbors[6] = nullptr;
            t0->neighbors[7] = nullptr;
            t1->neighbors[0] = nullptr;
            t1->neighbors[6] = nullptr;
            t1->neighbors[7] = nullptr;
        }
        //CENTRE I DRETA
        if(index_y-1<0){
            t0->neighbors[1] = nullptr;
            t1->neighbors[1] = nullptr;
            t0->neighbors[2] = nullptr;
            t1->neighbors[2] = nullptr;
        } else {
            t0->neighbors[1] = tile_mat[index_y-1][index_x][0];
            t1->neighbors[1] = tile_mat[index_y-1][index_x][1];
            t0->neighbors[2] = tile_mat[index_y-1][index_x+1][0];
            t1->neighbors[2] = tile_mat[index_y-1][index_x+1][1];
        }
        if(index_y+1>=Chunk::N_TILES_Y){
            t0->neighbors[4] = nullptr;
            t1->neighbors[4] = nullptr;
            t0->neighbors[5] = nullptr;
            t1->neighbors[5] = nullptr;
        }
        else{
            t0->neighbors[4] = tile_mat[index_y+1][index_x+1][0];
            t1->neighbors[4] = tile_mat[index_y+1][index_x+1][1];
            t0->neighbors[5] = tile_mat[index_y+1][index_x][0];
            t1->neighbors[5] = tile_mat[index_y+1][index_x][1];
        }
        t0->neighbors[3] = tile_mat[index_y][index_x+1][0];
        t1->neighbors[3] = tile_mat[index_y][index_x+1][1];

    } else if(index_x+1>=Chunk::N_TILES_X){
        //DRETA
        if(neighbors[1] != nullptr){
            if(index_y-1<0){
                t0->neighbors[2] = nullptr;
                t1->neighbors[2] = nullptr;
            } else {
                t0->neighbors[2] = neighbors[1]->tile_mat[index_y-1][0][0];
                t1->neighbors[2] = neighbors[1]->tile_mat[index_y-1][0][1];
            }
            if(index_y+1>=Chunk::N_TILES_Y){
                t0->neighbors[4] = nullptr;
                t1->neighbors[4] = nullptr;
            }
            else{
                t0->neighbors[4] = neighbors[1]->tile_mat[index_y+1][0][0];
                t1->neighbors[4] = neighbors[1]->tile_mat[index_y+1][0][1];
            }
            t0->neighbors[3] = neighbors[1]->tile_mat[index_y][0][0];
            t1->neighbors[3] = neighbors[1]->tile_mat[index_y][0][1];
        } else{
            t0->neighbors[2] = nullptr;
            t0->neighbors[3] = nullptr;
            t0->neighbors[4] = nullptr;
            t1->neighbors[2] = nullptr;
            t1->neighbors[3] = nullptr;
            t1->neighbors[4] = nullptr;
        }
        //CENTRE I ESQUERRA
        if(index_y-1<0){
            t0->neighbors[1] = nullptr;
            t1->neighbors[1] = nullptr;
            t0->neighbors[0] = nullptr;
            t1->neighbors[0] = nullptr;
        } else {
            t0->neighbors[1] = tile_mat[index_y-1][index_x][0];
            t1->neighbors[1] = tile_mat[index_y-1][index_x][1];
            t0->neighbors[0] = tile_mat[index_y-1][index_x-1][0];
            t1->neighbors[0] = tile_mat[index_y-1][index_x-1][1];
        }
        if(index_y+1>=Chunk::N_TILES_Y){
            t0->neighbors[6] = nullptr;
            t1->neighbors[6] = nullptr;
            t0->neighbors[5] = nullptr;
            t1->neighbors[5] = nullptr;
        }
        else{
            t0->neighbors[6] = tile_mat[index_y+1][index_x-1][0];
            t1->neighbors[6] = tile_mat[index_y+1][index_x-1][1];
            t0->neighbors[5] = tile_mat[index_y+1][index_x][0];
            t1->neighbors[5] = tile_mat[index_y+1][index_x][1];
        }
        t0->neighbors[7] = tile_mat[index_y][index_x-1][0];
        t1->neighbors[7] = tile_mat[index_y][index_x-1][1];
    } else {

        if(index_y-1<0){
            t0->neighbors[2] = nullptr;
            t1->neighbors[2] = nullptr;
            t0->neighbors[1] = nullptr;
            t1->neighbors[1] = nullptr;
            t0->neighbors[0] = nullptr;
            t1->neighbors[0] = nullptr;
        } else {
            t0->neighbors[2] = tile_mat[index_y-1][index_x+1][0];
            t1->neighbors[2] = tile_mat[index_y-1][index_x+1][1];
            t0->neighbors[1] = tile_mat[index_y-1][index_x][0];
            t1->neighbors[1] = tile_mat[index_y-1][index_x][1];
            t0->neighbors[0] = tile_mat[index_y-1][index_x-1][0];
            t1->neighbors[0] = tile_mat[index_y-1][index_x-1][1];
        }
        if(index_y+1>=Chunk::N_TILES_Y){
            t0->neighbors[6] = nullptr;
            t1->neighbors[6] = nullptr;
            t0->neighbors[5] = nullptr;
            t1->neighbors[5] = nullptr;
            t0->neighbors[4] = nullptr;
            t1->neighbors[4] = nullptr;
        }
        else{
            t0->neighbors[6] = tile_mat[index_y+1][index_x-1][0];
            t1->neighbors[6] = tile_mat[index_y+1][index_x-1][1];
            t0->neighbors[5] = tile_mat[index_y+1][index_x][0];
            t1->neighbors[5] = tile_mat[index_y+1][index_x][1];
            t0->neighbors[4] = tile_mat[index_y+1][index_x+1][0];
            t1->neighbors[4] = tile_mat[index_y+1][index_x+1][1];
        }
        t0->neighbors[7] = tile_mat[index_y][index_x-1][0];
        t1->neighbors[7] = tile_mat[index_y][index_x-1][1];
        t0->neighbors[3] = tile_mat[index_y][index_x+1][0];
        t1->neighbors[3] = tile_mat[index_y][index_x+1][1];
    }
    //posem a visible els bordes de la llum
    if(t0->id=="0" && t1->id=="0"){
        for(int i = 0; i<9; i++){
            //if(t0->neighbors[i] != nullptr && t0->neighbors[i]->id != "0") t0->neighbors[i]->reach_sun=true;
            //if(t1->neighbors[i] != nullptr && t1->neighbors[i]->id != "0") t1->neighbors[i]->reach_sun=true;
        }
    }

}
Chunk::Chunk(int pos, std::ofstream &myfile):
        render_array(sf::Quads , (uint)(4)),
        sky_array(sf::Quads , (uint)(4)),
        _surfacePosition{std::pair<int,std::string>(0,"0")}
{
    _need_sync = false;
    _is_dirty = true;
    _chunk_id = pos;

    Simplex2d* escarp = NoiseGenerator::getNoise("escarp");
    Simplex2d* altitud = NoiseGenerator::getNoise("altitud");
    Simplex2d* escarp_factor =NoiseGenerator::getNoise("escarp_factor");
    Simplex2d* noise_stone_to_dirt = NoiseGenerator::getNoise("noise_stone_to_dirt");
    Simplex2d* noise_stone_to_dirt2 = NoiseGenerator::getNoise("noise_stone_to_dirt2");
    Simplex2d* noise_transition_materials3 = NoiseGenerator::getNoise("noise_transition_materials3");
    Simplex2d* mount_factor = NoiseGenerator::getNoise("mount_factor");
    Simplex2d* mountains = NoiseGenerator::getNoise("mountains");
    Simplex2d* base_noise_temperature = NoiseGenerator::getNoise("base_noise_temperature");
    Simplex2d* noise_humidity = NoiseGenerator::getNoise("noise_humidity");
    Simplex2d* noiseCave = NoiseGenerator::getNoise("noiseCave");
    Simplex2d* caveFactor_x = NoiseGenerator::getNoise("caveFactor_x");
    Simplex2d* caveFactor_y = NoiseGenerator::getNoise("caveFactor_y");
    Simplex2d* caveHeight = NoiseGenerator::getNoise("caveHeight");
    Simplex2d* noiseGold = NoiseGenerator::getNoise("noiseGold");
    Simplex2d* noiseCoal = NoiseGenerator::getNoise("noiseCoal");
    Simplex2d* noiseIron = NoiseGenerator::getNoise("noiseIron");
    Simplex2d* noiseCuper = NoiseGenerator::getNoise("noiseCuper");
    Simplex2d* noiseDiamond = NoiseGenerator::getNoise("noiseDiamond");

    for(int i = 0; i<N_TILES_Y; ++i){
        for(int j = 0; j<N_TILES_X; ++j){
            int y_pos = N_TILES_Y-1-i;
            float current_global_x = _chunk_id*N_TILES_X*Settings::TILE_SIZE_HIGH+j*Settings::TILE_SIZE_HIGH;
            float current_global_y = y_pos*Settings::TILE_SIZE_HIGH;
            float height_factor = float(y_pos)/float(N_TILES_Y);
            float valFloor = altitud->valSimplex2D(0, current_global_x);
            float valEscarpAux = escarp->valSimplex2D(0, current_global_x);
            float valEscarpFact = escarp_factor->valSimplex2D(0, current_global_x);
            float valPlains = valFloor+(valEscarpAux*valEscarpFact);

            float valTransition = noise_transition_materials3->valSimplex2D(0, current_global_x);

            float mountains_noise = mountains->valSimplex2D(0, current_global_x);
            float mountains_factor = mount_factor->valSimplex2D(0, current_global_x);
            mountains_factor = std::max(float(0.0), mountains_factor);
            float valMountains = mountains_factor*mountains_noise;
            float valTerrain = valMountains+valPlains;

            float valCaveHeight = caveHeight->valSimplex2D(0, current_global_x);
            float valTerrainCaves = valTerrain+valCaveHeight-0.04;
            bool valHeightCaveMax = height_factor < valTerrainCaves;
            //float valFloor2 = sim2->valSimplex2D(0, current_global_x);
            ////float valReal1 = ((float) current_global_y/3000.0f > (valFloor+valFloor2)? 1 : 0);

            bool valHeightMax = height_factor < valTerrain;
            float valNoiseStoneDirt = noise_stone_to_dirt->valSimplex2D(0, current_global_x)+noise_stone_to_dirt2->valSimplex2D(0, current_global_x);
            float valHeightStone = (height_factor < (valTerrain+valNoiseStoneDirt+-0.04*(1-(mountains_factor*1.6+(1-height_factor)/2)))? 1 : 0);
            float valCave = noiseCave->valSimplex2D(current_global_y, current_global_x);
            float valCaveFactorX = caveFactor_x->valSimplex2D(current_global_y, current_global_x);
            float valCaveFactorY = caveFactor_y->valSimplex2D(current_global_y, current_global_x);
            bool isCave = valCave>0.7*(valCaveFactorX+valCaveFactorY)/2 && valHeightCaveMax;


            bool isGold = noiseGold->valSimplex2D(current_global_y, current_global_x)*(std::min(1.2f-height_factor,1.0f)) >0.93;
            isGold = isGold || noiseGold->valSimplex2D(current_global_y, current_global_x)*(std::min(0.4f+mountains_factor,1.0f)) >0.93;
            bool isIron = noiseIron->valSimplex2D(current_global_y, current_global_x) >0.88;
            bool isCuper = noiseCuper->valSimplex2D(current_global_y, current_global_x) >0.88;
            bool isDiamond = (noiseDiamond->valSimplex2D(current_global_y, current_global_x) >0.96)&& height_factor<0.2;
            bool isCoal = noiseCoal->valSimplex2D(current_global_y, current_global_x) >0.85;
            //float valStone = simStone->valSimplex2D(current_global_y, current_global_x);


            //float valReala = ((float) current_global_y/3000.0f > valFloor? 1 : 0);
            //std::cout << valFloor << std::endl;

            int valHumidity = int(noise_humidity->valSimplex2D(0, current_global_x));
            float heightTemp = (1-height_factor)*(Settings::MAX_TEMPERATURE-Settings::MIN_TEMPERATURE)+Settings::MIN_TEMPERATURE;
            int valTemperature = int(heightTemp)+int(base_noise_temperature->valSimplex2D(0, current_global_x));

            Tile* t = new Tile(0);
            Tile* t2 = new Tile(1);
            t->_humidity = valHumidity;
            t2->_humidity = valHumidity;
            t2->_temperature = valTemperature;
            t->_temperature = valTemperature;
            t->_mountain_factor = mountains_factor;
            t2->_mountain_factor = mountains_factor;

            Tile::Bioma bioma = Tile::STANDARD;
            if(mountains_factor>0) {
                if(valTemperature+valTransition*2 <0) bioma = Tile::ICE_MOUNTAIN;
                else bioma = Tile::MOUNTAIN;
            }
            else{
                if(valTemperature+valTransition*2 >30){
                    if(valHumidity+valTransition*2 >70) bioma = Tile::JUNGLE;
                    else if(valHumidity+valTransition*2 <40) bioma = Tile::DESERT;
                }
                else{
                    if(valHumidity+valTransition*2 >70) bioma = Tile::FOREST;
                    else if(valHumidity+valTransition*2 <40) bioma = Tile::PLAINS;
                }
            }

            //texMan.insert_block_all_values("K", "k", sf::Vector2i(0,32),32);    //cuper
            //texMan.insert_block_all_values("G", "g", sf::Vector2i(0,48),48);    //gold
            //texMan.insert_block_all_values("I", "i", sf::Vector2i(0,64),64);    //iron
            //texMan.insert_block_all_values("L", "l", sf::Vector2i(0,80),80);    //coal
            //texMan.insert_block_all_values("Y", "y", sf::Vector2i(0,96),96);    //diamond

            t->_bio = bioma;
            t2->_bio = bioma;
            if(i==N_TILES_Y-1){
                t->reload("b");
                t2->reload("B");
            }
            else {
                if (valHeightMax) {

                    if (valHeightStone) {
                        if(isDiamond) t->reload("Y");
                        else if(isGold)t->reload("g");
                        else if(isCoal)t->reload("l");
                        else if(isIron)t->reload("i");
                        else if(isCuper)t->reload("k");
                        else{
                            if (t->_bio == Tile::ICE_MOUNTAIN) t->reload("r");
                            else t->reload("c");
                        }
                    }
                    else {
                        if (t->_bio == Tile::DESERT) t->reload("n");
                        else if (t->_bio == Tile::JUNGLE) t->reload("j");
                        else if (t->_temperature < -5) t->reload("w");
                        else t->reload("d");
                    }
                    t->reach_floor = true;

                } else {
                    t->reload("0");
                    t->reach_floor = false;
                }

                if (valHeightMax) {
                    if (isCave) {
                        t2->reload("0");
                    } else {
                        if (valHeightStone) {
                            if(isDiamond) t2->reload("Y");
                            else if(isGold)t2->reload("g");
                            else if(isCoal)t2->reload("l");
                            else if(isIron)t2->reload("i");
                            else if(isCuper)t2->reload("k");
                            else{
                                if (t2->_bio == Tile::ICE_MOUNTAIN) t2->reload("R");
                                else t2->reload("C");
                            }
                        }
                        else {
                            if (t2->_bio == Tile::DESERT) t2->reload("N");
                            else if (t2->_bio == Tile::JUNGLE) t2->reload("J");
                            else if (t2->_temperature < -5) t2->reload("W");
                            else t2->reload("D");
                        }
                    }

                    t2->reach_floor = true;
                } else {
                    t2->reload("0");
                    t2->reach_floor = false;
                }
            }


            t->setPosition(_chunk_id * Settings::TILE_SIZE * N_TILES_X + j * Settings::TILE_SIZE,
                           i * Settings::TILE_SIZE);
            t->setSize(Settings::TILE_SIZE);
            t2->setPosition(_chunk_id * Settings::TILE_SIZE * N_TILES_X + j * Settings::TILE_SIZE,
                            i * Settings::TILE_SIZE);
            t2->setSize(Settings::TILE_SIZE);

            if((_surfacePosition[j].first == i-1 && t->id == "0" && t2->id == "0") || i == 0){
                t->_reach_sun = true;
                t2->_reach_sun = true;
                _surfacePosition[j].first = i;
                //_surfacePosition[j].second = t->neighbors[5]->id;
            }
            tile_mat[i][j][0] = t;
            tile_mat[i][j][1] = t2;
            myfile << t->id;
            myfile << t2->id;
            //std::cout << (valReal > 0 ? "X " : "  ");
            //SETEJEM ELS VEINS DE CADA TILE DEL CHUNK MENYS LES COLUMNES EXTERIORS
            if(j > 1 && i > 0){
                setTileNeighbors(j-1,i-1);
            }
            if(i==Chunk::N_TILES_Y-1){
                setTileNeighbors(j-1,i);
            }
        }
    }
    recalcReachFloor();

	
}

Chunk::Chunk(int pos, std::ifstream &myfile):
        render_array(sf::Quads , (uint)(4)),
        sky_array(sf::Quads , (uint)(4)),
        _surfacePosition{std::pair<int,std::string>(0,"0")}
{
    _need_sync = false;
    _is_dirty = true;
    _chunk_id = pos;
    std::cout << "creat" << _chunk_id << std::endl;
    typedef std::istreambuf_iterator<char> buf_iter;



    Simplex2d* mount_factor = NoiseGenerator::getNoise("mount_factor");
    Simplex2d* base_noise_temperature = NoiseGenerator::getNoise("base_noise_temperature");
    Simplex2d* noise_humidity = NoiseGenerator::getNoise("noise_humidity");

    buf_iter k(myfile), e;
    for(int i = 0; i<N_TILES_Y; ++i){
        for(int j = 0; j<N_TILES_X; ++j){
            int y_pos = N_TILES_Y-1-i;
            float current_global_x = _chunk_id*N_TILES_X*Settings::TILE_SIZE_HIGH+j*Settings::TILE_SIZE_HIGH;
            float current_global_y = y_pos*Settings::TILE_SIZE_HIGH;
            float height_factor = float(y_pos)/float(N_TILES_Y);

            Tile* t = new Tile(0);
            Tile* t2 = new Tile(1);
            char c1 = *k;
            ++k;
            char c2 = *k;
            ++k;
            //myfile.get(c1);
            //myfile.get(c2);
            t->reload(std::string(1, c1));
            t2->reload(std::string(1, c2));
            t->reach_floor = true;
            t2->reach_floor = true;


            float mountains_factor = mount_factor->valSimplex2D(0, current_global_x);
            mountains_factor = std::max(float(0.0), mountains_factor);
            int valHumidity = int(noise_humidity->valSimplex2D(0, current_global_x));
            float heightTemp = (1-height_factor)*(Settings::MAX_TEMPERATURE-Settings::MIN_TEMPERATURE)+Settings::MIN_TEMPERATURE;
            int valTemperature = int(heightTemp)+int(base_noise_temperature->valSimplex2D(0, current_global_x));
            t->_humidity = valHumidity;
            t2->_humidity = valHumidity;
            t2->_temperature = valTemperature;
            t->_temperature = valTemperature;
            t->_mountain_factor = mountains_factor;
            t2->_mountain_factor = mountains_factor;
            //if(t->id == "0") t->reach_sun=true;


            t->setPosition(_chunk_id * Settings::TILE_SIZE * N_TILES_X + j * Settings::TILE_SIZE,
                           i * Settings::TILE_SIZE);
            t->setSize(Settings::TILE_SIZE);
            t2->setPosition(_chunk_id * Settings::TILE_SIZE * N_TILES_X + j * Settings::TILE_SIZE,
                            i * Settings::TILE_SIZE);
            t2->setSize(Settings::TILE_SIZE);

            if((_surfacePosition[j].first == i-1 && t->id == "0" && t2->id == "0") || i == 0){
                t->_reach_sun = true;
                t2->_reach_sun = true;
                _surfacePosition[j].first = i;
            }

            tile_mat[i][j][0] = t;
            tile_mat[i][j][1] = t2;
            //std::cout << (valReal > 0 ? "X " : "  ");
            //SETEJEM ELS VEINS DE CADA TILE DEL CHUNK MENYS LES COLUMNES EXTERIORS
            if(j > 1 && i > 0){
                setTileNeighbors(j-1,i-1);
            }
            if(i==Chunk::N_TILES_Y-1){
                setTileNeighbors(j-1,i);
            }
        }
    }
    recalcReachFloor();

}

Chunk::~Chunk()
{
	//std::for_each(_gameObjects.begin(),_gameObjects.end(),GameObjectDeallocator());
}
void Chunk::recalcReachFloor(){
    for(int i= N_TILES_Y-1; i>=0; --i){
        for(int j=0; j<N_TILES_X; j++){
            if(i == (N_TILES_Y-1)){
                tile_mat[i][j][0]->rigid=true;
                tile_mat[i][j][1]->rigid=true;
                tile_mat[i][j][0]->reach_floor=true;
                tile_mat[i][j][1]->reach_floor=true;
            } else {
                Tile* t0 = tile_mat[i][j][0];
                Tile* t1 = tile_mat[i][j][1];

                tile_mat[i][j][0]->reach_floor = (t0->id!="0" &&(tile_mat[i+1][j][0]->reach_floor || tile_mat[i+1][j][1]->reach_floor));
                tile_mat[i][j][1]->reach_floor = (t1->id!="0" &&(tile_mat[i+1][j][1]->reach_floor || tile_mat[i+1][j][0]->reach_floor));

            }
        }
    }
}

void Chunk::calcLateralNeighborsTiles(int lateral){
    int x;
    if(lateral==1) x=Chunk::N_TILES_X-1;
    else x = 0;
    for(int y=0; y<Chunk::N_TILES_Y; y++){
        //std::cout << tile_mat[y][x][1]->id_temp << " set"<< std::endl;
        setTileNeighbors(x,y);
    }
}

Tile* Chunk::getTile(float x, float y, int z){
    sf::Vector2i position = getIndexFromGlobalPosition(sf::Vector2f(x,y));
    return tile_mat[position.y][position.x][z];
}
sf::Vector2i Chunk::getIndexFromGlobalPosition(sf::Vector2f pos){
    int size_chunk_x = Chunk::N_TILES_X*Settings::TILE_SIZE;
    int size_chunk_y = Chunk::N_TILES_Y*Settings::TILE_SIZE;
    int tile_x, tile_y;
    if(pos.x>=0){
        tile_x = ((int)pos.x%size_chunk_x)/Settings::TILE_SIZE;
        tile_y = ((int)pos.y%size_chunk_y)/Settings::TILE_SIZE;
    }
    else{
        tile_x = (((int)floorf(pos.x)%size_chunk_x)+size_chunk_x)/Settings::TILE_SIZE;
        tile_y = ((int)floorf(pos.y)%size_chunk_y)/Settings::TILE_SIZE;
        if((int)floorf(pos.x)%size_chunk_x==0) tile_x = 0;
    }
    return sf::Vector2i(abs(tile_x),abs(tile_y));
}
void Chunk::debugDraw(sf::RenderTarget &target, const std::string keyDebug, sf::Text &text){
    if(keyDebug=="linesChunks"){
        sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(getChunkPos(), 0)),
                sf::Vertex(sf::Vector2f(getChunkPos(), Settings::TILE_SIZE*N_TILES_Y))
        };
        target.draw(line, 2, sf::Lines);
    }
    else {
        for (int i = 0; i < Chunk::N_TILES_Y; ++i) {
            for (int j = 0; j < Chunk::N_TILES_X; ++j) {
                for(int k = 0; k < Chunk::N_TILES_Z; ++k){
                    Tile *t1 = tile_mat[i][j][k];
                    t1->debugTile(target, keyDebug, text,_chunk_id, sf::Vector2i(i,j));
                }
            }
        }
    }
}
float Chunk::getChunkPos(){
    return _chunk_id*Settings::TILE_SIZE*N_TILES_X;
}
Tile* Chunk::getTileByIndex(int x, int y, int z){
    return tile_mat[y][x][z];
}

sf::Vector2i Chunk::getTileIndex(float x, float y){
	//if(x<0) x = 0;
	//if(y<0) y = 0;

    if(x>=0) {
        int size_chunk_x = Chunk::N_TILES_X * Settings::TILE_SIZE;
        int size_chunk_y = Chunk::N_TILES_Y * Settings::TILE_SIZE;
        int tile_x = ((int) x % size_chunk_x) / Settings::TILE_SIZE;
        int tile_y = ((int) y % size_chunk_y) / Settings::TILE_SIZE;
        if(tile_x == Chunk::N_TILES_X) tile_x = 0;
        return sf::Vector2i(abs(tile_y), abs(tile_x));
    }
    else{
        int size_chunk_x = Chunk::N_TILES_X * Settings::TILE_SIZE;
        int size_chunk_y = Chunk::N_TILES_Y * Settings::TILE_SIZE;
        int tile_x = (((int) floorf(x) % size_chunk_x)+size_chunk_x) / Settings::TILE_SIZE;
        int tile_y = (((int) floorf(y) % size_chunk_y)) / Settings::TILE_SIZE;
        if(tile_x == Chunk::N_TILES_X) tile_x = 0;
        return sf::Vector2i(abs(tile_y), abs(tile_x));
    }
}

void Chunk::drawGrassTiles()
{
    for(int i=0; i<grass_tiles.size(); i++){
        grass_tiles[i]->drawGrass(render_array);
    }
}

void Chunk::update(float delta){
    if(_is_dirty){
        checkTreeTiles();
        prepareArrays();
        syncSurfaceAndUnderground();
        _is_dirty = false;
    }
    else if(_need_sync) syncSurfaceAndUnderground();
}
void Chunk::syncSurfaceAndUnderground(){
    Scene *s = Scene::getScene();
    sf::Vector2i interval = s->getIntervalEcosystem(_chunk_id);
    if(interval.x != interval.y){
        std::vector<std::vector<std::pair<int, bool> > >* surface =s->getSurface(interval);
        std::vector<std::vector<std::vector<int> > >* underground = s->getUnderground(interval);
        int index = _chunk_id-interval.x;

        for(int i = 0; i < Chunk::N_TILES_X; i++){
            bool is_sky = true;
            bool is_cave = true;
            (*underground)[index][i].clear();
            for(int j = 0; j< Chunk::N_TILES_Y; j++){
                if(tile_mat[j][i][0]->id != "0" && is_sky){
                    Tile *t = tile_mat[j][i][0];
                    (*surface)[index][i].first = j;
                    (*surface)[index][i].second = t->isDirt();
                    is_sky = false;
                } else if(!is_sky && tile_mat[j][i][1]->id != "0" && is_cave){
                    (*underground)[index][i].push_back(j);
                }
                is_cave = tile_mat[j][i][1]->id == "0";
            }
        }
        _need_sync = false;
    }
    else{
        _need_sync = true;
    }
}
void Chunk::checkTreeTiles(){
    int size = int(_trees.size());
    for(int i = 0; i<size; i++){
        _trees[i]->checkTreeTiles();
        //if(_trees[i]->isDead()){
            //Tree *tl = _trees[i]->getLeftTree();
            //Tree *tr = _trees[i]->getRightTree();
            //if(tl != nullptr) tl->setRightTree(tr);
            //if(tr != nullptr) tr->setLeftTree(tl);
        //    _trees.erase(_trees.begin()+i);
        //    i--;
        //    size = int(_trees.size());
        //}
    }
    
}
void Chunk::prepareArrays(){
    grass_tiles.clear();
    render_array.clear();
    sky_array.clear();
    memset(_surfacePosition,0,sizeof(_surfacePosition));
    for(int i = 0; i<Chunk::N_TILES_Y; ++i){
        for(int j = 0; j<Chunk::N_TILES_X; ++j){

            Tile* t1 = tile_mat[i][j][1];
            if((_surfacePosition[j].first == i-1 && t1->id == "0" && t1->neighbors[8]->id == "0" )|| i==0){
                t1->_reach_sun = true;
                t1->neighbors[8]->_reach_sun = true;
                _surfacePosition[j].first = i;
                _surfacePosition[j].second = t1->neighbors[5]->id;
            } else {
                t1->_reach_sun = false;
                t1->neighbors[8]->_reach_sun = false;
            }
            if(t1->id !="0"){
                t1->draw(render_array);
                if(t1->neighbors[1] != nullptr && t1->neighbors[1]->neighbors[8] != nullptr){
                    if(t1->neighbors[1]->id=="0" && t1->neighbors[1]->neighbors[8]->id =="0" && (t1->id=="D" ||t1->id=="J"||t1->id=="W")) grass_tiles.push_back(t1);
                }
                if(t1->isVisibleSun())t1->drawSkyArray(sky_array);
            }

            else{
                Tile* t0 = tile_mat[i][j][0];

                if(t0->isVisibleSun()){
                    if(
                       t0->neighbors[1]!=nullptr && t0->neighbors[1]->isVisibleSun() &&
                       t0->neighbors[3]!=nullptr && t0->neighbors[3]->isVisibleSun() &&
                       t0->neighbors[5]!=nullptr && t0->neighbors[5]->isVisibleSun() &&
                       t0->neighbors[7]!=nullptr && t0->neighbors[7]->isVisibleSun()){
                            t0->drawSkyArray(sky_array) ;
                    }
                    else t0->drawBorderSkyArray(sky_array);
                }
                if(t0->id != "0")t0->draw(render_array);
                else t0->drawOuts(render_array);
                t1->drawOuts(render_array);
            }
        }
    }
      drawGrassTiles();
}
void Chunk::syncNotRenderedTrees(){
    for(int i=0; i< _trees.size(); i++){
        Tree *tr = _trees[i];
        if(!tr->_rendered){
            _is_dirty = true;
            if(neighbors[0] != nullptr) neighbors[0]->_is_dirty = true;
            if(neighbors[1] != nullptr) neighbors[1]->_is_dirty = true;
            sf::Vector2i tileBase = tr->getPosition();
            Tile *ti = getTile(tileBase.x,tileBase.y,0);
            tr->treeToTiles(ti,1);
        }

    }
}
void Chunk::clearEntities(){
    _trees.clear();
}

void Chunk::addTreeToChunk(Tree *tr, int index_chunk_in_mat){
    sf::Vector2i tileBase = tr->getPosition();
    Tile *ti = getTile(tileBase.x,tileBase.y,0);
    _trees.push_back(tr);
    tr->treeToTiles(ti,index_chunk_in_mat);
    int posChunk = tr->hasTwoChunks();
    if(posChunk == -1 && neighbors[0] != nullptr) neighbors[0]->_is_dirty = true;
    else if(posChunk == 1 && neighbors[1] != nullptr) neighbors[1]->_is_dirty = true;
}

void Chunk::addEntitiesToChunk(){
    for(int i=0; i< _trees.size(); i++){
        //addTreeToChunk(_trees[i]);
    }
}
