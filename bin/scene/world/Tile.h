
#pragma once
#include "../../TextureManager.h"
class Tile
{
public:
	Tile(int id_temp, int layer, TextureManager &t);
	~Tile();

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
	int weight;
	float ms_to_remove;
	float ms_to_be_removed;
	bool being_removed;
	std::string id; //sha de privatitzar
	std::string id_pick; //sha de privatitzar
    int id_temp;
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

