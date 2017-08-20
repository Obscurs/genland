//
// Created by arnito on 6/08/17.
//

#ifndef GENLAND_TREE_H
#define GENLAND_TREE_H


#include "../world/Tile.h"

class Tree {
public:
    Tree(sf::Vector2i position, int amplitud, int altura, int corba, float branchAmount, float sizeBranch, float curveBranch, int leaveDensity, int leaveAmount, int leaveType);
    ~Tree();
    sf::Vector2i getPosition();
    void treeToTiles(Tile *t);
    void checkTreeTiles();
    Tree *getLeftTree();
    Tree *getRightTree();
    void setLeftTree(Tree *t);
    void setRightTree(Tree *t);
    bool isDead();
private:

    bool isValidPosition(Tile *t_first);
    void makeBranch(int size, float curve, sf::Vector2i initialPos, int direction);
    void growLeaves(Tile *t_first, int intensity);
    sf::Vector2i _position;
    int _amplitude;
    int _height;
    int _corb;
    int _densityLeave;
    int _amountLeave;
    int _typeLeave;
    int _min_x;
    int _max_x;
    Tree *_left_n;
    Tree * _right_n;
    std::vector<sf::Vector2i> _root;
    std::vector<sf::Vector2i> _branches;
};


#endif //GENLAND_TREE_H
