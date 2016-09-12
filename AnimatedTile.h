
#pragma once
#include "TextureManager.h"
class AnimatedTile
{
public:
	AnimatedTile();
	~AnimatedTile();

	void Reload(std::string id);
	void Draw(sf::RenderWindow & window, TextureManager &t);
	void Update(float elapsedTime);

	void SetPosition(float x, float y);
	void SetSize(float x);
	sf::Vector2f GetPosition() const;
	float GetWidth() const;
	float GetHeight() const;
	bool colisionable; //sha de privatitzar
	std::string id; //sha de privatitzar


	

private:
	float vx;
	float vy;

    sf::Vector2f position;
    sf::Vector2f size;
};

