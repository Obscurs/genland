//
// Created by arnito on 25/10/17.
//

#ifndef GENLAND_MOBMODULE_H
#define GENLAND_MOBMODULE_H


#include <SFML/Audio.hpp>
#include <SFML/Graphics/Sprite.hpp>

class MobModule {
public:
    static const int NUM_HEADS = 3;
    static const int NUM_BODIES = 3;
    static const int NUM_HANDS = 2;
    static const int NUM_LEGS = 6;
    static const int NUM_COMPLEMENTS = 3;
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
    bool hasAnimation(std::string name);
    void saveToFile(std::ofstream &myfile);
    bool die(float delta, float positionFloor);
    int getIdModule();
    sf::Vector2f _position;
    sf::Vector2f _positionCol;
    sf::Vector2i _sizeCol;
    std::string _typeModule;
    sf::FloatRect getBoundingBox(sf::Vector2f position, float scale);
    sf::Vector2f getOffset();
    float getScale();

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
    void setAnimations();

};


#endif //GENLAND_MOBMODULE_H
