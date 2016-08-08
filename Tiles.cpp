#include <stdio.h>


// TODO: reference additional headers your program requires here
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <map>
#include <iostream>
#include <cassert>
#include "Tiles.h"
//#include "Game.h"

Tiles::Tiles()
{
	
	Tile* t = new Tile();
	t->Load("cob.png");
	t->SetPosition(50,50);
	tiles[0][0] = t;
}

Tiles::~Tiles()
{
	//std::for_each(_gameObjects.begin(),_gameObjects.end(),GameObjectDeallocator());
}




void Tiles::DrawAll(sf::RenderWindow& renderWindow)
{
	(tiles[0][0])->Draw(renderWindow);
	/*
	std::map<std::string,VisibleGameObject*>::const_iterator itr = _gameObjects.begin();
	while(itr != _gameObjects.end())
	{
		itr->second->Draw(renderWindow);
		itr++;
	} */
}

void Tiles::UpdateAll()
{
	//std::cout << tiles[0][0] << std::endl;
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