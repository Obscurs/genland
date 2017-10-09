//
// Created by arnito on 5/10/17.
//

#include "Ecosystem.h"
#include "../Scene.h"
#include "../../Settings.h"
#include "../entities/Stairs.h"
#include "../entities/Torch.h"

Ecosystem::Ecosystem(sf::Vector2i interval):
    _threadSaveLoad(&Ecosystem::changeEcosystem,this){
    _interval = interval;
    _ecoReady = true;
    __auxPos = 0;
}
Ecosystem::~Ecosystem(){

}
void Ecosystem::setInterval(sf::Vector2i new_int){
    _interval = new_int;
}
void Ecosystem::update(float delta){
    if(_ecoReady){
        Scene *s = Scene::getScene();
        Clock *c = s->getClock();
        Map *m = s->getMap();
        int size = int(_trees.size());
        for(int i = 0; i<size; i++){
            if(_trees[i]->_dead){
                _trees[i]->kill();
                _trees.erase(_trees.begin()+i);
                i--;
                size = int(_trees.size());
            } else {
                if(_trees[i]->update(delta, c)){
                    Tree *res = _trees[i]->reproduce();
                    if(res != nullptr) {

                        _trees.push_back(res);
                        int index_chunk = m->getIndexMatChunk(res->_chunk);
                        if(index_chunk != -1){
                            m->_chunk_mat[index_chunk]->addTreeToChunk(res,index_chunk);
                            m->_chunk_mat[index_chunk]->_is_dirty = true;
                            m->_chunk_mat[1]->syncNotRenderedTrees();
                        }
                    }
                }
            }
        }

    }
}
void Ecosystem::syncEntitiesWithChunk(Chunk *c, int index_in_mat_chunks){
    c->clearEntities();
    for(int i = 0; i<_trees.size(); i++){
        if(_trees[i]->_chunk == c->_chunk_id) {
            c->addTreeToChunk(_trees[i],index_in_mat_chunks);
        }
    }
    for(int i = 0; i<_entities.size(); i++){
        if(_entities[i]->_chunk == c->_chunk_id) {
            c->addEntityToChunk(_entities[i],index_in_mat_chunks);
        }
    }
}
void Ecosystem::addEntity(Entity *e){
    _entities.push_back(e);
}
void Ecosystem::updateWithElapsedTime(Date *d){
    Clock *cGame = Scene::getScene()->getClock();
    Clock *c = new Clock();
    c->day = d->day;
    c->min = d->min;
    c->hour = d->hour;
    while(c->day<cGame->day || c->hour<cGame->hour || c->min < cGame->min){
        c->_clockSpeed = 1;
        c->update(Settings::SYNC_UPDATE_SPEED);
        int size = int(_trees.size());
        for(int i = 0; i<size; i++){
            if(_trees[i]->_dead){
                _trees[i]->kill();
                _trees.erase(_trees.begin()+i);
                i--;
                size = int(_trees.size());
            } else {
                if(_trees[i]->update(Settings::SYNC_UPDATE_SPEED, c)){
                    Tree *res = _trees[i]->reproduce();
                    if(res != nullptr) _trees.push_back(res);
                }
            }
        }
    }
}
void Ecosystem::launchSaveLoadThread(){
    _threadSaveLoad.launch();
}
void Ecosystem::setThreadData(int pos){
    __auxPos = pos;
}
bool Ecosystem::isReady(){
    return _ecoReady;
}
std::vector<std::vector<std::pair<int, bool> > >* Ecosystem::getSurface(){
    return &_surface;

}
std::vector<std::vector<std::vector<int> > >* Ecosystem::getUnderground() {
    return &_underground;
}
sf::Vector2i Ecosystem::getInterval(){
    return _interval;
}
void Ecosystem::changeEcosystem(){
    Scene *s = Scene::getScene();
    saveEntities();
    _interval = s->searchIntervalEcosystem(__auxPos);
    loadEntities();
}

