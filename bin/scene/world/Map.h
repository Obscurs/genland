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
    void init(int pos);
	void dirtyChunks();
    void drawViewMap(sf::RenderTarget &render);
	const static int N_CHUNKS_X = 3;	//must be on settings
    std::vector<Light> lights;
    std::vector<AnimatedTile*> falling_tiles;
	Tile* getTile(float x, float y, int z);
	Chunk* getChunk(float x, float y, float z);
	static int getChunkIndex(float globalx);
    int getIndexMatChunk(int chunk);
    void saveMap();
    void removeTile2(Tile* r_tile);
	void checkIntegrity(Tile* tileOrigin);
	void update(float delta, sf::Vector2f player_pos);
	void checkLoadedChunks(float x, float y);
    void searchDeserts();
    int getPosMap();
	static sf::Vector2i getGlobalPositionOfIndex(sf::Vector2i index, int chunk);
	std::vector<Tile*> getTilesCol(sf::Vector2f pos, sf::Vector2f size);
    Chunk* _chunk_mat[N_CHUNKS_X] = {nullptr};	//must ve value
	void syncEntitiesToChunk(int map_index);
	AnimatedTile* collidesWithAnimatedTile(sf::FloatRect rect);
    void setThreadParams(bool left, int pos);
private:
	bool _initialized;
	MapViewer _mapViewer;		//must be on scene
    inline bool exists_file (const std::string& name);
    int _posMap;
	void createMap(int map_index, int chunk_index);
    void calcPhysics2(Tile* first_tile, std::map<Tile*,bool> conected_bfs);
	sf::Vector2i getCordinatesRespectTile(sf::Vector2f pos_origen, sf::Vector2f pos_goal);
	bool _desertsReady;
    sf::Thread _threadSearchDeserts0;
    sf::Thread _threadSearchDeserts1;

    bool __left;
    int __pos;

};