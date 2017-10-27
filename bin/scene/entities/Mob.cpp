//
// Created by arnito on 11/10/17.
//
#include <fstream>
#include <sstream>
#include "Mob.h"
#include "../../Settings.h"
#include "../NoiseGenerator.h"
#include "../world/Chunk.h"
#include "../Scene.h"
#include "../../Resources.h"

Mob::Mob(): Entity("mob"),Colisionable(),_gens()
             {
    _target = nullptr;
    _dead = true;
    //_temperature = 0;
    //_humidity = 0;
    _position = sf::Vector2f(0,0);
    _positionCol = _position;
    _positionSpawn = sf::Vector2i(_position.x,_position.y);
    vx =0;
    vy = 0;
    _sizeCol = sf::Vector2i(64,64);
    _mobDecision = IDLE;
    _mobDirection = LEFT;
    _timeToReproduce = rand() % std::max(_gens._reproduceFactor,1) + 10;
    _life = rand() % std::max(_gens._health,1) + 10;
    _hunger = rand() % std::max(_gens._foodNeeds,1) + 10;
    _attackColdown = 0;

                 ___addModules();



}
Mob::Mob(MobGenetics* t,int chunk, sf::Vector2f position): Entity("mob"),Colisionable(){
    _gens = *t;
    _chunk = chunk;
    _target = nullptr;
    vx =0;
    vy = 0;
    _mobDirection = LEFT;
    _sizeCol = sf::Vector2i(64,64);
    setPosition(position);
    _positionSpawn = sf::Vector2i(_position.x,_position.y);
    _mobDecision = IDLE;
    _timeToReproduce = rand() % std::max(_gens._reproduceFactor,1) + 10;
    _life = rand() % std::max(_gens._health,1) + 10;
    _hunger = rand() % std::max(_gens._foodNeeds,1) + 10;
    _attackColdown = 0;
    _dead = false;
    ___addModules();

}
Mob::Mob(MobGenetics* t1, MobGenetics* t2,int chunk, sf::Vector2f position, int index): Entity("mob"),Colisionable(),_gens(t1,t2, (float)(t1->_strenghtGen)/100){
    _chunk = chunk;
    _target = nullptr;
    vx =0;
    vy = 0;
    _mobDirection = LEFT;
    _mobDecision = IDLE;
    _sizeCol = sf::Vector2i(64,64);
    setPosition(position);
    _positionSpawn = sf::Vector2i(_position.x,_position.y);
    setEcosystemIndex(index);
    _timeToReproduce = rand() % std::max(_gens._reproduceFactor,1) + 10;
    _life = rand() % std::max(_gens._health,1) + 10;
    _hunger = rand() % std::max(_gens._foodNeeds,1) + 10;
    _attackColdown = 0;
    _dead = false;
    ___addModules();

}
void Mob::___addModules(){
    MobModule* mm1 = new MobModule("leg1",0,sf::Vector2f(0,20),1);
    mm1->addAnimation("idle",0,1);
    mm1->addAnimation("walking",1,3);
    _modules.push_back(mm1);
    MobModule* mm2 = new MobModule("leg2",1,sf::Vector2f(0,20),1);
    mm2->addAnimation("idle",0,1);
    mm2->addAnimation("walking",1,3);
    _modules.push_back(mm2);
    MobModule* mm3 = new MobModule("head",2,sf::Vector2f(0,-40),1);
    mm3->addAnimation("idle",0,1);
    mm3->addAnimation("walking",1,1);
    _modules.push_back(mm3);
    MobModule* mm4 = new MobModule("body",3,sf::Vector2f(0,0),1);
    mm4->addAnimation("idle",0,1);
    mm4->addAnimation("walking",0,1);
    _modules.push_back(mm4);
    MobModule* mm5 = new MobModule("compl",4,sf::Vector2f(-40,-80),0.5);
    mm5->addAnimation("idle",0,1);
    mm5->addAnimation("walking",1,1);
    _modules.push_back(mm5);
    sf::FloatRect boundBox = getBoundingBox();
    _positionCol.x= boundBox.left;
    _positionCol.y= boundBox.top;

    _sizeCol.x= boundBox.width-boundBox.left;
    _sizeCol.y= boundBox.height-boundBox.top;
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
    for(int i=0; i< _modules.size(); i++){
        _modules[i]->draw(renderTar);
    }
}
void Mob::setPosition(sf::Vector2f position){
    _position = position;
    _positionCol = position;
    sf::FloatRect boundBox = getBoundingBox();
    _positionCol.x= boundBox.left;
    _positionCol.y= boundBox.top;

    _sizeCol.x= boundBox.width-boundBox.left;
    _sizeCol.y= boundBox.height-boundBox.top;
}
void Mob::saveToFile(int chunk, std::ofstream &myfile){
    myfile << "mob" << " ";
    myfile << chunk << " ";
    myfile << _position.x << " " << _position.y << " ";
    myfile << _positionSpawn.x << " " << _positionSpawn.y << " ";
    myfile << _sizeCol.x << " " << _sizeCol.y << " ";
    myfile << _life << " " << _timeToReproduce << " " << _hunger << " ";
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
    myfile >> _life >> _timeToReproduce >> _hunger;
    myfile >> _gens._cold >> _gens._hot >> _gens._humidity;
    myfile >> _gens._health >> _gens._reproduceFactor >> _gens._strenghtGen;
    myfile >> _gens._distanceMaxMove >> _gens._distanceMaxReproduce;
    _positionCol = _position;
    sf::FloatRect boundBox = getBoundingBox();
    _positionCol.x= boundBox.left;
    _positionCol.y= boundBox.top;
    _sizeCol.x= boundBox.width;
    _sizeCol.y= boundBox.height;

}

