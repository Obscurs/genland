#include <stdio.h>


// TODO: reference additional headers your program requires here
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <map>
#include <vector>
#include <iostream>
#include <cassert>
#include "Chunk.h"


//#include "Game.h"

Chunk::Chunk(sf::Vector2i pos)
{
	chunk_pos = pos;
	/*
	for(int i = 0; i<N_TILES_Y; ++i){
		for(int j = 0; j<N_TILES_X; ++j){
			Tile* t = new Tile();
			t->visible = true;
			t->id = "bed";
			t->Load();
			t->colisionable = false;
			t->SetPosition(chunk_pos.x*TILE_SIZE*N_TILES_X+j*TILE_SIZE, chunk_pos.y*TILE_SIZE*N_TILES_Y+i*TILE_SIZE);
			t->SetSize(TILE_SIZE);
			tile_mat[i][j][0] = t;
			if(pos.y==2 && pos.x !=2){
				Tile* t = new Tile();
				t->visible = true;
				t->id = "cob";
				t->Load();
				t->colisionable = true;
				t->SetPosition(chunk_pos.x*TILE_SIZE*N_TILES_X+j*TILE_SIZE, chunk_pos.y*TILE_SIZE*N_TILES_Y+i*TILE_SIZE);
				t->SetSize(TILE_SIZE);
				tile_mat[i][j][1] = t;
			} else if(pos.y==2 && pos.x ==2) {
				Tile* t = new Tile();
				t->visible = true;
				t->id = "red"; 
				t->Load();
				t->colisionable = true;
				t->SetPosition(chunk_pos.x*TILE_SIZE*N_TILES_X+j*TILE_SIZE, chunk_pos.y*TILE_SIZE*N_TILES_Y+i*TILE_SIZE);
				t->SetSize(TILE_SIZE);
				tile_mat[i][j][1] = t;
			}
			else{
				Tile* t = new Tile();
				t->visible = false;
				t->id = "0"; 
				t->Load();
				t->colisionable = false;
				t->SetPosition(chunk_pos.x*TILE_SIZE*N_TILES_X+j*TILE_SIZE, chunk_pos.y*TILE_SIZE*N_TILES_Y+i*TILE_SIZE);
				t->SetSize(TILE_SIZE);
				tile_mat[i][j][1] = t;
			}

			
			
		}
	}
	*/
	for(int i = 0; i<N_TILES_Y; ++i){
		for(int j = 0; j<N_TILES_X; ++j){
			if(pos.y == 0){
				Tile* t = new Tile();
				t->Reload("0");
				t->SetPosition(chunk_pos.x*TILE_SIZE*N_TILES_X+j*TILE_SIZE, chunk_pos.y*TILE_SIZE*N_TILES_Y+i*TILE_SIZE);
				t->SetSize(TILE_SIZE);
				tile_mat[i][j][0] = t;
			}
			else{
				Tile* t = new Tile();
				t->Reload("cob");
				t->reach_floor = true;
				t->SetPosition(chunk_pos.x*TILE_SIZE*N_TILES_X+j*TILE_SIZE, chunk_pos.y*TILE_SIZE*N_TILES_Y+i*TILE_SIZE);
				t->SetSize(TILE_SIZE);
				tile_mat[i][j][0] = t;
			}
			
			if(pos.y >=4){
				Tile* t = new Tile();
				t->Reload("bed");
				t->SetPosition(chunk_pos.x*TILE_SIZE*N_TILES_X+j*TILE_SIZE, chunk_pos.y*TILE_SIZE*N_TILES_Y+i*TILE_SIZE);
				t->SetSize(TILE_SIZE);
				tile_mat[i][j][1] = t;
			} else if(pos.y>=2 && pos.y <4) {
				Tile* t = new Tile();
				t->Reload("red");
				t->reach_floor = true;
				t->SetPosition(chunk_pos.x*TILE_SIZE*N_TILES_X+j*TILE_SIZE, chunk_pos.y*TILE_SIZE*N_TILES_Y+i*TILE_SIZE);
				t->SetSize(TILE_SIZE);
				tile_mat[i][j][1] = t;
			}
			else{
				Tile* t = new Tile();
				t->Reload("0");
				t->SetPosition(chunk_pos.x*TILE_SIZE*N_TILES_X+j*TILE_SIZE, chunk_pos.y*TILE_SIZE*N_TILES_Y+i*TILE_SIZE);
				t->SetSize(TILE_SIZE);
				tile_mat[i][j][1] = t;
			}

			
		}
	}

	
	
}

Chunk::~Chunk()
{
	//std::for_each(_gameObjects.begin(),_gameObjects.end(),GameObjectDeallocator());
}


Tile* Chunk::getTile(float x, float y, int z){
	//std::cout << 100.0 /64.0 << std::endl;
	if(x<0) x = 0;
	if(y<0) y = 0;
	int size_chunk_x = Chunk::N_TILES_X*Chunk::TILE_SIZE;
	int size_chunk_y = Chunk::N_TILES_Y*Chunk::TILE_SIZE;
	int tile_x = ((int)x%size_chunk_x)/Chunk::TILE_SIZE;
	int tile_y = ((int)y%size_chunk_y)/Chunk::TILE_SIZE;
	//std::cout << ((int)x) % size_chunk_x << std::endl;
	//std::cout << 641 % 640 << std::endl;
	return tile_mat[tile_y][tile_x][z];
}

sf::Vector2f Chunk::getTileIndex(float x, float y){
	if(x<0) x = 0;
	if(y<0) y = 0;
	int size_chunk_x = Chunk::N_TILES_X*Chunk::TILE_SIZE;
	int size_chunk_y = Chunk::N_TILES_Y*Chunk::TILE_SIZE;
	int tile_x = ((int)x%size_chunk_x)/Chunk::TILE_SIZE;
	int tile_y = ((int)y%size_chunk_y)/Chunk::TILE_SIZE;
	return sf::Vector2f(tile_y, tile_x);
}

void Chunk::DrawAll(sf::RenderWindow& renderWindow)
{
	for(int i = 0; i<N_TILES_Y; ++i){
		for(int j = 0; j<N_TILES_X; ++j){
			for(int k = 0; k<N_TILES_Z; ++k){
				Tile* t = tile_mat[i][j][k];
				if(t->visible)t->Draw(renderWindow);
			}
		}
	}
	//vector<int> v
	/*
	std::map<std::string,VisibleGameObject*>::const_iterator itr = _gameObjects.begin();
	while(itr != _gameObjects.end())
	{
		itr->second->Draw(renderWindow);
		itr++;
	} */
}



void Chunk::UpdateAll()
{
	//std::cout << Chunk[0][0] << std::endl;
	/*
	std::map<std::string,VisibleGameObject*>::const_iterator itr = _gameObjects.begin();
	float timeDelta = Game::GetWindow().GetFrameTime();

	while(itr != _gameObjects.end())
	{
		itr->second->Update(timeDelta);
		itr++;
	}
	*/
}