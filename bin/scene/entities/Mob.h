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
    static const int SPAWN_SPRITE_MAX_TIME = 1;
    static const int DIE_MAX_TIME = 50;
    Mob();
    ~Mob();
    Mob(MobGenetics* t,int chunk, sf::Vector2f position);
    Mob(MobGenetics* t1, MobGenetics* t2,std::vector<MobModule*>& modulesPartner1, int typeMobPartner1,std::vector<MobModule*>& modsPartner2,int typeMobPartner2,int chunk, sf::Vector2f position, int index);
    void mixModules(std::vector<MobModule*>& modu1, int typeMobPartner1,std::vector<MobModule*>& modu2, int typeMobPartner2,MobGenetics* t1, MobGenetics* t2);
    enum AnimationDirection{LEFT,RIGHT};
    enum Decision{IDLE, GO_LEFT,GO_RIGHT};
    void setPosition(sf::Vector2f position);
    bool update(float delta, Clock *c);
    bool mobIsOnReproduceArea();
    void updateVisible(float delta);
    Mob * reproduce();
    std::vector<MobModule*>& getModules();
    void kill();
    MobGenetics* getGenetics();
    void saveToFile(int chunk, std::ofstream &myfile);
    void loadFromFile(std::ifstream &myfile);
    void draw(sf::RenderTarget & renderTar);
    bool _dead;
    int _mobType;
    float _life;
    float _hunger;
    float _age;
    float _timeToReproduce;
    bool _focusDebug;
private:
    sf::FloatRect getBoundingBox();
    Entity* _target;
    int getRelationMob(int idRace);
    void searchNeighbors(std::vector<Mob*> &enemys,std::vector<Mob*> &friends,std::vector<Mob*> &neutral,std::vector<Mob*> &food);
    void targetAction();
    void attack(Mob* m);
    void simulateCombat(Mob* m);
    void createRandomBody();
    Entity* searchFoodTarget();
    Mob* searchMobTarget(std::vector<Mob*> &mobs);
    bool isNearTarget();
    float vx;
    float vy;
    float _attackColdown;
    sf::Vector2i _positionSpawn;
    MobGenetics _gens;
    sf::Sprite _spriteSpawn;
    sf::Vector2f size;
    Decision _mobDecision;
    AnimationDirection _mobDirection;
    std::vector<MobModule*> _modules;

    int _keyframe;
    float _spriteTime;

};


#endif //GENLAND_MOB_H
