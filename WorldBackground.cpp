//
// Created by arnito on 30/10/16.
//

#include "WorldBackground.h"
WorldBackground::WorldBackground(){
    sun_sprite.setRadius(40);
    sun_sprite.setFillColor(sf::Color(255, 250, 200));
    sun_sprite.setPosition(10,10);
    Background background1("resources/custom_back0.png",1.1, sf::Vector2f(755,2048));
    Background background2("resources/custom_back1.png",1.2, sf::Vector2f(755,2048));
    Background background3("resources/custom_back2.png",1.3, sf::Vector2f(755,2048));
    Background background4("resources/custom_back3.png",1.4, sf::Vector2f(755,2048));
    backgrounds ={background1,background2,background3,background4};
}


WorldBackground::~WorldBackground() {
}

void WorldBackground::Update(sf::Vector2f player_pos, Clock &clock){
    for(int i=0; i<backgrounds.size(); i++){

        backgrounds[i].updatePosition(player_pos);

    }
    sun_sprite.setPosition(player_pos.x+clock.min,player_pos.y/1.2-clock.min*30+1000);
}
void WorldBackground::Draw(sf::RenderTexture &texture_plain_sprite){
    for(int i=0; i< backgrounds.size(); i++){
        backgrounds[i].Draw(texture_plain_sprite);
        if(i==0)texture_plain_sprite.draw(sun_sprite);
    }

}