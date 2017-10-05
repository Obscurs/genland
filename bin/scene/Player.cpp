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
#include "../Settings.h"
#include "Scene.h"
#include "../Resources.h"


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
	_sprite.setPosition(0,0);
    _sprite.setTexture(*Resources::getTexture("playerSprite"));
    _resPhysics = 0;
    _health = MAX_HEALTH;
    _maxTemperatureSafe =MAX_TEMP_BASE;
    _minTemperatureSafe = MIN_TEMP_BASE;
    vx = 0;
    vy = 0;
    _spriteTime = 0;
    tile_being_removed=nullptr;
    _animationId = IDLE;
    _animationFrame = 0;
    _sprite.setTextureRect(sf::IntRect(PLAYER_SPRITE_SIZE*_animationFrame,PLAYER_SPRITE_SIZE*_animationId,PLAYER_SPRITE_SIZE,PLAYER_SPRITE_SIZE));
    _numFramesAnimation = 1;
    _colPosition = sf::Vector2f(0,0);
    _playerDirection = LEFT;
    _toolFactor = 1;
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
			//Chunk* c = map.getChunk(center2.x + Settings::TILE_SIZE, center2.y);
			Tile* t = map.getTile(center2.x + Settings::TILE_SIZE, center2.y, 1);
			if(t->id !="0"){
				//Chunk* c2 = map.getChunk(center2.x, center2.y - Settings::TILE_SIZE);
				Tile* t2 = map.getTile(center2.x, center2.y - Settings::TILE_SIZE, 1);
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
			//Chunk* c = map.getChunk(center2.x, center2.y - Settings::TILE_SIZE);
			Tile* t = map.getTile(center2.x, center2.y - Settings::TILE_SIZE, 1);
			if(t->id !="0"){
				//Chunk* c2 =  map.getChunk(center2.x + Settings::TILE_SIZE, center2.y);
				Tile* t2 = map.getTile(center2.x + Settings::TILE_SIZE, center2.y, 1);
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
			//Chunk* c = map.getChunk(center2.x - Settings::TILE_SIZE, center2.y);
			Tile* t = map.getTile(center2.x - Settings::TILE_SIZE, center2.y, 1);
			if(t->id !="0"){
				//Chunk* c2 = map.getChunk(center2.x, center2.y - Settings::TILE_SIZE);
				Tile* t2 = map.getTile(center2.x, center2.y - Settings::TILE_SIZE, 1);
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
			//Chunk* c = map.getChunk(center2.x, center2.y - Settings::TILE_SIZE);
			Tile* t = map.getTile(center2.x, center2.y - Settings::TILE_SIZE, 1);
			if(t->id !="0"){
				//Chunk* c2 =  map.getChunk(center2.x - Settings::TILE_SIZE, center2.y);
				Tile* t2 = map.getTile(center2.x - Settings::TILE_SIZE, center2.y, 1);
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
			//Chunk* c = map.getChunk(center2.x - Settings::TILE_SIZE, center2.y);
			Tile* t = map.getTile(center2.x - Settings::TILE_SIZE, center2.y, 1);
			if(t->id !="0"){
				//Chunk* c2 = map.getChunk(center2.x, center2.y + Settings::TILE_SIZE);
				Tile* t2 = map.getTile(center2.x, center2.y + Settings::TILE_SIZE, 1);
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
			
			//Chunk* c = map.getChunk(center2.x, center2.y + Settings::TILE_SIZE);
			Tile* t = map.getTile(center2.x, center2.y + Settings::TILE_SIZE, 1);
			if(t->id !="0"){
				//Chunk* c2 = map.getChunk(center2.x - Settings::TILE_SIZE, center2.y);
				Tile* t2 = map.getTile(center2.x - Settings::TILE_SIZE, center2.y, 1);
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
			//Chunk* c = map.getChunk(center2.x + Settings::TILE_SIZE, center2.y);
			Tile* t = map.getTile(center2.x + Settings::TILE_SIZE, center2.y, 1);
			if(t->id !="0"){
				//Chunk* c2 = map.getChunk(center2.x, center2.y + Settings::TILE_SIZE);
				Tile* t2 = map.getTile(center2.x, center2.y + Settings::TILE_SIZE, 1);
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
			//Chunk* c = map.getChunk(center2.x, center2.y + Settings::TILE_SIZE);
			Tile* t = map.getTile(center2.x, center2.y + Settings::TILE_SIZE, 1);
			if(t->id !="0"){
				//Chunk* c2 = map.getChunk(center2.x + Settings::TILE_SIZE, center2.y);
				Tile* t2 = map.getTile(center2.x + Settings::TILE_SIZE, center2.y, 1);
				
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
void Player::Draw2(sf::RenderTexture & tex)
{
	tex.draw(_sprite);
}
void Player::DrawStats(sf::RenderTarget &target){
    sf::View currentView = target.getView();
    sf::Vector2f centerView = currentView.getCenter();
    sf::Vector2f sizeView = currentView.getSize();

    int widthBar = 300;
    int heightBar = 20;
    float xBar = centerView.x-widthBar/2;
    float yBar = centerView.y-sizeView.y/2 + heightBar;

    sf::RectangleShape rectangle(sf::Vector2f(widthBar, heightBar));


    rectangle.setFillColor(sf::Color(145, 145, 145));
    rectangle.setOutlineThickness(2);
    rectangle.setOutlineColor(sf::Color(0, 0, 0));
    rectangle.setPosition(sf::Vector2f(xBar, yBar));
    target.draw(rectangle);
    rectangle.setPosition(sf::Vector2f(xBar+1, yBar+1));
    rectangle.setSize(sf::Vector2f(widthBar*_health/MAX_HEALTH-2, heightBar-2));
    rectangle.setFillColor(sf::Color(170, 0, 0));
    rectangle.setOutlineThickness(0);
    target.draw(rectangle);
}
void Player::DrawInventory(sf::RenderWindow & renderWindow)
{
	inventory->Draw(renderWindow);
}
void Player::setAnimation(ActionState act){
    if(act != _animationId){
        _animationId = act;
        _animationFrame = 0;
        switch(_animationId){
            case IDLE:
                _numFramesAnimation =1;
                break;
            case WALKING:
                _numFramesAnimation =6;
                break;
            default:
                break;
        }
    }
}
void Player::updateSprite(float delta){
    _spriteTime += delta;
    float maxtime = float(PLAYER_SPRITE_MAX_TIME)/10.0f;
    if(_spriteTime > maxtime){
        _spriteTime -= maxtime;
        _animationFrame +=1;
        if(_animationFrame >=_numFramesAnimation){
            _animationFrame = 0;
        }
    }
    _sprite.setTextureRect(sf::IntRect(PLAYER_SPRITE_SIZE*_animationFrame,PLAYER_SPRITE_SIZE*_animationId,PLAYER_SPRITE_SIZE,PLAYER_SPRITE_SIZE));
    if(vx >0) {
        setAnimation(WALKING);
        _playerDirection = RIGHT;
    }
    else if(vx < 0) {
        setAnimation(WALKING);
        _playerDirection = LEFT;
    }
    else {
        setAnimation(IDLE);
    }
}
void Player::updateToolsAndArmors() {
    Item* currentTool = inventory->getItemTool();
    Item* currentArmor = inventory->getItemArmor();
    if(currentTool != nullptr){
        if(currentTool->id == "pickaxe1"){
            _toolFactor = 2;
        } else {
            _toolFactor = 1;
        }
    } else {
        _toolFactor = 1;
    }
    if(currentArmor != nullptr){
        if(currentArmor->id == "armor1"){
            _maxTemperatureSafe = MAX_TEMP_BASE + 10;
            _minTemperatureSafe = MIN_TEMP_BASE - 20;
        } else {
            _maxTemperatureSafe = MAX_TEMP_BASE;
            _minTemperatureSafe = MIN_TEMP_BASE;
        }
    } else {
        _maxTemperatureSafe = MAX_TEMP_BASE;
        _minTemperatureSafe = MIN_TEMP_BASE;
    }
}
void Player::updateHealth(float delta){
    float global_temp = Scene::getScene()->getTemperature(_colPosition);
    float local_temp = Scene::getScene()->getTemperatureGlobal(_colPosition);
    float total_temp = global_temp + local_temp;
    float damage = 0;
    if (total_temp>_maxTemperatureSafe){
        damage = delta/5;
    }
    if(total_temp<_minTemperatureSafe){
        damage = delta/5;
    }
    _health = std::max(_health-damage,0.f);
}
void Player::Update(float delta, Map &map, sf::RenderWindow &window)
{
    updateSprite(delta);
	Scene *scene = Scene::getScene();
	float zoom = scene->getZoom();
	inventory->Update(window);
    updateToolsAndArmors();
    updateHealth(delta);
    sf::Vector2f position = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    sf::Vector2f position_center = sf::Vector2f(GetPosition().x+PLAYER_WIDTH/2,GetPosition().y+PLAYER_WIDTH/2);
    sf::Vector2f position_zoomed = (position-position_center)/zoom +position_center;
    position = position_zoomed;


	if (Inputs::KeyDown(Inputs::A)){
		vx = -PLAYER_SPEED_X;
	}
	else if (Inputs::KeyDown(Inputs::D)){
		vx = PLAYER_SPEED_X;
	} 
	else {
		vx = 0;
	}
	if(col_bottom==0){
		vy = (float)9.8*delta*100 + vy;
		//if (Inputs::KeyBreak(Inputs::SPACE)){
		//	vy = -PLAYER_SPEED_Y;
		//}
	}
	else{
		if (Inputs::KeyDown(Inputs::SPACE)){
			vy = -PLAYER_SPEED_Y;
		}
		else{
			vy = 0;
		}
		
	}
	if(col_top != 0){
		vy = 0;
	}
	
	//std::cout << delta << std::endl;
	float x0 = _colPosition.x;
	float y0 = _colPosition.y;
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
	sf::Vector2f size_aux(PLAYER_WIDTH, PLAYER_HEIGHT);
	
	std::vector<Tile*> tiles_col = map.getTilesCol(pos_aux, size_aux);
	//std::cout << "number of cols" << tiles_col._size() << std::endl;
	
	//std::cout << "lel" << std::endl;
	for(int i = 0; i< tiles_col.size(); ++i){
	//	//comproba colisions
		Tile* t = tiles_col[i];
		sf::Vector2f tile_size(t->getWidth(), t->getHeight());
		FixColision(pos_aux, size_aux, t->GetPosition(), tile_size, map);
	}
    bool valid_move = true;
    if(col_bottom && col_left && col_right && col_top) valid_move = false;
    if(abs(x-x0) > Settings::TILE_SIZE*5 || abs(y-y0) > Settings::TILE_SIZE*5) valid_move = false;
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
	while(!cool_trobat && pos_aux.x < sprite_pos.x+ getWidth()){
		Chunk* c = map.getChunk(pos_aux.x, (pos_aux.y+GetHeight()));
		Tile* t = c->getTile(pos_aux.x, (pos_aux.y+getHeight()), 1);
	} */

    if(valid_move) SetPosition(x,y);

    //COMPROBA MAP

    map.checkLoadedChunks(GetPosition().x, GetPosition().y);


	sf::FloatRect viewRect(
		window.getView().getCenter().x - window.getView().getSize().x/2,
		window.getView().getCenter().y - window.getView().getSize().y/2,
		window.getView().getSize().x,
		window.getView().getSize().y
	);
    //COMPROBA INPUTS
	if (Inputs::MouseDown(Inputs::M_LEFT) && !inventory->show_inventory && viewRect.contains(position.x, position.y))
	{
		//std::cout << _position.x << " " << _position.y << std::endl;
	    Tile* t = map.getTile(position.x, position.y, 1);
		//std::cout << t->id_temp << " " <<true_position.x << " " << true_position.y << std::endl;
	    int position_tile = 1;
	    if(t->id == "0"){
	    	position_tile = 0;
	    	t = map.getTile(position.x, position.y, 0);
	    }
			if (tile_being_removed != nullptr && t != tile_being_removed) {
				tile_being_removed->being_removed = false;
			}
	    //sf::Vector2f playerPos((GetPosition().x+getWidth())/2,(GetPosition().y+getHeight())/2);
	    //sf::Vector2f tilePos((t->GetPosition().x+t->getWidth())/2,(t->GetPosition().y+t->getHeight())/2);
	    //float dist = sqrt((playerPos.x-tilePos.x)*(playerPos.x-tilePos.x) + (playerPos.y-tilePos.y)*(playerPos.y-tilePos.y));

	    if(t->id != "0" && t->id !="B" && t->id != "b") {
				tile_being_removed = t;
				if (tile_being_removed->being_removed) {
					tile_being_removed->ms_to_be_removed -= delta*1000*_toolFactor;
					if (tile_being_removed->ms_to_be_removed < 0) {
                        map.removeTile2(t);
                        map.dirtyChunks();

					}
				}
				else {
					tile_being_removed->being_removed = true;
					tile_being_removed->ms_to_be_removed = tile_being_removed->ms_to_remove;
				}
	    }

	}
	else if (Inputs::MouseDown(Inputs::M_RIGHT) && !inventory->show_inventory && viewRect.contains(position.x, position.y))
	{

	    Tile* t = map.getTile(position.x, position.y, 0);
	    int position_tile = 0;
	    if(t->id != "0") {
	    	position_tile = 1;
	    	t = map.getTile(position.x, position.y, 1);
	    }
	    //sf::Vector2f playerPos((GetPosition().x+getWidth())/2,(GetPosition().y+getHeight())/2);
	    //sf::Vector2f tilePos((t->GetPosition().x+t->getWidth())/2,(t->GetPosition().y+t->getHeight())/2);
	    //float dist = sqrt((playerPos.x-tilePos.x)*(playerPos.x-tilePos.x) + (playerPos.y-tilePos.y)*(playerPos.y-tilePos.y));
		if (t != tile_being_removed && tile_being_removed != nullptr) {
			tile_being_removed->being_removed = false;
		}

	    if(t->id == "0") {
				std::string idOfTabItem = inventory->getIdItemAtTab();
				if(idOfTabItem !="0") {
					std::cout <<"0" << std::endl;
					tile_being_removed = t;
					if (tile_being_removed->being_removed) {
						std::cout <<tile_being_removed->ms_to_be_removed << std::endl;
						tile_being_removed->ms_to_be_removed -= delta * 1000;
						if (tile_being_removed->ms_to_be_removed < 0) {
                            std::string idItemInventory;
							if (position_tile == 0) idItemInventory = inventory->getItemAtTab()->id_set0;
							else idItemInventory = inventory->getItemAtTab()->id_set1;
                            if(idItemInventory != "-1"){
                                t->reload(idItemInventory);
                                map.dirtyChunks();
                                inventory->decrementItemAtTab();
                            }

						}
					}
					else {
						tile_being_removed->being_removed = true;
						tile_being_removed->ms_to_be_removed = tile_being_removed->ms_to_remove;
					}
				}
	    }

	}
	AnimatedTile* at =map.collidesWithAnimatedTile(sf::FloatRect(GetPosition().x, GetPosition().y,PLAYER_WIDTH, PLAYER_HEIGHT));
	if(at != nullptr){
        if(giveItem(at->_id_pick, 1)){
            at->deleted = 1;
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
        if(_playerDirection==RIGHT) {
            _sprite.setScale(sf::Vector2f(1,1));
            _sprite.setPosition(x-(PLAYER_SPRITE_SIZE-PLAYER_WIDTH-PLAYER_WIDTH/2),y-(PLAYER_SPRITE_SIZE-PLAYER_HEIGHT));
        }
        else {
            _sprite.setScale(sf::Vector2f(-1,1));
            _sprite.setPosition(x-(PLAYER_SPRITE_SIZE-PLAYER_WIDTH-PLAYER_WIDTH/2)+64,y-(PLAYER_SPRITE_SIZE-PLAYER_HEIGHT));
        }

        _colPosition = sf::Vector2f(x,y);
}
void Player::SetSize(float x)
{
	sf::Vector2f new_scale(x/PLAYER_SPRITE_SIZE, x/PLAYER_SPRITE_SIZE);
	_sprite.setScale(new_scale);
}

sf::Vector2f Player::GetPosition() const
{
	return _colPosition;
}



sf::Sprite& Player::GetSprite()
{
	return _sprite;
}
inline bool exists_file (const std::string& name) {
	struct stat buffer;
	return (stat (name.c_str(), &buffer) == 0);
}
void Player::saveStats(std::string pathGame){
	std::string route = pathGame;
	route.append("/player_data");
	std::ofstream myfile;
	myfile.open(route);
	std::string x = std::to_string(_colPosition.x);
	std::string y = std::to_string(_colPosition.y);
	myfile << x << " " << y << "\n";
	inventory->saveData(myfile);
	myfile.close();
}
void Player::loadStats(std::string pathGame){
	std::string route = pathGame;
	route.append("/player_data");


	if(exists_file(route)){
		std::ifstream myfile;
		myfile.open(route);
		std::string x;
		std::string y;
		myfile >> x >> y;
		SetPosition(std::stof(x),std::stof(y));

        inventory->loadData(myfile);
		myfile.close();
	}
}

