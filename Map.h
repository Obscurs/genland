#pragma once
#include "TextureManager.h"
#include "Chunk.h"
#include <vector>
#include <list>
#include <queue>
#include <random>
#include <algorithm>
#include "AnimatedTile.h"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
class Map
{
public:
	Map(int pos);
	~Map();

	const static int N_CHUNKS_X = 3;

	Chunk* getChunk(float x, float y);
	Tile* getTile(float x, float y, int z);
	int getChunkIndex(float x);
    int getIndexMatChunk(int x);

    void DrawMap(sf::RenderWindow& renderWindow);
    void DrawFrontItems(sf::RenderWindow& renderWindow);
	void removeTile(Tile* r_tile, int z);
	void UpdateAll(float delta);
	void checkLoadedChunks(float x, float y);
	std::vector<Tile*> getTilesCol(sf::Vector2f pos, sf::Vector2f size);
private:

    inline bool exists_file (const std::string& name);
    int posMap;
	std::mt19937 generator;
	void createMap(int map_index, int chunk_index, int &id_temp);
	bool calcPhysics(sf::Vector2f r_tile_pos_global, bool &conex_dreta, bool &conex_esquerra, bool &conex_abaix, sf::Vector2f eval_tile_pos, std::queue<Tile*> &queue_final_tiles, int position_case, std::queue<Tile*> &extension_tiles);
	void removeReachFloorCascade(float x, float y);
	sf::Vector2i getCordinatesRespectTile(sf::Vector2f pos_origen, sf::Vector2f pos_goal);
	std::vector<AnimatedTile*> falling_tiles;
	Chunk* chunk_mat[N_CHUNKS_X] = {nullptr};
	TextureManager* texMan;
};