//
// Created by arnito on 6/08/17.
//

#ifndef GENLAND_TREE_H
#define GENLAND_TREE_H


#include "../world/Tile.h"
#include "Entity.h"

class Tree : public Entity {
public:
    Tree();
    Tree(int chunk, sf::Vector2i position, int amplitud, int altura, int corba, float branchAmount, float sizeBranch, float curveBranch, int leaveDensity, int leaveAmount, int leaveType);
    ~Tree();
    sf::Vector2i getPosition();
    void treeToTiles(Tile *t, int index_chunk_in_mat);
    void checkTreeTiles();
    Tree *getLeftTree();
    Tree *getRightTree();
    void setLeftTree(Tree *t);
    void setRightTree(Tree *t);
    bool isDead();
    void saveToFile(int chunk, std::ofstream &myfile);
    void loadFromFile(std::ifstream &myfile);
    void addRoot(sf::Vector2i root);
    void addBranch(sf::Vector2i branch);
    int hasTwoChunks();
    bool _rendered;
    bool _dead;
private:

    bool isValidPosition(Tile *t_first);
    void makeBranch(int size, float curve, sf::Vector2i initialPos, int direction);
    void growLeaves(Tile *t_first, int intensity);

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
