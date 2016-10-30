//
// Created by arnito on 30/10/16.
//

#include "WorldBackground.h"
WorldBackground::WorldBackground(){
    Background background1("resources/custom_back0.png",1.1, sf::Vector2f(755,2048));
    Background background2("resources/custom_back1.png",1.2, sf::Vector2f(755,2048));
    Background background3("resources/custom_back2.png",1.3, sf::Vector2f(755,2048));
    Background background4("resources/custom_back3.png",1.4, sf::Vector2f(755,2048));
    backgrounds ={background1,background2,background3,background4};
}


WorldBackground::~WorldBackground() {
}

void WorldBackground::Update(sf::Vector2f player_pos){
    for(int i=0; i<backgrounds.size(); i++){
        backgrounds[i].updatePosition(player_pos);
    }
}
void WorldBackground::Draw(sf::RenderTexture &texture_plain_sprite){
    for(int i=0; i< backgrounds.size(); i++){
        backgrounds[i].Draw(texture_plain_sprite);
    }
}