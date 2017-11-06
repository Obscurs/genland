#pragma once
#include "Tile.h"
#include "Simplex2d.h"
#include "../../TextureManager.h"
#include "../entities/Tree.h"
#include "../entities/AnimatedTile.h"
#include "../entities/Mob.h"

class Chunk
{
public:	
	Chunk(int pos, std::ofstream &myfile);
    Chunk(int pos, std::ifstream &myfile);
	~Chunk();


	const static int N_TILES_X = 100;		//must be on settings
	const static int N_TILES_Y = 300;
	const static int N_TILES_Z = 2;
	int _chunk_id;
    bool _is_dirty;
	Tile* getTile(float x, float y, int z);
    Tile* getTileByIndex(int x, int y, int z);

	sf::Vector2i getTileIndex(float x, float y);
	void setTileNeighbors(int index_x, int index_y);
	void calcLateralNeighborsTiles(int latelal);
    void saveToFile(std::string path);
	Chunk* neighbors[2] = { nullptr };

	void prepareArrays();
	void update(float delta);
    void debugDraw(sf::RenderTarget &target, const std::string keyDebug,sf::Text &text);
	sf::VertexArray render_array;
	sf::VertexArray sky_array;
    static sf::Vector2i getIndexFromGlobalPosition(sf::Vector2f pos);
    void addTreeToChunk(Tree *t, int index_chunk_in_mat);
	void addEntityToChunk(Entity *t, int index_chunk_in_mat);
	void addMobToChunk(Mob *t, int index_chunk_in_mat);
    void clearEntities();
    void syncNotRenderedTrees();
    std::vector<AnimatedTile*> _falling_tiles;
	std::vector<Entity*> _entities;
	std::vector<Mob*> _mobs;
    AnimatedTile* collidesWithAnimatedTile(sf::FloatRect rect);
    void addFallingTile(std::string id,std::string id_pick, sf::Vector2f pos, int size);
	void drawTreesSpawn(sf::RenderTarget & renderTar);
private:
	bool _need_sync;
    void recalcReachFloor();
	float getChunkPos();
    void checkTreeTiles();
	void drawGrassTiles();
    void syncSurfaceAndUnderground();
	Tile* tile_mat[N_TILES_Y][N_TILES_X][N_TILES_Z];
    std::pair<int,std::string> _surfacePosition[N_TILES_X];
    //array de llums
    //array de animals
    std::vector<Tree*> _trees; //array d'abres

    std::vector<Tile*> grass_tiles;



};