Mob::~Mob(){

}
int Mob::getRelationMob(int idRace){
    for(int i=0; i<_gens._enemys.size(); i++){
        if(_gens._enemys[i]==idRace) return 0;
    }
    for(int i=0; i<_gens._friends.size(); i++){
        if(_gens._friends[i]==idRace) return 1;
    }
    for(int i=0; i<_gens._food.size(); i++){
        if(_gens._food[i]==idRace) return 2;
    }
    for(int i=0; i<_gens._neutral.size(); i++){
        if(_gens._neutral[i]==idRace) return 3;
    }
}
void Mob::searchNeighbors(std::vector<Mob*> enemys,std::vector<Mob*> friends,std::vector<Mob*> neutral,std::vector<Mob*> food){
    Scene* s = Scene::getScene();
    std::vector<Mob*> neighbours;
    s->getMobsOnArea(neighbours,sf::Vector2i(_position),_gens._distanceMaxMove*100,_ecosystemIndex);
    for(int i=0; i<neighbours.size(); i++){
        int race = neighbours[i]->_gens._race;
        int relation = getRelationMob(race);
        if(relation==0){
            enemys.push_back(neighbours[i]);
        }
        else if(relation==1){
            friends.push_back(neighbours[i]);
        }
        else if(relation==2){
            food.push_back(neighbours[i]);
        }
        else if(relation==3){
            neutral.push_back(neighbours[i]);
        }
    }
}
Entity* Mob::searchFoodTarget(){
    Scene* s = Scene::getScene();
    std::vector<Entity*> v;
    Entity* res = nullptr;
    float dist = -1;
    s->getEntityesArea(v,sf::Vector2i(_position),_gens._distanceMaxMove*100,_ecosystemIndex);
    for(int i=0; i<v.size(); i++){
        sf::Vector2f posE = v[i]->getPositionCol();
        int distance = sqrt((posE.x-_positionCol.x)*(posE.x-_positionCol.x)+(posE.y-_positionCol.y)*(posE.y-_positionCol.y));
        if(dist== -1 || distance < dist){
            dist = distance;
            res = v[i];
        }
    }
    return res;
}
Mob* Mob::searchMobTarget(std::vector<Mob*> &mobs){
    Mob* res = nullptr;
    float dist = -1;
    for(int i=0; i<mobs.size(); i++){
        sf::Vector2f posE = mobs[i]->getPositionCol();
        int distance = sqrt((posE.x-_positionCol.x)*(posE.x-_positionCol.x)+(posE.y-_positionCol.y)*(posE.y-_positionCol.y));
        if(dist== -1 || distance < dist){
            dist = distance;
            res = mobs[i];
        }
    }
    return res;
}
bool Mob::update(float delta, Clock *c){
    _attackColdown = std::max(0.f,_attackColdown-delta*10);
    Map* map = Scene::getScene()->getMap();
    bool is_visible_chunk = map->getIndexMatChunk(map->getChunkIndex(_positionCol.x)) != -1;
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
    std::vector<Mob*> enemys;
    std::vector<Mob*> friends;
    std::vector<Mob*> neutral;
    std::vector<Mob*> food;
    searchNeighbors(enemys, friends, neutral, food);
    if(_life<=0) {
        std::cout << "a mob died" << std::endl;
        _dead = true;
        return false;
    }
    else {
        _timeToReproduce -= (delta/10)*Settings::GEN_SPEED;
        if(_timeToReproduce <0) {

            return true;
        }
        _hunger -= (delta/10)*Settings::GEN_SPEED;

        if(_hunger < 0){
            Entity* foodTar = searchFoodTarget();
            if(is_visible_chunk){
                if(foodTar == nullptr){
                    Mob* m = searchMobTarget(food);
                    if(m != nullptr) _target = m;
                } else _target = foodTar;
            } else{
                if(foodTar == nullptr){
                    Mob* m = searchMobTarget(food);
                    if(m != nullptr) {
                        simulateCombat(m);
                        _hunger = _gens._foodNeeds;
                    }
                } else {
                    _hunger = _gens._foodNeeds;
                    std::cout << "mob eating far away" << std::endl;
                }
            }

            _life -= (delta/10);
        } else if(_hunger < 10){

        } else{

            Mob* m = searchMobTarget(enemys);
            if(is_visible_chunk) {
                if (m != nullptr) _target = m;
            } else{
                if (m != nullptr) simulateCombat(m);
            }
        }
    }
    if(is_visible_chunk && isNearTarget()){
        targetAction();
    }

    return false;
}
sf::FloatRect Mob::getBoundingBox(){
    float left = _position.x;
    float right = _position.x+64;
    float top = _position.y;
    float bot = _position.y+64;
    bool first = true;
    for(int i=0; i<_modules.size(); i++){
        sf::FloatRect bb = _modules[i]->getBoundingBox(_position,2);
        if(first){
            left = bb.left;
            right = bb.width;
            top = bb.top;
            bot = bb.height;
            first = false;
        } else{
            left = std::min(bb.left,left);
            right = std::max(bb.width,right);
            top = std::min( bb.top,top);
            bot = std::max(bb.height,bot);
        }
    }
    return sf::FloatRect(left,top,right,bot);
}
void Mob::simulateCombat(Mob* m){
    std::cout << "mob atackking far!!!!!!!!!!" << std::endl;
    float dps1 = _gens._atackSpeed*_gens._strenght*0.01;
    float dps2 = m->_gens._atackSpeed*m->_gens._strenght*0.01;
    float time1 = m->_life/dps1;
    float time2 = _life/dps2;
    if(time1<time2){
        m->_life=-1;
        _life -= time1*dps2;
    } else{
        _life=-1;
        m->_life -= time2*dps1;
    }
}
void Mob::attack(Mob* m){
    m->_target = this;
    if(_attackColdown ==0){
        m->_life -= _gens._strenght;
        _attackColdown= 100-_gens._atackSpeed+10;
        std::cout << "mob atackking!!!!!!!!!!" << std::endl;
    }

}
void Mob::targetAction() {
    if(_target->_typeEntity=="mob"){
        Mob *m = static_cast<Mob*>(_target);
        attack(m);
    } else{
        _target->_removed = true;
        _hunger = _gens._foodNeeds;
        std::cout << "mob eating" << std::endl;
    }
}
bool Mob::isNearTarget() {
    if(_target !=nullptr && !_target->_removed){
        sf::Vector2f posE = _target->getPositionCol();
        int distance = sqrt((posE.x-_positionCol.x)*(posE.x-_positionCol.x)+(posE.y-_positionCol.y)*(posE.y-_positionCol.y));
        if(distance<DISTANCE_NEAR) return true;
        return false;
    } else _target = nullptr;
    return false;
}
void Mob::updateVisible(float delta){
    int newDecision;
    //MODULES
    for(int i = 0; i<_modules.size(); i++){

        if(vx !=0) _modules[i]->update(delta,_position,2,"walking",_mobDirection);
        else _modules[i]->update(delta,_position,2,"idle",_mobDirection);
    }
    if(_target == nullptr) newDecision = rand() % 1000;
    else {

        if(_target->getPositionCol().x > getPositionCol().x) newDecision = 1;
        else newDecision = 0;
    }


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

    sf::Vector2f newPosCol = evalCollisions(sf::Vector2f(x0,y0),sf::Vector2f(x,y),sf::Vector2f(_sizeCol.x,_sizeCol.y));
    int distance_from_base = sqrt((newPosCol.x-_positionSpawn.x)*(newPosCol.x-_positionSpawn.x)+(newPosCol.y-_positionSpawn.y)*(newPosCol.y-_positionSpawn.y));
    if(distance_from_base< _gens._distanceMaxMove*100);
    sf::Vector2f displacement(newPosCol.x-x0, newPosCol.y-y0);
    sf::Vector2f newPos(_position.x+displacement.x, _position.y+displacement.y);
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
    s->getPositionsOnArea(reproduce_spots,sf::Vector2i(_position),_gens._distanceMaxReproduce*100+256,_ecosystemIndex);
    if(reproduce_spots.size() >0){
        s->getMobsOnArea(neighbours,sf::Vector2i(_position),_gens._distanceMaxReproduce*100+256,_ecosystemIndex);
        sf::Vector2i position = reproduce_spots[rand() % reproduce_spots.size()];
        //position.y -= Settings::TILE_SIZE;
        Mob *partner = neighbours[rand() % neighbours.size()];
        int chunk = Map::getChunkIndex(position.x);
        result = new Mob(getGenetics(), partner->getGenetics(),chunk,sf::Vector2f(position),_ecosystemIndex);
        _timeToReproduce = _gens._reproduceFactor+OFFSET_REPRODUCE;
        partner->_timeToReproduce = partner->_gens._reproduceFactor+OFFSET_REPRODUCE;
    }
    return result;


}
void Mob::kill(){
    _removed = true;

}
