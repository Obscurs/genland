#pragma once
#include "Tile.h"
#include "Simplex2d.h"

class Chunk
{
public:	
	Chunk(sf::Vector2i pos, std::mt19937 *generator, std::ofstream &myfile, std::map<std::string, sf::Texture> &texture_map);
    Chunk(sf::Vector2i pos, std::ifstream &myfile, int &id_temp, std::map<std::string, sf::Texture> &texture_map);
	~Chunk();


	const static int N_TILES_X = 100;
	const static int N_TILES_Y = 200;
	const static int N_TILES_Z = 2;
	const static int TILE_SIZE = 16;

	sf::Vector2i chunk_pos;

	Tile* getTile(float x, float y, int z);
    Tile* getTileByIndex(int x, int y, int z);
	void DrawAll(sf::RenderWindow& renderWindow);
    void DrawChunk(sf::RenderWindow& renderWindow, sf::Vector2f pos1, sf::Vector2f pos2, std::map<std::string, sf::Texture> &texture_map);
	sf::Vector2i getTileIndex(float x, float y);
    void saveToFile();
    void initializeLights();
	void UpdateAll();

private:
	Tile* tile_mat[N_TILES_Y][N_TILES_X][N_TILES_Z];


};