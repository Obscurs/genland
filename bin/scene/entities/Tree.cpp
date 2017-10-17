//
// Created by arnito on 6/08/17.
//

#include <cstdlib>
#include <algorithm>
#include "Tree.h"
#include "../Scene.h"
#include "../../Settings.h"
#include "../NoiseGenerator.h"

Tree::Tree(): Entity("tree"),
    _gens(){
    _left_n = nullptr;
    _right_n = nullptr;
    _rendered = false;
    _dead = true;
    _chunk = 0;
    _life = 0;
    _temperature = 0;
    _humidity = 0;
    _timeToReproduce = 0;
}
Tree::Tree(TreeGenetics* t,int chunk, sf::Vector2i position): Entity("tree"){
    _gens = *t;
    _chunk = chunk;
    setPosition(position);
    buildTree();
    _timeToReproduce = rand() % _gens._reproduceFactor + 10;
    _life = rand() % _gens._health + 10;
}
Tree::Tree(TreeGenetics* t1, TreeGenetics* t2,int chunk, sf::Vector2i position, int index): Entity("tree"),_gens(t1,t2, (float)(t1->_strenghtGen)/100){
    _chunk = chunk;
    setPosition(position);
    setEcosystemIndex(index);
    buildTree();
}
void Tree::setPosition(sf::Vector2i position){
    _position = sf::Vector2f(position);

    int y_pos = Chunk::N_TILES_Y-1-_position.y/Settings::TILE_SIZE_HIGH;
    float height_factor = float(y_pos)/float(Chunk::N_TILES_Y);
    int valHumidity = int(NoiseGenerator::getNoise("noise_humidity")->valSimplex2D(0, _position.x));
    float heightTemp = (1-height_factor)*(Settings::MAX_TEMPERATURE-Settings::MIN_TEMPERATURE)+Settings::MIN_TEMPERATURE;
    int valTemperature = int(heightTemp)+int(NoiseGenerator::getNoise("base_noise_temperature")->valSimplex2D(0, _position.x));
    _humidity = valHumidity;
    _temperature = valTemperature;
}
void Tree::buildTree(){
    int height;
    if(_gens._height==1) height = rand()% 4 + 2;
    else if(_gens._height==2) height = rand() % 10 +5;
    else if(_gens._height==3) height = rand() % 15 +15;
    int amplitude;
    if(height > 15) amplitude = std::max(_gens._amplitude,2);
    else if(height > 5) amplitude = std::min(_gens._amplitude,2);
    else amplitude = 1;
    _min_x = 0;
    _max_x = 0;
    _life = _gens._health;
    _timeToReproduce =_gens._reproduceFactor+OFFSET_REPRODUCE;
    int x_deviation = 0;
    int corb;
    if(_gens._corb==1) corb = rand()% 9 -9;
    else if(_gens._corb==2) corb = rand()% 10 -5;
    else corb = rand()% 9 +1;
    bool left_branch_ant = false;
    bool right_branch_ant = false;
    _left_n = nullptr;
    _right_n = nullptr;
    _rendered = false;
    _dead = false;

    for(int y = 0; y < height; y++){
        int old_deviation = x_deviation;
        int currentSizeBranch = std::min(int(height*_gens._sizeBranch),height-y-1);
        if(corb>0){
            x_deviation += (rand()% (11 -corb) ==0);
        } else{
            x_deviation += (-1)*(rand()% (11 +corb) ==0);
        }

        if(!left_branch_ant && y>height/5){
            bool branch = (rand()% std::max((int((1-_gens._branchAmount)*10)+1),1))==0;
            if(branch) {
                left_branch_ant = true;
                makeBranch(currentSizeBranch, _gens._curveBranch, sf::Vector2i(x_deviation,y), -1);
            }
        }
        else  left_branch_ant = false;
        if(amplitude==1 && old_deviation !=x_deviation) _root.push_back(sf::Vector2i(old_deviation,y));
        for(int x = 0; x < amplitude; x++){
            _root.push_back(sf::Vector2i(x+x_deviation,y));
            if((x+x_deviation)> _max_x) _max_x =x+x_deviation;
            if((x+x_deviation)< _min_x) _min_x =x+x_deviation;
            if(y>=height-1) _branches.push_back(sf::Vector2i(x+x_deviation,y));


        }
        if(!right_branch_ant && y>height/5){
            bool branch = (rand()% std::max((int((1-_gens._branchAmount)*10)+1),1))==0;
            if(branch) {
                right_branch_ant = true;
                makeBranch(currentSizeBranch, _gens._curveBranch, sf::Vector2i(x_deviation-1+amplitude,y), 1);
            }
        }
        else  right_branch_ant = false;
    }
}

