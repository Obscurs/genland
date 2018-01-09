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
#include "../../SoundManager.hpp"

//Load from file
Mob::Mob(): Entity("mob"),Colisionable(),_gens()
             {
    _target = nullptr;
    _dead = true;

    //_temperature = 0;
    //_humidity = 0;
    _position = sf::Vector2f(0,0);
                 _position.y -=128;
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
    _age = rand() % std::max(_gens._age,1) + 10;
    _attackColdown = 0;
                 _mobType = -1;
    _keyframe =0;
                 _currHurtAnim = 0;
                 _keyframeHurt =0;
                 _spriteTime = 0;
                 _spriteTimeHurt = 0;
                 _focusDebug = false;
                 _dying = false;
                 _dieTime = 10;
                 _bfsCounter = 0;

}
//Random spawn creator
Mob::Mob(MobGenetics* t,int chunk, sf::Vector2f position): Entity("mob"),Colisionable(){
    _keyframe =0;
    _keyframeHurt =0;
    _currHurtAnim = 0;
    _gens = *t;
    _chunk = chunk;
    _target = nullptr;
    vx =0;
    vy = 0;
    _focusDebug = false;
    _mobDirection = LEFT;
    _sizeCol = sf::Vector2i(64,64);
    position.y -=128;
    setPosition(position);
    _positionSpawn = sf::Vector2i(_position.x,_position.y);
    _mobDecision = IDLE;
    _timeToReproduce = rand() % std::max(_gens._reproduceFactor,1) + 10;
    _life = rand() % std::max(_gens._health,1) + 10;
    _hunger = rand() % std::max(_gens._foodNeeds,1) + 10;
    _age = rand() % std::max(_gens._age,1) + 10;
    _attackColdown = 0;
    _dead = false;
    _mobType = -1;
    _spriteTime = 0;
    _spriteTimeHurt = 0;
    _dying = false;
    _dieTime = 10;
    _bfsCounter = 0;
    createRandomBody();

}
//Reproduction spawn
Mob::Mob(MobGenetics* t1, MobGenetics* t2,std::vector<MobModule*>& modulesPartner1, int typeMobPartner1,std::vector<MobModule*>& modulesPartner2, int typeMobPartner2,int chunk, sf::Vector2f position, int index): Entity("mob"),Colisionable(),_gens(t1,t2, (float)(t1->_strenghtGen)/100){
    _keyframe =0;
    _keyframeHurt =0;
    _spriteTime = 0;
    _spriteTimeHurt = 0;
    _currHurtAnim = 0;
    _chunk = chunk;
    _target = nullptr;
    vx =0;
    vy = 0;
    _mobDirection = LEFT;
    _mobDecision = IDLE;
    _sizeCol = sf::Vector2i(64,64);
    _focusDebug = false;
    position.y -=128;
    setPosition(position);
    _positionSpawn = sf::Vector2i(_position.x,_position.y);
    setEcosystemIndex(index);
    _timeToReproduce = rand() % std::max(_gens._reproduceFactor,1) + 10;
    _life = _gens._health;
    _hunger = _gens._foodNeeds;
    _age = _gens._age;
    _attackColdown = 0;
    _dead = false;
    _mobType = -1;
    _dying = false;
    _dieTime = 10;
    _bfsCounter = 0;
    mixModules(modulesPartner1,typeMobPartner1,modulesPartner2,typeMobPartner2,t1,t2);

}
void Mob::mixModules(std::vector<MobModule*>& modu1, int typeMobPartner1,std::vector<MobModule*>& modu2, int typeMobPartner2,MobGenetics* t1, MobGenetics* t2){
    int headAux = -1;
    int handAux = -1;
    int bodyAux = -1;
    int leg1Aux = -1;
    int leg2Aux = -1;
    int compAux = -1;

    if(t1->_strenghtGen > t2->_strenghtGen){
        _mobType = typeMobPartner1;
        for(int i=0; i< modu2.size(); i++){
            if(std::rand() % 100 < t2->_strenghtGen){
                if(modu2[i]->_typeModule=="head"){
                    headAux = modu2[i]->getIdModule();
                } else if(modu2[i]->_typeModule=="hand"){
                    handAux = modu2[i]->getIdModule();
                } else if(modu2[i]->_typeModule=="body"){
                    bodyAux = modu2[i]->getIdModule();
                } else if(modu2[i]->_typeModule=="leg"){
                    leg1Aux = modu2[i]->getIdModule();
                } else if(modu2[i]->_typeModule=="legBack"){
                    leg2Aux = modu2[i]->getIdModule();
                } else if(modu2[i]->_typeModule=="complement"){
                    compAux = modu2[i]->getIdModule();
                }
            }
        }
        for(int i=0; i<modu1.size(); i++){
            MobModule* fatherModule = modu1[i];
            int id;
            if(fatherModule->_typeModule=="head"){
                if(headAux == -1) id = fatherModule->getIdModule();
                else id = headAux;
            } else if(fatherModule->_typeModule=="hand"){
                if(handAux == -1) id = fatherModule->getIdModule();
                else id = handAux;
            } else if(fatherModule->_typeModule=="body"){
                if(bodyAux == -1) id = fatherModule->getIdModule();
                else id = bodyAux;
            } else if(fatherModule->_typeModule=="leg"){
                if(leg1Aux == -1) id = fatherModule->getIdModule();
                else id = leg1Aux;
            } else if(fatherModule->_typeModule=="legBack"){
                if(leg2Aux == -1) id = fatherModule->getIdModule();
                else id = leg2Aux;
            } else if(fatherModule->_typeModule=="complement"){
                if(compAux == -1) id = fatherModule->getIdModule();
                else id = compAux;
            }
            MobModule* newModule = new MobModule(fatherModule->_typeModule,id,fatherModule->getOffset(),fatherModule->getScale());
            _modules.push_back(newModule);
        }
    } else{
        _mobType = typeMobPartner2;
        for(int i=0; i< modu1.size(); i++){
            if(std::rand() % 100 < t2->_strenghtGen){
                if(modu1[i]->_typeModule=="head"){
                    headAux = modu1[i]->getIdModule();
                } else if(modu1[i]->_typeModule=="hand"){
                    handAux = modu1[i]->getIdModule();
                } else if(modu1[i]->_typeModule=="body"){
                    bodyAux = modu1[i]->getIdModule();
                } else if(modu1[i]->_typeModule=="leg"){
                    leg1Aux = modu1[i]->getIdModule();
                } else if(modu1[i]->_typeModule=="legBack"){
                    leg2Aux = modu1[i]->getIdModule();
                } else if(modu1[i]->_typeModule=="complement"){
                    compAux = modu1[i]->getIdModule();
                }
            }
        }
        for(int i=0; i<modu2.size(); i++){
            MobModule* fatherModule = modu2[i];
            int id;
            if(fatherModule->_typeModule=="head"){
                if(headAux == -1) id = fatherModule->getIdModule();
                else id = headAux;
            } else if(fatherModule->_typeModule=="hand"){
                if(handAux == -1) id = fatherModule->getIdModule();
                else id = handAux;
            } else if(fatherModule->_typeModule=="body"){
                if(bodyAux == -1) id = fatherModule->getIdModule();
                else id = bodyAux;
            } else if(fatherModule->_typeModule=="leg"){
                if(leg1Aux == -1) id = fatherModule->getIdModule();
                else id = leg1Aux;
            } else if(fatherModule->_typeModule=="legBack"){
                if(leg2Aux == -1) id = fatherModule->getIdModule();
                else id = leg2Aux;
            } else if(fatherModule->_typeModule=="complement"){
                if(compAux == -1) id = fatherModule->getIdModule();
                else id = compAux;
            }
            MobModule* newModule = new MobModule(fatherModule->_typeModule,id,fatherModule->getOffset(),fatherModule->getScale());
            _modules.push_back(newModule);
        }
    }
}
void Mob::createRandomBody(){
    int type = std::rand() %3;
    _mobType = type;
    if(type==0){
        //humanoid
        MobModule* modHead = new MobModule("head",std::rand() % MobModule::NUM_HEADS,sf::Vector2f(0,-40),1);
        MobModule* modBody = new MobModule("body",std::rand() % MobModule::NUM_BODIES,sf::Vector2f(0,0),1);
        int legId = std::rand() % MobModule::NUM_LEGS;
        MobModule* modLeg1 = new MobModule("leg",legId,sf::Vector2f(0,50),1);
        MobModule* modLeg2 = new MobModule("legBack",legId,sf::Vector2f(0,50),1);
        MobModule* modHand = new MobModule("hand",std::rand() % MobModule::NUM_HANDS,sf::Vector2f(0,0),1);
        if(std::rand()%2){
            MobModule* modComplement = new MobModule("complement",std::rand() % MobModule::NUM_COMPLEMENTS,sf::Vector2f(0,-80),1);
            _modules.push_back(modComplement);
        }

        _modules.push_back(modLeg2);
        _modules.push_back(modBody);
        _modules.push_back(modHead);
        _modules.push_back(modLeg1);
        _modules.push_back(modHand);
    } else if(type==1){
        //quadruped
        MobModule* modHead = new MobModule("head",std::rand() % MobModule::NUM_HEADS,sf::Vector2f(-40,0),1);
        MobModule* modBody = new MobModule("body",std::rand() % MobModule::NUM_BODIES,sf::Vector2f(0,0),1);
        int legId = std::rand() % MobModule::NUM_LEGS;
        MobModule* modLeg1 = new MobModule("leg",legId,sf::Vector2f(-20,60),1);
        MobModule* modLeg2 = new MobModule("legBack",legId,sf::Vector2f(-20,60),1);
        MobModule* modLeg3 = new MobModule("leg",legId,sf::Vector2f(20,60),1);
        MobModule* modLeg4 = new MobModule("legBack",legId,sf::Vector2f(20,60),1);

        if(std::rand()%2){
            MobModule* modComplement = new MobModule("complement",std::rand() % MobModule::NUM_COMPLEMENTS,sf::Vector2f(-80,-40),1);
            _modules.push_back(modComplement);
        }

        _modules.push_back(modLeg2);
        _modules.push_back(modLeg4);
        _modules.push_back(modBody);
        _modules.push_back(modHead);
        _modules.push_back(modLeg1);
        _modules.push_back(modLeg3);
        if(std::rand()%2){
            MobModule* modHand = new MobModule("hand",std::rand() % MobModule::NUM_HANDS,sf::Vector2f(0,0),1);
            _modules.push_back(modHand);
        }

    } else if(type==2){
        //wormy
        MobModule* modHead = new MobModule("head",std::rand() % MobModule::NUM_HEADS,sf::Vector2f(-60,0),1);

        if(std::rand()%2){
            MobModule* modComplement = new MobModule("complement",std::rand() % MobModule::NUM_COMPLEMENTS,sf::Vector2f(-80,-40),1);
            _modules.push_back(modComplement);
        }
        _modules.push_back(modHead);

        int bodyId = std::rand() % MobModule::NUM_BODIES;
        int numbodies = std::rand() % 4 +1;
        for(int i=0; i<numbodies; i++){
            MobModule* modBody = new MobModule("body",bodyId,sf::Vector2f(32*i-25,0),1);
            _modules.push_back(modBody);
        }
    }
    sf::FloatRect boundBox = getBoundingBox();
    _positionCol.x= boundBox.left;
    _positionCol.y= boundBox.top;

    _sizeCol.x= boundBox.width-boundBox.left;
    _sizeCol.y= boundBox.height-boundBox.top;

}

