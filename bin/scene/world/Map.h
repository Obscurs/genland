#pragma once
#include "../../TextureManager.h"
#include "Chunk.h"
#include "MapView.h"
#include <vector>
#include <list>
#include <queue>
#include <random>
#include <algorithm>
#include "../entities/AnimatedTile.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include "Background.h"
#include "../entities/Light.h"

class Map
{
public:
    Map();
	~Map();
	std::string seed;
    void init(int pos, std::string game_path, std::string seed,TextureManager& texMan);
	void dirtyChunks();
    void drawViewMap(sf::RenderTarget &render);
	const static int N_CHUNKS_X = 3;

    std::vector<Light> lights;
    Chunk* chunk_mat[N_CHUNKS_X] = {nullptr};
    std::vector<AnimatedTile*> falling_tiles;


	Tile* getTile(float x, float y, int z);
	int getChunkIndex(float x);
    int getIndexMatChunk(int x);
    bool initialized;
    void saveMap();
    void removeTile2(Tile* r_tile);
	void UpdateAll(float delta, sf::Vector2f player_pos);
	void checkLoadedChunks(float x, float y);
	std::vector<Tile*> getTilesCol(sf::Vector2f pos, sf::Vector2f size);
private:
    MapViewer _mapViewer;
	TextureManager *texMan;
	std::string save_path;
    inline bool exists_file (const std::string& name);
    int posMap;
	std::mt19937 generator;
	void createMap(int map_index, int chunk_index, int &id_temp);
    void calcPhysics2(Tile* first_tile, std::map<Tile*,bool> conected_bfs);



	sf::Vector2i getCordinatesRespectTile(sf::Vector2f pos_origen, sf::Vector2f pos_goal);


};