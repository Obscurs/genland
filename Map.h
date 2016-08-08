#pragma once
#include "Chunk.h"
#include <vector>
#include <list>
#include <queue>
#include <algorithm>
#include "AnimatedTile.h"
class Map
{
public:	
	Map();
	~Map();

	const static int N_CHUNKS_X = 5;
	const static int N_CHUNKS_Y = 5;

	Chunk* getChunk(float x, float y);
	Tile* getTile(float x, float y, int z);
	sf::Vector2f getChunkIndex(float x, float y);

	void DrawAll(sf::RenderWindow& renderWindow);
	void removeTile(Tile* r_tile, int z);
	void UpdateAll(float delta);
	std::vector<Tile*> getTilesCol(sf::Vector2f pos, sf::Vector2f size);
private:

	bool calcPhysics(sf::Vector2f r_tile_pos_global, bool &conex_dreta, bool &conex_esquerra, bool &conex_abaix, sf::Vector2f eval_tile_pos, std::queue<Tile*> &queue_final_tiles, int position_case, std::queue<Tile*> &extension_tiles);
	void removeReachFloorCascade(float x, float y);
	sf::Vector2i getCordinatesRespectTile(sf::Vector2f pos_origen, sf::Vector2f pos_goal);
	std::vector<AnimatedTile*> falling_tiles;
	Chunk* chunk_mat[N_CHUNKS_X][N_CHUNKS_Y];
};