void Mob::draw(sf::RenderTarget & renderTar) {
    if (!_removed && !_dead) {

        if(_keyframe>2){
            for(int i=0; i< _modules.size(); i++){
                _modules[i]->draw(renderTar);
            }
        }
        if(_keyframe<10){
            sf::Texture *t = Resources::getTexture("entities");
            _spriteSpawn.setTexture(*t);
            _spriteSpawn.setTextureRect(sf::IntRect(0,128,64,64));
            _spriteSpawn.setPosition(_positionSpawn.x, _positionSpawn.y+_sizeCol.y);
            _spriteSpawn.setTextureRect(sf::IntRect(64*_keyframe,64*3,64,64));
            renderTar.draw(_spriteSpawn);
        }
        if(_hurted){
            _spriteHurt.setPosition(_positionCol.x, _positionCol.y);

            _spriteHurt.setTextureRect(sf::IntRect(64*_keyframeHurt,64*_currHurtAnim,64,64));
            renderTar.draw(_spriteHurt);
        }
        if(_focusDebug){

            sf::CircleShape circle1(_gens._distanceMaxReproduce*Settings::RADIUS_MOB_MULTIPLYER+Settings::MIN_RADIUS_MOB);
            circle1.setFillColor(sf::Color(242,107,255,100));
            circle1.setOutlineColor(sf::Color::Red);
            circle1.setPointCount(256);
            circle1.setOutlineThickness(2);
            circle1.setPosition(_positionSpawn.x-circle1.getRadius(),_positionSpawn.y-circle1.getRadius());

            sf::CircleShape circle2(_gens._distanceMaxMove*Settings::RADIUS_MOB_MULTIPLYER+_gens._distanceMaxReproduce*Settings::RADIUS_MOB_MULTIPLYER+Settings::MIN_RADIUS_MOB);
            circle2.setFillColor(sf::Color(166,255,107,50));
            circle2.setOutlineColor(sf::Color::Green);
            circle2.setPointCount(256);
            circle2.setOutlineThickness(3);
            circle2.setPosition(_positionSpawn.x-circle2.getRadius(),_positionSpawn.y-circle2.getRadius());
            renderTar.draw(circle1);
            renderTar.draw(circle2);

            sf::RectangleShape rectangle;
            rectangle.setSize(sf::Vector2f(_sizeCol.x, _sizeCol.y));
            rectangle.setOutlineColor(sf::Color::Red);
            rectangle.setFillColor(sf::Color(150,150,150,100));
            rectangle.setOutlineThickness(1);
            rectangle.setPosition(_positionCol.x, _positionCol.y);
            renderTar.draw(rectangle);
            if(_target != nullptr){
                sf::Vector2f origin(_positionCol.x+_sizeCol.x/2,_positionCol.y+_sizeCol.y/2);
                sf::Vector2f dest(_target->getPositionCol().x+_target->_sizeCol.x/2,_target->getPositionCol().y+_target->_sizeCol.y/2);
                sf::Vertex line[] =
                        {
                                sf::Vertex(origin),
                                sf::Vertex(dest)
                        };
                if(_target->_typeEntity=="mob")line->color.Red;
                else line->color.Green;
                renderTar.draw(line,2,sf::Lines);
            }


        }
    }
    _focusDebug = false;

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
    myfile << _life << " " << _timeToReproduce << " " << _hunger << " " << _age << " " << _mobType << " ";
    myfile << _gens._cold << " " << _gens._hot << " " << _gens._humidity << " " << _gens._playerHostile << " ";
    myfile << _gens._health << " " << _gens._reproduceFactor << " " << _gens._strenghtGen << " ";
    myfile << _gens._distanceMaxMove << " " << _gens._distanceMaxReproduce << " " << _gens._age << " ";
    myfile << _gens._food.size() << " ";
    for(int i=0; i<_gens._food.size(); i++){
        myfile << _gens._food[i] << " ";
    }
    myfile << _gens._enemys.size() << " ";
    for(int i=0; i<_gens._enemys.size(); i++){
        myfile << _gens._enemys[i] << " ";
    }
    myfile << _gens._friends.size() << " ";
    for(int i=0; i<_gens._friends.size(); i++){
        myfile << _gens._friends[i] << " ";
    }
    myfile << _gens._neutral.size() << " ";
    for(int i=0; i<_gens._neutral.size(); i++){
        myfile << _gens._neutral[i] << " ";
    }
    myfile << _modules.size() << " ";
    for(int i=0; i<_modules.size(); i++){
        _modules[i]->saveToFile(myfile);
    }
}
void Mob::loadFromFile(std::ifstream &myfile){
    _dead = false;
    _gens._food.clear();
    _gens._neutral.clear();
    _gens._enemys.clear();
    _gens._friends.clear();
    int num_modules, num_food, num_enemys, num_neutral, num_friends, race;
    myfile >> _chunk;
    myfile >> _position.x;
    myfile >> _position.y;
    myfile >> _positionSpawn.x;
    myfile >> _positionSpawn.y;
    myfile >> _sizeCol.x;
    myfile >> _sizeCol.y;
    myfile >> _life >> _timeToReproduce >> _hunger >> _age >> _mobType;
    myfile >> _gens._cold >> _gens._hot >> _gens._humidity >> _gens._playerHostile;
    myfile >> _gens._health >> _gens._reproduceFactor >> _gens._strenghtGen;
    myfile >> _gens._distanceMaxMove >> _gens._distanceMaxReproduce >> _gens._age;
    myfile >> num_food;
    for(int i=0; i<num_food; i++){
        myfile >> race;
        _gens._food.push_back(race);
    }
    myfile >> num_enemys;
    for(int i=0; i<num_enemys; i++){
        myfile >> race;
        _gens._enemys.push_back(race);
    }
    myfile >> num_friends;
    for(int i=0; i<num_friends; i++){
        myfile >> race;
        _gens._friends.push_back(race);
    }
    myfile >> num_neutral;
    for(int i=0; i<num_neutral; i++){
        myfile >> race;
        _gens._neutral.push_back(race);
    }
    myfile >> num_modules;
    for(int i=0; i<num_modules; i++){
        std::string typeMod;
        int idModule;
        sf::Vector2f offset;
        float localScale;
        myfile >> typeMod >>  idModule >>  offset.x >>  offset.y  >>localScale;
        MobModule* module = new MobModule(typeMod,idModule,offset,localScale);
        _modules.push_back(module);
    }
    _positionCol = _position;
    sf::FloatRect boundBox = getBoundingBox();
    _positionCol.x= boundBox.left;
    _positionCol.y= boundBox.top;
    _sizeCol.x= boundBox.left-boundBox.width;
    _sizeCol.y= boundBox.top-boundBox.height;


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
void Mob::searchNeighbors(std::vector<Mob*> &enemys,std::vector<Mob*> &friends,std::vector<Mob*> &neutral,std::vector<Mob*> &food){
    Scene* s = Scene::getScene();
    std::vector<Mob*> neighbours;
    s->getMobsOnArea(neighbours,sf::Vector2i(_positionSpawn),_gens._distanceMaxMove*Settings::RADIUS_MOB_MULTIPLYER+Settings::MIN_RADIUS_MOB+_gens._distanceMaxReproduce*Settings::RADIUS_MOB_MULTIPLYER,_ecosystemIndex);
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
std::pair<Entity*,Mob::Decision> Mob::getTargetUsingBfs(std::vector<Entity*> &targets, float radius){
    Map* map = Scene::getScene()->getMap();
    Player* player = Scene::getScene()->getPlayer();
    sf::Vector2f first_pos( _positionSpawn.x-radius, _positionSpawn.y-radius);
    sf::Vector2f last_pos( _positionSpawn.x+radius, _positionSpawn.y+radius);
    int cols = 0;
    int rows = 0;
    //CREAMOS LA ESTRUCTURA DEL MAPA A VISITAR
    //std::vector<std::vector<std::pair<bool, Entity*> > > bfsMap;
    for(float i = first_pos.x; i<last_pos.x; i+=Settings::TILE_SIZE){
        rows = 0;
        for(float j = first_pos.y; j < last_pos.y; j+=Settings::TILE_SIZE){
            ++rows;
        }
        ++cols;
    }
    std::pair<bool, Entity*> bfsMap2[rows][cols];
    /*
    for(int i = 0; i<rows; ++i){

        for(int j = 0; j < cols; ++j){
            Tile* t =map->getTile(j,i,1);
            int distance = sqrt((i-_positionCol.x)*(i-_positionCol.x)+(j-_positionCol.y)*(j-_positionCol.y))+Settings::TILE_SIZE;
            //PONEMOS SI EL BLOQUE ES TRAVESABLE O NO
            bool currentValueMap(t != nullptr && t->id =="0" && distance<radius);
            bfsMap2[i][j] = std::pair<bool, Entity*>(currentValueMap, nullptr);

        }

    }
     */
    int ii = 0;
    for(float i = first_pos.x; i<last_pos.x; i+=Settings::TILE_SIZE){
        int jj=0;
        for(float j = first_pos.y; j < last_pos.y; j+=Settings::TILE_SIZE){
            Tile* t =map->getTile(i,j,1);
            int distance = sqrt((i-_positionCol.x)*(i-_positionCol.x)+(j-_positionCol.y)*(j-_positionCol.y))+Settings::TILE_SIZE;
            //PONEMOS SI EL BLOQUE ES TRAVESABLE O NO
            bool currentValueMap(t != nullptr && t->id =="0" && distance<radius);
            bfsMap2[jj][ii] = std::pair<bool, Entity*>(currentValueMap, nullptr);
            ++jj;
        }
        ++ii;
    }
    /*
    for(float i = first_pos.x; i<last_pos.x; i+=Settings::TILE_SIZE){
        std::vector<std::pair<bool, Entity*> > currentRow;
        cols = 0;
        for(float j = first_pos.y; j < last_pos.y; j+=Settings::TILE_SIZE){
            std::pair<bool, Entity*> currentElement;
            currentElement.second = nullptr;
            Tile* t =map->getTile(i,j,1);
            int distance = sqrt((i-_positionCol.x)*(i-_positionCol.x)+(j-_positionCol.y)*(j-_positionCol.y))+Settings::TILE_SIZE;
            //PONEMOS SI EL BLOQUE ES TRAVESABLE O NO
            bool currentValueMap(t != nullptr && t->id =="0" && distance<radius);
            currentElement.first = currentValueMap;
            currentRow.push_back(currentElement);
            ++cols;
        }
        bfsMap.push_back(currentRow);
        ++rows;
    }
    */
    //ELIMINAMOS LAS POSICIONES INVIABLES DEVIDO EL TAMAÑO DEL MOB
    sf::Vector2i sizeMob(std::ceil(_sizeCol.x/float(Settings::TILE_SIZE))-1,std::ceil(_sizeCol.y/float(Settings::TILE_SIZE))-1);
    for(int i = 0; i< rows; ++i){
        for(int j = 0; j< cols; ++j){
            if(bfsMap2[i][j].first){
                if(i+sizeMob.x>=cols || j+sizeMob.y>=rows){
                    bfsMap2[i][j].first = false;
                } else{
                    bool fits = true;
                    for(int ii=i; ii< i+sizeMob.x+1 && fits; ++ii){
                        for(int jj=j; jj< j+sizeMob.y+1 && fits; ++jj){
                            if(!bfsMap2[ii][jj].first) {
                                bfsMap2[i][j].first = false;
                                fits = false;
                            }
                        }
                    }
                }
            }
        }
    }

    //METEMOS LAS ENTIDADES EN LA ESTRUCTURA
    for(int m = 0; m<targets.size(); ++m){
        int aux1 =targets[m]->getPositionCol().x;
        int aux2 = int(targets[m]->getPositionCol().x-first_pos.x);
        int aux3 = int(targets[m]->getPositionCol().x-first_pos.x)/Settings::TILE_SIZE;
        sf::Vector2i indexMatEntity(int(targets[m]->getPositionCol().x-first_pos.x)/Settings::TILE_SIZE,int(targets[m]->getPositionCol().y-first_pos.y)/Settings::TILE_SIZE);
        sf::FloatRect bb = targets[m]->getBoundingBox();
        sf::Vector2f sizeEntityF(bb.width-bb.left,bb.height-bb.top);
        sf::Vector2i sizeEntity(std::ceil(sizeEntityF.x/float(Settings::TILE_SIZE))-1,std::ceil(sizeEntityF.y/float(Settings::TILE_SIZE))-1);
        __positionTarget = indexMatEntity;
        /*
        for(int i = indexMatEntity.y-sizeMob.y+1; i< indexMatEntity.y + sizeEntity.y+1; ++i){
            for(int j = indexMatEntity.x-sizeMob.x-1; j< indexMatEntity.x+sizeEntity.x+1; ++j){
                if(j <cols && i <rows && j>=0 && i >= 0){
                    bfsMap2[i][j].second = targets[m];
                }
            }
        } */
        for(int i = indexMatEntity.y-sizeMob.y; i< indexMatEntity.y + sizeEntity.y; ++i){
            for(int j = indexMatEntity.x-sizeMob.x+1; j< indexMatEntity.x+sizeEntity.x; ++j){
                if(j <cols && i <rows && j>=0 && i >= 0){
                    bfsMap2[i][j].second = targets[m];
                }
            }
        }
    }
    sf::Vector2i indexMatMob(int(_positionCol.x-first_pos.x)/Settings::TILE_SIZE,int(_positionCol.y-first_pos.y)/Settings::TILE_SIZE);
    __positionMob = indexMatMob;
    //BFS
    bool visited[rows][cols] = { false };
    int distance[rows][cols] = { -1 };
    sf::Vector2i father[rows][cols] = {sf::Vector2i(-1,-1)}; //man what to do with this
    for(int i=0; i<rows; ++i){
        for(int j=0; j<cols; ++j){
            father[i][j] = sf::Vector2i(-1,-1);
            distance[i][j] = -1;
        }
    }
    visited[indexMatMob.y][indexMatMob.x] = true;
    distance[indexMatMob.y][indexMatMob.x] = 0;
    //father[indexMatMob.y][indexMatMob.x] = {sf::Vector2i(-1,-1)};

    std::queue<sf::Vector2i >cuaBfs;
    cuaBfs.push(sf::Vector2i(indexMatMob.x,indexMatMob.y));

    int positionAuxX[4] = {-1,1,0,0};
    int positionAuxY[4] = {0,0,1,-1};
    while(!cuaBfs.empty()){
        sf::Vector2i currPos = cuaBfs.front();
        cuaBfs.pop();
        for(int i=0; i<4; ++i){
            sf::Vector2i neighbor(currPos.x+positionAuxX[i],currPos.y+positionAuxY[i]);
            if(neighbor.x <cols && neighbor.y <rows && neighbor.x>=0 && neighbor.y >= 0){
                if(!visited[neighbor.y][neighbor.x] && bfsMap2[neighbor.y][neighbor.x].first){
                    visited[neighbor.y][neighbor.x] = true;
                    distance[neighbor.y][neighbor.x] = distance[currPos.y][currPos.x] + 1;
                    father[neighbor.y][neighbor.x] = currPos;
                    cuaBfs.push(neighbor);
                }
            }
        }
    }
    int min_dist = -1;
    sf::Vector2i min_pos(-1,-1);
    for(int i = 0; i< rows; ++i){
        for(int j = 0; j< cols; ++j){
            if(bfsMap2[i][j].second !=nullptr){
                if(distance[i][j] != -1 && (distance[i][j] < min_dist || min_dist == -1)){
                    min_dist = distance[i][j];
                    min_pos = sf::Vector2i(j,i);
                }
            }
        }
    }
    Decision result = IDLE;
    Entity* target = nullptr;
    sf::Vector2i targetPos = min_pos;
    if(min_dist != -1){
        target = bfsMap2[min_pos.y][min_pos.x].second;
        __auxRoute.clear();
        __auxRoute.push_back(min_pos);
        while(min_pos != indexMatMob && father[min_pos.y][min_pos.x] != indexMatMob){
            min_pos = father[min_pos.y][min_pos.x];
            __auxRoute.push_back(min_pos);
        }
        if(min_pos.x>indexMatMob.x) result = GO_RIGHT;
        else if(min_pos.x<indexMatMob.x) result = GO_LEFT;
        else if(min_pos.y<indexMatMob.y) result = GO_UP;
        else if(min_pos.y>indexMatMob.y) result = GO_DOWN;
        else result = IDLE;
    }
    return std::pair<Entity*,Decision>(target,result);

}
/*
bool Mob::searchFoodTarget(std::vector<AnimatedTile*> &v){
    std::vector<std::pair<AnimatedTile*,int> > vecAux;
    for(int i=0; i<v.size(); i++){
        sf::Vector2f posE = v[i]->getPositionCol();
        int distance = sqrt((posE.x-_positionCol.x)*(posE.x-_positionCol.x)+(posE.y-_positionCol.y)*(posE.y-_positionCol.y));
        vecAux.push_back(std::pair<AnimatedTile*,int>(v[i],distance));
    }
    std::sort(vecAux.begin(), vecAux.end(),pairSortCompare);
    for(int i=0; i<v.size(); i++){
        v[i] = vecAux[i].first;
    }
    return (v.size()>0);
}

bool Mob::searchMobTarget(std::vector<Mob*> &mobs){
    std::vector<std::pair<Mob*,int> > vecAux;
    for(int i=0; i<mobs.size(); i++){
        sf::Vector2f posE = mobs[i]->getPositionCol();
        int distance = sqrt((posE.x-_positionCol.x)*(posE.x-_positionCol.x)+(posE.y-_positionCol.y)*(posE.y-_positionCol.y));
        vecAux.push_back(std::pair<Mob*,int>(mobs[i],distance));
    }
    std::sort(vecAux.begin(), vecAux.end(),pairSortCompare);
    for(int i=0; i<mobs.size(); i++){
        mobs[i] = vecAux[i].first;
    }
    return (mobs.size()>0);
} */

bool Mob::update(float delta, Clock *c, int num_mobs_race, int size_eco){
    if(_bfsCounter<=0){
        _target = nullptr;
    } else{
        _bfsCounter = std::max(0, _bfsCounter-1);
    }
    _spriteTime += delta;

    float maxtime = float(SPAWN_SPRITE_MAX_TIME)/10.0f;
    if(_spriteTime > maxtime && _keyframe <10){
        _spriteTime -= maxtime;
        {
            _keyframe +=1;
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
    if(_dying){
        _dieTime = std::max(_dieTime-delta*10, 0.f);
        for(int i=0; i <_modules.size(); ++i){
            _modules[i]->_die_anim_percent = (10-_dieTime)/10.f;
            _modules[i]->_y_floor_die = _positionCol.y+_sizeCol.y;
        }
        if(_dieTime <= 0) _dead = true;
    }
    else {
        _attackColdown = std::max(0.f, _attackColdown - delta * 50);
        Map *map = Scene::getScene()->getMap();
        bool is_visible_chunk = map->getIndexMatChunk(map->getChunkIndex(_positionCol.x)) != -1;
        Simplex2d *base_noise_temperature = NoiseGenerator::getNoise("base_noise_temperature");
        Simplex2d *noise_humidity = NoiseGenerator::getNoise("noise_humidity");
        int y_pos = Chunk::N_TILES_Y - 1 - (_positionCol.y / Settings::TILE_SIZE);
        float height_factor = float(y_pos) / float(Chunk::N_TILES_Y);
        float heightTemp = (1 - height_factor) * (Settings::MAX_TEMPERATURE - Settings::MIN_TEMPERATURE) +
                           Settings::MIN_TEMPERATURE;

        int valHumidity = int(noise_humidity->valSimplex2D(0, _positionCol.x));
        int valTemperature = int(heightTemp) + int(base_noise_temperature->valSimplex2D(0, _positionCol.x));

        int totalTemp = valTemperature + c->_globalTemperature;
        int totalHum = valHumidity + c->_globalHumidity;
        float humDamage = totalHum * (1 - float(_gens._humidity) / 100) * delta;
        float tempDamage;
        if (totalTemp > 0) tempDamage = totalTemp * (1 - float(_gens._hot) / 100) * delta;
        else tempDamage = -totalTemp * (1 - float(_gens._cold) / 100) * delta;

        std::vector<Mob *> enemys;
        std::vector<Mob *> friends;
        std::vector<Mob *> neutral;
        std::vector<Mob *> food;
        searchNeighbors(enemys, friends, neutral, food);
        float comunityDamage = (float(num_mobs_race * num_mobs_race) / float(size_eco)) * delta * 10;

        _age = std::max(0.f, _age - ((tempDamage + humDamage + comunityDamage + delta) / 2000) * Settings::GEN_SPEED);
        if (_age <= 0) _life = 0;
        if (_life <= 0) {
            std::cout << "a mob died" << std::endl;
            _dying = true;
            return false;
        }
        else {
            _timeToReproduce -= (delta / 10) * Settings::GEN_SPEED;
            if (_timeToReproduce < 0) {
                return true;
            }
            if (num_mobs_race < 5) _hunger = std::max(-1.f, _hunger - (delta / 100) * Settings::GEN_SPEED);
            else if (num_mobs_race < 10) _hunger = std::max(-1.f, _hunger - (delta / 10) * Settings::GEN_SPEED);
            else _hunger = std::max(-1.f, _hunger - (delta / 1) * Settings::GEN_SPEED);

            std::vector<Entity*> targets;
            for(int i=0; i<enemys.size(); ++i){
                targets.push_back(enemys[i]);
            }
            sf::Vector2f posPlayer = Scene::getScene()->getPlayer()->_positionCol;
            float distance_form_player = sqrt((posPlayer.x-_positionSpawn.x)*(posPlayer.x-_positionSpawn.x)+(posPlayer.y-_positionSpawn.y)*(posPlayer.y-_positionSpawn.y));
            if(distance_form_player <_gens._distanceMaxMove*Settings::RADIUS_MOB_MULTIPLYER+Settings::MIN_RADIUS_MOB+_gens._distanceMaxReproduce*Settings::RADIUS_MOB_MULTIPLYER){
                targets.push_back(Scene::getScene()->getPlayer());
            }
            if (_hunger < 0) {
                for(int i=0; i< food.size(); i++){
                    targets.push_back(food[i]);
                }

                Scene* s = Scene::getScene();
                std::vector<AnimatedTile*> v;
                s->getFallingTilesArea(v,sf::Vector2i(_positionSpawn),_gens._distanceMaxMove*Settings::RADIUS_MOB_MULTIPLYER+Settings::MIN_RADIUS_MOB+_gens._distanceMaxReproduce*Settings::RADIUS_MOB_MULTIPLYER);
                for(int i=0; i<v.size(); i++){
                    targets.push_back(v[i]);
                }


                _life = std::max(0.f, _life - (delta / 10));
            }
            if(is_visible_chunk && targets.size() >0){
                if(_bfsCounter<=0){
                    _target = nullptr;
                    std::pair<Entity*,Decision> target = getTargetUsingBfs(targets,_gens._distanceMaxMove*Settings::RADIUS_MOB_MULTIPLYER+Settings::MIN_RADIUS_MOB+_gens._distanceMaxReproduce*Settings::RADIUS_MOB_MULTIPLYER);
                    _target = target.first;
                    _mobDecision = target.second;
                    _bfsCounter = 100;
                }
            }
            if (_target != nullptr && (is_visible_chunk || isNearTarget())) {
                targetAction(is_visible_chunk, friends);
            }
        }

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
        float ageFactor = 1.f-_age/(float)_gens._age;
        sf::FloatRect bb = _modules[i]->getBoundingBox(_position,_gens._size/80.f*ageFactor+0.2,_mobDirection);
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
void Mob::setEnemy(int race){
    _gens.setEnemy(race);
}
void Mob::attackTarget(){

    if(_target->_typeEntity=="mob") {
        Mob *m = static_cast<Mob*>(_target);

        if(_attackColdown ==0){
            m->setEnemy(_gens._race);
            sf::Vector2f posPlayer = Scene::getScene()->getPlayer()->getPositionCol();
            float distance_form_player = sqrt((posPlayer.x-_positionCol.x)*(posPlayer.x-_positionCol.x)+(posPlayer.y-_positionCol.y)*(posPlayer.y-_positionCol.y));
            if(distance_form_player < Settings::TILE_SIZE*200){
                int num = std::rand()%3+1;
                float volume =(Settings::TILE_SIZE*200-distance_form_player)/(Settings::TILE_SIZE*200)*100;
                SoundManager::setVolumeSound(volume/1.5f,"m_attack_"+std::to_string(num));
                SoundManager::playSound("m_attack_"+std::to_string(num));
            }
            m->hurt(_gens._strenght/10);
            _attackColdown= 100-_gens._atackSpeed+10;
            std::cout << "mob atackking!!!!!!!!!!" << std::endl;
        }
    } else if (_target->_typeEntity=="player"){
        Player *p = static_cast<Player*>(_target);
        if(_attackColdown ==0){
            int num = std::rand()%3+1;
            SoundManager::playSound("m_attack_"+std::to_string(num));
            p->hurt(_gens._strenght/10);
            _attackColdown= 100-_gens._atackSpeed+10;
            std::cout << "playyerrr attack!!!!!!!!!!" << std::endl;
        }
    }


}
void Mob::hurt(float amount){
    sf::Texture *t = Resources::getTexture("entities");
    _spriteHurt.setTexture(*t);
    _currHurtAnim = (rand()%3+4);

    _keyframeHurt=0;
    _hurted = true;
    _life -= amount;
}

void Mob::targetAction(bool visible, std::vector<Mob*> & friends) {
    if(visible){
        if(isNearTarget()){
            if(_target->_typeEntity=="mob"){
                attackTarget();
            } else if(_target->_typeEntity=="fallingTile"){
                _target->_removed = true;
                _hunger = _gens._foodNeeds;
                std::cout << "mob eating" << std::endl;
            } else{
                attackTarget();
                std::cout << "attacking player" << std::endl;
            }
        }

    } else{
        if(_target->_typeEntity=="mob"){
            simulateCombat(static_cast<Mob*>(_target));
            if(_life>0){
                _hunger = _gens._foodNeeds;
                for (int i = 0; i < friends.size(); i++) {
                    friends[i]->_hunger = std::min(friends[i]->getGenetics()->_foodNeeds,
                                                   int(_gens._foodNeeds / (friends.size())));
                }
                std::cout << "mob combat and eating far" << std::endl;
            }
        } else if(_target->_typeEntity=="fallingTile"){
            _target->_removed = true;
            _hunger = _gens._foodNeeds;
            for (int i = 0; i < friends.size(); i++) {
                friends[i]->_hunger = std::min(friends[i]->getGenetics()->_foodNeeds,
                                               int(_gens._foodNeeds / (friends.size())));
            }
            std::cout << "mob eating far" << std::endl;
        }
    }

}
bool Mob::isNearTarget() {
    if(_target !=nullptr && !_target->_removed){
        sf::FloatRect boundBoxE = _target->getBoundingBox();
        sf::FloatRect boundBox = getBoundingBox();
        boundBoxE.width = boundBoxE.width-boundBoxE.left;
        boundBoxE.height = boundBoxE.height-boundBoxE.top;
        boundBox.width = boundBox.width-boundBox.left;
        boundBox.height = boundBox.height-boundBox.top;
        return boundBox.intersects(boundBoxE);
    } else _target = nullptr;
    return false;
}
void Mob::updateVisible(float delta){
    //if(!_dying && !_dead && !_removed){
        int newDecision;

        //MODULES
        for(int i = 0; i<_modules.size(); i++){
            float ageFactor = 1.f-_age/(float)_gens._age;
            if(_target != nullptr && _modules[i]->hasAnimation("attacking")) _modules[i]->update(delta,_position,_gens._size/80.f*ageFactor+0.2,"attacking",_mobDirection);
            else if(vx !=0 && _modules[i]->hasAnimation("walking")) _modules[i]->update(delta,_position,_gens._size/80.f*ageFactor+0.2,"walking",_mobDirection);
            else _modules[i]->update(delta,_position,_gens._size/80.f*ageFactor+0.2,"idle",_mobDirection);
        }
        if(_target == nullptr) {
            newDecision = rand() % 1000;

            if(newDecision ==0){
                _mobDecision = GO_LEFT;
            }
            else if(newDecision ==1){
                _mobDecision = GO_RIGHT;
            }
            else if(newDecision ==2){
                _mobDecision = IDLE;
            }
        }
        else {
            if(_target->_removed) _target = nullptr;
            //else if(isNearTarget()) newDecision = 2;
            //else if(_target->getPositionCol().x > getPositionCol().x) newDecision = 1;
            //else newDecision = 0;
        }
        if(_dying) {
            _mobDecision = IDLE;
            _target = nullptr;
        }

        if(_target == nullptr){
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
        } else {
            if(col_bottom==0) {
                vy = (float) 9.8 * delta * 100 + vy;
            } else vy = 0;
            if (_mobDecision == GO_LEFT){
                vx = -100;
            }
            else if (_mobDecision == GO_RIGHT){
                vx = 100;
            } else if(_mobDecision == GO_UP){
                vx = 0;
                if(col_bottom>0) {
                    vy = -400;
                }
            } else if(_mobDecision == GO_DOWN){
                vx = 0;
            }
            else if(_mobDecision == IDLE){
                vx = 0;
            }
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
        if(distance_from_base< _gens._distanceMaxMove*Settings::RADIUS_MOB_MULTIPLYER+Settings::MIN_RADIUS_MOB+_gens._distanceMaxReproduce*Settings::RADIUS_MOB_MULTIPLYER){
            sf::Vector2f displacement(newPosCol.x-x0, newPosCol.y-y0);
            sf::Vector2f newPos(_position.x+displacement.x, _position.y+displacement.y);
            setPosition(newPos);
        }
        Player *pl = Scene::getScene()->getPlayerOnArea(sf::Vector2i(_positionSpawn),_gens._distanceMaxReproduce*Settings::RADIUS_MOB_MULTIPLYER+Settings::MIN_RADIUS_MOB);
        if(_gens._playerHostile && pl != nullptr) {
            //_target = pl;
        }


}
bool Mob::mobIsOnReproduceArea(){
    float radius =_gens._distanceMaxReproduce*Settings::RADIUS_MOB_MULTIPLYER+Settings::MIN_RADIUS_MOB;
    int distance = sqrt((_positionSpawn.x-_positionCol.x)*(_positionSpawn.x-_positionCol.x)+(_positionSpawn.y-_positionCol.y)*(_positionSpawn.y-_positionCol.y));
    return distance<radius;

}
MobGenetics* Mob::getGenetics(){
    return &_gens;
}
Mob * Mob::reproduce(){
    Scene *s = Scene::getScene();
    std::vector<Mob*> neighbours;
    std::vector<sf::Vector2i> reproduce_spots;
    Mob *result = nullptr;
    s->getPositionsOnArea(reproduce_spots,sf::Vector2i(_positionSpawn),_gens._distanceMaxReproduce*Settings::RADIUS_MOB_MULTIPLYER+Settings::MIN_RADIUS_MOB,_ecosystemIndex);
    if(reproduce_spots.size() >0){
        s->getMobsOnArea(neighbours,sf::Vector2i(_positionSpawn),_gens._distanceMaxReproduce*Settings::RADIUS_MOB_MULTIPLYER+Settings::MIN_RADIUS_MOB,_ecosystemIndex);
        if(neighbours.size() >0){
            sf::Vector2i position = reproduce_spots[rand() % reproduce_spots.size()];
            //position.y -= Settings::TILE_SIZE;
            Mob *partner = neighbours[rand() % neighbours.size()];
            int chunk = Map::getChunkIndex(position.x);
            result = new Mob(getGenetics(), partner->getGenetics(),getModules(),_mobType,partner->getModules(),partner->_mobType,chunk,sf::Vector2f(position),_ecosystemIndex);
            _timeToReproduce = _gens._reproduceFactor+OFFSET_REPRODUCE;
            partner->_timeToReproduce = partner->_gens._reproduceFactor+OFFSET_REPRODUCE;
        }
        else {
            std::cout << "error on reproduce, mob on illegal area?" << std::endl;
        }
    }
    return result;


}
std::vector<MobModule*>& Mob::getModules(){
    return _modules;
}
void Mob::kill(){
    _removed = true;
    Map *map = Scene::getScene()->getMap();
    Tile* t = map->getTile(_positionCol.x, _positionCol.y, 1);
    if(t != nullptr){
        int chunkE = map->getChunkIndex(t->GetPosition().x);
        int index_chunk = map->getIndexMatChunk(chunkE);
        if(index_chunk != -1 && _age >0) {
            map->_chunk_mat[index_chunk]->addFallingTile("food","food",_positionCol,Settings::TILE_SIZE);
        }
    }

}
