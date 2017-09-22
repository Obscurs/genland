//
// Created by ant on 06/11/16.
//

#include "Scene.h"
#include "../Settings.h"
#include "NoiseGenerator.h"

Scene::Scene()
    : _map_curr(),
      _player(),
      _backgrounds(),
      _clock(),
      _drawer(&_map_curr,&_player,&_backgrounds, &_clock),
      _viewGame()

{
    _seed = "0";
    _zoom = 1.0;
    _player.Load("blue.png");
    _player.SetPosition(0,0);
    _player.SetSize(32);
    _pathGame = "null";
    _initialized = false;
    _currentEcosystem1 = sf::Vector2i(0,0);
    _currentEcosystem2 = sf::Vector2i(0,0);
}
void Scene::update(sf::RenderWindow &window,float delta){
    //sf::View currentView = window.getView();
    //currentView.setCenter(_player.GetPosition().x+(_player.getWidth()/2), _player.GetPosition().y+(_player.getHeight()/2));
    //window.setView(currentView);

    int wheel = Inputs::GetWheel();
    if(wheel == 1 && Inputs::KeyDown(Inputs::CONTROL)){
        if(_zoom < 3) _zoom = std::min(_zoom+0.05, 3.0);
    }
    else if(wheel == -1 && Inputs::KeyDown(Inputs::CONTROL)){
        if(_zoom > 1) _zoom = std::max(_zoom-0.05, 1.0);
    }

    _player.Update(delta, _map_curr, window);

    _backgrounds.Update(_player.GetPosition(),_clock);
    _map_curr.update(delta, _player.GetPosition());

    _clock.update(delta);

    _viewGame.setCenter(_player.GetPosition().x+(_player.GetWidth()/2), _player.GetPosition().y+(_player.GetHeight()/2));
    updateEcosystems(delta);

}
bool Scene::betweenInts(sf::Vector2i interval, int i){
    return (i >= interval.x && i < interval.y && interval.x != interval.y);
}
std::vector<std::vector<std::pair<int, bool> > >* Scene::getSurface(sf::Vector2i interval){
    if(interval == _currentEcosystem1) return &_surface1;
    else if(interval == _currentEcosystem2) return &_surface2;
    else {
        std::cout << "estas fent servir un interval incorrecte" << std::endl;
        return nullptr;
    }
};
std::vector<std::vector<std::vector<int> > >* Scene::getUnderground(sf::Vector2i interval){
    if(interval == _currentEcosystem1) return &_underground1;
    else if(interval == _currentEcosystem2) return &_underground2;
    else {
        std::cout << "estas fent servir un interval incorrecte" << std::endl;
        return nullptr;
    }
}
void Scene::addTreeToEntities(Tree t, sf::Vector2i interval){
    //if(interval == _currentEcosystem1) _entities1.push_back(t);
    //else if(interval == _currentEcosystem2) _entities2.push_back(t);
    //else {
    //    std::cout << "estas fent servir un interval incorrecte man" << std::endl;
    //}
}
sf::Vector2i Scene::searchIntervalEcosystem(int ind){
    bool first_elem = true;
    bool first_elem2 = true;
    sf::Vector2i result(0,0);
    for(int i = _biomeLimitsLeft.size()-1; i >=0; i--){
        if(first_elem){
            first_elem = false;
            result.x = _biomeLimitsLeft[i];
        }
        else if(first_elem2){
            first_elem2 = false;
            result.y = _biomeLimitsLeft[i];
        }
        else if(betweenInts(result,ind)){
            return result;
        } else{
            result.x = result.y;
            result.y = _biomeLimitsLeft[i];
        }
    }
    for(int i = 0; i < _biomeLimitsRight.size(); i++){
        if(first_elem){
            first_elem = false;
            result.x = _biomeLimitsRight[i];
        }
        else if(first_elem2){
            first_elem2 = false;
            result.y = _biomeLimitsRight[i];
        }
        else if(betweenInts(result,ind)){
            return result;
        } else{
            result.x = result.y;
            result.y = _biomeLimitsRight[i];
        }
    }
    if(betweenInts(result,ind)){
        return result;
    } else return sf::Vector2i(0,0);
}
sf::Vector2i Scene::getIntervalEcosystem(int ind){
    sf::Vector2i result(0,0);
    if(betweenInts(_currentEcosystem1,ind)) return _currentEcosystem1;
    else if(betweenInts(_currentEcosystem2,ind)) return _currentEcosystem2;
    else return result;
}
void Scene::updateEcosystems(float delta){
    sf::Vector2i oldEco1 = _currentEcosystem1;
    sf::Vector2i oldEco2 = _currentEcosystem2;
    int pos0 = _map_curr.getPosMap();
    int pos1 = pos0+2;
    if(!betweenInts(oldEco1, pos0) && !betweenInts(oldEco2, pos0)){
        if(betweenInts(oldEco2, pos1)){
            saveEntities(0);
            _currentEcosystem1 = searchIntervalEcosystem(pos0);
            loadEntities(0);
        } else {
            saveEntities(1);
            _currentEcosystem2 = searchIntervalEcosystem(pos0);
            loadEntities(1);
        }
    } else if(!betweenInts(oldEco1, pos1) && !betweenInts(oldEco2, pos1)){
        if(betweenInts(oldEco2, pos0)){
            saveEntities(0);
            _currentEcosystem1 = searchIntervalEcosystem(pos1);
            loadEntities(0);
        } else {
            saveEntities(1);
            _currentEcosystem2 = searchIntervalEcosystem(pos1);
            loadEntities(1);
        }
    }
    int size = int(_entities1.size());
    for(int i = 0; i<size; i++){
        if(_entities1[i]->_dead){
            _entities1[i]->kill();
            _entities1.erase(_entities1.begin()+i);
            i--;
            size = int(_entities1.size());
        } else {
            if(_entities1[i]->update(delta, &_clock)){
                Tree *res = _entities1[i]->reproduce();
                if(res != nullptr) _entities1.push_back(res);
            }
        }
    }
    size = int(_entities2.size());
    for(int i = 0; i<size; i++){
        if(_entities2[i]->_dead){
            _entities2[i]->kill();
            _entities2.erase(_entities2.begin()+i);
            i--;
            size = int(_entities2.size());
        } else {
            if(_entities2[i]->update(delta, &_clock)){
                Tree *res = _entities2[i]->reproduce();
                if(res != nullptr) _entities2.push_back(res);
            }
        }
    }


}
void Scene::updateWithElapsedTime(std::vector<Tree*> &entities, date *d){
    Clock *c = new Clock();
    c->day = d->day;
    c->min = d->min;
    c->hour = d->hour;
    while(c->day<_clock.day || c->hour<_clock.hour || c->min < _clock.min){
        c->_clockSpeed = 1;
        c->update(Settings::SYNC_UPDATE_SPEED);
        int size = int(entities.size());
        for(int i = 0; i<size; i++){
            if(entities[i]->_dead){
                entities[i]->kill();
                entities.erase(entities.begin()+i);
                i--;
                size = int(entities.size());
            } else {
                if(entities[i]->update(Settings::SYNC_UPDATE_SPEED, c)){
                    Tree *res = entities[i]->reproduce();
                    if(res != nullptr) entities.push_back(res);
                }
            }
        }
    }
}
void Scene::draw(sf::RenderWindow &window){
  const sf::View &aux = window.getView();
  window.setView(_viewGame);
  _drawer.Draw(window);
  window.setView(aux);
}
inline bool exists_file (const std::string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}
void Scene::init(std::string path, sf::RenderWindow &window, std::string seed){
    _seed = seed;
    NoiseGenerator::init(_seed);
    _initialized = true;
    Player *newPlayer = new Player();
    _player = *newPlayer;
    _player.Load("blue.png");
    _player.SetPosition(0,0);
    _player.SetSize(Settings::TILE_SIZE*2);
    _pathGame = path;
    _player.loadStats(_pathGame);

    sf::Vector2f player_pos = _player.GetPosition();
    int chunk_player = (int)round(player_pos.x/(Settings::TILE_SIZE*Chunk::N_TILES_X));
    _map_curr.init(chunk_player);

    WorldBackground *newBack = new WorldBackground();
    _backgrounds = *newBack;
    Clock *newClock = new Clock();
    _clock = *newClock;
    _viewGame.setRenderTarget(&window);
    _viewGame.setMode(MagicView::crop);
    _viewGame.setResolution(sf::Vector2i(Settings::GAME_WIDTH,Settings::GAME_HEIGHT));
    _viewGame.update();

    ////////////////////////////////
    ////////// DATA ////////////////
    std::string route = _pathGame;
    route.append("/data");

    std::ifstream myfile(route);
    std::string data_seed= "def", data_name = "def";
    std::string day= "0", hour= "0", min = "0";
    myfile >> data_seed >> data_name;
    myfile >> day >> hour >> min;
    _clock.day = stoi(day);
    _clock.hour = stoi(hour);
    _clock.min = stoi(min);
    myfile.close();

    ////////////////////////////////
    ///////// ECO //////////////////
    route = _pathGame;
    route.append("/ecosystems");

    if(exists_file(route)){
        std::ifstream myfile2(route);
        std::string value = "START";
        while(value != "END"){
            myfile2 >> value;
            if(value != "END"){
                (std::stoi(value) >= 0) ? _biomeLimitsRight.push_back(std::stoi(value)) : _biomeLimitsLeft.push_back(std::stoi(value));
            }
        }
        value = "START";
        while(value != "END"){
            date *d = new date();
            myfile2 >> value;
            if(value != "END") {
                d->interval.x = std::stoi(value);
                myfile2  >> d->interval.y >> d->day >> d->hour >> d->min;
                _entitiesLastUpdate.push_back(d);
            }
        }
        myfile2.close();
    }
    if(!firstBiomeCreated()){
        _map_curr.searchDeserts(0, chunk_player-1);
        _map_curr.searchDeserts(1, chunk_player+3);


    }
    _currentEcosystem1 = searchIntervalEcosystem(chunk_player);
    if(searchIntervalEcosystem(chunk_player+2) !=_currentEcosystem1) _currentEcosystem2 = searchIntervalEcosystem(chunk_player+2);
    loadEntities(0);
    loadEntities(1);
    _map_curr.syncEntitiesToChunk(0);
    _map_curr.syncEntitiesToChunk(1);
    _map_curr.syncEntitiesToChunk(2);

}
void Scene::saveGame(){
    _player.saveStats(_pathGame);
    _map_curr.saveMap();

    std::string route = _pathGame;
    route.append("/data");

    std::ifstream myfile(route);
    std::string data_seed= "def", data_name = "def";
    myfile >> data_seed >> data_name;
    myfile.close();

    std::ofstream myfile2(route);
    myfile2 << data_seed << "\n" << data_name << "\n" ;
    myfile2 << _clock.day << " " << _clock.hour << " " << _clock.min;
    myfile2.close();


    ////////////////////////////////
    ///////// ECO //////////////////
    saveEntities(0);
    saveEntities(1);
    route = _pathGame;
    route.append("/ecosystems");

    std::ofstream myfile3(route);
    for(int i = 0; i< _biomeLimitsLeft.size(); i++){
        myfile3 << _biomeLimitsLeft[i] << " ";
    }
    for(int i = 0; i< _biomeLimitsRight.size(); i++){
        myfile3 << _biomeLimitsRight[i] << " ";
    }
    myfile3 << "END ";
    for(int i=0; i< _entitiesLastUpdate.size(); i++){
        date *d = _entitiesLastUpdate[i];
        myfile3 << d->interval.x << " " << d->interval.y << " " << d->day << " " << d->hour << " " << d->min << " ";
    }
    myfile3 << "END";
    myfile3.close();


}

