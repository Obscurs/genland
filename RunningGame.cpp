//
// Created by ant on 06/11/16.
//

#include "RunningGame.h"
#include "Settings.h"

RunningGame::RunningGame(sf::RenderWindow &window)
    : map_curr(),
      player(),
      backgrounds(),
      clock(),
      drawer(&map_curr,&player,&backgrounds, &clock),
      view_game(&window,MagicView::crop,sf::Vector2i(Settings::GAME_WIDTH,Settings::GAME_HEIGHT))
{
    zoom = 1.0;
    player.Load("blue.png");
    player.SetPosition(0,0);
    player.SetSize(32);
    pathGame = "null";
}
void RunningGame::update(sf::RenderWindow &window,float delta,Inputs &inputs){
    //sf::View currentView = window.getView();
    //currentView.setCenter(player.GetPosition().x+(player.GetWidth()/2), player.GetPosition().y+(player.GetHeight()/2));
    //window.setView(currentView);

    int wheel = inputs.getKey("wheel").x;
    sf::Vector2i controlKey = inputs.getKey("Control");
    if(wheel == 1 && controlKey.x){
        if(zoom < 3) zoom = std::min(zoom+0.05, 3.0);
    }
    else if(wheel == -1 && controlKey.x){
        if(zoom > 1) zoom = std::max(zoom-0.05, 1.0);
    }

    player.Update(delta, map_curr, inputs, window, zoom);

    backgrounds.Update(player.GetPosition(),clock);
    map_curr.UpdateAll(delta, player.GetPosition());

    clock.Update(delta);

    view_game.setCenter(player.GetPosition().x+(player.GetWidth()/2), player.GetPosition().y+(player.GetHeight()/2));
}
void RunningGame::draw(sf::RenderWindow &window){
  const sf::View &aux = window.getView();
  window.setView(view_game);
  drawer.Draw(window, zoom);
  window.setView(aux);
}
void RunningGame::restart(std::string path,sf::RenderWindow &window, std::string seed){

    Player *newPlayer = new Player();
    player = *newPlayer;
    player.Load("blue.png");
    player.SetPosition(0,0);
    player.SetSize(Settings::TILE_SIZE*2);
    pathGame = path;
    player.loadStats(pathGame);

    sf::Vector2f player_pos = player.GetPosition();
    int chunk_player = (int)round(player_pos.x/(Settings::TILE_SIZE*Chunk::N_TILES_X));
    Map *newMap = new Map(chunk_player,path, seed);
    map_curr = *newMap;
    map_curr.init(chunk_player,path, seed);

    WorldBackground *newBack = new WorldBackground();
    backgrounds = *newBack;
    Clock *newClock = new Clock();
    clock = *newClock;

    view_game.update();


    std::string route = pathGame;
    route.append("/data");

    std::ifstream myfile(route);
    std::string data_seed= "def", data_name = "def";
    std::string day= "0", hour= "0", min = "0";
    myfile >> data_seed >> data_name;
    myfile >> day >> hour >> min;
    clock.day = stoi(day);
    clock.hour = stoi(hour);
    clock.min = stoi(min);
    myfile.close();

}
void RunningGame::saveGame(){
    player.saveStats(pathGame);
    map_curr.saveMap();

    std::string route = pathGame;
    route.append("/data");

    std::ifstream myfile(route);
    std::string data_seed= "def", data_name = "def";
    myfile >> data_seed >> data_name;
    myfile.close();

    std::ofstream myfile2(route);
    myfile2 << data_seed << "\n" << data_name << "\n" ;
    myfile2 << clock.day << " " << clock.hour << " " << clock.min;
    myfile2.close();



}


RunningGame::~RunningGame() {
}
