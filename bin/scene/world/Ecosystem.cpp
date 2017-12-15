//
// Created by arnito on 5/10/17.
//
#include <math.h>
#include <tgmath.h>
#include "Ecosystem.h"
#include "../Scene.h"
#include "../../Settings.h"
#include "../entities/Stairs.h"
#include "../entities/Torch.h"
#include "../entities/Mob.h"
#include "../../Resources.h"

Ecosystem::Ecosystem(sf::Vector2i interval, int index):
    _threadSaveLoad(&Ecosystem::changeEcosystem,this){
    _interval = interval;
    _ecoReady = true;
    __auxPos = 0;
    _index = index;
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
        if(_mobs.size()< (_interval.y-_interval.x)*Settings::MIN_MOBS_ECO) spawnMobOnLessPoblated((_interval.y-_interval.x)*Settings::MIN_MOBS_ECO);
        if(_trees.size()< (_interval.y-_interval.x)*Settings::MIN_TREES_ECO) spawnTreeOnLessPoblated((_interval.y-_interval.x)*Settings::MIN_TREES_ECO);
        int size = int(_trees.size());
        for(int i = 0; i<size; i++){
            if(_trees[i]->_dead){
                _trees[i]->kill();
                _trees.erase(_trees.begin()+i);
                i--;
                size = int(_trees.size());
            } else {
                if(_trees[i]->update(delta, c)){
                    //int direction = rand() % 2;
                    Tree *res = _trees[i]->reproduce2();
                    //if(direction==0) direction = 1;
                    //else direction = 0;
                    //if(res==nullptr) res = _trees[i]->reproduce(direction);
                    if(res != nullptr  && _trees.size()< (_interval.y-_interval.x)*Settings::MAX_TREES_ECO) {
                        if(res->_left_n !=nullptr)res->_left_n->setRightTree(res);
                        if(res->_right_n !=nullptr)res->_right_n->setLeftTree(res);
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
        for(int i = 0; i< MobGenetics::MOB_RACES; ++i){
            _mobsPerRace[i] = 0;
        }
        for(int i = 0; i < _mobs.size(); ++i){
            ++_mobsPerRace[_mobs[i]->getGenetics()->_race];
        }
        size = int(_mobs.size());
        for(int i = 0; i<size; i++){
            if(_mobs[i]->_dead){

                _mobs[i]->kill();
                _mobs.erase(_mobs.begin()+i);
                i--;
                size = int(_mobs.size());
                std::cout << "mob removed " << _mobs.size() << std::endl;
            } else {
                if(_mobs[i]->update(delta, c, _mobsPerRace[_mobs[i]->getGenetics()->_race], _interval.y-_interval.x) && _mobs[i]->mobIsOnReproduceArea()){

                    Mob *res = _mobs[i]->reproduce();
                    if(res != nullptr && _mobs.size()< (_interval.y-_interval.x)*Settings::MAX_MOBS_ECO) {

                        _mobs.push_back(res);
                        std::cout << "new mob created " << _mobs.size() << std::endl;
                        int index_chunk = m->getIndexMatChunk(res->_chunk);
                        if(index_chunk != -1){
                            m->_chunk_mat[index_chunk]->addMobToChunk(res,index_chunk);
                            m->_chunk_mat[index_chunk]->_is_dirty = true;
                        }
                    }
                    //else std::cout << "mob no created" << std::endl;
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
    for(int i = 0; i<_mobs.size(); i++){
        if(_mobs[i]->_chunk == c->_chunk_id) {
            c->addMobToChunk(_mobs[i],index_in_mat_chunks);
        }
    }
}
void Ecosystem::addEntity(Entity *e){
    _entities.push_back(e);
}
void Ecosystem::drawDebugEntities(){
    sf::RenderWindow* target = Scene::getScene()->getWindowDebug();
    if(target != nullptr && _interval.x != _interval.y){
        sf::Event currentEvent;
        while(target->pollEvent(currentEvent))
        {

        }
        sf::View currentView = target->getView();
        sf::Vector2f centerView = currentView.getCenter();
        sf::Vector2f sizeView = currentView.getSize();

        int posIniEco= _interval.x*Settings::TILE_SIZE*Chunk::N_TILES_X;
        int posEndEco= _interval.y*Settings::TILE_SIZE*Chunk::N_TILES_X;
        int distanceWorld = posEndEco-posIniEco;
        float offset = float(sizeView.x)/float(distanceWorld);
        float offsetY = float(sizeView.y/2)/float(Settings::TILE_SIZE*Chunk::N_TILES_Y);
        sf::RectangleShape rectangle(sf::Vector2f(sizeView.x, sizeView.y/2-5));
        rectangle.setFillColor(sf::Color(145, 145, 145));
        rectangle.setOutlineThickness(1);
        rectangle.setOutlineColor(sf::Color(0, 0, 0));
        rectangle.setPosition(sf::Vector2f(centerView.x-sizeView.x/2, centerView.y-sizeView.y/2+sizeView.y*_index));

        target->clear(sf::Color::Blue);
        target->draw(rectangle);
        for(int i=0; i< _trees.size(); i++){
            rectangle.setSize(sf::Vector2f(3,3));
            rectangle.setFillColor(sf::Color(0, 0, 0));
            rectangle.setOutlineThickness(0);
            rectangle.setOutlineColor(sf::Color(0, 0, 0));
            rectangle.setPosition(sf::Vector2f(centerView.x-sizeView.x/2+(_trees[i]->getPosition().x-posIniEco)*offset, centerView.y-sizeView.y/2+sizeView.y*_index+(_trees[i]->getPosition().y)*offsetY));
            target->draw(rectangle);
        }
        for(int i=0; i< _mobs.size(); i++){
            rectangle.setSize(sf::Vector2f(5,5));

            switch(_mobs[i]->getGenetics()->_race){
                case 1:
                    rectangle.setFillColor(sf::Color(0, 0, 255));
                    break;
                case 2:
                    rectangle.setFillColor(sf::Color(0, 255, 0));
                    break;
                case 3:
                    rectangle.setFillColor(sf::Color(0, 255, 255));
                    break;
                case 4:
                    rectangle.setFillColor(sf::Color(255, 0, 0));
                    break;
                case 5:
                    rectangle.setFillColor(sf::Color(255, 0, 255));
                    break;
                case 6:
                    rectangle.setFillColor(sf::Color(255, 255, 0));
                    break;
                case 7:
                    rectangle.setFillColor(sf::Color(255, 255, 255));
                    break;
                case 8:
                    rectangle.setFillColor(sf::Color(0, 0, 100));
                    break;
                case 9:
                    rectangle.setFillColor(sf::Color(0, 100, 0));
                    break;
                case 10:
                    rectangle.setFillColor(sf::Color(0, 100, 100));
                    break;
                case 11:
                    rectangle.setFillColor(sf::Color(100, 0, 0));
                    break;
                case 12:
                    rectangle.setFillColor(sf::Color(100, 0, 100));
                    break;
                case 13:
                    rectangle.setFillColor(sf::Color(100, 100, 0));
                    break;
                case 14:
                    rectangle.setFillColor(sf::Color(100, 100, 100));
                    break;
                case 0:
                    rectangle.setFillColor(sf::Color(50, 100, 200));
                    break;
                default:
                    rectangle.setFillColor(sf::Color(50, 100, 200));
                    break;
            }
            rectangle.setOutlineThickness(0);
            rectangle.setOutlineColor(sf::Color(0, 0, 0));
            rectangle.setPosition(sf::Vector2f(centerView.x-sizeView.x/2+(_mobs[i]->getPosition().x-posIniEco)*offset, centerView.y-sizeView.y/2+sizeView.y*_index+(_mobs[i]->getPosition().y)*offsetY));
            target->draw(rectangle);
        }
        sf::Text text;
        text.setCharacterSize(24);
        text.setColor(sf::Color::Red);
        text.setStyle(sf::Text::Bold | sf::Text::Underlined);
        text.setPosition(centerView.x-sizeView.x/2, centerView.y-sizeView.y/2+sizeView.y*_index);
        sf::String str("Mob popluation: " +std::to_string(_mobs.size())+ "\n Tree population: "+std::to_string(_trees.size()));
        text.setString(str);

        text.setFont(*Resources::getFont("debugFont"));
        target->draw(text);
        target->display();
        sf::sleep(sf::milliseconds(50));
    }


}

void Ecosystem::updateWithElapsedTime(Date *d, bool debug){
    if(_interval.x != _interval.y){
        Clock *cGame = Scene::getScene()->getClock();
        Clock *c = new Clock();
        c->day = d->day;
        c->min = d->min;
        c->hour = d->hour;
        while(c->day<cGame->day || c->hour<cGame->hour || c->min < cGame->min){
            if(debug) drawDebugEntities();
            c->_clockSpeed = 1;
            c->update(Settings::SYNC_UPDATE_SPEED);
            std::random_shuffle ( _trees.begin(), _trees.end());
            std::random_shuffle ( _mobs.begin(), _mobs.end());
            int size = int(_trees.size());
            if(_trees.size()< (_interval.y-_interval.x)*Settings::MIN_TREES_ECO) spawnTreeOnLessPoblated((_interval.y-_interval.x)*Settings::MIN_TREES_ECO);
            if(_mobs.size()< (_interval.y-_interval.x)*Settings::MIN_MOBS_ECO) spawnMobOnLessPoblated((_interval.y-_interval.x)*Settings::MIN_MOBS_ECO);
            for(int i = 0; i<size; i++){
                if(_trees[i]->_dead){
                    _trees[i]->kill();
                    _trees.erase(_trees.begin()+i);
                    i--;
                    size = int(_trees.size());
                } else {
                    if(_trees[i]->update(Settings::SYNC_UPDATE_SPEED, c)){
                        Tree *res = _trees[i]->reproduce2();
                        if(res != nullptr  && _trees.size()< (_interval.y-_interval.x)*Settings::MAX_TREES_ECO) {
                            if(res->_left_n !=nullptr)res->_left_n->setRightTree(res);
                            if(res->_right_n !=nullptr)res->_right_n->setLeftTree(res);
                            _trees.push_back(res);
                        }
                    }
                }
            }
            for(int i = 0; i< MobGenetics::MOB_RACES; ++i){
                _mobsPerRace[i] = 0;
            }
            for(int i = 0; i < _mobs.size(); ++i){
                ++_mobsPerRace[_mobs[i]->getGenetics()->_race];
            }
            size = int(_mobs.size());
            for(int i = 0; i<size; i++){
                if(_mobs[i]->_dead){
                    _mobs[i]->kill();
                    _mobs.erase(_mobs.begin()+i);
                    i--;
                    size = int(_mobs.size());
                } else {
                    if(_mobs[i]->update(Settings::SYNC_UPDATE_SPEED, c,_mobsPerRace[_mobs[i]->getGenetics()->_race], _interval.y-_interval.x) && _mobs[i]->mobIsOnReproduceArea()){
                        Mob *res = _mobs[i]->reproduce();
                        if(res != nullptr && _mobs.size()< (_interval.y-_interval.x)*Settings::MAX_MOBS_ECO) _mobs.push_back(res);
                    }
                }
            }
        }
    }

}
sf::Vector3i Ecosystem::getMobPopulationAndTreshold(){
    return sf::Vector3i(_mobs.size(),(_interval.y-_interval.x)*Settings::MIN_MOBS_ECO,(_interval.y-_interval.x)*Settings::MAX_MOBS_ECO);
}
sf::Vector3i Ecosystem::getTreePopulationAndTreshold(){
    return sf::Vector3i(_trees.size(),(_interval.y-_interval.x)*Settings::MIN_TREES_ECO,(_interval.y-_interval.x)*Settings::MAX_TREES_ECO);
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
    for(int i=0; i<_mobs.size(); ++i){
        if(_mobs[i]->pointHitsEntity(pos) && !_mobs[i]->_removed) return _mobs[i];
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
        std::vector<std::pair<int, int> > chunksMobs[end-start];
        for(int i = 0; i<_trees.size(); i++){
            chunksTrees[_trees[i]->_chunk-start].push_back(std::pair<int, int>(i, _trees[i]->_position.x));
        }
        for(int i = 0; i<_entities.size(); i++){
            chunksEntities[_entities[i]->_chunk-start].push_back(std::pair<int, int>(i, _entities[i]->_position.x));
        }
        for(int i = 0; i<_mobs.size(); i++){
            chunksMobs[_mobs[i]->_chunk-start].push_back(std::pair<int, int>(i, _mobs[i]->_position.x));
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
            for(int i=0; i< chunksMobs[index-start].size(); i++){
                _mobs[chunksMobs[index-start][i].first]->saveToFile(index,myfile);
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
void Ecosystem::getEntitiesOnArea(std::vector<Entity*> &mobs, sf::Vector2i position, int radius){
    for(int i = 0; i<_entities.size(); i++){
        sf::Vector2f posMob = _entities[i]->getPositionCol();
        int distance = sqrt((posMob.x-position.x)*(posMob.x-position.x)+(posMob.y-position.y)*(posMob.y-position.y));
        if(distance < radius) mobs.push_back(_entities[i]);
    }
}
void Ecosystem::getMobsOnArea(std::vector<Mob*> &mobs, sf::Vector2i position, int radius){
    for(int i = 0; i<_mobs.size(); i++){
        sf::Vector2f posMob = _mobs[i]->getPositionCol();
        int distance = sqrt((posMob.x-position.x)*(posMob.x-position.x)+(posMob.y-position.y)*(posMob.y-position.y));
        if(distance < radius) mobs.push_back(_mobs[i]);
    }
}
void Ecosystem::getTreesOnArea(std::vector<Tree*> &trees, sf::Vector2i position, int radius){
    for(int i = 0; i<_trees.size(); i++){
        sf::Vector2f posMob = _trees[i]->_position;
        int distance = sqrt((posMob.x-position.x)*(posMob.x-position.x)+(posMob.y-position.y)*(posMob.y-position.y));
        if(distance < radius) trees.push_back(_trees[i]);
    }
}
void Ecosystem::spawnMobOnLessPoblated(int num_candidates){
    int distances[num_candidates] = {-1};
    sf::Vector2i positions[num_candidates];
    int numPlaces = _surface.size()*Chunk::N_TILES_X + getSizeUnderground();
    for(int i=0; i<num_candidates; i++){
        int randPlace = rand() % numPlaces;
        if(randPlace < _surface.size()*Chunk::N_TILES_X){
            int c = randPlace/Chunk::N_TILES_X;
            int col = randPlace % Chunk::N_TILES_X;
            sf::Vector2i posTileIndex = sf::Vector2i(col,_surface[c][col].first);
            sf::Vector2i posTileGlobal = Map::getGlobalPositionOfIndex(posTileIndex,c+_interval.x);
            positions[i] = posTileGlobal;
        } else {
            int c = rand() % _underground.size();
            int col =  rand() % _underground[c].size();
            int row =  rand() % _underground[col].size();
            sf::Vector2i posTileIndex = sf::Vector2i(col,_underground[c][col][row]);
            sf::Vector2i posTileGlobal = Map::getGlobalPositionOfIndex(posTileIndex,c+_interval.x);
            positions[i] = posTileGlobal;
        }
    }
    for(int i = 0; i<_mobs.size(); i++){
        if(!_mobs[i]->_removed && !_mobs[i]->_dead){
            for(int j=0; j<num_candidates; j++){
                sf::Vector2f position = _mobs[i]->getPosition();
                sf::Vector2i posTileGlobal = positions[j];
                int distance = sqrt((posTileGlobal.x-position.x)*(posTileGlobal.x-position.x)+(posTileGlobal.y-position.y)*(posTileGlobal.y-position.y));
                if(distances[j] <0 || distance < distances[j]) distances[j] = distance;
            }
        }
    }
    int max_dist = 0;
    sf::Vector2i positionNewMob;
    for(int i=0; i<num_candidates; i++){
        if(distances[i] > max_dist){
            max_dist = distances[i];
            positionNewMob = positions[i];
        }
    }
    if(max_dist >0){
        std::cout << "repoblating mob at position: " << positionNewMob.x << " " << positionNewMob.y << std::endl;
        MobGenetics *mobGen = new MobGenetics();
        Mob *mob = new Mob(mobGen,Map::getChunkIndex(positionNewMob.x), sf::Vector2f(positionNewMob));
        mob->setEcosystemIndex(_index);
        _mobs.push_back(mob);
        Map *m = Scene::getScene()->getMap();
        int index_chunk = m->getIndexMatChunk(Map::getChunkIndex(positionNewMob.x));
        if(index_chunk != -1){
            m->_chunk_mat[index_chunk]->addMobToChunk(mob,index_chunk);
            m->_chunk_mat[index_chunk]->_is_dirty = true;
        }
    }

}


void Ecosystem::spawnTreeOnLessPoblated(int num_candidates){
    int distances[num_candidates] = {-1};
    sf::Vector2i positions[num_candidates];
    std::vector<std::vector<sf::Vector2i > > _grassPositions;
    int numPlaces =0;
    for(int i=0; i< _surface.size(); i++){
        for(int j=0; j< _surface[i].size();j++){
            if(_surface[i][j].second) {
                numPlaces++;
                _grassPositions[i].push_back(sf::Vector2i(j,_surface[i][j].first));
            }
        }
    }
    for(int i=0; i<num_candidates; i++){
        int randChunk = rand() % _grassPositions.size();
        int randInd = rand() % _grassPositions[randChunk].size();

        sf::Vector2i posTileIndex = _grassPositions[randChunk][randInd];
        sf::Vector2i posTileGlobal = Map::getGlobalPositionOfIndex(posTileIndex,randChunk+_interval.x);
        positions[i] = posTileGlobal;

    }
    for(int i = 0; i<_trees.size(); i++){
        if(!_trees[i]->_removed && !_trees[i]->_dead){
            for(int j=0; j<num_candidates; j++){
                sf::Vector2f position = _trees[i]->_position;
                sf::Vector2i posTileGlobal = positions[j];
                int distance = sqrt((posTileGlobal.x-position.x)*(posTileGlobal.x-position.x)+(posTileGlobal.y-position.y)*(posTileGlobal.y-position.y));
                if(distances[j] <0 || distance < distances[j]) distances[j] = distance;
            }
        }
    }
    int max_dist = 0;
    sf::Vector2i positionNewTree;
    for(int i=0; i<num_candidates; i++){
        if(distances[i] > max_dist){
            max_dist = distances[i];
            positionNewTree = positions[i];
        }
    }
    if(max_dist >0){
        std::cout << "repoblating tree at position: " << positionNewTree.x << " " << positionNewTree.y << std::endl;

        TreeGenetics *treGen = new TreeGenetics();
        Tree *t = new Tree(treGen,Map::getChunkIndex(positionNewTree.x), positionNewTree);
        t->setEcosystemIndex(_index);
        _trees.push_back(t);


        Map *m = Scene::getScene()->getMap();
        int index_chunk = m->getIndexMatChunk(Map::getChunkIndex(positionNewTree.x));
        if(index_chunk != -1){
            m->_chunk_mat[index_chunk]->addTreeToChunk(t,index_chunk);
            m->_chunk_mat[0]->_is_dirty = true;
            m->_chunk_mat[1]->_is_dirty = true;
            m->_chunk_mat[2]->_is_dirty = true;
        }
    }
    linkTrees();

}
int Ecosystem::getSizeUnderground(){
    int result = 0;
    for(int i =0; i< _underground.size(); i++){
        for(int j =0; j< _underground[i].size(); j++){
            result += _underground[i][j].size();
        }
    }
}
void Ecosystem::getPositionsOnArea(std::vector<sf::Vector2i> &positions,sf::Vector2i position, int radius){
    int first_chunk_index = std::max(Map::getChunkIndex(position.x-radius)-_interval.x,0);
    int last_chunk_index = std::min(Map::getChunkIndex(position.x+radius)-_interval.x,_interval.y-_interval.x-1);
    int first_position_index = Chunk::getIndexFromGlobalPosition(sf::Vector2f(position.x-radius,0)).x;
    int last_position_index = Chunk::N_TILES_X-1;
    for(int c=first_chunk_index; c<=last_chunk_index; ++c){
        if(c==last_chunk_index) last_position_index = Chunk::getIndexFromGlobalPosition(sf::Vector2f(position.x+radius,0)).x;
        for(int i=first_position_index; i <= last_position_index; i++){
            sf::Vector2i posTileIndex = sf::Vector2i(i,_surface[c][i].first);
            sf::Vector2i posTileGlobal = Map::getGlobalPositionOfIndex(posTileIndex,c+_interval.x);
            int distance = sqrt((posTileGlobal.x-position.x)*(posTileGlobal.x-position.x)+(posTileGlobal.y-position.y)*(posTileGlobal.y-position.y));
            if(distance < radius) positions.push_back(posTileGlobal);
            for(int j = 0; j<_underground[c][i].size();++j){
                posTileIndex = sf::Vector2i(i,_underground[c][i][j]);
                posTileGlobal = Map::getGlobalPositionOfIndex(posTileIndex,c+_interval.x);
                distance = sqrt((posTileGlobal.x-position.x)*(posTileGlobal.x-position.x)+(posTileGlobal.y-position.y)*(posTileGlobal.y-position.y));
                if(distance < radius) positions.push_back(posTileGlobal);
            }
        }
        first_position_index = 0;
    }
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
                    t->setEcosystemIndex(_index);
                    _trees.push_back(t);
                }
                else if(entity =="mob"){
                    Mob *m = new Mob();
                    m->loadFromFile(myfile);
                    m->setEcosystemIndex(_index);
                    _mobs.push_back(m);
                }
                else if(entity == "stairs"){
                    Stairs *e = new Stairs();
                    e->loadFromFile(myfile);
                    e->setEcosystemIndex(_index);
                    _entities.push_back(e);
                } else if(entity == "torch"){
                    Torch *e = new Torch();
                    e->loadFromFile(myfile);
                    e->setEcosystemIndex(_index);
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

                updateWithElapsedTime(d, false);
                d->day = c->day;
                d->min = c->min;
                d->hour = c->hour;
            }
        }
    }
    _ecoReady = true;

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