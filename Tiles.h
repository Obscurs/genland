#pragma once
#include "Tile.h"


class Tiles
{
public:	
	Tiles();
	~Tiles();

	//void Add(std::string name, VisibleGameObject* gameObject);
	//void Remove(std::string name);
	//int GetObjectCount() const;
	//VisibleGameObject* Get(std::string name) const;

	void DrawAll(sf::RenderWindow& renderWindow);
	void UpdateAll();

private:
	Tile* tiles[10][10][2];
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