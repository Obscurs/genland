//
// Created by arnito on 11/10/17.
//
#include <fstream>
#include "Mob.h"
#include "../../Settings.h"
#include "../NoiseGenerator.h"
#include "../world/Chunk.h"
#include "../Scene.h"
#include "../../Resources.h"

Mob::Mob(): Entity("mob"),Colisionable(),_gens()
             {

    _dead = true;
    _chunk = 0;
    _life = 0;
    //_temperature = 0;
    //_humidity = 0;
    _timeToReproduce = 0;
    _position = sf::Vector2f(0,0);
    _positionCol = _position;
    _positionSpawn = sf::Vector2i(_position.x,_position.y);
    vx =0;
    vy = 0;
    _sizeCol = sf::Vector2i(64,64);
    _mobDecision = IDLE;
    _mobDirection = LEFT;
}
Mob::Mob(MobGenetics* t,int chunk, sf::Vector2f position): Entity("mob"),Colisionable(){
    _gens = *t;
    _chunk = chunk;
    vx =0;
    vy = 0;
    _mobDirection = LEFT;
    _sizeCol = sf::Vector2i(64,64);
    setPosition(position);
    _positionSpawn = sf::Vector2i(_position.x,_position.y);
    _mobDecision = IDLE;
    _timeToReproduce = rand() % _gens._reproduceFactor + 10;
    _life = rand() % _gens._health + 10;

}
Mob::Mob(MobGenetics* t1, MobGenetics* t2,int chunk, sf::Vector2f position, int index): Entity("mob"),Colisionable(),_gens(t1,t2, (float)(t1->_strenghtGen)/100){
    _chunk = chunk;
    vx =0;
    vy = 0;
    _mobDirection = LEFT;
    _mobDecision = IDLE;
    _sizeCol = sf::Vector2i(64,64);
    setPosition(position);
    _positionSpawn = sf::Vector2i(_position.x,_position.y);
    setEcosystemIndex(index);
}
void Mob::draw(sf::RenderTarget & renderTar) {
    if (!_removed && !_dead) {
        sf::Texture *t = Resources::getTexture("entities");
        _sprite.setTexture(*t);
        _sprite.setTextureRect(sf::IntRect(0,128,64,64));
        _sprite.setPosition(getPosition().x, getPosition().y);

        if(_mobDirection==RIGHT) {
            _sprite.setScale(sf::Vector2f(1,1));
            _sprite.setPosition(getPosition().x-(64-64-64/2),getPosition().y-(64-64));
        }
        else {
            _sprite.setScale(sf::Vector2f(-1,1));
            _sprite.setPosition(getPosition().x-(64-64-64/2)+64,getPosition().y-(64-64));
        }

        renderTar.draw(_sprite);
    }
}
void Mob::setPosition(sf::Vector2f position){
    _position = position;
    _positionCol = position;
}
void Mob::saveToFile(int chunk, std::ofstream &myfile){
    myfile << "mob" << " ";
    myfile << chunk << " ";
    myfile << _position.x << " " << _position.y << " ";
    myfile << _positionSpawn.x << " " << _positionSpawn.y << " ";
    myfile << _sizeCol.x << " " << _sizeCol.y << " ";
    myfile << _life << " " << _timeToReproduce << " ";
    myfile << _gens._cold << " " << _gens._hot << " " << _gens._humidity << " ";
    myfile << _gens._health << " " << _gens._reproduceFactor << " " << _gens._strenghtGen << " ";
    myfile << _gens._distanceMaxMove << " " << _gens._distanceMaxReproduce << " ";
}
void Mob::loadFromFile(std::ifstream &myfile){
    _dead = false;
    myfile >> _chunk;
    myfile >> _position.x;
    myfile >> _position.y;
    myfile >> _positionSpawn.x;
    myfile >> _positionSpawn.y;
    myfile >> _sizeCol.x;
    myfile >> _sizeCol.y;
    myfile >> _life >> _timeToReproduce;
    myfile >> _gens._cold >> _gens._hot >> _gens._humidity;
    myfile >> _gens._health >> _gens._reproduceFactor >> _gens._strenghtGen;
    myfile >> _gens._distanceMaxMove >> _gens._distanceMaxReproduce;
    _positionCol = _position;
}

