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
#include "../SoundManager.hpp"
#include "../Mouse.h"
#include "entities/Picture.h"
#include "entities/Table.h"


Player::Player(): Colisionable(), Entity("player")
{
	inventory = new Inventory();
	_sprite.setPosition(0,0);
    _sprite.setTexture(*Resources::getTexture("playerSprite"));
    _spriteTool.setPosition(_sprite.getPosition());
    _spriteTool.setTexture(*Resources::getTexture("playerSprite"));
    _resPhysics = 0;
    _health = MAX_HEALTH;
    _hunger = MAX_HUNGER;
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
    _positionCol = sf::Vector2f(0,0);
    _playerDirection = LEFT;
    _armor = NONE_A;
    _tool = NONE_T;
    _toolFactor = 1;
    _attacking = false;
    _mining = false;
    _dead = false;
    _dieTime = 100;
    _spriteTimeHurt = 0;
    _currHurtAnim = 0;
    _keyframeHurt =0;
    _sizeCol = sf::Vector2i(PLAYER_WIDTH,PLAYER_HEIGHT);
    _attackColdown = 0;
    _damage = 0;
    _squarePosition = sf::Vector2f(0,0);
    _drawSquareTile = false;
}


Player::~Player()
{
	
}


void Player::Draw(sf::RenderWindow & renderWindow)
{
	renderWindow.draw(_sprite);
    if(_tool != NONE_T) renderWindow.draw(_spriteTool);
    if(_hurted){
        _spriteHurt.setPosition(_positionCol.x, _positionCol.y);

        _spriteHurt.setTextureRect(sf::IntRect(64*_keyframeHurt,64*_currHurtAnim,64,64));
        renderWindow.draw(_spriteHurt);
    }
}
void Player::Draw2(sf::RenderTexture & tex)
{
	tex.draw(_sprite);
    if(_tool != NONE_T) tex.draw(_spriteTool);
    if(_hurted){
        _spriteHurt.setPosition(_positionCol.x, _positionCol.y);

        _spriteHurt.setTextureRect(sf::IntRect(64*_keyframeHurt,64*_currHurtAnim,64,64));
        tex.draw(_spriteHurt);
    }
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
    rectangle.setSize(sf::Vector2f(std::max(widthBar*_health/MAX_HEALTH-2,1.f), heightBar-2));
    rectangle.setFillColor(sf::Color(170, 0, 0));
    rectangle.setOutlineThickness(0);
    target.draw(rectangle);

    TextureManager *t = Resources::getTextureManager("tileMap");

        sf::Sprite s;
        sf::Vector2f pos_heart(xBar-20,yBar);
        t->generateSprite("heart", pos_heart, s, sf::Vector2f(16,16));
        target.draw(s);


    rectangle.setFillColor(sf::Color(145, 145, 145));
    rectangle.setOutlineThickness(2);
    rectangle.setOutlineColor(sf::Color(0, 0, 0));
    rectangle.setPosition(sf::Vector2f(xBar, yBar+heightBar+5));
    rectangle.setSize(sf::Vector2f(widthBar, heightBar));
    target.draw(rectangle);
    rectangle.setPosition(sf::Vector2f(xBar+1, yBar+heightBar+6));
    rectangle.setSize(sf::Vector2f(std::max(widthBar*_hunger/MAX_HUNGER-2,1.f), heightBar-2));
    rectangle.setFillColor(sf::Color(0, 170, 0));
    rectangle.setOutlineThickness(0);
    target.draw(rectangle);

    sf::Sprite s2;
    sf::Vector2f pos_food(xBar-20,yBar+heightBar+5);
    t->generateSprite("food", pos_food, s2, sf::Vector2f(16,16));
    target.draw(s2);
    if(_dead){
        sf::Text text;
        text.setFont(*Resources::getFont("debugFont"));
        text.setCharacterSize(30);
        text.setColor(sf::Color(0, 0, 0, sf::Uint8(100-_dieTime+155)));
        std::string str ="DAYS SURVIVED: "+ std::to_string (Scene::getScene()->getClock()->day);
        text.setString(str);
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width/2.0f,
                       textRect.top  + textRect.height/2.0f);
        text.setPosition(sf::Vector2f(centerView.x, centerView.y));
        target.draw(text);
        rectangle.setPosition(sf::Vector2f(centerView.x-sizeView.x/2.f, centerView.y-sizeView.y/2.f));
        rectangle.setSize(sf::Vector2f(sizeView.x, sizeView.y));
        rectangle.setFillColor(sf::Color(0, 0, 0, sf::Uint8(100-_dieTime)));
        rectangle.setOutlineThickness(0);
        target.draw(rectangle);
    }
    else {
        if(_drawSquareTile){
            float zoom = Scene::getScene()->getZoom();

            rectangle.setFillColor(sf::Color(255, 255, 255,0));
            rectangle.setPosition(_squarePosition);
            rectangle.setSize(sf::Vector2f(Settings::TILE_SIZE*zoom,Settings::TILE_SIZE*zoom));
            rectangle.setOutlineThickness(1);
            rectangle.setOutlineColor(sf::Color(0, 0, 0));
            target.draw(rectangle);
            if(_tool == HAMMER){
                TextureManager *t = Resources::getTextureManager("tileMap");
                sf::Sprite s;
                sf::Vector2f item_pos = _squarePosition;
                t->generateSprite(inventory->getIdItemAtTab(), item_pos, s, sf::Vector2f(Settings::TILE_SIZE*zoom,Settings::TILE_SIZE*zoom));
                s.setColor(sf::Color(255, 255, 255, 128));
                target.draw(s);
            }
        }


    }

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
            case DEAD:
                _numFramesAnimation =5;
                break;
            default:
                break;
        }
    }
}
void Player::updateSprite(float delta){
    _spriteTime += delta;

    Entity *e = Scene::getScene()->getEntity(sf::FloatRect(_positionCol.x,_positionCol.y,PLAYER_WIDTH,PLAYER_HEIGHT));
    if(_dead){
        setAnimation(DEAD);
        _tool = NONE_T;
    }
    else if(e != nullptr && e->_typeEntity =="stairs"){
        setAnimation(STAIRS);
    } else {
        if(_animationId == STAIRS) _animationId = IDLE;
    }
    float maxtime = float(PLAYER_SPRITE_MAX_TIME)/10.0f;
    if(_animationFrame==0){
        switch(_animationId){
            case ATTACKING:
                //SoundManager::playSound("punch");
                break;
            default:
                break;
        }
    }
    if(_spriteTime > maxtime){
        _spriteTime -= maxtime;
        if(_animationId==STAIRS){
            if(vy !=0){
                _animationFrame +=1;
                if(_animationFrame >=_numFramesAnimation){
                    _animationFrame = 0;
                }
            }
        }
        else if(_animationId==DEAD){
            if(_animationFrame <_numFramesAnimation-1){
                _animationFrame +=1;
            }
        } else{
            _animationFrame +=1;
            if(_animationFrame >=_numFramesAnimation){
                _animationFrame = 0;
            }
        }

    }
    if(_hurted)_spriteTimeHurt += delta;
    if(_spriteTimeHurt > maxtime && _keyframeHurt <7){
        _spriteTimeHurt -= maxtime;
        {
            _keyframeHurt +=1;
        }

    } else if(_keyframeHurt>=7){
        _spriteTimeHurt =0;
        _hurted = false;
        _keyframeHurt =0;
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
    if(_animationId !=DEAD){
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
        case I_PICKAXE:
        {
            int row =2;
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
        case HAMMER:
        {
            int row =3;
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
            _damage = 5;
        }
        else if(currentTool->id == "hammer"){
            _toolFactor = 1;
            _tool = HAMMER;
            _damage = 5;
        }
        else if(currentTool->id == "pickaxe2"){
            _toolFactor = 4;
            _tool = I_PICKAXE;
            _damage = 7;
        } else if(currentTool->id == "sword1"){
            _toolFactor = 1;
            _tool = W_SWORD;
            _damage = 10;
        } else {
            _toolFactor = 1;
            _tool = NONE_T;
            _damage = 2;
        }
    } else {
        _damage = 2;
        _toolFactor = 1;
        _tool = NONE_T;
    }
    if(currentArmor != nullptr){
        if(currentArmor->id == "armor1"){ //winter
            _maxTemperatureSafe = MAX_TEMP_BASE - 10;
            _minTemperatureSafe = MIN_TEMP_BASE - 20;
            _armor = ARMOR2;
        } else if(currentArmor->id == "armor2"){ //summer
            _maxTemperatureSafe = MAX_TEMP_BASE + 20;
            _minTemperatureSafe = MIN_TEMP_BASE + 10;
            _armor = ARMOR1;
        } else if(currentArmor->id == "armor3"){ //iron
            _maxTemperatureSafe = MAX_TEMP_BASE - 15;
            _minTemperatureSafe = MIN_TEMP_BASE - 25;
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
void Player::updateStats(float delta){
    float global_temp = Scene::getScene()->getTemperature(_positionCol);
    float local_temp = Scene::getScene()->getTemperatureGlobal(_positionCol);
    float total_temp = global_temp + local_temp;
    float damage = 0;
    if (total_temp>_maxTemperatureSafe){
        damage = delta/5;
    }
    if(total_temp<_minTemperatureSafe){
        damage = delta/5;
    }
    if(_hunger==0) damage += delta;
    _health = std::min(_health+delta/10.f,float(MAX_HEALTH));
    _health = std::max(_health-damage,0.f);
    _hunger = std::max(_hunger-delta/20.f,0.f);
    if(_health<=0) _dead = true;
}
void Player::mine(Tile *t, float delta){
    Map* map = Scene::getScene()->getMap();
    if (tile_being_removed != nullptr && t != tile_being_removed) {
        tile_being_removed->being_removed = false;
    }

    if (t->id != "0" && t->id != "B" && t->id != "b") {
        tile_being_removed = t;
        if (tile_being_removed->being_removed) {

            tile_being_removed->ms_to_be_removed -= delta * 100 * _toolFactor;
            if (tile_being_removed->ms_to_be_removed < 0) {
                t->reproduceSoundRemove();
                map->removeTile2(t);
                map->dirtyChunks();

            }
        }
        else {
            tile_being_removed->being_removed = true;
            tile_being_removed->ms_to_be_removed = tile_being_removed->ms_to_remove;
        }
        _mining = true;
    }
}
void Player::build(Tile *t, float delta, int position_tile){
    Scene* scene = Scene::getScene();
    Map* map = scene->getMap();
    _attacking = false;
    _mining = true;
    if (t != tile_being_removed && tile_being_removed != nullptr) {
        tile_being_removed->being_removed = false;
    }

    if (t->id == "0") {
        std::string idOfTabItem = inventory->getIdItemAtTab();
        if (idOfTabItem != "0") {
            std::cout << "0" << std::endl;
            tile_being_removed = t;
            if (tile_being_removed->being_removed) {
                std::cout << tile_being_removed->ms_to_be_removed << std::endl;
                tile_being_removed->ms_to_be_removed -= delta * 1000;
                if (tile_being_removed->ms_to_be_removed < 0) {
                    std::string idItemInventory;
                    if (position_tile == 0) idItemInventory = inventory->getItemAtTab()->id_set0;
                    else idItemInventory = inventory->getItemAtTab()->id_set1;
                    Entity *e = scene->getEntity(
                            sf::FloatRect(t->GetPosition().x, t->GetPosition().y, Settings::TILE_SIZE,
                                          Settings::TILE_SIZE));
                    if (idItemInventory != "-1" && e == nullptr) {
                        if (inventory->getItemAtTab()->_isEntity) {
                            if (idItemInventory == "stairs") {
                                Stairs *st = new Stairs();
                                st->setPosition(t->GetPosition().x, t->GetPosition().y);
                                st->setPositionCol(t->GetPosition().x, t->GetPosition().y);
                                st->_chunk = map->getChunkIndex(t->GetPosition().x);
                                scene->addEntity(st);
                                int index_chunk = map->getIndexMatChunk(st->_chunk);
                                if (index_chunk != -1) {
                                    SoundManager::playSoundNoRestart("place");
                                    map->_chunk_mat[index_chunk]->addEntityToChunk(st, index_chunk);
                                    map->_chunk_mat[index_chunk]->_is_dirty = true;
                                }
                            }
                            else if (idItemInventory == "torch") {
                                Torch *st = new Torch(0);
                                st->setPosition(t->GetPosition().x, t->GetPosition().y);
                                st->setPositionCol(t->GetPosition().x, t->GetPosition().y);
                                st->_chunk = map->getChunkIndex(t->GetPosition().x);
                                scene->addEntity(st);
                                int index_chunk = map->getIndexMatChunk(st->_chunk);
                                if (index_chunk != -1) {
                                    SoundManager::playSoundNoRestart("place");
                                    map->_chunk_mat[index_chunk]->addEntityToChunk(st, index_chunk);
                                    map->_chunk_mat[index_chunk]->_is_dirty = true;
                                }
                            }
                            else if (idItemInventory == "fireplace") {
                                Torch *st = new Torch(1);
                                st->setPosition(t->GetPosition().x, t->GetPosition().y);
                                st->setPositionCol(t->GetPosition().x, t->GetPosition().y);
                                st->_chunk = map->getChunkIndex(t->GetPosition().x);
                                scene->addEntity(st);
                                int index_chunk = map->getIndexMatChunk(st->_chunk);
                                if (index_chunk != -1) {
                                    SoundManager::playSoundNoRestart("place");
                                    map->_chunk_mat[index_chunk]->addEntityToChunk(st, index_chunk);
                                    map->_chunk_mat[index_chunk]->_is_dirty = true;
                                }
                            }
                            else if (idItemInventory == "fireplaceCamp") {
                                Torch *st = new Torch(2);
                                st->setPosition(t->GetPosition().x, t->GetPosition().y);
                                st->setPositionCol(t->GetPosition().x, t->GetPosition().y);
                                st->_chunk = map->getChunkIndex(t->GetPosition().x);
                                scene->addEntity(st);
                                int index_chunk = map->getIndexMatChunk(st->_chunk);
                                if (index_chunk != -1) {
                                    SoundManager::playSoundNoRestart("place");
                                    map->_chunk_mat[index_chunk]->addEntityToChunk(st, index_chunk);
                                    map->_chunk_mat[index_chunk]->_is_dirty = true;
                                }
                            }
                            else if (idItemInventory == "picture") {
                                Picture *st = new Picture();
                                st->setPosition(t->GetPosition().x, t->GetPosition().y);
                                st->setPositionCol(t->GetPosition().x, t->GetPosition().y);
                                st->_chunk = map->getChunkIndex(t->GetPosition().x);
                                scene->addEntity(st);
                                int index_chunk = map->getIndexMatChunk(st->_chunk);
                                if (index_chunk != -1) {
                                    SoundManager::playSoundNoRestart("place");
                                    map->_chunk_mat[index_chunk]->addEntityToChunk(st, index_chunk);
                                    map->_chunk_mat[index_chunk]->_is_dirty = true;
                                }
                            }
                            else if (idItemInventory == "table") {
                                Table *st = new Table();
                                st->setPosition(t->GetPosition().x, t->GetPosition().y);
                                st->setPositionCol(t->GetPosition().x, t->GetPosition().y);
                                st->_chunk = map->getChunkIndex(t->GetPosition().x);
                                scene->addEntity(st);
                                int index_chunk = map->getIndexMatChunk(st->_chunk);
                                if (index_chunk != -1) {
                                    SoundManager::playSoundNoRestart("place");
                                    map->_chunk_mat[index_chunk]->addEntityToChunk(st, index_chunk);
                                    map->_chunk_mat[index_chunk]->_is_dirty = true;
                                }
                            }
                        }
                        else {
                            SoundManager::playSound("placeBloc");
                            t->reload(idItemInventory);
                            map->checkIntegrity(t);
                        }
                        map->dirtyChunks();
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
void Player::Update(float delta, Map &map, sf::RenderWindow &window) {
    updateSprite(delta);
    if (!_dead) {
        _attackColdown = std::max(0.f,_attackColdown-delta*50);
        Scene *scene = Scene::getScene();
        float zoom = scene->getZoom();
        inventory->Update(window);
        updateToolsAndArmors();
        updateStats(delta);
        sf::Vector2f position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::Vector2f position_unzoomed = position;
        sf::Vector2f position_center = sf::Vector2f(GetPosition().x + PLAYER_WIDTH / 2, GetPosition().y + PLAYER_WIDTH / 2);
        sf::Vector2f position_zoomed = (position - position_center) / zoom + position_center;
        position = position_zoomed;

        sf::Vector2f playerPos(_positionCol.x+PLAYER_WIDTH/2, +_positionCol.y+PLAYER_HEIGHT/2);
        float dist = sqrt((playerPos.x-position.x)*(playerPos.x-position.x) + (playerPos.y-position.y)*(playerPos.y-position.y));
        bool allowedActionDistance = dist < DIST_ACTION;

        SoundManager::playSoundNoRestart("rain0");
        if(_animationId == WALKING){
            SoundManager::setVolumeSound(30,"step");
            SoundManager::playSoundNoRestart("step");
        }
        if (Inputs::KeyDown(Inputs::A) && !Debuger::isTerminalActive()) {
            vx = -PLAYER_SPEED_X;
        }
        else if (Inputs::KeyDown(Inputs::D) && !Debuger::isTerminalActive()) {
            vx = PLAYER_SPEED_X;
        }
        else {
            vx = 0;
        }
        if (_animationId == STAIRS) {
            if (Inputs::KeyDown(Inputs::W) && !Debuger::isTerminalActive()) {
                vy = -PLAYER_SPEED_X;
            } else if (Inputs::KeyDown(Inputs::S) && !Debuger::isTerminalActive()) {
                vy = +PLAYER_SPEED_X;
            } else {
                vy = 0;
            }
        }
        else {
            if (col_bottom == 0) {
                vy = (float) 9.8 * delta * 100 + vy;
                //if (Inputs::KeyBreak(Inputs::SPACE)){
                //	vy = -PLAYER_SPEED_Y;
                //}
            }
            else {
                if (Inputs::KeyDown(Inputs::SPACE) && !Debuger::isTerminalActive()) {
                    vy = -PLAYER_SPEED_Y;
                }
                else {
                    vy = 0;
                }

            }
        }

        if (col_top != 0) {
            vy = 0;
        }

        //std::cout << delta << std::endl;
        float x0 = _positionCol.x;
        float y0 = _positionCol.y;
        float x = x0 + vx * delta;
        float y = y0 + vy * delta;


        sf::Vector2f newPosDesired(x, y);
        sf::Vector2f oldPos(x0, y0);
        sf::Vector2f newPos = evalCollisions(oldPos, newPosDesired, sf::Vector2f(PLAYER_WIDTH, PLAYER_HEIGHT));

        SetPosition(newPos.x, newPos.y);

        //COMPROBA MAP

        map.checkLoadedChunks(GetPosition().x, GetPosition().y);


        sf::FloatRect viewRect(
                window.getView().getCenter().x - window.getView().getSize().x / 2,
                window.getView().getCenter().y - window.getView().getSize().y / 2,
                window.getView().getSize().x,
                window.getView().getSize().y
        );
        if(allowedActionDistance && !inventory->show_inventory){
            Entity *e = scene->getEntity(sf::FloatRect(position.x, position.y, 1, 1));
            if(e != nullptr){
                _drawSquareTile = false;
                Mouse::setCursor("sword",0);
            } else{
                Tile *t = map.getTile(position.x, position.y, 1);
                if(t != nullptr && _tool != HAMMER){
                    _drawSquareTile = true;
                    _squarePosition = t->GetPosition();
                    _squarePosition = zoom*_squarePosition+position_center-zoom*position_center;
                    _squarePosition.y += (1-zoom)*(Settings::TILE_SIZE/3*1.55); //hardcoded number
                    Mouse::setCursor("pick",0);
                } else if(_tool == HAMMER && t->id == "0" && inventory->getIdItemAtTab() != "0" && (inventory->getItemAtTab()->id_set0 != "-1" || inventory->getItemAtTab()->id_set1 != "-1")){
                    if(inventory->getItemAtTab()->id_set0 != "-1" || t->neighbors[8]->id !="0"){
                        _drawSquareTile = true;
                        _squarePosition = t->GetPosition();
                        _squarePosition = zoom*_squarePosition+position_center-zoom*position_center;
                        _squarePosition.y += (1-zoom)*(Settings::TILE_SIZE/3*1.55); //hardcoded number

                        Mouse::setCursor("hammer",0);
                    }
                    else {
                        _drawSquareTile = false;
                        Mouse::setCursor("default",0);
                    }
                }
                else{
                    _drawSquareTile = false;
                    Mouse::setCursor("default",0);
                }
            }
        } else{
            _drawSquareTile = false;
            Mouse::setCursor("default",0);
        }
        //COMPROBA INPUTS
        if (Inputs::MouseDown(Inputs::M_LEFT) && !inventory->show_inventory && viewRect.contains(position.x, position.y)) {
            //std::cout << _position.x << " " << _position.y << std::endl;
            if(allowedActionDistance) {


                Entity *e = scene->getEntity(sf::FloatRect(position.x, position.y, 1, 1));

                if (e != nullptr) {
                    if (e->_typeEntity == "mob") {
                        if (_attackColdown == 0) {
                            Mob *m = static_cast<Mob *>(e);
                            m->hurt(_damage);
                            m->getGenetics()->_playerHostile = true;
                            _attackColdown = 50;
                            std::cout << "player atackking!!!!!!!!!!" << std::endl;
                            _attacking = true;
                            _mining = false;
                        }
                    } else {
                        e->_removed = true;
                        Tile *t = map.getTile(position.x, position.y, 1);
                        int chunkE = map.getChunkIndex(t->GetPosition().x);
                        int index_chunk = map.getIndexMatChunk(chunkE);
                        if (index_chunk != -1) {
                            std::string idFalling = e->_typeEntity;
                            if(e->_typeEntity=="torch"){
                                int type = static_cast<Torch*>(e)->_typeTorx;
                                if(type==1) idFalling = "fireplace";
                                else if(type==2) idFalling = "fireplaceCamp";
                            }
                            map._chunk_mat[index_chunk]->addFallingTile(idFalling, idFalling, position,
                                                                        Settings::TILE_SIZE);
                            SoundManager::playSoundNoRestart("bubble");
                        }
                        _mining = true;
                        _attacking = false;
                    }

                } else {
                    Tile *t = map.getTile(position.x, position.y, 1);
                    if(_tool == HAMMER){
                        if(t->id == "0" && t->neighbors[8]->id != "0"){
                            build(t,delta,1);
                        }
                    }
                    else{

                        if(t->id != "0"){
                            mine(t,delta);
                        }
                    }

                }
            }

        }
        else if (Inputs::MouseDown(Inputs::M_RIGHT) && !inventory->show_inventory &&
                 viewRect.contains(position.x, position.y)) {
            if(allowedActionDistance) {
                Tile *t = map.getTile(position.x, position.y, 0);
                if(_tool == HAMMER){
                    if(t->id == "0" && t->neighbors[8]->id == "0"){
                        build(t,delta,0);
                    }
                }
                else{
                    if(t->neighbors[8]->id == "0"){
                        mine(t, delta);
                    }
                }
            }
        } else {
            _attacking = false;
            _mining = false;
        }
        if (Inputs::KeyDown(Inputs::L_CONTR)) {
            AnimatedTile *at = map.collidesWithAnimatedTile(
                    sf::FloatRect(GetPosition().x, GetPosition().y, PLAYER_WIDTH, PLAYER_HEIGHT));
            if (at != nullptr) {
                if (giveItem(at->_id_pick, 1)) {
                    at->deleted = 1;
                }
            }
        }
        if (Inputs::KeyBreak(Inputs::E)) {
            std::string idOfTabItem = inventory->getIdItemAtTab();
            if(idOfTabItem=="food"){
                _hunger = std::min(_hunger+10,float(MAX_HUNGER));
                inventory->decrementItemAtTab();
            }
        }
        if(_mining || _attacking){
            if(_tool == NONE_T ) SoundManager::playSoundNoRestart("punch");
            else if(_tool == W_PICKAXE || _tool == I_PICKAXE || _tool == HAMMER) SoundManager::playSoundNoRestart("pick");
            else if(_tool == W_SWORD ) SoundManager::playSoundNoRestart("sword");
        }
    } else {
        _dieTime = std::max(0.f, _dieTime-delta*50);
    }
    float rainFactor = Scene::getScene()->getClock()->_rainFactor*100;
    rainFactor = rainFactor*1.3f;
    //if(rainFactor<25) rainFactor =0;
    //else{
    //    rainFactor = (rainFactor-25)*1.33;
    //}
    float lightFactor = Scene::getScene()->getClock()->_lightFactor*100;
    float heightFactor = (1-_positionCol.y/(Settings::TILE_SIZE*Chunk::N_TILES_Y))*100;
    float caveFactor = (_positionCol.y/(Settings::TILE_SIZE*Chunk::N_TILES_Y))*100;
    if(heightFactor<50) heightFactor =0;
    else{
        heightFactor = (heightFactor-50)*2;
    }
    if(caveFactor<50) caveFactor =0;
    else{
        caveFactor = (caveFactor-50)*2;
    }
    float global_temp = Scene::getScene()->getTemperature(_positionCol);
    float local_temp = Scene::getScene()->getTemperatureGlobal(_positionCol);
    float total_temp = global_temp + local_temp;
    if(total_temp<0) rainFactor = std::max(0.f,rainFactor+2*total_temp);
    SoundManager::playSoundNoRestart("rain0");
    SoundManager::playSoundNoRestart("rain1");
    SoundManager::playSoundNoRestart("rain2");
    SoundManager::playSoundNoRestart("rain3");
    SoundManager::playSoundNoRestart("day");
    SoundManager::playSoundNoRestart("night");
    SoundManager::playSoundNoRestart("wind");
    SoundManager::playSoundNoRestart("cave");
    if(rainFactor<=25){
        SoundManager::setVolumeSound(std::max(rainFactor*4-caveFactor,0.f),"rain0");
        SoundManager::setVolumeSound(0,"rain1");
        SoundManager::setVolumeSound(0,"rain2");
        SoundManager::setVolumeSound(0,"rain3");
    } else if(rainFactor<=50){
        SoundManager::setVolumeSound(100,"rain0");
        SoundManager::setVolumeSound(std::max((rainFactor-25)*4-caveFactor,0.f),"rain1");
        SoundManager::setVolumeSound(0,"rain2");
        SoundManager::setVolumeSound(0,"rain3");
    } else if(rainFactor<=75){
        SoundManager::setVolumeSound(100,"rain1");
        SoundManager::setVolumeSound(std::max((rainFactor-50)*4-caveFactor,0.f),"rain2");
        SoundManager::setVolumeSound(std::max(100-(rainFactor-50)*4-caveFactor,0.f),"rain0");
        SoundManager::setVolumeSound(0,"rain3");
    } else if(rainFactor<=75){
        SoundManager::setVolumeSound(std::max(100-caveFactor,0.f),"rain2");
        SoundManager::setVolumeSound(std::max((rainFactor-75)*4-caveFactor,0.f),"rain3");
        SoundManager::setVolumeSound(0,"rain0");
        SoundManager::setVolumeSound(std::max(100-(rainFactor-75)*4-caveFactor,0.f),"rain1");
    }
    if(lightFactor <50){
        SoundManager::setVolumeSound(std::max(100-lightFactor*2-rainFactor,0.f),"night");
        SoundManager::setVolumeSound(std::max(lightFactor-rainFactor-heightFactor-caveFactor,0.f),"day");

    } else if(lightFactor >=50){
        SoundManager::setVolumeSound(std::max(lightFactor-rainFactor-heightFactor-caveFactor,0.f),"day");
        SoundManager::setVolumeSound(0,"night");
    }
    SoundManager::setVolumeSound(heightFactor,"wind");
    SoundManager::setVolumeSound(caveFactor,"cave");

}
bool Player::giveItem(std::string id_item, int amount_item){

	int res = inventory->stackItem(id_item, amount_item);
	if(res==0) return true;
	else return false;
}
Inventory* Player::getInventory(){
    return inventory;
}
void Player::SetPosition(float x, float y)
{
        if(_playerDirection==RIGHT) {
            _sprite.setScale(sf::Vector2f(1,1));
            _position = sf::Vector2f(x-(PLAYER_SPRITE_SIZE-PLAYER_WIDTH-PLAYER_WIDTH/2),y-(PLAYER_SPRITE_SIZE-PLAYER_HEIGHT));
            _sprite.setPosition(x-(PLAYER_SPRITE_SIZE-PLAYER_WIDTH-PLAYER_WIDTH/2),y-(PLAYER_SPRITE_SIZE-PLAYER_HEIGHT));
        }
        else {
            _sprite.setScale(sf::Vector2f(-1,1));
            _position = sf::Vector2f(x-(PLAYER_SPRITE_SIZE-PLAYER_WIDTH-PLAYER_WIDTH/2)+64,y-(PLAYER_SPRITE_SIZE-PLAYER_HEIGHT));
            _sprite.setPosition(x-(PLAYER_SPRITE_SIZE-PLAYER_WIDTH-PLAYER_WIDTH/2)+64,y-(PLAYER_SPRITE_SIZE-PLAYER_HEIGHT));
        }
        _spriteTool.setScale(_sprite.getScale());
        _spriteTool.setPosition(_sprite.getPosition());
        _positionCol = sf::Vector2f(x,y);
}
void Player::SetSize(float x)
{
	sf::Vector2f new_scale(x/PLAYER_SPRITE_SIZE, x/PLAYER_SPRITE_SIZE);
	_sprite.setScale(new_scale);
    _spriteTool.setScale(_sprite.getScale());
}

sf::Vector2f Player::GetPosition() const
{
	return _positionCol;
}

void Player::hurt(float amount){
    sf::Texture *t = Resources::getTexture("entities");
    _spriteHurt.setTexture(*t);
    _currHurtAnim = (rand()%3+4);
    if(_armor==ARMOR3) amount = amount/3;
    _keyframeHurt=0;
    _hurted = true;
    _health -= amount;
    int num = std::rand()%3+1;
    SoundManager::playSound("p_hurt_"+std::to_string(num));
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
	std::string x = std::to_string(_positionCol.x);
	std::string y = std::to_string(_positionCol.y);
	myfile << x << " " << y << "\n";
    myfile << _health << "\n";
    myfile << _hunger << "\n";
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
        myfile >> _health >> _hunger;
		SetPosition(std::stof(x),std::stof(y));

        inventory->loadData(myfile);
		myfile.close();
	}
}

