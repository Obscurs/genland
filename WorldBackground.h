//
// Created by arnito on 30/10/16.
//

#ifndef GENLAND_WORLDBACKGROUND_H
#define GENLAND_WORLDBACKGROUND_H
#include "Background.h"
#include "Sun.h"
class WorldBackground {
public:
    WorldBackground();
    ~WorldBackground();
    std::vector<Background> backgrounds;
    void Update(sf::Vector2f player_pos);
    void Draw(sf::RenderTexture &texture_plain_sprite);
    Sun sun;
};


#endif //GENLAND_WORLDBACKGROUND_H
