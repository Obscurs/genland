#include <stdio.h>


// TODO: reference additional headers your program requires here
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <map>
#include <iostream>
#include <cassert>


#include "Player.h"




Player::Player()
{
	inventory = new Inventory();
	col_bottom = 0;
	col_top = 0;
	col_left = 0;
	col_right = 0;
	col_bottom_dist = 0;
	col_top_dist = 0;
	col_left_dist = 0;
	col_right_dist = 0;
}


Player::~Player()
{
	
}



void Player::FixColision(sf::Vector2f pos1, sf::Vector2f size1, sf::Vector2f pos2, sf::Vector2f size2, Map& map){
    //std::cout << "1x " << pos1.x << " 1y "<< pos1.y << " 2x" << pos2.x << " 2y " << pos2.y  << std::endl;
	sf::Vector2f center1((pos1.x*2 + size1.x)/2, (pos1.y*2 + size1.y)/2);
	sf::Vector2f center2((pos2.x*2 + size2.x)/2, (pos2.y*2 + size2.y)/2);
    //std::cout << "cx " << center1.x << " cy "<< center1.y << " c2x" << center2.x << " c2y " << center2.y  << std::endl;
	float top1 = pos1.y;
	float bottom1 = pos1.y+size1.y;
	float left1 = pos1.x;
	float right1 = pos1.x+ size1.x;

	float top2 = pos2.y;
	float bottom2 = pos2.y+size2.y;
	float left2 = pos2.x;
	float right2 = pos2.x+ size2.x;

	//primer quadrant



	if(center1.x > center2.x && center1.y <= center2.y){
		//std::cout << "primer quadrant " << std::endl;
		float dist_left = right2-left1;
		float dist_bottom = bottom1 - top2;
		//std::cout << right2 << " "<< left1 << " " << dist_bottom << " " << dist_left << std::endl;
		//std::cout << "top " << size2.x << " bottom "<< bottom2 << " left" << left2 << " right " << right2 << std::endl;
		if(dist_left <= dist_bottom){
			//Chunk* c = map.getChunk(center2.x + Chunk::TILE_SIZE, center2.y);
			Tile* t = map.getTile(center2.x + Chunk::TILE_SIZE, center2.y, 1);
			if(t->id !="0"){
				//Chunk* c2 = map.getChunk(center2.x, center2.y - Chunk::TILE_SIZE);
				Tile* t2 = map.getTile(center2.x, center2.y - Chunk::TILE_SIZE, 1);
				if(t2->id !="0"){
					if(col_left_dist < dist_left) col_left_dist = dist_left;
					if(col_bottom_dist < dist_bottom) col_bottom_dist = dist_bottom;
					++col_bottom;
					++col_left;
				}
				else{
					if(col_bottom_dist < dist_bottom) col_bottom_dist = dist_bottom;
					++col_bottom;
				}
			}else{
				if(col_left_dist < dist_left) col_left_dist = dist_left;
				++col_left;
			}
			
		}
		else {
			//Chunk* c = map.getChunk(center2.x, center2.y - Chunk::TILE_SIZE);
			Tile* t = map.getTile(center2.x, center2.y - Chunk::TILE_SIZE, 1);
			if(t->id !="0"){
				//Chunk* c2 =  map.getChunk(center2.x + Chunk::TILE_SIZE, center2.y);
				Tile* t2 = map.getTile(center2.x + Chunk::TILE_SIZE, center2.y, 1);
				if(t2->id !="0"){
					if(col_left_dist < dist_left) col_left_dist = dist_left;
					if(col_bottom_dist < dist_bottom) col_bottom_dist = dist_bottom;
					++col_left;
					++col_bottom;
				}
				else{
					if(col_left_dist < dist_left) col_left_dist = dist_left;
					++col_left;
				}
			}
			else{
				if(col_bottom_dist < dist_bottom) col_bottom_dist = dist_bottom;
				++col_bottom;
			}
			
		}
		
	}


	//segon quadrant
	else if(center1.x <= center2.x && center1.y < center2.y){
		//std::cout << "segon quadrant " << std::endl;
		float dist_right = right1 - left2;
		float dist_bottom = bottom1 - top2;

		if(dist_right <= dist_bottom){
			//Chunk* c = map.getChunk(center2.x - Chunk::TILE_SIZE, center2.y);
			Tile* t = map.getTile(center2.x - Chunk::TILE_SIZE, center2.y, 1);
			if(t->id !="0"){
				//Chunk* c2 = map.getChunk(center2.x, center2.y - Chunk::TILE_SIZE);
				Tile* t2 = map.getTile(center2.x, center2.y - Chunk::TILE_SIZE, 1);
				if(t2->id !="0"){
					if(col_right_dist < dist_right) col_right_dist = dist_right;
					if(col_bottom_dist < dist_bottom) col_bottom_dist = dist_bottom;
					++col_bottom;
					++col_right;
				}
				else{
					if(col_bottom_dist < dist_bottom) col_bottom_dist = dist_bottom;
					++col_bottom;
				}
			}
			else{
				if(col_right_dist < dist_right) col_right_dist = dist_right;
				++col_right;
			}
		}
		else {
			//Chunk* c = map.getChunk(center2.x, center2.y - Chunk::TILE_SIZE);
			Tile* t = map.getTile(center2.x, center2.y - Chunk::TILE_SIZE, 1);
			if(t->id !="0"){
				//Chunk* c2 =  map.getChunk(center2.x - Chunk::TILE_SIZE, center2.y);
				Tile* t2 = map.getTile(center2.x - Chunk::TILE_SIZE, center2.y, 1);
				if(t2->id !="0"){
					if(col_right_dist < dist_right) col_right_dist = dist_right;
					if(col_bottom_dist < dist_bottom) col_bottom_dist = dist_bottom;
					++col_right;
					++col_bottom;
				}
				else{
					if(col_right_dist < dist_right) col_right_dist = dist_right;
					++col_right;
				}
			}
			else{
				if(col_bottom_dist < dist_bottom) col_bottom_dist = dist_bottom;
				++col_bottom;
			}
		}
		
	}
	//tercer quadrant
	else if(center1.x < center2.x && center1.y >= center2.y){
		//std::cout << "tercer quadrant " << std::endl;
		float dist_right = right1 - left2;
		float dist_top = bottom2 - top1;

		if(dist_right <= dist_top){
			//Chunk* c = map.getChunk(center2.x - Chunk::TILE_SIZE, center2.y);
			Tile* t = map.getTile(center2.x - Chunk::TILE_SIZE, center2.y, 1);
			if(t->id !="0"){
				//Chunk* c2 = map.getChunk(center2.x, center2.y + Chunk::TILE_SIZE);
				Tile* t2 = map.getTile(center2.x, center2.y + Chunk::TILE_SIZE, 1);
				if(t2->id !="0"){
					if(col_right_dist < dist_right) col_right_dist = dist_right;
					if(col_top_dist < dist_top) col_top_dist = dist_top;
					++col_top;
					++col_right;
				}
				else{
					if(col_top_dist < dist_top) col_top_dist = dist_top;
					++col_top;
				}
			}
			else{
				if(col_right_dist < dist_right) col_right_dist = dist_right;
				++col_right;
			}

		}
		else {
			
			//Chunk* c = map.getChunk(center2.x, center2.y + Chunk::TILE_SIZE);
			Tile* t = map.getTile(center2.x, center2.y + Chunk::TILE_SIZE, 1);
			if(t->id !="0"){
				//Chunk* c2 = map.getChunk(center2.x - Chunk::TILE_SIZE, center2.y);
				Tile* t2 = map.getTile(center2.x - Chunk::TILE_SIZE, center2.y, 1);
				if(t2->id !="0"){
					if(col_right_dist < dist_right) col_right_dist = dist_right;
					if(col_top_dist < dist_top) col_top_dist = dist_top;
					++col_right;
					++col_top;
				}
				else{
					if(col_right_dist < dist_right) col_right_dist = dist_right;
					++col_right;
				}
			}
			else{
				if(col_top_dist < dist_top) col_top_dist = dist_top;
				++col_top;
			}
		}
	}
	//quart quadrant
	else if(center1.x >= center2.x && center1.y > center2.y){
		//std::cout << "quart quadrant " << std::endl;
		float dist_left = right2-left1;
		float dist_top = bottom2 - top1;
		if(dist_left <= dist_top){
			//Chunk* c = map.getChunk(center2.x + Chunk::TILE_SIZE, center2.y);
			Tile* t = map.getTile(center2.x + Chunk::TILE_SIZE, center2.y, 1);
			if(t->id !="0"){
				//Chunk* c2 = map.getChunk(center2.x, center2.y + Chunk::TILE_SIZE);
				Tile* t2 = map.getTile(center2.x, center2.y + Chunk::TILE_SIZE, 1);
				if(t2->id !="0"){
					if(col_left_dist < dist_left) col_left_dist = dist_left;
					if(col_top_dist < dist_top) col_top_dist = dist_top;
					++col_top;
					++col_left;
				}
				else{
					if(col_top_dist < dist_top) col_top_dist = dist_top;
					++col_top;
				}
			}else{
				if(col_left_dist < dist_left) col_left_dist = dist_left;
				++col_left;
			}
		}
		else {
			//Chunk* c = map.getChunk(center2.x, center2.y + Chunk::TILE_SIZE);
			Tile* t = map.getTile(center2.x, center2.y + Chunk::TILE_SIZE, 1);
			if(t->id !="0"){
				//Chunk* c2 = map.getChunk(center2.x + Chunk::TILE_SIZE, center2.y);
				Tile* t2 = map.getTile(center2.x + Chunk::TILE_SIZE, center2.y, 1);
				
				if(t2->id !="0"){
					if(col_left_dist < dist_left) col_left_dist = dist_left;
					if(col_top_dist < dist_top) col_top_dist = dist_top;
					++col_left;
					++col_top;
				}
				else{
					if(col_left_dist < dist_left) col_left_dist = dist_left;
					++col_left;
				}
			}else{
				if(col_top_dist < dist_top) col_top_dist = dist_top;
				++col_top;
				
			}
		}
	}
}

