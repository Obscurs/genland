#include "Map.h"
#include "Inventory.h"
#include <vector>
class Player
{
public:
	Player();
	~Player();
	
	void Load(std::string filename);
	void Draw(sf::RenderWindow & window);
	void Update(float delta, Map &map, Inputs &inputs, sf::RenderWindow &window);

	void SetPosition(float x, float y);
	void SetSize(float x);
	sf::Vector2f GetPosition() const;
	float GetWidth() const;
	float GetHeight() const;
	
	bool IsLoaded() const;

private:
	std::vector<Tile*> getTilesCol(sf::Vector2f pos, sf::Vector2f size);
	bool Collide(sf::Vector2f pos1, sf::Vector2f size1, sf::Vector2f pos2, sf::Vector2f size2);
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

	//SHOW
	sf::Sprite  _sprite;
	sf::Texture _image;
	std::string _filename;
	bool _isLoaded;
};

