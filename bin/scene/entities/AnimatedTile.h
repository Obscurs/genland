
#pragma once
#include "../../TextureManager.h"
#include "../world/Chunk.h"

class AnimatedTile
{
public:
	AnimatedTile();
	~AnimatedTile();

	void Reload(std::string id);
	void Draw(sf::RenderTarget & window, TextureManager &t);
	void Update(float elapsedTime, Chunk* c1,Chunk* c2,Chunk* c3 , int posMap);

	void SetPosition(float x, float y);
	void SetSize(float x);
    void setFactor(float dist_x, float dist_y);
	sf::Vector2f GetPosition() const;
	float GetWidth() const;
	float GetHeight() const;
	bool colisionable; //sha de privatitzar
	std::string id; //sha de privatitzar
    bool wall_left;
    bool wall_right;
    int deleted;
	

private:

	float vx;
	float vy;
    float disp_factor;
    float rotation;
    sf::Vector2f position;
    sf::Vector2f size;
    bool ColideWorld(Chunk* c1,Chunk* c2,Chunk* c3, int posMap);
};

