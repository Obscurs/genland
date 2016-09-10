
#pragma once
#include "TextureManager.h"
class Tile
{
public:
	Tile(int id_temp, int layer);
	~Tile();

	void Reload(std::string id);
	void Draw(sf::RenderWindow & window, TextureManager &t);
	void Update(float elapsedTime);

	void SetPosition(float x, float y);
	void SetSize(float x);
	sf::Vector2f GetPosition() const;
	float GetWidth() const;
	float GetHeight() const;
	bool visible; //sha de privatitzar
	bool rigid;
	bool reach_floor;
	int layer;
	int max_tension;
	int weight;
	std::string id; //sha de privatitzar
    int id_temp;
	Tile* neighbors[9] = {nullptr};
	bool drawable();


	

private:

	sf::Vector2f position;
	sf::Vector2f size;
};