void Tree::addRoot(sf::Vector2i root){
    _root.push_back(root);
}
void Tree::addBranch(sf::Vector2i branch){
    _branches.push_back(branch);
}
void Tree::loadFromFile(std::ifstream &myfile){
    _dead = false;
    myfile >> _chunk;
    myfile >> _position.x;
    myfile >> _position.y;
    myfile >> _humidity >> _temperature >> _life >> _timeToReproduce;
    myfile >> _gens._branchAmount >> _gens._sizeBranch >> _gens._curveBranch;
    myfile >> _gens._cold >> _gens._hot >> _gens._humidity;
    myfile >> _gens._health >> _gens._reproduceFactor >> _gens._strenghtGen;
    myfile >> _gens._amplitude >> _gens._height >> _gens._corb >> _gens._densityLeave >> _gens._amountLeave >> _gens._typeLeave >> _min_x >> _max_x;
    int num_root;
    int num_branches;
    myfile >> num_root;
    for(int i=0; i< num_root; i++){
        int x, y;
        myfile >> x >> y;
        addRoot(sf::Vector2i(x,y));
    }
    myfile >> num_branches;
    for(int i=0; i< num_branches; i++){
        int x, y;
        myfile >> x >> y;
        addBranch(sf::Vector2i(x,y));
    }
}

