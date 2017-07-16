
#pragma once
#include "../../TextureManager.h"
class Tile
{
public:
	Tile(int layer, TextureManager &t);
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

	void Reload(std::string id);
	void Draw(sf::VertexArray &vertexArray);
    void DrawOuts(sf::VertexArray &vertexArray);
    void DrawGrass(sf::VertexArray &vertexArray);
	void Update(float elapsedTime);
	void drawSkyArray(sf::VertexArray &skyArray);
	void drawBorderSkyArray(sf::VertexArray &skyArray);
	void SetPosition(float x, float y);
	void SetSize(float x);
	sf::Vector2f GetPosition() const;
	float GetWidth() const;
	float GetHeight() const;
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
	void appendSpriteToArray(sf::VertexArray &vertexArray, int mini_pos, std::string id_in_tex, int rotation);
	void debugTile(sf::RenderTarget &target,const std::string keyDebug, sf::Text &text, int chunk_id, sf::Vector2i posTile);
	

private:
	TextureManager *texMan;
	void setReachFloorCascade();
	void removeReachFloorCascade();
    void DrawFadeOut(sf::VertexArray &vertexArray);
    void DrawIns(sf::VertexArray &vertexArray);
    void DrawAmbientOclusion(sf::VertexArray &vertexArray);
	sf::Vector2f position;
	sf::Vector2f size;

};

