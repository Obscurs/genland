//
// Created by ant on 06/11/16.
//

#include "Scene.h"
#include "../Settings.h"

Scene::Scene()
    : _map_curr(),
      _player(),
      _backgrounds(),
      _clock("0"),
      _drawer(&_map_curr,&_player,&_backgrounds, &_clock),
      _viewGame()

{
    _zoom = 1.0;
    _player.Load("blue.png");
    _player.SetPosition(0,0);
    _player.SetSize(32);
    _pathGame = "null";
    _initialized = false;
}
void Scene::update(sf::RenderWindow &window,float delta){
    //sf::View currentView = window.getView();
    //currentView.setCenter(_player.GetPosition().x+(_player.GetWidth()/2), _player.GetPosition().y+(_player.GetHeight()/2));
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
    _map_curr.UpdateAll(delta, _player.GetPosition());

    _clock.Update(delta);

    _viewGame.setCenter(_player.GetPosition().x+(_player.GetWidth()/2), _player.GetPosition().y+(_player.GetHeight()/2));
}
void Scene::draw(sf::RenderWindow &window){
  const sf::View &aux = window.getView();
  window.setView(_viewGame);
  _drawer.Draw(window);
  window.setView(aux);
}
void Scene::init(std::string path, sf::RenderWindow &window, std::string seed){
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
    _map_curr.init(chunk_player, seed);

    WorldBackground *newBack = new WorldBackground();
    _backgrounds = *newBack;
    Clock *newClock = new Clock(seed);
    _clock = *newClock;
    _viewGame.setRenderTarget(&window);
    _viewGame.setMode(MagicView::crop);
    _viewGame.setResolution(sf::Vector2i(Settings::GAME_WIDTH,Settings::GAME_HEIGHT));
    _viewGame.update();


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



}


Scene::~Scene() {
}
void Scene::updateView(){
    _viewGame.update();
}

int Scene::getTemperature(sf::Vector2f pos){
    Tile *t = _map_curr.getTile(pos.x,pos.y,0);
    return t->_temperature;
}
int Scene::getHumidity(sf::Vector2f pos){
    Tile *t = _map_curr.getTile(pos.x,pos.y,0);
    return t->_humidity;
}
float Scene::getMountFactor(sf::Vector2f pos){
    Tile *t = _map_curr.getTile(pos.x,pos.y,0);
    return t->_mountain_factor;
}

Map &Scene::getMap(){
    return _map_curr;
}
WorldBackground &Scene::getBackgrounds(){
    return _backgrounds;
}
Clock &Scene::getClock(){
    return _clock;
}
Drawer &Scene::getDrawer(){
    return _drawer;
}
Player &Scene::getPlayer(){
    return _player;
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
