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
    static const int SPAWN_SPRITE_MAX_TIME = 1;
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
    void draw(sf::RenderTarget & renderTar);
    int hasTwoChunks();
    bool _rendered;
    bool _dead;
    Tree *_left_n;
    Tree * _right_n;
    void kill();
    Tree * reproduce(int direction);
    Tree * reproduce2();
    TreeGenetics* getGenetics();
    float _life;
    float _timeToReproduce;
    int _temperature;
    int _humidity;
    int _min_x;
    int _max_x;

    float _debug_last_damage_distance;
    float _debug_last_damage_temp;
    float _debug_last_damage_hum;
    float _debug_last_damage_time;
private:
    void buildTree();
    bool isValidPosition(Tile *t_first);
    void makeBranch(int size, float curve, sf::Vector2i initialPos, int direction);
    void growLeaves(Tile *t_first, int intensity);
    sf::Vector2i getPositionReproduce(Tree *left, Tree *right);

    TreeGenetics _gens;
    std::vector<sf::Vector2i> _root;
    std::vector<sf::Vector2i> _branches;
    int _keyframe;
    float _spriteTime;
    sf::Sprite _spriteSpawn;
};


#endif //GENLAND_TREE_H
