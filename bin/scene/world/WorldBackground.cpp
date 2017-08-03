//
// Created by arnito on 30/10/16.
//

#include "WorldBackground.h"
WorldBackground::WorldBackground(){
    _sun_sprite.setRadius(40);
    _sun_sprite.setFillColor(sf::Color(255, 250, 200));
    _sun_sprite.setPosition(10,10);
    Background background1("resources/custom_back0.png",1.1, sf::Vector2f(755,2048));
    Background background2("resources/custom_back1.png",1.2, sf::Vector2f(755,2048));
    Background background3("resources/custom_back2.png",1.3, sf::Vector2f(755,2048));
    Background background4("resources/custom_back3.png",1.4, sf::Vector2f(755,2048));
    _backgrounds ={background1,background2,background3,background4};
}


WorldBackground::~WorldBackground() {
}

void WorldBackground::Update(sf::Vector2f player_pos, Clock &clock){
    for(int i=0; i<_backgrounds.size(); i++){

        _backgrounds[i].updatePosition(player_pos);

    }
    _sun_sprite.setPosition(player_pos.x+(clock.hour+clock.min/60)*2,player_pos.y/1.2-(clock.hour+clock.min/60)*60+700);
}
void WorldBackground::Draw(sf::RenderTexture &texture_plain_sprite){

    for(int i=0; i< _backgrounds.size(); i++){
        _backgrounds[i].Draw(texture_plain_sprite);
        if(i==0)texture_plain_sprite.draw(_sun_sprite);
        texture_plain_sprite.display();
    }

}