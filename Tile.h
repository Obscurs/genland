
#pragma once
#include "TextureManager.h"
class Tile
{
public:
	Tile(int id_temp);
	~Tile();
	
	void Load();
	void Reload(std::string id);
	//void Remove();
	void Draw(sf::RenderWindow & window, int layer, TextureManager &t);
	void Update(float elapsedTime);

	void SetPosition(float x, float y);
	void SetSize(float x);
	sf::Vector2f GetPosition() const;
	float GetWidth() const;
	float GetHeight() const;
	bool colisionable; //sha de privatitzar
	bool visible; //sha de privatitzar
	bool rigid;
	bool reach_floor;
	int max_tension;
	int weight;
	bool IsLoaded() const;
	std::string id; //sha de privatitzar
    int id_temp;
	bool lights[8];


	

private:

	sf::Vector2f position;
	sf::Vector2f size;
	bool _isLoaded;
};

