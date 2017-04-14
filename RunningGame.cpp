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

    player.Update(delta, map_curr, inputs, window);

    backgrounds.Update(player.GetPosition(),clock);
    map_curr.UpdateAll(delta, player.GetPosition());

    clock.Update(delta);

    view_game.setCenter(player.GetPosition().x+(player.GetWidth()/2), player.GetPosition().y+(player.GetHeight()/2));
}
void RunningGame::draw(sf::RenderWindow &window){
  const sf::View &aux = window.getView();

  window.setView(view_game);
  drawer.Draw(window);
  window.setView(aux);
}
void RunningGame::restart(std::string path,sf::RenderWindow &window, std::string seed){

    Map *newMap = new Map(-1,path, seed);
    map_curr = *newMap;
    map_curr.init(-1,path, seed);
    Player *newPlayer = new Player();
    player = *newPlayer;
    WorldBackground *newBack = new WorldBackground();
    backgrounds = *newBack;
    Clock *newClock = new Clock();
    clock = *newClock;

    view_game.update();
    player.Load("blue.png");
    player.SetPosition(0,0);
    player.SetSize(Settings::TILE_SIZE*2);
    pathGame = path;
    player.loadStats(pathGame);

}
void RunningGame::saveGame(){
    player.saveStats(pathGame);
    map_curr.saveMap();
}


RunningGame::~RunningGame() {
}
