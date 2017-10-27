//
// Created by arnito on 25/10/17.
//

#ifndef GENLAND_MOBMODULE_H
#define GENLAND_MOBMODULE_H


#include <SFML/Audio.hpp>
#include <SFML/Graphics/Sprite.hpp>

class MobModule {
public:
    struct Animation{
        std::string a_name;
        int a_frameIni;
        int a_numFrames;
    };
    static const int MOB_SPRITE_SIZE = 64;
    static const int MODULE_SPRITE_MAX_TIME = 1;
    MobModule(std::string type, int id, sf::Vector2f offset, float localScale);
    ~MobModule();
    void draw(sf::RenderTarget & renderTar);
    void update(float delta, sf::Vector2f mobPosition, float scale, std::string currentAnimation, int direction);
    void addAnimation(std::string name, int frameIni, int numFrames);
    sf::Vector2f _position;
    sf::Vector2f _positionCol;
    sf::Vector2i _sizeCol;
    std::string _typeModule;
    sf::FloatRect getBoundingBox(sf::Vector2f position, float scale);

private:

    int _idModule;
    int _keyframe;
    sf::Sprite _sprite;
    sf::Vector2f _offset;
    float _localScale;
    float _spriteTime;
    std::vector<Animation*> _animations;
    Animation* _currentAnimation;
    bool _initialized;

};


#endif //GENLAND_MOBMODULE_H
