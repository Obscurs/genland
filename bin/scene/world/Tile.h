
#pragma once
#include "../../TextureManager.h"
class Tile
{
public:
	Tile(int layer);
	~Tile();
	enum Bioma {
		STANDARD,
		ICE_MOUNTAIN,
		MOUNTAIN,
		JUNGLE,
		DESERT,
		FOREST,
		PLAINS,
		NUM_BIOMS
	};

	void reload(std::string id);
	void draw(sf::VertexArray &vertexArray);
    void drawOuts(sf::VertexArray &vertexArray);
    void drawGrass(sf::VertexArray &vertexArray);
	void update(float elapsedTime);
	void drawSkyArray(sf::VertexArray &skyArray);
	void drawBorderSkyArray(sf::VertexArray &skyArray);
	void setPosition(float x, float y);
	void setSize(float x);
	sf::Vector2f GetPosition() const;
	float getWidth() const;
	float getHeight() const;
	bool reach_sun; //sha de privatitzar
	bool rigid;
	bool reach_floor;
	int layer;
	int max_tension;
    float last_tension_debug;
	int weight;
	float ms_to_remove;
	float ms_to_be_removed;
	bool being_removed;
	int _temperature;
	int _humidity;
	float _mountain_factor;
	Bioma _bio;
	std::string id; //sha de privatitzar
	std::string id_pick; //sha de privatitzar
	Tile* neighbors[9] = {nullptr};
	bool drawable();
	void appendSpriteToArray(sf::VertexArray &vertexArray, int mini_pos, std::string id_in_tex, int rotation, sf::Vector2i displacement);
	void debugTile(sf::RenderTarget &target,const std::string keyDebug, sf::Text &text, int chunk_id, sf::Vector2i posTile);
	

private:
	void setReachFloorCascade();
	void removeReachFloorCascade();
    void drawFadeOut(sf::VertexArray &vertexArray);
    void drawIns(sf::VertexArray &vertexArray);
    void drawAmbientOclusion(sf::VertexArray &vertexArray);
    void drawAmbientOclusion2(sf::VertexArray &vertexArray);
	sf::Vector2f position;
	sf::Vector2f size;

};

