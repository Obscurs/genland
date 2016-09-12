#pragma once
#include "TextureManager.h"

class Item
{
public:	
	Item(std::string id_item);
	~Item();

	void Reload(std::string filename);

	void SetPosition(float x, float y);
	sf::Vector2f GetPosition() const;
	float GetWidth() const;
	float GetHeight() const;
	void SetSize(float x);
	void decrement(int amount_dec);
	Item* getItemCopy();
	std::map<std::string, int> craft_cost;
	std::string id; //sha de privatitzar
	int amount; //sha de privatitzar
	int max_stack_amount; //sha de privatitzar

	void Draw(sf::RenderWindow & window, TextureManager &t, sf::Text &text);



protected:
	

private:
	sf::Vector2f position;
	sf::Vector2f size;
};
