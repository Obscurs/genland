//
// Created by arnito on 30/10/16.
//

#ifndef GENLAND_WORLDBACKGROUND_H
#define GENLAND_WORLDBACKGROUND_H
#include "Background.h"
#include "../Clock.h"
class WorldBackground {
public:
    WorldBackground(bool isMenu);
    ~WorldBackground();

    void Update(sf::Vector2f player_pos, float time);
    void Draw(sf::RenderTarget &texture_plain_sprite);
    sf::RenderTexture* getRainTexture();
private:
    std::vector<Background*> _backgrounds;
    sf::CircleShape _sun_sprite;
    Background* _rainBackground;
};


#endif //GENLAND_WORLDBACKGROUND_H
