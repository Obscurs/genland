//
// Created by arnito on 9/09/17.
//

#ifndef GENLAND_ENTITY_HPP
#define GENLAND_ENTITY_HPP


#include <SFML/Audio.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class Entity {
public:
    Entity(std::string type);
    int _chunk;
    sf::Vector2i _position;
    sf::Vector2i _positionCol;
    sf::Vector2i _sizeCol;
    std::string _typeEntity;
    virtual void setPosition(int x, int y);
    sf::Vector2i getPosition();
    void setPositionCol(int x, int y);
    sf::Vector2i getPositionCol();
    bool _removed;
    bool pointHitsEntity(sf::FloatRect);
    virtual void draw(sf::RenderTarget & renderTar);
    virtual void saveToFile(int chunk, std::ofstream &myfile);
    virtual void loadFromFile(std::ifstream &myfile);
protected:

};


#endif //GENLAND_ENTITY_HPP
