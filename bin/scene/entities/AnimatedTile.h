
#pragma once
#include "../../TextureManager.h"


class AnimatedTile
{
public:
	AnimatedTile(std::string new_id,std::string id_pick);
	~AnimatedTile();

	void Draw(sf::RenderTarget & window);
	void Update(float elapsedTime);

	void SetPosition(float x, float y);
	void SetSize(float x);
    void setFactor(float dist_x, float dist_y);
	sf::Vector2f GetPosition() const;
	float GetWidth() const;
	float GetHeight() const;
	bool colisionable; //sha de privatitzar
	std::string id; //sha de privatitzar
    std::string _id_pick;
    bool wall_left;
    bool wall_right;
    int deleted;
	bool collideWithRectangle(sf::FloatRect rect);

private:

	float vx;
	float vy;
    float disp_factor;
    float rotation;
    sf::Vector2f position;
    sf::Vector2f size;
    bool ColideWorld();
};

