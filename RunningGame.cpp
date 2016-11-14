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
void RunningGame::update(sf::RenderWindow &window,MagicView &view_game,float delta,Inputs &inputs){
    //sf::View currentView = window.getView();
    //currentView.setCenter(player.GetPosition().x+(player.GetWidth()/2), player.GetPosition().y+(player.GetHeight()/2));
    //window.setView(currentView);

    player.Update(delta, map_curr, inputs, window);

    MagicView view_game2;
    view_game2.setRenderTarget(&window);
    view_game2.setViewport({0,0,1,1});
    view_game2.setResolution(sf::Vector2i(SCREEN_WIDTH,SCREEN_HEIGHT));
    view_game2.setMode(MagicView::crop);
    view_game.setCenter(player.GetPosition().x+(player.GetWidth()/2), player.GetPosition().y+(player.GetHeight()/2));
    window.setView(view_game);

    backgrounds.Update(player.GetPosition(),clock);
    map_curr.UpdateAll(delta, player.GetPosition());


    clock.Update(delta);


}
void RunningGame::draw(sf::RenderWindow &window){
    drawer.Draw(window);
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