Mob::~Mob(){

}
bool Mob::update(float delta, Clock *c){
    Simplex2d* base_noise_temperature = NoiseGenerator::getNoise("base_noise_temperature");
    Simplex2d* noise_humidity = NoiseGenerator::getNoise("noise_humidity");
    int y_pos = Chunk::N_TILES_Y-1-(_positionCol.y/Settings::TILE_SIZE);
    float height_factor = float(y_pos)/float(Chunk::N_TILES_Y);
    float heightTemp = (1-height_factor)*(Settings::MAX_TEMPERATURE-Settings::MIN_TEMPERATURE)+Settings::MIN_TEMPERATURE;

    int valHumidity = int(noise_humidity->valSimplex2D(0, _positionCol.x));
    int valTemperature = int(heightTemp)+int(base_noise_temperature->valSimplex2D(0, _positionCol.x));

    int totalTemp = valTemperature + c->_globalTemperature;
    int totalHum = valHumidity + c->_globalHumidity;
    float humDamage = totalHum*(1-float(_gens._humidity)/100)*delta;
    float tempDamage;
    if(totalTemp>0) tempDamage = totalTemp*(1-float(_gens._hot)/100)*delta;
    else tempDamage = -totalTemp*(1-float(_gens._cold)/100)*delta;
    _life -= ((tempDamage+humDamage+delta)/2000)*Settings::GEN_SPEED;
    if(_life<=0) {
        std::cout << "a mob died" << std::endl;
        _dead = true;
        return false;
    }
    else {
        _timeToReproduce -= (delta/10)*Settings::GEN_SPEED;
        if(_timeToReproduce <0) {
            std::cout << "new mob created" << std::endl;
            return true;
        }
    }
    return false;
}
void Mob::updateVisible(float delta){
    //vx = -PLAYER_SPEED_X;

    int newDecision = rand() % 1000;
    if(newDecision ==0){
        _mobDecision = GO_LEFT;
    }
    else if(newDecision ==1){
        _mobDecision = GO_RIGHT;
    }
    else if(newDecision ==2){
        _mobDecision = IDLE;
    }
    if (_mobDecision == GO_LEFT){
        vx = -100;
    }
    else if (_mobDecision == GO_RIGHT){
        vx = 100;
    } else vx =0;

    if(col_bottom==0){
        vy = (float)9.8*delta*100 + vy;
    } else if(col_bottom>0 && col_left>0 && _mobDecision == GO_LEFT){
        vy = -400;
    } else if(col_bottom>0 && col_right>0 && _mobDecision == GO_RIGHT){
        vy = -400;
    }
    else{
        vy = 0;
    }


    if(col_top != 0){
        vy = 0;
    }
    if(vx >0) {
        _mobDirection = RIGHT;
    }
    else if(vx < 0) {
        _mobDirection = LEFT;
    }
    float x0 = _positionCol.x;
    float y0 = _positionCol.y;
    float x = x0+vx*delta;
    float y = y0+vy*delta;

    sf::Vector2f newPos = evalCollisions(sf::Vector2f(x0,y0),sf::Vector2f(x,y),sf::Vector2f(_sizeCol.x,_sizeCol.y));
    int distance_from_base = sqrt((newPos.x-_positionSpawn.x)*(newPos.x-_positionSpawn.x)+(newPos.y-_positionSpawn.y)*(newPos.y-_positionSpawn.y));
    if(distance_from_base< _gens._distanceMaxMove*100);
    setPosition(newPos);
}
MobGenetics* Mob::getGenetics(){
    return &_gens;
}
Mob * Mob::reproduce(){
    Scene *s = Scene::getScene();
    std::vector<Mob*> neighbours;
    std::vector<sf::Vector2i> reproduce_spots;
    Mob *result = nullptr;
    s->getPositionsOnArea(reproduce_spots,sf::Vector2i(_position),_gens._distanceMaxReproduce*100,_ecosystemIndex);
    if(reproduce_spots.size() >0){
        s->getMobsOnArea(neighbours,sf::Vector2i(_position),_gens._distanceMaxReproduce*100,_ecosystemIndex);
        sf::Vector2i position = reproduce_spots[rand() % reproduce_spots.size()];
        Mob *partner = neighbours[rand() % neighbours.size()];
        int chunk = Map::getChunkIndex(position.x);
        result = new Mob(getGenetics(), partner->getGenetics(),chunk,sf::Vector2f(position),_ecosystemIndex);
        _timeToReproduce = _gens._reproduceFactor+OFFSET_REPRODUCE;
        partner->_timeToReproduce = partner->_gens._reproduceFactor+OFFSET_REPRODUCE;
    }
    return result;


}
void Mob::kill(){
    /*
    Tree *tl = getLeftTree();
    Tree *tr = getRightTree();
    if(tl != nullptr) tl->setRightTree(tr);
    if(tr != nullptr) tr->setLeftTree(tl);
     */
}