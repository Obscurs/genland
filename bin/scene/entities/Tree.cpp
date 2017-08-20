//
// Created by arnito on 6/08/17.
//

#include <cstdlib>
#include <algorithm>
#include "Tree.h"
#include "../Scene.h"
#include "../../Settings.h"

Tree::Tree(sf::Vector2i position, int amplitud, int altura, int corba, float branchAmount, float sizeBranch, float curveBranch, int leaveDensity, int leaveAmount, int leaveType){
    if(altura==1) _height = rand()% 4 + 2;
    else if(altura==2) _height = rand() % 10 +5;
    else if(altura==3) _height = rand() % 15 +15;
    _position = position;
    if(_height > 15) _amplitude = std::max(amplitud,2);
    else if(_height > 5) _amplitude = std::min(amplitud,2);
    else _amplitude = 1;
    _amountLeave = leaveAmount;
    _densityLeave = leaveDensity;
    _typeLeave = leaveType;
    _min_x = 0;
    _max_x = 0;
    int x_deviation = 0;
    if(corba==1) _corb = rand()% 10 -10;
    else if(corba==2) _corb = rand()% 10 -5;
    else _corb = rand()% 10 +1;
    bool left_branch_ant = false;
    bool right_branch_ant = false;
    _left_n = nullptr;
    _right_n = nullptr;
    for(int y = 0; y < _height; y++){
        int old_deviation = x_deviation;
        int currentSizeBranch = std::min(int(_height*sizeBranch),_height-y-1);
        if(_corb>0){
            x_deviation += (rand()% (11 -_corb) ==0);
        } else{
            x_deviation += (-1)*(rand()% (11 +_corb) ==0);
        }

        if(!left_branch_ant && y>_height/5){
            bool branch = (rand()% (int((1-branchAmount)*10)+1))==0;
            if(branch) {
                left_branch_ant = true;
                makeBranch(currentSizeBranch, curveBranch, sf::Vector2i(x_deviation,y), -1);
            }
        }
        else  left_branch_ant = false;
        if(_amplitude==1 && old_deviation !=x_deviation) _root.push_back(sf::Vector2i(old_deviation,y));
        for(int x = 0; x < _amplitude; x++){
            _root.push_back(sf::Vector2i(x+x_deviation,y));
            if((x+x_deviation)> _max_x) _max_x =x+x_deviation;
            if((x+x_deviation)< _min_x) _min_x =x+x_deviation;
            if(y>=_height-1) _branches.push_back(sf::Vector2i(x+x_deviation,y));


        }
        if(!right_branch_ant && y>_height/5){
            bool branch = (rand()% (int((1-branchAmount)*10)+1))==0;
            if(branch) {
                right_branch_ant = true;
                makeBranch(currentSizeBranch, curveBranch, sf::Vector2i(x_deviation-1+_amplitude,y), 1);
            }
        }
        else  right_branch_ant = false;
    }

}
Tree::~Tree(){

}
sf::Vector2i Tree::getPosition(){
    return _position;
}
bool Tree::isValidPosition(Tile *t_first){
    sf::Vector2i pos_ant = sf::Vector2i(0,0);
    Tile *t = t_first;
    bool is_valid = true;
    int i = 0;
    while(is_valid && i<_root.size()){
        sf::Vector2i pos_curr = _root[i];
        for(int x=pos_ant.x; x < pos_curr.x; x++){
            t = t->neighbors[3];
        }
        for(int x=pos_ant.x; x > pos_curr.x; x--){
            t = t->neighbors[7];
        }
        for(int y=pos_ant.y; y < pos_curr.y; y++){
            t = t->neighbors[1];
        }
        for(int y=pos_ant.y; y > pos_curr.y; y--){
            t = t->neighbors[5];
        }
        if(t->id != "0" || t->neighbors[8]->id != "0") is_valid = false;
        pos_ant = pos_curr;
        i++;
    }
    i = 0;
    while(is_valid && i<_branches.size()){
        sf::Vector2i pos_curr = _branches[i];
        for(int x=pos_ant.x; x < pos_curr.x; x++){
            t = t->neighbors[3];
        }
        for(int x=pos_ant.x; x > pos_curr.x; x--){
            t = t->neighbors[7];
        }
        for(int y=pos_ant.y; y < pos_curr.y; y++){
            t = t->neighbors[1];
        }
        for(int y=pos_ant.y; y > pos_curr.y; y--){
            t = t->neighbors[5];
        }
        if(t->id != "0" || t->neighbors[8]->id != "0") is_valid = false;
        pos_ant = pos_curr;
        i++;
    }
    return is_valid;
}
void Tree::treeToTiles(Tile *t_first){
    sf::Vector2i pos_ant = sf::Vector2i(0,0);
    Tile *t = t_first;
    for(int i=0; i< _root.size(); i++){
        sf::Vector2i pos_curr = _root[i];
        for(int x=pos_ant.x; x < pos_curr.x; x++){
            t = t->neighbors[3];
        }
        for(int x=pos_ant.x; x > pos_curr.x; x--){
            t = t->neighbors[7];
        }
        for(int y=pos_ant.y; y < pos_curr.y; y++){
            t = t->neighbors[1];
        }
        for(int y=pos_ant.y; y > pos_curr.y; y--){
            t = t->neighbors[5];
        }
        t->reload("t");
        pos_ant = pos_curr;
    }
    for(int i=0; i< _branches.size(); i++){
        sf::Vector2i pos_curr = _branches[i];
        for(int x=pos_ant.x; x < pos_curr.x; x++){
            t = t->neighbors[3];
        }
        for(int x=pos_ant.x; x > pos_curr.x; x--){
            t = t->neighbors[7];
        }
        for(int y=pos_ant.y; y < pos_curr.y; y++){
            t = t->neighbors[1];
        }
        for(int y=pos_ant.y; y > pos_curr.y; y--){
            t = t->neighbors[5];
        }
        t->reload("t");
        growLeaves(t,_amountLeave);
        pos_ant = pos_curr;
    }
}
void Tree::growLeaves(Tile *t_first, int intensity){
    if(intensity>0){
        if(t_first->neighbors[8]->id =="0") t_first->neighbors[8]->reloadLeave("F", std::to_string(_densityLeave),std::to_string(_typeLeave));
    }
    Tile *t7 =  t_first->neighbors[7];
    Tile *t1 =  t_first->neighbors[1];
    Tile *t3 =  t_first->neighbors[3];
    Tile *t5 =  t_first->neighbors[5];
    if(intensity>1){
        if(t7 !=nullptr && t7->id == "0") t7->reloadLeave("f", std::to_string(_densityLeave),std::to_string(_typeLeave));
        if(t1 !=nullptr && t1->id == "0") t1->reloadLeave("f", std::to_string(_densityLeave),std::to_string(_typeLeave));
        if(t3 !=nullptr && t3->id == "0") t3->reloadLeave("f", std::to_string(_densityLeave),std::to_string(_typeLeave));
        if(t5 !=nullptr && t5->id == "0") t5->reloadLeave("f", std::to_string(_densityLeave),std::to_string(_typeLeave));
    }
    if(intensity>2){
        if(t7 !=nullptr && t7->neighbors[8]->id == "0") {
            t7->neighbors[8]->reloadLeave("F", std::to_string(_densityLeave),std::to_string(_typeLeave));
            if(t7->neighbors[7] != nullptr && t7->neighbors[7]->id =="0") t7->neighbors[7]->reloadLeave("f", std::to_string(_densityLeave),std::to_string(_typeLeave));
            if(t7->neighbors[5] != nullptr && t7->neighbors[5]->id =="0") t7->neighbors[5]->reloadLeave("f", std::to_string(_densityLeave),std::to_string(_typeLeave));
            if(t7->neighbors[1] != nullptr && t7->neighbors[1]->id =="0") t7->neighbors[1]->reloadLeave("f", std::to_string(_densityLeave),std::to_string(_typeLeave));
        }
        if(t1 !=nullptr && t1->neighbors[8]->id == "0") {
            t1->neighbors[8]->reloadLeave("F", std::to_string(_densityLeave),std::to_string(_typeLeave));
            if(t1->neighbors[7] != nullptr && t1->neighbors[7]->id =="0") t1->neighbors[7]->reloadLeave("f", std::to_string(_densityLeave),std::to_string(_typeLeave));
            if(t1->neighbors[3] != nullptr && t1->neighbors[3]->id =="0") t1->neighbors[3]->reloadLeave("f", std::to_string(_densityLeave),std::to_string(_typeLeave));
            if(t1->neighbors[1] != nullptr && t1->neighbors[1]->id =="0") t1->neighbors[1]->reloadLeave("f", std::to_string(_densityLeave),std::to_string(_typeLeave));
        }
        if(t3 !=nullptr && t3->neighbors[8]->id == "0") {
            t3->neighbors[8]->reloadLeave("F", std::to_string(_densityLeave),std::to_string(_typeLeave));
            if(t3->neighbors[5] != nullptr && t3->neighbors[5]->id =="0") t3->neighbors[5]->reloadLeave("f", std::to_string(_densityLeave),std::to_string(_typeLeave));
            if(t3->neighbors[3] != nullptr && t3->neighbors[3]->id =="0") t3->neighbors[3]->reloadLeave("f", std::to_string(_densityLeave),std::to_string(_typeLeave));
            if(t3->neighbors[1] != nullptr && t3->neighbors[1]->id =="0") t3->neighbors[1]->reloadLeave("f", std::to_string(_densityLeave),std::to_string(_typeLeave));
        }
        if(t5 !=nullptr && t5->neighbors[8]->id == "0") {
            t5->neighbors[8]->reloadLeave("F", std::to_string(_densityLeave),std::to_string(_typeLeave));
            if(t5->neighbors[5] != nullptr && t5->neighbors[5]->id =="0") t5->neighbors[5]->reloadLeave("f", std::to_string(_densityLeave),std::to_string(_typeLeave));
            if(t5->neighbors[3] != nullptr && t5->neighbors[3]->id =="0") t5->neighbors[3]->reloadLeave("f", std::to_string(_densityLeave),std::to_string(_typeLeave));
            if(t5->neighbors[7] != nullptr && t5->neighbors[7]->id =="0") t5->neighbors[7]->reloadLeave("f", std::to_string(_densityLeave),std::to_string(_typeLeave));
        }
    }
}
void Tree::makeBranch(int size, float curve, sf::Vector2i initialPos, int direction){
    int displacement_y = 0;
    for(int x = 0; x < size; x++){
        bool disp = (rand()% (int((1-curve)*10)+1))==0;
        if(disp) _branches.push_back(sf::Vector2i(direction*x+initialPos.x,initialPos.y+displacement_y));
        displacement_y+=disp;
        _branches.push_back(sf::Vector2i(direction*x+initialPos.x,initialPos.y+displacement_y));
    }
    if(size+initialPos.x >_max_x ) _max_x =size+initialPos.x;
    if(initialPos.x-size >_min_x ) _min_x =initialPos.x-size;
}

void Tree::checkTreeTiles(){
    Map* map = Scene::getScene()->getMap();
    int size = int(_root.size());
    for(int i = 0; i< size; i++){
        Tile *t = map->getTile(_position.x+_root[i].x*Settings::TILE_SIZE, _position.y-_root[i].y*Settings::TILE_SIZE,0);
        if(t != nullptr && !t->_isTree) {
            _root.erase(_root.begin()+i);
            size = int(_root.size());
            i--;
        }
    }
    size = int(_branches.size());
    for(int i = 0; i< size; i++){
        Tile *t = map->getTile(_position.x+_branches[i].x*Settings::TILE_SIZE, _position.y-_branches[i].y*Settings::TILE_SIZE,0);
        if(t != nullptr && !t->_isTree) {
            _branches.erase(_branches.begin()+i);
            size = int(_branches.size());
            i--;
        }
    }
}
bool Tree::isDead(){
    return (_branches.size()==0 && _root.size() ==0);
}
Tree* Tree::getLeftTree(){
    return _left_n;
}
Tree* Tree::getRightTree(){
    return _right_n;
}
void Tree::setLeftTree(Tree *t){
    _left_n = t;
}
void Tree::setRightTree(Tree *t){
    _right_n = t;
}