void Scene::saveEntities(bool arrayChosen){
    if(!arrayChosen){
        int start = _currentEcosystem1.x;
        int end = _currentEcosystem1.y;
        if(start != end){
            for(int i=0; i<_entitiesLastUpdate.size(); i++){
                date *d = _entitiesLastUpdate[i];
                if(d->interval == sf::Vector2i(_currentEcosystem1.x,_currentEcosystem1.y)){
                    d->day = _clock.day;
                    d->min = _clock.min;
                    d->hour = _clock.hour;
                }
            }
            std::vector<std::pair<int, int> > chunks[end-start];
            for(int i = 0; i<_entities1.size(); i++){
                chunks[_entities1[i]->_chunk-start].push_back(std::pair<int, int>(i, _entities1[i]->_position.x));
            }
            int index = start;
            while(index<end){
                std::sort(chunks[index-start].begin(), chunks[index-start].end(), [](const std::pair<int,int> &left, const std::pair<int,int> &right) {
                    return left.second < right.second;
                });
                std::string filenameEnt = _pathGame;
                filenameEnt.append("/entities/");
                filenameEnt.append(std::to_string(index));
                filenameEnt.append(".txt");
                std::ofstream myfile;
                myfile.open(filenameEnt);
                for(int i=0; i< chunks[index-start].size(); i++){
                    _entities1[chunks[index-start][i].first]->saveToFile(index,myfile);
                }
                myfile << "END";

                for(int i = 0; i < Chunk::N_TILES_X; i++){
                    myfile << " " << _surface1[index-start][i].first << " " << _surface1[index-start][i].second;
                }
                for(int i = 0; i < Chunk::N_TILES_X; i++){
                    myfile << " " << _underground1[index-start][i].size();
                    for(int j = 0; j < _underground1[index-start][i].size(); ++j){
                        myfile << " " << _underground1[index-start][i][j];
                    }
                }
                myfile.close();
                index = index+1;
            }
        }
    } else{
        int start = _currentEcosystem2.x;
        int end = _currentEcosystem2.y;
        if(start != end){
            for(int i=0; i<_entitiesLastUpdate.size(); i++){
                date *d = _entitiesLastUpdate[i];
                if(d->interval == sf::Vector2i(_currentEcosystem2.x,_currentEcosystem2.y)){
                    d->day = _clock.day;
                    d->min = _clock.min;
                    d->hour = _clock.hour;
                }
            }
            std::vector<std::pair<int, int> > chunks[end-start];
            for(int i = 0; i<_entities2.size(); i++){
                chunks[_entities2[i]->_chunk-start].push_back(std::pair<int, int>(i, _entities2[i]->_position.x));
            }
            int index = start;
            while(index<end){
                std::sort(chunks[index-start].begin(), chunks[index-start].end(), [](const std::pair<int,int> &left, const std::pair<int,int> &right) {
                    return left.second < right.second;
                });
                std::string filenameEnt = _pathGame;
                filenameEnt.append("/entities/");
                filenameEnt.append(std::to_string(index));
                filenameEnt.append(".txt");
                std::ofstream myfile;
                myfile.open(filenameEnt);
                for(int i=0; i< chunks[index-start].size(); i++){
                    _entities2[chunks[index-start][i].first]->saveToFile(index,myfile);
                }
                myfile << "END";
                for(int i = 0; i < Chunk::N_TILES_X; i++){
                    myfile << " " << _surface2[index-start][i].first << " " << _surface2[index-start][i].second;
                }
                for(int i = 0; i < Chunk::N_TILES_X; i++){
                    myfile << " " << _underground2[index-start][i].size();
                    for(int j = 0; j < _underground2[index-start][i].size(); ++j){
                        myfile << " " << _underground2[index-start][i][j];
                    }
                }

                myfile.close();
                index = index+1;
            }
        }
    }
}
void Scene::loadEntities(bool arrayChosen){
    if(!arrayChosen){
        _entities1.clear();
        _surface1.clear();
        _underground1.clear();
        int start = _currentEcosystem1.x;
        int end = _currentEcosystem1.y;
        if(start != end){

            int index = start;
            while(index<end){
                std::string filenameEnt = _pathGame;
                filenameEnt.append("/entities/");
                filenameEnt.append(std::to_string(index));
                filenameEnt.append(".txt");
                std::ifstream myfile;
                myfile.open(filenameEnt);
                std::string entity;
                myfile >> entity;
                while(entity != "END"){
                    Tree *t = new Tree();
                    t->loadFromFile(myfile);
                    if(t != nullptr && (t->_chunk > 100 || t->_chunk < -100)){
                        std::cout << "lol" << std::endl;
                    }
                    if(t != nullptr && t->_right_n != nullptr && (t->_right_n->_chunk > 100 || t->_right_n->_chunk < -100)){
                        std::cout << "lol" << std::endl;
                    }
                    if(t != nullptr && t->_left_n != nullptr && (t->_left_n->_chunk > 100 || t->_left_n->_chunk < -100)){
                        std::cout << "lol" << std::endl;
                    }
                    _entities1.push_back(t);
                    myfile >> entity;
                }

                std::vector<std::pair<int, bool> >currentSurface;
                _surface1.push_back(currentSurface);
                for(int i = 0; i < Chunk::N_TILES_X; i++){
                    int pos;
                    bool isDirt;
                    myfile >> pos >> isDirt;
                    _surface1[index-start].push_back(std::pair<int,bool>(pos,isDirt));
                }

                std::vector<std::vector<int> >currentUnderground;
                _underground1.push_back(currentUnderground);
                for(int i = 0; i < Chunk::N_TILES_X; i++){
                    int numGrounds;
                    myfile >> numGrounds;
                    std::vector<int> currentY;
                    _underground1[index-start].push_back(currentY);
                    for(int j = 0; j<numGrounds; j++){
                        int pos;
                        myfile >> pos;
                        _underground1[index-start][i].push_back(pos);
                    }
                }
                myfile.close();
                index = index+1;
            }
            linkTrees(0);
            for(int i=0; i<_entitiesLastUpdate.size(); i++){
                date *d = _entitiesLastUpdate[i];
                if(d->interval == sf::Vector2i(_currentEcosystem1.x,_currentEcosystem1.y)){

                    updateWithElapsedTime(_entities1, d);
                    d->day = _clock.day;
                    d->min = _clock.min;
                    d->hour = _clock.hour;
                }
            }
        }
    } else{
        _entities2.clear();
        _surface2.clear();
        _underground2.clear();
        int start = _currentEcosystem2.x;
        int end = _currentEcosystem2.y;
        if(start != end){
            int index = start;
            while(index<end){
                std::string filenameEnt = _pathGame;
                filenameEnt.append("/entities/");
                filenameEnt.append(std::to_string(index));
                filenameEnt.append(".txt");
                std::ifstream myfile;
                myfile.open(filenameEnt);
                std::string entity;
                myfile >> entity;
                while(entity != "END"){
                    Tree *t = new Tree();
                    t->loadFromFile(myfile);
                    _entities2.push_back(t);
                    myfile >> entity;
                }
                std::vector<std::pair<int, bool> >currentSurface;
                _surface2.push_back(currentSurface);
                for(int i = 0; i < Chunk::N_TILES_X; i++){
                    int pos;
                    bool isDirt;
                    myfile >> pos >> isDirt;
                    _surface2[index-start].push_back(std::pair<int,bool>(pos,isDirt));
                }

                std::vector<std::vector<int> >currentUnderground;
                _underground2.push_back(currentUnderground);
                for(int i = 0; i < Chunk::N_TILES_X; i++){
                    int numGrounds;
                    myfile >> numGrounds;
                    std::vector<int> currentY;
                    _underground2[index-start].push_back(currentY);
                    for(int j = 0; j<numGrounds; j++){
                        int pos;
                        myfile >> pos;
                        _underground2[index-start][i].push_back(pos);
                    }
                }
                myfile.close();
                index = index+1;
            }
            linkTrees(1);
            for(int i=0; i<_entitiesLastUpdate.size(); i++){
                date *d = _entitiesLastUpdate[i];
                if(d->interval == sf::Vector2i(_currentEcosystem2.x,_currentEcosystem2.y)){
                    updateWithElapsedTime(_entities2, d);
                    d->day = _clock.day;
                    d->min = _clock.min;
                    d->hour = _clock.hour;
                }
            }
        }

    }
}
void Scene::linkTrees(bool arrayChosen){
    if(!arrayChosen) {
        Tree *oldTree = nullptr;
        for(int i=0; i<_entities1.size(); i++){
            _entities1[i]->setLeftTree(oldTree);
            if(oldTree != nullptr) oldTree->setRightTree(_entities1[i]);
            oldTree = _entities1[i];
        }
        if(oldTree != nullptr) oldTree->setRightTree(nullptr);
    } else{
        Tree *oldTree = nullptr;
        for(int i=0; i<_entities2.size(); i++){
            _entities2[i]->setLeftTree(oldTree);
            if(oldTree != nullptr) oldTree->setRightTree(_entities2[i]);
            oldTree = _entities2[i];
        }
        if(oldTree != nullptr) oldTree->setRightTree(nullptr);
    }

}
void Scene::syncTreesWithChunk(Chunk *c,int index_in_mat_chunks){

    c->clearEntities();
    std::vector<Tree*> *trees;
    if(betweenInts(_currentEcosystem1,c->_chunk_id)){
        trees = &_entities1;
    } else if(betweenInts(_currentEcosystem2,c->_chunk_id)){
        trees = &_entities2;
    } else return;
    for(int i = 0; i<trees->size(); i++){
        if((*trees)[i]->_chunk == c->_chunk_id) {
            c->addTreeToChunk((*trees)[i],index_in_mat_chunks);
        }
    }
}
void Scene::syncNotRenderedTrees(Chunk *c){
    c->syncNotRenderedTrees();
}
Scene::~Scene() {
}
void Scene::updateView(){
    _viewGame.update();
}
int Scene::getTemperatureGlobal(sf::Vector2f pos){
    return int(_clock._globalTemperature);
}
int Scene::getTemperature(sf::Vector2f pos){
    Tile *t = _map_curr.getTile(pos.x,pos.y,0);
    return t->_temperature;
}
int Scene::getHumidity(sf::Vector2f pos){
    Tile *t = _map_curr.getTile(pos.x,pos.y,0);
    return t->_humidity;
}
int Scene::getHumidityGlobal(sf::Vector2f pos){
    return int(_clock._globalHumidity);
}
float Scene::getMountFactor(sf::Vector2f pos){
    Tile *t = _map_curr.getTile(pos.x,pos.y,0);
    return t->_mountain_factor;
}


