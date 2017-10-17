//
// Created by arnito on 6/08/17.
//

#ifndef GENLAND_TREE_H
#define GENLAND_TREE_H


#include "../world/Tile.h"
#include "Entity.h"
#include "TreeGenetics.h"
#include "../Clock.h"

class Tree : public Entity {
public:

    static const int OFFSET_REPRODUCE = 50;
    Tree();
    Tree(TreeGenetics* t,int chunk, sf::Vector2i position);
    Tree(TreeGenetics* t1, TreeGenetics* t2,int chunk, sf::Vector2i position, int index);
    ~Tree();
    sf::Vector2i getPosition();
    void setPosition(sf::Vector2i position);
    bool update(float delta, Clock *c);
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
    Tree *_left_n;
    Tree * _right_n;
    void kill();
    Tree * reproduce();
    TreeGenetics* getGenetics();
    float _life;
    float _timeToReproduce;
    int _temperature;
    int _humidity;

private:
    void buildTree();
    bool isValidPosition(Tile *t_first);
    void makeBranch(int size, float curve, sf::Vector2i initialPos, int direction);
    void growLeaves(Tile *t_first, int intensity);
    sf::Vector2i getPositionReproduce(Tree *left, Tree *right);
    int _min_x;
    int _max_x;
    TreeGenetics _gens;
    std::vector<sf::Vector2i> _root;
    std::vector<sf::Vector2i> _branches;
};


#endif //GENLAND_TREE_H
