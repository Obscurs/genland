//
// Created by arnito on 11/10/17.
//

#ifndef GENLAND_MOB_H
#define GENLAND_MOB_H


#include "Entity.h"
#include "../Clock.h"
#include "MobGenetics.h"
#include "../Colisionable.h"

class Mob : public Entity, public Colisionable {
public:
    static const int OFFSET_REPRODUCE = 50;
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
    float _timeToReproduce;
private:
    float vx;
    float vy;
    sf::Vector2i _positionSpawn;
    MobGenetics _gens;
    sf::Sprite _sprite;
    sf::Vector2f size;
    Decision _mobDecision;

    AnimationDirection _mobDirection;
};


#endif //GENLAND_MOB_H
