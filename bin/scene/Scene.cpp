//
// Created by ant on 06/11/16.
//

#include "Scene.h"
#include "../Settings.h"
#include "NoiseGenerator.h"
#include "../Inputs.h"

Scene::Scene()
    : _map_curr(),
      _player(),
      _backgrounds(),
      _clock(),
      _drawer(&_map_curr,&_player,&_backgrounds, &_clock),
      _viewGame()
{
    Ecosystem *e0 = new Ecosystem(sf::Vector2i(0,0), 0);
    Ecosystem *e1 = new Ecosystem(sf::Vector2i(0,0), 1);
    _ecosystems.first = e0;
    _ecosystems.second = e1;
    _seed = "0";
    _zoom = 1.0;
    //_player.Load("blue.png");
    _player.SetPosition(0,0);
    //_player.SetSize(32);
    _pathGame = "null";
    _initialized = false;
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

    _viewGame.setCenter(_player.GetPosition().x+(Player::PLAYER_WIDTH/2), _player.GetPosition().y+(Player::PLAYER_HEIGHT/2));

    checkEcosystems();
    _ecosystems.first->update(delta);
    _ecosystems.second->update(delta);

}
Entity *Scene::getEntity(sf::FloatRect pos){
    Entity *e = nullptr;
    int chunk = _map_curr.getChunkIndex(pos.left);
    sf::Vector2i result(0,0);
    if(betweenInts(_ecosystems.first->getInterval(),chunk)) return _ecosystems.first->getEntity(pos);
    else if(betweenInts(_ecosystems.second->getInterval(),chunk)) return _ecosystems.second->getEntity(pos);

}
bool Scene::betweenInts(sf::Vector2i interval, int i){
    return (i >= interval.x && i < interval.y && interval.x != interval.y);
}
std::vector<std::vector<std::pair<int, bool> > >* Scene::getSurface(sf::Vector2i interval){
    /*
    if(interval == _currentEcosystem1) return &_surface1;
    else if(interval == _currentEcosystem2) return &_surface2;
    else {
        std::cout << "estas fent servir un interval incorrecte" << std::endl;
        return nullptr;
    } */
    if(interval == _ecosystems.first->getInterval()) return _ecosystems.first->getSurface();
    else if(interval == _ecosystems.second->getInterval()) return _ecosystems.second->getSurface();
    else {
        std::cout << "estas fent servir un interval incorrecte" << std::endl;
        return nullptr;
    }
};
std::vector<std::vector<std::vector<int> > >* Scene::getUnderground(sf::Vector2i interval){
    /*if(interval == _currentEcosystem1) return &_underground1;
    else if(interval == _currentEcosystem2) return &_underground2;
    else {
        std::cout << "estas fent servir un interval incorrecte" << std::endl;
        return nullptr;
    }*/
    if(interval == _ecosystems.first->getInterval()) return _ecosystems.first->getUnderground();
    else if(interval == _ecosystems.second->getInterval()) return _ecosystems.second->getUnderground();
    else {
        std::cout << "estas fent servir un interval incorrecte" << std::endl;
        return nullptr;
    }
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
    if(betweenInts(_ecosystems.first->getInterval(),ind)) return _ecosystems.first->getInterval();
    else if(betweenInts(_ecosystems.second->getInterval(),ind)) return _ecosystems.second->getInterval();
    else return result;
}

void Scene::checkEcosystems(){
    sf::Vector2i oldEco1 = _ecosystems.first->getInterval();
    sf::Vector2i oldEco2 = _ecosystems.second->getInterval();
    int pos0 = _map_curr.getPosMap();
    int pos1 = pos0+2;
    if(!betweenInts(oldEco1, pos0) && !betweenInts(oldEco2, pos0)){
        if(betweenInts(oldEco2, pos1)){
            if(_ecosystems.first->isReady()){
                _ecosystems.first->setThreadData(pos0);
                _ecosystems.first->launchSaveLoadThread();
            }
        } else {
            if(_ecosystems.second->isReady()){
                _ecosystems.second->setThreadData(pos0);
                _ecosystems.second->launchSaveLoadThread();
            }
        }
    } else if(!betweenInts(oldEco1, pos1) && !betweenInts(oldEco2, pos1)){
        if(betweenInts(oldEco2, pos0)){
            if(_ecosystems.first->isReady()) {
                _ecosystems.first->setThreadData(pos1);
                _ecosystems.first->launchSaveLoadThread();
            }
        } else {
            if(_ecosystems.second->isReady()){
                _ecosystems.second->setThreadData(pos1);
                _ecosystems.second->launchSaveLoadThread();
            }
        }
    }

}
sf::RenderWindow* Scene::getWindowDebug(){
    return _winForDebug;
}
void Scene::draw(sf::RenderWindow &window){
    _winForDebug = &window;
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
    _winForDebug = nullptr;
    NoiseGenerator::init(_seed);
    _initialized = true;
    Player *newPlayer = new Player();
    _player = *newPlayer;
    //_player.Load("blue.png");
    _player.SetPosition(0,0);
    //_player.SetSize(64);
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
    std::string day= "0", hour= "6", min = "0";
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
            Date *d = new Date();
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
        _map_curr.setThreadParams(0,chunk_player-1);
        _map_curr.searchDeserts();
        _map_curr.setThreadParams(1,chunk_player+3);
        _map_curr.searchDeserts();
    }
    sf::Vector2i i0 = searchIntervalEcosystem(chunk_player);
    sf::Vector2i i1 = searchIntervalEcosystem(chunk_player+2);
    _ecosystems.first->setInterval(i0);
    if(i0 != i1) _ecosystems.first->setInterval(i1);
    _ecosystems.first->loadEntities();
    _ecosystems.second->loadEntities();
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
    _ecosystems.first->saveEntities();
    _ecosystems.second->saveEntities();
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
        Date *d = _entitiesLastUpdate[i];
        myfile3 << d->interval.x << " " << d->interval.y << " " << d->day << " " << d->hour << " " << d->min << " ";
    }
    myfile3 << "END";
    myfile3.close();


}

