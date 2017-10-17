//
// Created by arnito on 5/10/17.
//

#ifndef GENLAND_ECOSYSTEM_H
#define GENLAND_ECOSYSTEM_H


#include "../../Utils.h"
#include "Chunk.h"


class Ecosystem {
public:
    Ecosystem(sf::Vector2i interval, int index);
    ~Ecosystem();
    void update(float delta);
    std::vector<std::vector<std::pair<int, bool> > >* getSurface();
    std::vector<std::vector<std::vector<int> > >* getUnderground();
    sf::Vector2i getInterval();
    bool isReady();
    void setThreadData(int pos);
    void launchSaveLoadThread();
    void setInterval(sf::Vector2i new_int);
    void saveEntities();
    void loadEntities();
    void syncEntitiesWithChunk(Chunk *c, int index_in_mat_chunks);
    void addEntity(Entity *e);
    void getMobsOnArea(std::vector<Mob*> &mobs, sf::Vector2i position, int radius);
    void getPositionsOnArea(std::vector<sf::Vector2i> &positions,sf::Vector2i position, int radius);

    Entity *getEntity(sf::FloatRect pos);
private:
    std::vector<Tree*> _trees;
    std::vector<Mob*> _mobs;
    std::vector<Entity*> _entities;
    sf::Vector2i _interval;
    std::vector<std::vector<std::pair<int, bool> > > _surface;
    std::vector<std::vector<std::vector<int> > >_underground;
    int getSizeUnderground();
    void spawnMobOnLessPoblated(int num_candidates);
    void changeEcosystem();
    void linkTrees();
    void updateWithElapsedTime(Date *d);

    sf::Thread _threadSaveLoad;
    int __auxPos;
    bool _ecoReady;
    int _index;
};


#endif //GENLAND_ECOSYSTEM_H
