//
// Created by arnito on 30/10/16.
//

#ifndef GENLAND_WORLDBACKGROUND_H
#define GENLAND_WORLDBACKGROUND_H
#include "Background.h"
#include "Clock.h"
class WorldBackground {
public:
    WorldBackground();
    ~WorldBackground();
    std::vector<Background> backgrounds;
    void Update(sf::Vector2f player_pos, Clock &clock);
    void Draw(sf::RenderTexture &texture_plain_sprite);
    sf::CircleShape sun_sprite;
};


#endif //GENLAND_WORLDBACKGROUND_H
