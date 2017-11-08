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
#include "../Inputs.h"
#include "../Debuger.h"
#include "entities/Stairs.h"
#include "entities/Torch.h"


Player::Player(): Colisionable()
{
	inventory = new Inventory();
	_sprite.setPosition(0,0);
    _sprite.setTexture(*Resources::getTexture("playerSprite"));
    _spriteTool.setPosition(_sprite.getPosition());
    _spriteTool.setTexture(*Resources::getTexture("playerSprite"));
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
    _armor = NONE_A;
    _tool = NONE_T;
    _toolFactor = 1;
    _attacking = false;
    _mining = false;
}


Player::~Player()
{
	
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
        _spriteTool.setTexture(_image);

	}
}

void Player::Draw(sf::RenderWindow & renderWindow)
{
	renderWindow.draw(_sprite);
    if(_tool != NONE_T) renderWindow.draw(_spriteTool);
}
void Player::Draw2(sf::RenderTexture & tex)
{
	tex.draw(_sprite);
    if(_tool != NONE_T) tex.draw(_spriteTool);
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
            case FALLING:
                _numFramesAnimation =1;
                break;
            case ATTACKING:
                _numFramesAnimation =4;
                break;
            case STAIRS:
                _numFramesAnimation =4;
                break;
            default:
                break;
        }
    }
}
void Player::updateSprite(float delta){
    _spriteTime += delta;

    Entity *e = Scene::getScene()->getEntity(sf::FloatRect(_colPosition.x,_colPosition.y,PLAYER_WIDTH,PLAYER_HEIGHT));
    if(e != nullptr && e->_typeEntity =="stairs"){
        setAnimation(STAIRS);
    } else {
        if(_animationId == STAIRS) _animationId = IDLE;
    }
    float maxtime = float(PLAYER_SPRITE_MAX_TIME)/10.0f;
    if(_spriteTime > maxtime){
        _spriteTime -= maxtime;
        if(_animationId==STAIRS){
            if(vy !=0){
                _animationFrame +=1;
                if(_animationFrame >=_numFramesAnimation){
                    _animationFrame = 0;
                }
            }
        }else{
            _animationFrame +=1;
            if(_animationFrame >=_numFramesAnimation){
                _animationFrame = 0;
            }
        }

    }
    int armorOffset = _armor*6;
    _sprite.setTextureRect(sf::IntRect(PLAYER_SPRITE_SIZE*_animationFrame,PLAYER_SPRITE_SIZE*(_animationId+armorOffset),PLAYER_SPRITE_SIZE,PLAYER_SPRITE_SIZE));
    //direction
    if(vx >0 && _animationId != STAIRS) {
        _playerDirection = RIGHT;
    }
    else if(vx < 0 && _animationId != STAIRS) {
        _playerDirection = LEFT;
    }
    //animation
    if(_attacking || _mining){
        setAnimation(ATTACKING);
    }
    else if(vx >0 && _animationId != STAIRS && vy ==0) {
        setAnimation(WALKING);
    }
    else if(vx < 0 && _animationId != STAIRS && vy ==0) {
        setAnimation(WALKING);
    }
    else if(_animationId != STAIRS && vy ==0){
        setAnimation(IDLE);
    }
    else if(_animationId != STAIRS && vy !=0){
        setAnimation(FALLING);
    } else {
        setAnimation(STAIRS);
    }
    switch(_tool){
        case W_PICKAXE:
        {
            int row =0;
            int col;
            switch(_animationId){
                case IDLE:
                    col = 8;
                    break;
                case FALLING:
                    col = 12;
                    break;
                case WALKING:
                    col = 9 + (_animationFrame % 3);
                    break;
                case STAIRS:
                    col = 7;
                    break;
                case ATTACKING:
                    col = 13 + (_animationFrame % 3);
                    break;
                default:
                    break;
            }
            _spriteTool.setTextureRect(sf::IntRect(PLAYER_SPRITE_SIZE*col,PLAYER_SPRITE_SIZE*row,PLAYER_SPRITE_SIZE,PLAYER_SPRITE_SIZE));
        }
            break;
        case W_SWORD:
        {
            int row =1;
            int col;
            switch(_animationId){
                case IDLE:
                    col = 8;
                    break;
                case FALLING:
                    col = 12;
                    break;
                case WALKING:
                    col = 9 + (_animationFrame % 3);
                    break;
                case STAIRS:
                    col = 7;
                    break;
                case ATTACKING:
                    col = 13 + (_animationFrame % 3);
                    break;
                default:
                    break;
            }
            _spriteTool.setTextureRect(sf::IntRect(PLAYER_SPRITE_SIZE*col,PLAYER_SPRITE_SIZE*row,PLAYER_SPRITE_SIZE,PLAYER_SPRITE_SIZE));
        }
            break;
        case NONE_T:
            break;
        default:
            break;
    }

}
void Player::updateToolsAndArmors() {
    Item* currentTool = inventory->getItemTool();
    Item* currentArmor = inventory->getItemArmor();
    if(currentTool != nullptr){
        if(currentTool->id == "pickaxe1"){
            _toolFactor = 2;
            _tool = W_PICKAXE;
        } else if(currentTool->id == "sword1"){
            _toolFactor = 1;
            _tool = W_SWORD;
        } else {
            _toolFactor = 1;
            _tool = NONE_T;
        }
    } else {
        _toolFactor = 1;
        _tool = NONE_T;
    }
    if(currentArmor != nullptr){
        if(currentArmor->id == "armor1"){
            _maxTemperatureSafe = MAX_TEMP_BASE + 10;
            _minTemperatureSafe = MIN_TEMP_BASE - 20;
            _armor = ARMOR2;
        } else if(currentArmor->id == "armor2"){
            _maxTemperatureSafe = MAX_TEMP_BASE + 10;
            _minTemperatureSafe = MIN_TEMP_BASE - 20;
            _armor = ARMOR1;
        } else if(currentArmor->id == "armor3"){
            _maxTemperatureSafe = MAX_TEMP_BASE + 10;
            _minTemperatureSafe = MIN_TEMP_BASE - 20;
            _armor = ARMOR3;
        }else {
            _maxTemperatureSafe = MAX_TEMP_BASE;
            _minTemperatureSafe = MIN_TEMP_BASE;
            _armor = NONE_A;
        }
    } else {
        _maxTemperatureSafe = MAX_TEMP_BASE;
        _minTemperatureSafe = MIN_TEMP_BASE;
        _armor = NONE_A;
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


	if (Inputs::KeyDown(Inputs::A) && !Debuger::isTerminalActive()){
		vx = -PLAYER_SPEED_X;
	}
	else if (Inputs::KeyDown(Inputs::D)&& !Debuger::isTerminalActive()){
		vx = PLAYER_SPEED_X;
	} 
	else {
		vx = 0;
	}
    if(_animationId == STAIRS){
        if (Inputs::KeyDown(Inputs::W)&& !Debuger::isTerminalActive()){
            vy = -PLAYER_SPEED_X;
        } else if (Inputs::KeyDown(Inputs::S)&& !Debuger::isTerminalActive()){
            vy = +PLAYER_SPEED_X;
        } else {
            vy = 0;
        }
    }
    else{
        if(col_bottom==0){
            vy = (float)9.8*delta*100 + vy;
            //if (Inputs::KeyBreak(Inputs::SPACE)){
            //	vy = -PLAYER_SPEED_Y;
            //}
        }
        else{
            if (Inputs::KeyDown(Inputs::SPACE)&& !Debuger::isTerminalActive()){
                vy = -PLAYER_SPEED_Y;
            }
            else{
                vy = 0;
            }

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


    sf::Vector2f newPosDesired(x,y);
    sf::Vector2f oldPos(x0,y0);
	sf::Vector2f newPos = evalCollisions(oldPos,newPosDesired,sf::Vector2f(PLAYER_WIDTH,PLAYER_HEIGHT));

    SetPosition(newPos.x,newPos.y);

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
        Entity *e = scene->getEntity(sf::FloatRect(position.x,position.y,1,1));
        if(e != nullptr){
            e->_removed = true;
            Tile* t = map.getTile(position.x, position.y, 1);
            int chunkE = map.getChunkIndex(t->GetPosition().x);
            int index_chunk = map.getIndexMatChunk(chunkE);
            if(index_chunk != -1) {
                map._chunk_mat[index_chunk]->addFallingTile(e->_typeEntity,e->_typeEntity,position,Settings::TILE_SIZE);
            }
        } else {
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
                _mining = true;
            }
            _attacking = false;
        }


	}
	else if (Inputs::MouseDown(Inputs::M_RIGHT) && !inventory->show_inventory && viewRect.contains(position.x, position.y))
	{
        _attacking = false;
        _mining = false;
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
                            Entity *e = scene->getEntity(sf::FloatRect(t->GetPosition().x,t->GetPosition().y,Settings::TILE_SIZE,Settings::TILE_SIZE));
							if(idItemInventory != "-1" && e == nullptr){
								if(inventory->getItemAtTab()->_isEntity){
									if(idItemInventory=="stairs"){
										Stairs *st = new Stairs();
										st->setPosition(t->GetPosition().x,t->GetPosition().y);
                                        st->setPositionCol(t->GetPosition().x,t->GetPosition().y);
										st->_chunk = map.getChunkIndex(t->GetPosition().x);
										scene->addEntity(st);
										int index_chunk = map.getIndexMatChunk(st->_chunk);
										if(index_chunk != -1){
											map._chunk_mat[index_chunk]->addEntityToChunk(st,index_chunk);
											map._chunk_mat[index_chunk]->_is_dirty = true;
										}
									}
                                    else if(idItemInventory=="torch"){
                                        Torch *st = new Torch();
                                        st->setPosition(t->GetPosition().x,t->GetPosition().y);
                                        st->setPositionCol(t->GetPosition().x,t->GetPosition().y);
                                        st->_chunk = map.getChunkIndex(t->GetPosition().x);
                                        scene->addEntity(st);
                                        int index_chunk = map.getIndexMatChunk(st->_chunk);
                                        if(index_chunk != -1){
                                            map._chunk_mat[index_chunk]->addEntityToChunk(st,index_chunk);
                                            map._chunk_mat[index_chunk]->_is_dirty = true;
                                        }
                                    }
								}
								else{
									t->reload(idItemInventory);
								}
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

	} else {
        _attacking = false;
        _mining = false;
    }
    if(Inputs::KeyDown(Inputs::L_CONTR)){
        AnimatedTile* at =map.collidesWithAnimatedTile(sf::FloatRect(GetPosition().x, GetPosition().y,PLAYER_WIDTH, PLAYER_HEIGHT));
        if(at != nullptr){
            if(giveItem(at->_id_pick, 1)){
                at->deleted = 1;
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
        if(_playerDirection==RIGHT) {
            _sprite.setScale(sf::Vector2f(1,1));
            _sprite.setPosition(x-(PLAYER_SPRITE_SIZE-PLAYER_WIDTH-PLAYER_WIDTH/2),y-(PLAYER_SPRITE_SIZE-PLAYER_HEIGHT));
        }
        else {
            _sprite.setScale(sf::Vector2f(-1,1));
            _sprite.setPosition(x-(PLAYER_SPRITE_SIZE-PLAYER_WIDTH-PLAYER_WIDTH/2)+64,y-(PLAYER_SPRITE_SIZE-PLAYER_HEIGHT));
        }
        _spriteTool.setScale(_sprite.getScale());
        _spriteTool.setPosition(_sprite.getPosition());
        _colPosition = sf::Vector2f(x,y);
}
void Player::SetSize(float x)
{
	sf::Vector2f new_scale(x/PLAYER_SPRITE_SIZE, x/PLAYER_SPRITE_SIZE);
	_sprite.setScale(new_scale);
    _spriteTool.setScale(_sprite.getScale());
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
    myfile << _health << "\n";
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
        myfile >> _health;
		SetPosition(std::stof(x),std::stof(y));

        inventory->loadData(myfile);
		myfile.close();
	}
}

