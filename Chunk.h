#pragma once
#include "Tile.h"


class Chunk
{
public:	
	Chunk(sf::Vector2i pos);
	~Chunk();

	//void Add(std::string name, VisibleGameObject* gameObject);
	//void Remove(std::string name);
	//int GetObjectCount() const;
	//VisibleGameObject* Get(std::string name) const;
	const static int N_TILES_X = 10;
	const static int N_TILES_Y = 10;
	const static int N_TILES_Z = 2;
	const static int TILE_SIZE = 32;

	sf::Vector2i chunk_pos;

	Tile* getTile(float x, float y, int z);
	void DrawAll(sf::RenderWindow& renderWindow);
	sf::Vector2f getTileIndex(float x, float y);

	void UpdateAll();

private:
	Tile* tile_mat[N_TILES_Y][N_TILES_X][N_TILES_Z];
	//std::map<std::string, VisibleGameObject*> _gameObjects;
	/*
	struct GameObjectDeallocator
	{
		void operator()(const std::pair<std::string,VisibleGameObject*> & p) const
		{
			delete p.second;
		}
	};
	*/
};