//
// Created by arnito on 6/08/17.
//

#ifndef GENLAND_TREE_H
#define GENLAND_TREE_H


#include "../world/Tile.h"

class Tree {
public:
    Tree(int amplitud, int altura, int corba, float branchAmount, float sizeBranch, float curveBranch, int leaveDensity, int leaveAmount, int leaveType);
    ~Tree();
    sf::Vector2i getPosition();
    void treeToTiles(Tile *t);
private:
    bool isValidPosition(Tile *t_first);
    void makeBranch(float size, float curve, sf::Vector2i initialPos, int direction);
    void growLeaves(Tile *t_first, int intensity);
    sf::Vector2i _position;
    int _amplitude;
    int _height;
    int _corb;
    int _densityLeave;
    int _amountLeave;
    int _typeLeave;
    Tile *_base_tile;
    std::vector<sf::Vector2i> _root;
    std::vector<sf::Vector2i> _branches;
};


#endif //GENLAND_TREE_H
