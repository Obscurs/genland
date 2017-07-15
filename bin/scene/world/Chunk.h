#pragma once
#include "Tile.h"
#include "Simplex2d.h"
#include "../../TextureManager.h"
class Chunk
{
public:	
	Chunk(sf::Vector2i pos, std::mt19937 *generator,int seed, std::ofstream &myfile, TextureManager& texMan);
    Chunk(sf::Vector2i pos, std::mt19937 *generator,int seed, std::ifstream &myfile, TextureManager& texMan);
	~Chunk();


	const static int N_TILES_X = 100;
	const static int N_TILES_Y = 300;
	const static int N_TILES_Z = 2;
	int chunk_id;

	Tile* getTile(float x, float y, int z);
    Tile* getTileByIndex(int x, int y, int z);

	sf::Vector2i getTileIndex(float x, float y);
	void setTileNeighbors(int index_x, int index_y);
	void calcLateralNeighborsTiles(int latelal);
    void saveToFile(std::string path);
	Chunk* neighbors[2] = { nullptr };
	void recalcReachFloor();
	void prepareArrays();
	void update(float delta);
    void debugDraw(sf::RenderTarget &target, const std::string keyDebug,sf::Text &text);
	bool is_dirty;
	sf::VertexArray render_array;
	sf::VertexArray sky_array;
private:
	float getChunkPos();
	void DrawGrassTiles();
	Tile* tile_mat[N_TILES_Y][N_TILES_X][N_TILES_Z];
    std::vector<Tile*> grass_tiles;



};