void Player::Load(std::string filename)
{
	_filename = "resources/";
	_filename.append(filename);
	if(!_image.loadFromFile(_filename))
	{
		_filename =  "";
	}
	else
	{
		_sprite.setTexture(_image);

	}
}

void Player::Draw(sf::RenderWindow & renderWindow)
{
		renderWindow.draw(_sprite);
}
void Player::DrawInventory(sf::RenderWindow & renderWindow)
{
	inventory->Draw(renderWindow);
}


void Player::Update(float delta, Map &map, Inputs &inputs, sf::RenderWindow &window, std::vector<Background> &backs)
{
	inventory->Update(inputs, window);
	sf::Vector2i mouseLeft = inputs.getKey("mouseLeft");
	sf::Vector2i mouseRight = inputs.getKey("mouseRight");
	sf::Vector2i keyA = inputs.getKey("A");
	sf::Vector2i keyD = inputs.getKey("D");
	sf::Vector2i keySpace = inputs.getKey("Space");

    sf::Vector2f position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	//sf::View currentView = window.getView();
	//sf::Vector2f centerView = currentView.getCenter();
	//sf::Vector2f sizeView = currentView.getSize();

	//position.x += centerView.x-sizeView.x/2;
	//position.y += centerView.y-sizeView.y/2;

	if (keyA.x){
		vx = -300;
	}
	else if (keyD.x){
		vx = 300;
	} 
	else {
		vx = 0;
	}
	if(col_bottom==0){
		vy = (float)9.8*delta*100 + vy;
	}
	else{
		if (keySpace.x){
			vy = -400;
		}
		else{
			vy = 0;
		}
		
	}
	if(col_top != 0){
		vy = 0;
	}
	
	//std::cout << delta << std::endl;
	float x0 = _sprite.getPosition().x;
	float y0 = _sprite.getPosition().y;
	float x = x0+vx*delta;
	float y = y0+vy*delta;

	col_bottom = 0;
	col_top = 0;
	col_left = 0;
	col_right = 0;
	col_bottom_dist = 0;
	col_top_dist = 0;
	col_left_dist = 0;
	col_right_dist = 0;
	
	sf::Vector2f pos_aux(x, y);
	sf::Vector2f size_aux(GetWidth(), GetHeight());
	
	std::vector<Tile*> tiles_col = map.getTilesCol(pos_aux, size_aux);
	//std::cout << "number of cols" << tiles_col.size() << std::endl;
	
	//std::cout << "lel" << std::endl;
	for(int i = 0; i< tiles_col.size(); ++i){
	//	//comproba colisions
		Tile* t = tiles_col[i];
		sf::Vector2f tile_size(t->GetWidth(), t->GetHeight());
		FixColision(pos_aux, size_aux, t->GetPosition(), tile_size, map);
	}
    bool valid_move = true;
    if(col_bottom && col_left && col_right && col_top) valid_move = false;
    if(abs(x-x0) > Chunk::TILE_SIZE*5 || abs(y-y0) > Chunk::TILE_SIZE*5) valid_move = false;
	if(col_bottom >= col_top){
		col_top = 0;
		y = y - col_bottom_dist;
	}
	else{
		col_bottom = 0;
		y = y + col_top_dist;
	}
	if(col_left >= col_right){
		col_right = 0;
		x = x + col_left_dist;
	}
	else{
		col_left = 0;
		x = x - col_right_dist;
	}
	/*
	while(!cool_trobat && pos_aux.x < sprite_pos.x+ GetWidth()){
		Chunk* c = map.getChunk(pos_aux.x, (pos_aux.y+GetHeight()));
		Tile* t = c->getTile(pos_aux.x, (pos_aux.y+GetHeight()), 1);
	} */

    if(valid_move) SetPosition(x,y);

    //COMPROBA MAP

    map.checkLoadedChunks(GetPosition().x, GetPosition().y);

	//BACKGROUNDS
	sf::Vector2f back_pos_ref = GetPosition();
	for(int i=0; i<backs.size(); i++){
		backs[i].updatePosition(back_pos_ref);
	}



    //COMPROBA INPUTS
	if (mouseLeft.x == 1 && !inventory->show_inventory)
	{
		//std::cout << position.x << " " << position.y << std::endl;
	    Tile* t = map.getTile(position.x, position.y, 1);
		//std::cout << t->id_temp << " " <<true_position.x << " " << true_position.y << std::endl;
	    int position_tile = 1;
	    if(t->id =="0"){
	    	position_tile = 0;
	    	t = map.getTile(position.x, position.y, 0);
	    }
	    sf::Vector2f playerPos((GetPosition().x+GetWidth())/2,(GetPosition().y+GetHeight())/2);
	    sf::Vector2f tilePos((t->GetPosition().x+t->GetWidth())/2,(t->GetPosition().y+t->GetHeight())/2);
	    float dist = sqrt((playerPos.x-tilePos.x)*(playerPos.x-tilePos.x) + (playerPos.y-tilePos.y)*(playerPos.y-tilePos.y));
   
	    if(dist<Chunk::TILE_SIZE*2 && position_tile == 1 && t->visible) {

	    	if(giveItem(t->id, 1)){

	    		map.removeTile(t,1);
	    	}
	    }

	}
	else if (mouseRight.x == 1 && !inventory->show_inventory)
	{

	    Tile* t = map.getTile(position.x, position.y, 1);
	    int position_tile = 1;
	    if(t->id =="0"){
	    	position_tile = 0;
	    	t = map.getTile(position.x, position.y, 0);
	    }
	    sf::Vector2f playerPos((GetPosition().x+GetWidth())/2,(GetPosition().y+GetHeight())/2);
	    sf::Vector2f tilePos((t->GetPosition().x+t->GetWidth())/2,(t->GetPosition().y+t->GetHeight())/2);
	    float dist = sqrt((playerPos.x-tilePos.x)*(playerPos.x-tilePos.x) + (playerPos.y-tilePos.y)*(playerPos.y-tilePos.y));
   
	    if(dist<Chunk::TILE_SIZE*2 && position_tile == 0 && t->visible) {

	    	if(giveItem(t->id, 1)){
	    		//t->Remove();
	    		map.removeTile(t,0);
	    	}
	    }

	}


}
bool Player::giveItem(std::string id_item, int amount_item){

	int res = inventory->stackItem(id_item, amount_item);
	if(res==0) return true;
	else return false;
}

void Player::SetPosition(float x, float y)
{

		_sprite.setPosition(x,y);

}
void Player::SetSize(float x)
{
	sf::Vector2f new_scale(x/_sprite.getTexture()->getSize().x, x/_sprite.getTexture()->getSize().y);
	_sprite.setScale(new_scale);
}

sf::Vector2f Player::GetPosition() const
{

	return _sprite.getPosition();

}

float Player::GetHeight() const
{
	return _sprite.getTexture()->getSize().y*_sprite.getScale().y;
}

float Player::GetWidth() const
{
	return _sprite.getTexture()->getSize().x*_sprite.getScale().x;
}


sf::Sprite& Player::GetSprite()
{
	return _sprite;
}

