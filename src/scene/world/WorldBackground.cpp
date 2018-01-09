//
// Created by arnito on 30/10/16.
//

#include "WorldBackground.h"
#include "../../Settings.h"

WorldBackground::WorldBackground(bool isMenu){
    _sun_sprite.setRadius(40);
    _sun_sprite.setFillColor(sf::Color(255, 250, 200));
    _sun_sprite.setPosition(10,10);
    if(!isMenu){


        Background* background1 = new Background("resources/custom_back0.png",1.1, sf::Vector2f(755,2048),1.1);
        Background* background2= new Background("resources/custom_back1.png",1.2, sf::Vector2f(755,2048),1.2);
        Background* background3= new Background("resources/custom_back2.png",1.3, sf::Vector2f(755,2048),1.3);
        Background* background4= new Background("resources/custom_back3.png",1.4, sf::Vector2f(755,2048),1.4);
        Background* rain= new Background(5, sf::Vector2f(Settings::GAME_WIDTH,2048),1);
        _rainBackground = rain;

        _backgrounds.push_back(background1);
        _backgrounds.push_back(background2);
        _backgrounds.push_back(background3);
        _backgrounds.push_back(background4);
        _backgrounds.push_back(rain);
    }
    else{
        Background* background1 = new Background("resources/custom_back0.png",1.1, sf::Vector2f(755,2048),16);
        Background* background2= new Background("resources/custom_back1.png",1.2, sf::Vector2f(755,2048),8);
        Background* background3= new Background("resources/custom_back2.png",1.3, sf::Vector2f(755,2048),4);
        Background* background4= new Background("resources/custom_back3.png",1.4, sf::Vector2f(755,2048),1);
        Background* rain= new Background(1.1, sf::Vector2f(Settings::GAME_WIDTH,2048),1.4);
        _rainBackground = rain;
        _backgrounds.push_back(background1);
        _backgrounds.push_back(background2);
        _backgrounds.push_back(background3);
        _backgrounds.push_back(background4);
        //_backgrounds.push_back(rain);
    }

}


WorldBackground::~WorldBackground() {
}
sf::RenderTexture* WorldBackground::getRainTexture(){
    return _rainBackground->getTexture();
}
void WorldBackground::Update(sf::Vector2f player_pos,float time){
    for(int i=0; i<_backgrounds.size(); i++){

        _backgrounds[i]->updatePosition(player_pos);

    }
    _sun_sprite.setPosition(player_pos.x+(time)*2,player_pos.y/1.2-(time)*60+700);
}
void WorldBackground::Draw(sf::RenderTarget &texture_plain_sprite){

    for(int i=0; i< _backgrounds.size(); i++){
        _backgrounds[i]->Draw(texture_plain_sprite);
        if(i==0)texture_plain_sprite.draw(_sun_sprite);
        //texture_plain_sprite.display();
    }

}