bool Scene::ecosystemsReady() {
    return _ecosystems.first->isReady() && _ecosystems.second->isReady();
}
std::vector<Date*>* Scene::getEcosystemLastUpdateList(){
    return &_entitiesLastUpdate;
}
void Scene::syncEntitiesWithLoadedChunk(Chunk *c,int index_in_mat_chunks){
    if(betweenInts(_ecosystems.first->getInterval(),c->_chunk_id)){
        _ecosystems.first->syncEntitiesWithChunk(c, index_in_mat_chunks);
    } else if(betweenInts(_ecosystems.second->getInterval(),c->_chunk_id)){
        _ecosystems.second->syncEntitiesWithChunk(c, index_in_mat_chunks);
    } else return;
}
Scene::~Scene() {
}
void Scene::updateView(){
    _viewGame.update();
}
void Scene::addEntity(Entity *e) {
    if(betweenInts(_ecosystems.first->getInterval(),e->_chunk)){
        e->setEcosystemIndex(0);
        _ecosystems.first->addEntity(e);
    } else if(betweenInts(_ecosystems.second->getInterval(),e->_chunk)){
        e->setEcosystemIndex(1);
        _ecosystems.second->addEntity(e);
    } else {
        std::cout << "incorrect chunk to add entity" << std::endl;
        return;
    }

}
void Scene::getEntityesArea(std::vector<Entity*> &entities, sf::Vector2i position, int radius, int indexEcosystem){
    if(indexEcosystem==0){
        _ecosystems.first->getEntitiesOnArea(entities,position,radius);
    } else if(indexEcosystem==1){
        _ecosystems.second->getEntitiesOnArea(entities,position,radius);
    } else std::cout << "alguna entitat te ecosystem index -1" << std::endl;
}
void Scene::getFallingTilesArea(std::vector<AnimatedTile*> &entities, sf::Vector2i position, int radius){

    for(int i=0; i<Map::N_CHUNKS_X; ++i){
        for(int j=0; j< _map_curr._chunk_mat[i]->_falling_tiles.size(); j++){
            sf::Vector2f posEnt = _map_curr._chunk_mat[i]->_falling_tiles[j]->getPositionCol();
            int distance = sqrt((posEnt.x-position.x)*(posEnt.x-position.x)+(posEnt.y-position.y)*(posEnt.y-position.y));
            if(distance < radius) entities.push_back(_map_curr._chunk_mat[i]->_falling_tiles[j]);
        }
    }
}
void Scene::getTreesOnArea(std::vector<Tree*> &trees, sf::Vector2i position, int radius, int indexEcosystem){
    if(indexEcosystem==0){
        _ecosystems.first->getTreesOnArea(trees,position,radius);
    } else if(indexEcosystem==1){
        _ecosystems.second->getTreesOnArea(trees,position,radius);
    } else std::cout << "algun tree te ecosystem index -1" << std::endl;
}
void Scene::getMobsOnArea(std::vector<Mob*> &mobs, sf::Vector2i position, int radius, int indexEcosystem){
    if(indexEcosystem==0){
        _ecosystems.first->getMobsOnArea(mobs,position,radius);
    } else if(indexEcosystem==1){
        _ecosystems.second->getMobsOnArea(mobs,position,radius);
    } else std::cout << "algun mob te ecosystem index -1" << std::endl;
}
void Scene::getPositionsOnArea(std::vector<sf::Vector2i> &positions,sf::Vector2i position, int radius, int indexEcosystem){
    if(indexEcosystem==0){
        _ecosystems.first->getPositionsOnArea(positions,position,radius);
    } else if(indexEcosystem==1){
        _ecosystems.second->getPositionsOnArea(positions,position,radius);
    } else std::cout << "alguna posicio te ecosystem index -1" << std::endl;
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

Ecosystem* Scene::getEcosystem(int index){
    if(index==0) return _ecosystems.first;
    else return _ecosystems.second;
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
            Date *d = new Date();
            d->interval.x = _biomeLimitsRight[_biomeLimitsRight.size()-2];
            d->interval.y = _biomeLimitsRight[_biomeLimitsRight.size()-1];
            d->day = 0;
            d->min = 0;
            d->hour = 0;
            _entitiesLastUpdate.push_back(d);
        }
        else if(_biomeLimitsLeft.size() > 0){
            Date *d = new Date();
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
            Date *d = new Date();
            d->interval.y = _biomeLimitsLeft[_biomeLimitsRight.size()-2];
            d->interval.x = _biomeLimitsLeft[_biomeLimitsRight.size()-1];
            d->day = 0;
            d->min = 0;
            d->hour = 0;
            _entitiesLastUpdate.push_back(d);
        }
        else if(_biomeLimitsRight.size() > 0){
            Date *d = new Date();
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