Map* Scene::getMap(){
    return &_map_curr;
}
WorldBackground* Scene::getBackgrounds(){
    return &_backgrounds;
}
Clock* Scene::getClock(){
    return &_clock;
}
Drawer* Scene::getDrawer(){
    return &_drawer;
}
Player* Scene::getPlayer(){
    return &_player;
}
bool Scene::isInit() {
    return _initialized;
}
float Scene::getZoom(){
    return _zoom;
}
std::string Scene::getGamePath(){
    return _pathGame;
}
std::string Scene::getSeed(){
    return _seed;
}

sf::Vector2i Scene::getLimsBiome(){
    sf::Vector2i lims(0,0);
    if(_biomeLimitsLeft.size() >0)lims.x = _biomeLimitsLeft[_biomeLimitsLeft.size()-1];
    else if(_biomeLimitsRight.size() >0) lims.x = _biomeLimitsRight[0];
    if(_biomeLimitsRight.size() > 0) lims.y = _biomeLimitsRight[_biomeLimitsRight.size()-1];
    else if(_biomeLimitsLeft.size() >0) lims.y = _biomeLimitsLeft[0];
    return lims;
}

void Scene::addLimit(int newLimit){
    if(newLimit >= 0) {
        _biomeLimitsRight.push_back(newLimit);
        if(_biomeLimitsRight.size() >1){
            date *d = new date();
            d->interval.x = _biomeLimitsRight[_biomeLimitsRight.size()-2];
            d->interval.y = _biomeLimitsRight[_biomeLimitsRight.size()-1];
            d->day = 0;
            d->min = 0;
            d->hour = 0;
            _entitiesLastUpdate.push_back(d);
        }
        else if(_biomeLimitsLeft.size() > 0){
            date *d = new date();
            d->interval.x = _biomeLimitsLeft[0];
            d->interval.y = _biomeLimitsRight[_biomeLimitsRight.size()-1];
            d->day = 0;
            d->min = 0;
            d->hour = 0;
            _entitiesLastUpdate.push_back(d);
        }
    }
    else {
        _biomeLimitsLeft.push_back(newLimit);
        if(_biomeLimitsLeft.size() >1){
            date *d = new date();
            d->interval.y = _biomeLimitsLeft[_biomeLimitsRight.size()-2];
            d->interval.x = _biomeLimitsLeft[_biomeLimitsRight.size()-1];
            d->day = 0;
            d->min = 0;
            d->hour = 0;
            _entitiesLastUpdate.push_back(d);
        }
        else if(_biomeLimitsRight.size() > 0){
            date *d = new date();
            d->interval.y = _biomeLimitsRight[0];
            d->interval.x = _biomeLimitsLeft[_biomeLimitsLeft.size()-1];
            d->day = 0;
            d->min = 0;
            d->hour = 0;
            _entitiesLastUpdate.push_back(d);
        }
    }
}
bool Scene::firstBiomeCreated() {
    return (_biomeLimitsRight.size()+_biomeLimitsLeft.size() >=2);
}