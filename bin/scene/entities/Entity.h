//
// Created by arnito on 9/09/17.
//
#ifndef GENLAND_ENTITY_HPP
#define GENLAND_ENTITY_HPP


#include <SFML/Audio.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class Entity {

public:
    static bool pairSortCompare(const std::pair<Entity*,int>& firstElem, const std::pair<Entity*,int>& secondElem) {
        return firstElem.second < secondElem.second;
    }
    static void sortEntities(std::vector<Entity*> &vec, sf::Vector2f position){
        std::vector<std::pair<Entity*,int> > vecAux;
        for(int i=0; i<vec.size(); i++){
            sf::Vector2f posE = vec[i]->getPositionCol();
            int distance = sqrt((posE.x-position.x)*(posE.x-position.x)+(posE.y-position.y)*(posE.y-position.y));
            vecAux.push_back(std::pair<Entity*,int>(vec[i],distance));
        }
        std::sort(vecAux.begin(), vecAux.end(),pairSortCompare);
        for(int i=0; i<vec.size(); i++){
            vec[i] = vecAux[i].first;
        }
    }
    Entity(std::string type);
    int _chunk;
    sf::Vector2f _position;
    sf::Vector2f _positionCol;
    sf::Vector2i _sizeCol;
    std::string _typeEntity;
    virtual void setPosition(int x, int y);
    sf::Vector2f getPosition();
    void setPositionCol(int x, int y);
    void setEcosystemIndex(int ind);
    int getEcosystemIndex();
    sf::Vector2f getPositionCol();
    bool _removed;
    bool pointHitsEntity(sf::FloatRect);
    virtual void draw(sf::RenderTarget & renderTar);
    virtual void saveToFile(int chunk, std::ofstream &myfile);
    virtual void loadFromFile(std::ifstream &myfile);
    virtual sf::FloatRect getBoundingBox();
protected:
    int _ecosystemIndex;
};


#endif //GENLAND_ENTITY_HPP