Tree::~Tree(){

}
sf::Vector2i Tree::getPosition(){
    return sf::Vector2i(_position);
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
int Tree::hasTwoChunks(){
    int chunk_mid =_position.x/(Chunk::N_TILES_X*Settings::TILE_SIZE);
    int chunk_left =(_position.x+(_min_x-2)*Settings::TILE_SIZE)/(Chunk::N_TILES_X*Settings::TILE_SIZE);
    int chunk_right =(_position.x+(_max_x+2)*Settings::TILE_SIZE)/(Chunk::N_TILES_X*Settings::TILE_SIZE);
    if(chunk_mid == chunk_left && chunk_mid == chunk_right) return 0;
    else if(chunk_left != chunk_mid) return -1;
    else return 1;
}

void Tree::treeToTiles(Tile *t_first, int index_chunk_in_mat){
    float c_mid_aux = (float)_position.x/(Chunk::N_TILES_X*Settings::TILE_SIZE);
    float c_left_aux =((float)_position.x+(_min_x-2)*Settings::TILE_SIZE)/(Chunk::N_TILES_X*Settings::TILE_SIZE);
    float c_right_aux =((float)_position.x+(_max_x+2)*Settings::TILE_SIZE)/(Chunk::N_TILES_X*Settings::TILE_SIZE);
    c_mid_aux = int(_position.x)%(Chunk::N_TILES_X*Settings::TILE_SIZE) == 0 ? c_mid_aux+1 : c_mid_aux;
    int chunk_mid = (c_mid_aux < 0) ? (int)c_mid_aux-1 : (int)c_mid_aux;
    int chunk_left = (c_left_aux < 0) ? (int)c_left_aux-1 : (int)c_left_aux;
    int chunk_right = (c_right_aux < 0) ? (int)c_right_aux-1 : (int)c_right_aux;
    if((chunk_mid == chunk_left && chunk_mid == chunk_right) ||
            index_chunk_in_mat == 1 ||
            (index_chunk_in_mat == 0 && chunk_left == chunk_mid) ||
            (index_chunk_in_mat == 2 && chunk_right == chunk_mid)
            ) {
        _rendered = true;
        sf::Vector2i pos_ant = sf::Vector2i(0, 0);
        Tile *t = t_first;
        for (int i = 0; i < _root.size(); i++) {
            sf::Vector2i pos_curr = _root[i];
            for (int x = pos_ant.x; x < pos_curr.x; x++) {
                t = t->neighbors[3];
            }
            for (int x = pos_ant.x; x > pos_curr.x; x--) {
                t = t->neighbors[7];
            }
            for (int y = pos_ant.y; y < pos_curr.y; y++) {
                t = t->neighbors[1];
            }
            for (int y = pos_ant.y; y > pos_curr.y; y--) {
                t = t->neighbors[5];
            }
            t->reload("t");
            pos_ant = pos_curr;
        }
        for (int i = 0; i < _branches.size(); i++) {
            sf::Vector2i pos_curr = _branches[i];
            for (int x = pos_ant.x; x < pos_curr.x; x++) {
                t = t->neighbors[3];
            }
            for (int x = pos_ant.x; x > pos_curr.x; x--) {
                t = t->neighbors[7];
            }
            for (int y = pos_ant.y; y < pos_curr.y; y++) {
                t = t->neighbors[1];
            }
            for (int y = pos_ant.y; y > pos_curr.y; y--) {
                t = t->neighbors[5];
            }
            t->reload("t");
            growLeaves(t, _gens._amountLeave);
            pos_ant = pos_curr;
        }
    } else{
        _rendered = false;
    }
}
void Tree::growLeaves(Tile *t_first, int intensity){
    if(intensity>0){
        if(t_first->neighbors[8]->id =="0") t_first->neighbors[8]->reloadLeave("F", std::to_string(_gens._densityLeave),std::to_string(_gens._typeLeave));
    }
    Tile *t7 =  t_first->neighbors[7];
    Tile *t1 =  t_first->neighbors[1];
    Tile *t3 =  t_first->neighbors[3];
    Tile *t5 =  t_first->neighbors[5];
    if(intensity>1){
        if(t7 !=nullptr && t7->id == "0") t7->reloadLeave("f", std::to_string(_gens._densityLeave),std::to_string(_gens._typeLeave));
        if(t1 !=nullptr && t1->id == "0") t1->reloadLeave("f", std::to_string(_gens._densityLeave),std::to_string(_gens._typeLeave));
        if(t3 !=nullptr && t3->id == "0") t3->reloadLeave("f", std::to_string(_gens._densityLeave),std::to_string(_gens._typeLeave));
        if(t5 !=nullptr && t5->id == "0") t5->reloadLeave("f", std::to_string(_gens._densityLeave),std::to_string(_gens._typeLeave));
    }
    if(intensity>2){
        if(t7 !=nullptr && t7->neighbors[8]->id == "0") {
            t7->neighbors[8]->reloadLeave("F", std::to_string(_gens._densityLeave),std::to_string(_gens._typeLeave));
            if(t7->neighbors[7] != nullptr && t7->neighbors[7]->id =="0") t7->neighbors[7]->reloadLeave("f", std::to_string(_gens._densityLeave),std::to_string(_gens._typeLeave));
            if(t7->neighbors[5] != nullptr && t7->neighbors[5]->id =="0") t7->neighbors[5]->reloadLeave("f", std::to_string(_gens._densityLeave),std::to_string(_gens._typeLeave));
            if(t7->neighbors[1] != nullptr && t7->neighbors[1]->id =="0") t7->neighbors[1]->reloadLeave("f", std::to_string(_gens._densityLeave),std::to_string(_gens._typeLeave));
        }
        if(t1 !=nullptr && t1->neighbors[8]->id == "0") {
            t1->neighbors[8]->reloadLeave("F", std::to_string(_gens._densityLeave),std::to_string(_gens._typeLeave));
            if(t1->neighbors[7] != nullptr && t1->neighbors[7]->id =="0") t1->neighbors[7]->reloadLeave("f", std::to_string(_gens._densityLeave),std::to_string(_gens._typeLeave));
            if(t1->neighbors[3] != nullptr && t1->neighbors[3]->id =="0") t1->neighbors[3]->reloadLeave("f", std::to_string(_gens._densityLeave),std::to_string(_gens._typeLeave));
            if(t1->neighbors[1] != nullptr && t1->neighbors[1]->id =="0") t1->neighbors[1]->reloadLeave("f", std::to_string(_gens._densityLeave),std::to_string(_gens._typeLeave));
        }
        if(t3 !=nullptr && t3->neighbors[8]->id == "0") {
            t3->neighbors[8]->reloadLeave("F", std::to_string(_gens._densityLeave),std::to_string(_gens._typeLeave));
            if(t3->neighbors[5] != nullptr && t3->neighbors[5]->id =="0") t3->neighbors[5]->reloadLeave("f", std::to_string(_gens._densityLeave),std::to_string(_gens._typeLeave));
            if(t3->neighbors[3] != nullptr && t3->neighbors[3]->id =="0") t3->neighbors[3]->reloadLeave("f", std::to_string(_gens._densityLeave),std::to_string(_gens._typeLeave));
            if(t3->neighbors[1] != nullptr && t3->neighbors[1]->id =="0") t3->neighbors[1]->reloadLeave("f", std::to_string(_gens._densityLeave),std::to_string(_gens._typeLeave));
        }
        if(t5 !=nullptr && t5->neighbors[8]->id == "0") {
            t5->neighbors[8]->reloadLeave("F", std::to_string(_gens._densityLeave),std::to_string(_gens._typeLeave));
            if(t5->neighbors[5] != nullptr && t5->neighbors[5]->id =="0") t5->neighbors[5]->reloadLeave("f", std::to_string(_gens._densityLeave),std::to_string(_gens._typeLeave));
            if(t5->neighbors[3] != nullptr && t5->neighbors[3]->id =="0") t5->neighbors[3]->reloadLeave("f", std::to_string(_gens._densityLeave),std::to_string(_gens._typeLeave));
            if(t5->neighbors[7] != nullptr && t5->neighbors[7]->id =="0") t5->neighbors[7]->reloadLeave("f", std::to_string(_gens._densityLeave),std::to_string(_gens._typeLeave));
        }
    }
}
void Tree::makeBranch(int size, float curve, sf::Vector2i initialPos, int direction){
    int displacement_y = 0;
    for(int x = 0; x < size; x++){
        bool disp = (rand()% std::max((int((1-curve)*10)+1),1))==0;
        if(disp) _branches.push_back(sf::Vector2i(direction*x+initialPos.x,initialPos.y+displacement_y));
        displacement_y+=disp;
        if(direction*x+initialPos.x >_max_x ) _max_x =direction*x+initialPos.x;
        if(direction*x+initialPos.x <_min_x ) _min_x =direction*x+initialPos.x;
        _branches.push_back(sf::Vector2i(direction*x+initialPos.x,initialPos.y+displacement_y));
    }

}


void Tree::checkTreeTiles(){
    if(_rendered){
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
    if(_root.size() ==0) _dead = true;

}
void Tree::saveToFile(int chunk, std::ofstream &myfile){
    myfile << "tree" << " ";
    myfile << chunk << " ";
    myfile << _position.x << " " << _position.y << " ";
    myfile << _humidity << " " << _temperature << " " << _life << " " << _timeToReproduce << " ";
    myfile << _gens._branchAmount << " " << _gens._sizeBranch << " " << _gens._curveBranch << " ";
    myfile << _gens._cold << " " << _gens._hot << " " << _gens._humidity << " ";
    myfile << _gens._health << " " << _gens._reproduceFactor << " " << _gens._strenghtGen << " ";
    myfile << _gens._amplitude << " " << _gens._height << " " << _gens._corb << " " << _gens._densityLeave << " " << _gens._amountLeave << " " << _gens._typeLeave << " " << _min_x << " " << _max_x << " ";
    myfile << _root.size() << " ";
    for(int i = 0; i < _root.size(); i++){
        myfile << _root[i].x << " " <<_root[i].y << " ";
    }
    myfile << _branches.size() << " ";
    for(int i = 0; i < _branches.size(); i++){
        myfile << _branches[i].x << " " <<_branches[i].y << " ";
    }

}

bool Tree::isDead(){
    if(_root.size() ==0) _dead = true;
    return (_root.size() ==0);
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
void Tree::kill(){
    Tree *tl = getLeftTree();
    Tree *tr = getRightTree();
    if(tl != nullptr) tl->setRightTree(tr);
    if(tr != nullptr) tr->setLeftTree(tl);
}
bool Tree::update(float delta, Clock *c){
    int totalTemp = _temperature + c->_globalTemperature;
    int totalHum = _humidity + c->_globalHumidity;
    float humDamage = totalHum*(1-float(_gens._humidity)/100)*delta;
    float tempDamage;
    if(totalTemp>0) tempDamage = totalTemp*(1-float(_gens._hot)/100)*delta;
    else tempDamage = -totalTemp*(1-float(_gens._cold)/100)*delta;
    _life -= ((tempDamage+humDamage+delta)/2000)*Settings::GEN_SPEED;
    if(_life<=0) {
        std::cout << "a tree died" << std::endl;
        _dead = true;
        return false;
    }
    else {
        _timeToReproduce -= (delta/10)*Settings::GEN_SPEED;
        if(_timeToReproduce <0) {
            std::cout << "new tree created" << std::endl;
            return true;
        }
    }
    return false;
}
TreeGenetics* Tree::getGenetics(){
    return &_gens;
}
Tree * Tree::reproduce(){

    int direction = rand() % 2;
    Tree *t_ini, *t_end;
    Tree *resultTree = nullptr;
    int chunk_ini, chunk_end;
    int offset_ini, offset_end;
    if(!direction){
        t_ini = _left_n;
        t_end = this;
    } else{
        t_ini = this;
        t_end = _right_n;
    }
    Scene *s = Scene::getScene();
    sf::Vector2i intervalEco =s->searchIntervalEcosystem(_chunk);
    _timeToReproduce = _gens._reproduceFactor+OFFSET_REPRODUCE;
    if(t_ini == nullptr) {
        t_end->_timeToReproduce = t_end->_gens._reproduceFactor+OFFSET_REPRODUCE;
        chunk_ini= intervalEco.x;
        chunk_end = _chunk;
        offset_ini = 0;
        resultTree = new Tree(t_end->getGenetics(),t_end->getGenetics(),0, sf::Vector2i(0,0), t_end->getEcosystemIndex());
        offset_end = Chunk::getIndexFromGlobalPosition(sf::Vector2f(_position.x,_position.y)).x+_min_x-2-resultTree->_max_x;
    }
    else if(t_end == nullptr){
        t_ini->_timeToReproduce = t_ini->_gens._reproduceFactor+OFFSET_REPRODUCE;
        chunk_ini = _chunk;
        chunk_end = intervalEco.y;
        offset_end = Chunk::N_TILES_X-1;
        resultTree = new Tree(t_ini->getGenetics(),t_ini->getGenetics(),0, sf::Vector2i(0,0),t_ini->getEcosystemIndex());
        offset_ini = Chunk::getIndexFromGlobalPosition(sf::Vector2f(_position.x,_position.y)).x+_max_x+2-resultTree->_min_x;
    } else {
        t_ini->_timeToReproduce = t_ini->_gens._reproduceFactor+OFFSET_REPRODUCE;
        t_end->_timeToReproduce = t_end->_gens._reproduceFactor+OFFSET_REPRODUCE;
        chunk_ini = t_ini->_chunk;
        chunk_end = t_end->_chunk;
        resultTree = new Tree(t_end->getGenetics(),t_ini->getGenetics(),0, sf::Vector2i(0,0),t_ini->getEcosystemIndex());
        offset_ini = Chunk::getIndexFromGlobalPosition(sf::Vector2f(t_ini->_position.x,t_ini->_position.y)).x+t_ini->_max_x+2-resultTree->_min_x;
        offset_end = Chunk::getIndexFromGlobalPosition(sf::Vector2f(t_end->_position.x,t_end->_position.y)).x+t_end->_min_x-2-resultTree->_max_x;
    }

    bool valid_emplacement = true;
    if(offset_ini > Chunk::N_TILES_X-1){
        offset_ini = offset_ini-Chunk::N_TILES_X;
        chunk_ini +=1;
        if(chunk_ini>intervalEco.y) valid_emplacement = false;
    }
    if(offset_end<0){
        offset_end = offset_end+Chunk::N_TILES_X;
        chunk_end -=1;
        if(chunk_end < intervalEco.x) valid_emplacement = false;
    }
    if(chunk_ini == chunk_end && offset_end <offset_ini) valid_emplacement = false;

    if(valid_emplacement){
        int current_chunk = chunk_ini;
        std::vector<std::vector<std::pair<int, bool> > > *surface = s->getSurface(intervalEco);
        std::vector<sf::Vector3i> grassPositions;
        while(current_chunk < chunk_end){

            int index_current_chunk = current_chunk-intervalEco.x;
            for(int i=0; i<Chunk::N_TILES_X; i++){
                std::pair<int, bool> surfaceTile = (*surface)[index_current_chunk][i];
                if(surfaceTile.second){

                    int globalPosTileY = surfaceTile.first*Settings::TILE_SIZE;
                    int globalPosTileX = i*Settings::TILE_SIZE + current_chunk*Settings::TILE_SIZE*Chunk::N_TILES_X;

                    grassPositions.push_back(sf::Vector3i(globalPosTileX,globalPosTileY,current_chunk));

                }

            }
            current_chunk +=1;

        }

        if(grassPositions.size()==0) valid_emplacement = false;
        if(valid_emplacement){

            sf::Vector3i rand_position = grassPositions[rand() % grassPositions.size()];


            resultTree->setPosition(sf::Vector2i(rand_position.x,rand_position.y));
            resultTree->_chunk = rand_position.z;
            resultTree->setRightTree(t_end);
            resultTree->setLeftTree(t_ini);
            if(t_ini != nullptr && (t_ini->_chunk > 100 || t_ini->_chunk < -100)){
                std::cout << "lol" << std::endl;
            }
            if(t_ini !=nullptr) t_ini->setRightTree(resultTree);
            if(t_end !=nullptr) t_end->setLeftTree(resultTree);
            //std::cout << " tree Reproduced " << resultTree->_chunk << " " << resultTree->_position.x << " " << resultTree->_position.y << std::endl;
            return resultTree;
        }
    }
    //std::cout << " imposible reproduce " << std::endl;
    return nullptr;

}
sf::Vector2i Tree::getPositionReproduce(Tree *left, Tree *right){
    Scene *s = Scene::getScene();
    int chunkIni, chunkEnd;
    //if(left != nullptr)
}