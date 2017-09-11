#pragma once
#include "world/Map.h"
#include "world/Background.h"
#include "Inventory.h"
#include <vector>
class Player
{
public:
	Player();
	~Player();
	const static int PLAYER_SPEED_X = 2000;
	const static int PLAYER_SPEED_Y = 400;
	void Load(std::string filename);
	void Draw(sf::RenderWindow & window);
	void Draw2(sf::RenderTexture & tex);
	void DrawInventory(sf::RenderWindow & window);
	void Update(float delta, Map &map, sf::RenderWindow &window);
    int id;
	void SetPosition(float x, float y);
	void SetSize(float x);
	void saveStats(std::string pathGame);
	void loadStats(std::string pathGame);
	sf::Vector2f GetPosition() const;
	float GetWidth() const;
	float GetHeight() const;


private:
	void FixColision(sf::Vector2f pos1, sf::Vector2f size1, sf::Vector2f pos2, sf::Vector2f size2, Map& map);
	sf::Sprite& GetSprite();
	bool giveItem(std::string id, int amount);
	
	//PHYSICS
	float vx;
	float vy;
	int col_bottom;
	int col_top;
	int col_left;
	int col_right;
	float col_bottom_dist;
	float col_top_dist;
	float col_left_dist;
	float col_right_dist;

	//INVENTORY
	Inventory* inventory;

	//REMOVE AND PUT TILES;
	Tile* tile_being_removed;

	//SHOW
	sf::Sprite _sprite;
	sf::Texture _image;
	std::string _filename;
};