Entity *Ecosystem::getEntity(sf::FloatRect pos){
    for(int i=0; i<_entities.size(); ++i){
        if(_entities[i]->pointHitsEntity(pos) && !_entities[i]->_removed) return _entities[i];
    }
    return nullptr;
}
void Ecosystem::saveEntities(){
    _ecoReady = false;
    int start = _interval.x;
    int end = _interval.y;
    Scene *s = Scene::getScene();
    Clock *c = s->getClock();
    std::vector<Date*>* lastUpdate = s->getEcosystemLastUpdateList();
    if(start != end){
        for(int i=0; i<lastUpdate->size(); i++){
            Date *d = (*lastUpdate)[i];
            if(d->interval == sf::Vector2i(_interval.x,_interval.y)){
                d->day = c->day;
                d->min = c->min;
                d->hour = c->hour;
            }
        }
        std::vector<std::pair<int, int> > chunksTrees[end-start];
        std::vector<std::pair<int, int> > chunksEntities[end-start];
        for(int i = 0; i<_trees.size(); i++){
            chunksTrees[_trees[i]->_chunk-start].push_back(std::pair<int, int>(i, _trees[i]->_position.x));
        }
        for(int i = 0; i<_entities.size(); i++){
            chunksEntities[_entities[i]->_chunk-start].push_back(std::pair<int, int>(i, _entities[i]->_position.x));
        }
        int index = start;
        while(index<end){
            std::sort(chunksTrees[index-start].begin(), chunksTrees[index-start].end(), [](const std::pair<int,int> &left, const std::pair<int,int> &right) {
                return left.second < right.second;
            });
            std::string filenameEnt = s->getGamePath();
            filenameEnt.append("/entities/");
            filenameEnt.append(std::to_string(index));
            filenameEnt.append(".txt");
            std::ofstream myfile;
            myfile.open(filenameEnt);
            for(int i=0; i< chunksTrees[index-start].size(); i++){
                _trees[chunksTrees[index-start][i].first]->saveToFile(index,myfile);
            }
            for(int i=0; i< chunksEntities[index-start].size(); i++){
                _entities[chunksEntities[index-start][i].first]->saveToFile(index,myfile);
            }
            myfile << "END";

            for(int i = 0; i < Chunk::N_TILES_X; i++){
                myfile << " " << _surface[index-start][i].first << " " << _surface[index-start][i].second;
            }
            for(int i = 0; i < Chunk::N_TILES_X; i++){
                myfile << " " << _underground[index-start][i].size();
                for(int j = 0; j < _underground[index-start][i].size(); ++j){
                    myfile << " " << _underground[index-start][i][j];
                }
            }
            myfile.close();
            index = index+1;
        }
    }
    _ecoReady = true;

}
void Ecosystem::loadEntities(){
    _ecoReady = false;
    _entities.clear();
    _trees.clear();
    _surface.clear();
    _underground.clear();
    int start = _interval.x;
    int end = _interval.y;
    if(start != end){
        Scene *s = Scene::getScene();
        int index = start;
        while(index<end){
            std::string filenameEnt = s->getGamePath();
            filenameEnt.append("/entities/");
            filenameEnt.append(std::to_string(index));
            filenameEnt.append(".txt");
            std::ifstream myfile;
            myfile.open(filenameEnt);
            std::string entity;
            myfile >> entity;
            while(entity != "END"){
                if(entity =="tree"){
                    Tree *t = new Tree();
                    t->loadFromFile(myfile);
                    _trees.push_back(t);
                }
                else if(entity == "stairs"){
                    Stairs *e = new Stairs();
                    e->loadFromFile(myfile);
                    _entities.push_back(e);
                } else if(entity == "torch"){
                    Torch *e = new Torch();
                    e->loadFromFile(myfile);
                    _entities.push_back(e);
                }
                myfile >> entity;
            }

            std::vector<std::pair<int, bool> >currentSurface;
            _surface.push_back(currentSurface);
            for(int i = 0; i < Chunk::N_TILES_X; i++){
                int pos;
                bool isDirt;
                myfile >> pos >> isDirt;
                _surface[index-start].push_back(std::pair<int,bool>(pos,isDirt));
            }

            std::vector<std::vector<int> >currentUnderground;
            _underground.push_back(currentUnderground);
            for(int i = 0; i < Chunk::N_TILES_X; i++){
                int numGrounds;
                myfile >> numGrounds;
                std::vector<int> currentY;
                _underground[index-start].push_back(currentY);
                for(int j = 0; j<numGrounds; j++){
                    int pos;
                    myfile >> pos;
                    _underground[index-start][i].push_back(pos);
                }
            }
            myfile.close();
            index = index+1;
        }
        linkTrees();

        Clock *c = s->getClock();
        std::vector<Date*>* lastUpdate = s->getEcosystemLastUpdateList();
        for(int i=0; i<lastUpdate->size(); i++){
            Date *d = (*lastUpdate)[i];
            if(d->interval == sf::Vector2i(_interval.x,_interval.y)){

                updateWithElapsedTime(d);
                d->day = c->day;
                d->min = c->min;
                d->hour = c->hour;
            }
        }
        _ecoReady = true;
    }

}
void Ecosystem::linkTrees(){
    Tree *oldTree = nullptr;
    for(int i=0; i<_trees.size(); i++){
        _trees[i]->setLeftTree(oldTree);
        if(oldTree != nullptr) oldTree->setRightTree(_trees[i]);
        oldTree = _trees[i];
    }
    if(oldTree != nullptr) oldTree->setRightTree(nullptr);
}