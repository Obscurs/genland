//
// Created by ant on 06/11/16.
//

#include "RunningGame.h"
RunningGame::RunningGame(std::string path,sf::RenderWindow &window)
    : map_curr(-1,path),
      player(),
      backgrounds(),
      clock(),
      drawer(&map_curr,&player,&backgrounds, &clock){
    player.Load("blue.png");
    player.SetPosition(0,0);
    player.SetSize(32);
    pathGame = path;
}
void RunningGame::update(sf::RenderWindow &window, MagicView &game_view,float delta,Inputs &inputs){
    //sf::View currentView = window.getView();
    game_view.setCenter(player.GetPosition().x+(player.GetWidth()/2), player.GetPosition().y+(player.GetHeight()/2));
    window.setView(game_view);
    player.Update(delta, map_curr, inputs, window);
    map_curr.UpdateAll(delta, player.GetPosition());
    clock.Update(delta);
    backgrounds.Update(player.GetPosition(),clock);

}
void RunningGame::draw(sf::RenderWindow &window,MagicView &mv){
    drawer.Draw(window,mv);
}
void RunningGame::restart(std::string path,sf::RenderWindow &window){
    Map *newMap = new Map(-1,path);
    map_curr = *newMap;
    Player *newPlayer = new Player();
    player = *newPlayer;
    WorldBackground *newBack = new WorldBackground();
    backgrounds = *newBack;
    Clock *newClock = new Clock();
    clock = *newClock;


    player.Load("blue.png");
    player.SetPosition(0,0);
    player.SetSize(32);
    pathGame = path;

}
void RunningGame::saveGame(){
    map_curr.saveMap();
}


RunningGame::~RunningGame() {
}