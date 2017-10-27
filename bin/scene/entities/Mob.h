//
// Created by arnito on 11/10/17.
//

#ifndef GENLAND_MOB_H
#define GENLAND_MOB_H


#include "Entity.h"
#include "../Clock.h"
#include "MobGenetics.h"
#include "../Colisionable.h"
#include "MobModule.h"

class Mob : public Entity, public Colisionable {
public:
    static const int OFFSET_REPRODUCE = 50;
    static const int DISTANCE_NEAR = 128;
    Mob();
    ~Mob();
    Mob(MobGenetics* t,int chunk, sf::Vector2f position);
    Mob(MobGenetics* t1, MobGenetics* t2,int chunk, sf::Vector2f position, int index);
    enum AnimationDirection{LEFT,RIGHT};
    enum Decision{IDLE, GO_LEFT,GO_RIGHT};
    void setPosition(sf::Vector2f position);
    bool update(float delta, Clock *c);
    void updateVisible(float delta);
    Mob * reproduce();
    void kill();
    MobGenetics* getGenetics();
    void saveToFile(int chunk, std::ofstream &myfile);
    void loadFromFile(std::ifstream &myfile);
    void draw(sf::RenderTarget & renderTar);
    bool _dead;
    float _life;
    float _hunger;
    float _timeToReproduce;
private:
    sf::FloatRect getBoundingBox();
    Entity* _target;
    int getRelationMob(int idRace);
    void searchNeighbors(std::vector<Mob*> enemys,std::vector<Mob*> friends,std::vector<Mob*> neutral,std::vector<Mob*> food);
    void targetAction();
    void attack(Mob* m);
    void simulateCombat(Mob* m);
    Entity* searchFoodTarget();
    Mob* searchMobTarget(std::vector<Mob*> &mobs);
    bool isNearTarget();
    float vx;
    float vy;
    float _attackColdown;
    sf::Vector2i _positionSpawn;
    MobGenetics _gens;
    sf::Sprite _sprite;
    sf::Vector2f size;
    Decision _mobDecision;
    AnimationDirection _mobDirection;
    std::vector<MobModule*> _modules;
    void ___addModules();
};


#endif //GENLAND_MOB_H
