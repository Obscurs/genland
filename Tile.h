
#pragma once
#include "TextureManager.h"
class Tile
{
public:
	Tile(int id_temp, int layer);
	~Tile();

	void Reload(std::string id);
	void Draw(sf::RenderWindow & window, TextureManager &t, sf::Shader &tile_shader, sf::VertexArray &vertexArray);
    void DrawOuts(sf::RenderWindow & renderWindow, TextureManager &t, sf::VertexArray &vertexArray);
    void DrawGrass(sf::RenderWindow & renderWindow, TextureManager &t, sf::VertexArray &vertexArray);
	void Update(float elapsedTime);
	void drawSkyArray(sf::VertexArray &skyArray);
	void drawBorderSkyArray(sf::VertexArray &skyArray,TextureManager &t);
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
	std::string id; //sha de privatitzar
    int id_temp;
	Tile* neighbors[9] = {nullptr};
	bool drawable();
	void appendSpriteToArray(TextureManager &t, sf::VertexArray &vertexArray, int mini_pos, std::string id_in_tex, int rotation);

	

private:

    void DrawFadeOut(sf::RenderWindow & renderWindow, TextureManager &t, sf::VertexArray &vertexArray);
    void DrawIns(sf::RenderWindow & renderWindow, TextureManager &t, sf::Shader &tile_shader, sf::VertexArray &vertexArray);
    void DrawAmbientOclusion(sf::RenderWindow & renderWindow, TextureManager &t, sf::VertexArray &vertexArray);
	sf::Vector2f position;
	sf::Vector2f size;

};

