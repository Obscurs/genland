//
// Created by arnito on 9/09/17.
//

#ifndef GENLAND_ENTITY_HPP
#define GENLAND_ENTITY_HPP


#include <SFML/Audio.hpp>

class Entity {
public:
    Entity();
    int _chunk;
    sf::Vector2i _position;
    virtual void saveToFile(int chunk, std::ofstream &myfile);
    virtual void loadFromFile(std::ifstream &myfile);
protected:

};


#endif //GENLAND_ENTITY_HPP
