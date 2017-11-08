//
// Created by arnito on 17/10/17.
//

#include <SFML/Audio.hpp>
#include "Colisionable.h"
#include "../Settings.h"
#include "world/Tile.h"
#include "Scene.h"

Colisionable::Colisionable(){
    col_bottom = 0;
    col_top = 0;
    col_left = 0;
    col_right = 0;
    col_bottom_dist = 0;
    col_top_dist = 0;
    col_left_dist = 0;
    col_right_dist = 0;
}
Colisionable::~Colisionable(){

}
sf::Vector2f Colisionable::evalCollisions(sf::Vector2f posOld, sf::Vector2f posNew, sf::Vector2f size){
    /*
    float dist= sqrt((posNew.x-posOld.x)*(posNew.x-posOld.x)+(posNew.y-posOld.y)*(posNew.y-posOld.y));
    while(dist>Settings::TILE_SIZE){
        sf::Vector2f posOldOld = posOld;
        float ratio = (Settings::TILE_SIZE-1)/dist;
        sf::Vector2f newPosAux(((1-ratio)*posOld.x+ratio*posNew.x),((1-ratio)*posOld.y+ratio*posNew.y));
        posOld = evalCollisions(posOld,newPosAux,size);
        if(posOldOld == posOld) return posOld;
        dist= sqrt((posNew.x-posOld.x)*(posNew.x-posOld.x)+(posNew.y-posOld.y)*(posNew.y-posOld.y));
    }*/

    sf::Vector2f pos_aux(posNew.x, posNew.y);
    sf::Vector2f size_aux(size.x, size.y);
    Map* map = Scene::getScene()->getMap();
    Tile* firstT = map->getTile(pos_aux.x, pos_aux.y, 1);
    Tile* lastT = map->getTile(pos_aux.x+size_aux.x, pos_aux.y+size_aux.y, 1);
    if(firstT != nullptr && lastT != nullptr){
        col_bottom = 0;
        col_top = 0;
        col_left = 0;
        col_right = 0;
        col_bottom_dist = 0;
        col_top_dist = 0;
        col_left_dist = 0;
        col_right_dist = 0;
        std::vector<Tile*> tiles_col = map->getTilesCol(pos_aux, size_aux);
        //std::cout << "number of cols" << tiles_col._size() << std::endl;

        //std::cout << "lel" << std::endl;
        for(int i = 0; i< tiles_col.size(); ++i){
            //	//comproba colisions
            Tile* t = tiles_col[i];
            sf::Vector2f tile_size(t->getWidth(), t->getHeight());
            FixColision(pos_aux, size_aux, t->GetPosition(), tile_size);
        }
        bool valid_move = true;
        if(bool(col_bottom) + bool(col_left) + bool(col_right) + bool(col_top) >=3) valid_move = false;
        if(abs(posNew.x-posOld.x) > Settings::TILE_SIZE*3 || abs(posNew.y-posOld.y) > Settings::TILE_SIZE*3) {
            //float dist= sqrt((posNew.x-posOld.x)*(posNew.x-posOld.x)+(posNew.y-posOld.y)*(posNew.y-posOld.y));

            valid_move = false;
        }
        if(col_bottom >= col_top){
            col_top = 0;
            posNew.y = posNew.y - col_bottom_dist;
        }
        else{
            col_bottom = 0;
            posNew.y = posNew.y + col_top_dist;
        }
        if(col_left >= col_right){
            col_right = 0;
            posNew.x = posNew.x + col_left_dist;
        }
        else{
            col_left = 0;
            posNew.x = posNew.x - col_right_dist;
        }
        if(valid_move) return posNew;
        else {
            return evalCollisions(sf::Vector2f(posOld.x,posOld.y-32),sf::Vector2f(posOld.x,posOld.y-32),size);
        }
    }
    return posOld;
}
void Colisionable::FixColision(sf::Vector2f pos1, sf::Vector2f size1, sf::Vector2f pos2, sf::Vector2f size2){
    Map* map = Scene::getScene()->getMap();
    //std::cout << "1x " << pos1.x << " 1y "<< pos1.y << " 2x" << pos2.x << " 2y " << pos2.y  << std::endl;
    sf::Vector2f center1((pos1.x*2 + size1.x)/2, (pos1.y*2 + size1.y)/2);
    sf::Vector2f center2((pos2.x*2 + size2.x)/2, (pos2.y*2 + size2.y)/2);
    //std::cout << "cx " << center1.x << " cy "<< center1.y << " c2x" << center2.x << " c2y " << center2.y  << std::endl;
    float top1 = pos1.y;
    float bottom1 = pos1.y+size1.y;
    float left1 = pos1.x;
    float right1 = pos1.x+ size1.x;

    float top2 = pos2.y;
    float bottom2 = pos2.y+size2.y;
    float left2 = pos2.x;
    float right2 = pos2.x+ size2.x;

    //primer quadrant



    if(center1.x > center2.x && center1.y <= center2.y){
        //std::cout << "primer quadrant " << std::endl;
        float dist_left = right2-left1;
        float dist_bottom = bottom1 - top2;
        //std::cout << right2 << " "<< left1 << " " << dist_bottom << " " << dist_left << std::endl;
        //std::cout << "top " << size2.x << " bottom "<< bottom2 << " left" << left2 << " right " << right2 << std::endl;
        if(dist_left <= dist_bottom){
            //Chunk* c = map->getChunk(center2.x + Settings::TILE_SIZE, center2.y);
            Tile* t = map->getTile(center2.x + Settings::TILE_SIZE, center2.y, 1);
            if(t->id !="0"){
                //Chunk* c2 = map->getChunk(center2.x, center2.y - Settings::TILE_SIZE);
                Tile* t2 = map->getTile(center2.x, center2.y - Settings::TILE_SIZE, 1);
                if(t2->id !="0"){
                    if(col_left_dist < dist_left) col_left_dist = dist_left;
                    if(col_bottom_dist < dist_bottom) col_bottom_dist = dist_bottom;
                    ++col_bottom;
                    ++col_left;
                }
                else{
                    if(col_bottom_dist < dist_bottom) col_bottom_dist = dist_bottom;
                    ++col_bottom;
                }
            }else{
                if(col_left_dist < dist_left) col_left_dist = dist_left;
                ++col_left;
            }

        }
        else {
            //Chunk* c = map->getChunk(center2.x, center2.y - Settings::TILE_SIZE);
            Tile* t = map->getTile(center2.x, center2.y - Settings::TILE_SIZE, 1);
            if(t->id !="0"){
                //Chunk* c2 =  map->getChunk(center2.x + Settings::TILE_SIZE, center2.y);
                Tile* t2 = map->getTile(center2.x + Settings::TILE_SIZE, center2.y, 1);
                if(t2->id !="0"){
                    if(col_left_dist < dist_left) col_left_dist = dist_left;
                    if(col_bottom_dist < dist_bottom) col_bottom_dist = dist_bottom;
                    ++col_left;
                    ++col_bottom;
                }
                else{
                    if(col_left_dist < dist_left) col_left_dist = dist_left;
                    ++col_left;
                }
            }
            else{
                if(col_bottom_dist < dist_bottom) col_bottom_dist = dist_bottom;
                ++col_bottom;
            }

        }

    }


        //segon quadrant
    else if(center1.x <= center2.x && center1.y < center2.y){
        //std::cout << "segon quadrant " << std::endl;
        float dist_right = right1 - left2;
        float dist_bottom = bottom1 - top2;

        if(dist_right <= dist_bottom){
            //Chunk* c = map->getChunk(center2.x - Settings::TILE_SIZE, center2.y);
            Tile* t = map->getTile(center2.x - Settings::TILE_SIZE, center2.y, 1);
            if(t->id !="0"){
                //Chunk* c2 = map->getChunk(center2.x, center2.y - Settings::TILE_SIZE);
                Tile* t2 = map->getTile(center2.x, center2.y - Settings::TILE_SIZE, 1);
                if(t2->id !="0"){
                    if(col_right_dist < dist_right) col_right_dist = dist_right;
                    if(col_bottom_dist < dist_bottom) col_bottom_dist = dist_bottom;
                    ++col_bottom;
                    ++col_right;
                }
                else{
                    if(col_bottom_dist < dist_bottom) col_bottom_dist = dist_bottom;
                    ++col_bottom;
                }
            }
            else{
                if(col_right_dist < dist_right) col_right_dist = dist_right;
                ++col_right;
            }
        }
        else {
            //Chunk* c = map->getChunk(center2.x, center2.y - Settings::TILE_SIZE);
            Tile* t = map->getTile(center2.x, center2.y - Settings::TILE_SIZE, 1);
            if(t->id !="0"){
                //Chunk* c2 =  map->getChunk(center2.x - Settings::TILE_SIZE, center2.y);
                Tile* t2 = map->getTile(center2.x - Settings::TILE_SIZE, center2.y, 1);
                if(t2->id !="0"){
                    if(col_right_dist < dist_right) col_right_dist = dist_right;
                    if(col_bottom_dist < dist_bottom) col_bottom_dist = dist_bottom;
                    ++col_right;
                    ++col_bottom;
                }
                else{
                    if(col_right_dist < dist_right) col_right_dist = dist_right;
                    ++col_right;
                }
            }
            else{
                if(col_bottom_dist < dist_bottom) col_bottom_dist = dist_bottom;
                ++col_bottom;
            }
        }

    }
        //tercer quadrant
    else if(center1.x < center2.x && center1.y >= center2.y){
        //std::cout << "tercer quadrant " << std::endl;
        float dist_right = right1 - left2;
        float dist_top = bottom2 - top1;

        if(dist_right <= dist_top){
            //Chunk* c = map->getChunk(center2.x - Settings::TILE_SIZE, center2.y);
            Tile* t = map->getTile(center2.x - Settings::TILE_SIZE, center2.y, 1);
            if(t->id !="0"){
                //Chunk* c2 = map->getChunk(center2.x, center2.y + Settings::TILE_SIZE);
                Tile* t2 = map->getTile(center2.x, center2.y + Settings::TILE_SIZE, 1);
                if(t2->id !="0"){
                    if(col_right_dist < dist_right) col_right_dist = dist_right;
                    if(col_top_dist < dist_top) col_top_dist = dist_top;
                    ++col_top;
                    ++col_right;
                }
                else{
                    if(col_top_dist < dist_top) col_top_dist = dist_top;
                    ++col_top;
                }
            }
            else{
                if(col_right_dist < dist_right) col_right_dist = dist_right;
                ++col_right;
            }

        }
        else {

            //Chunk* c = map->getChunk(center2.x, center2.y + Settings::TILE_SIZE);
            Tile* t = map->getTile(center2.x, center2.y + Settings::TILE_SIZE, 1);
            if(t->id !="0"){
                //Chunk* c2 = map->getChunk(center2.x - Settings::TILE_SIZE, center2.y);
                Tile* t2 = map->getTile(center2.x - Settings::TILE_SIZE, center2.y, 1);
                if(t2->id !="0"){
                    if(col_right_dist < dist_right) col_right_dist = dist_right;
                    if(col_top_dist < dist_top) col_top_dist = dist_top;
                    ++col_right;
                    ++col_top;
                }
                else{
                    if(col_right_dist < dist_right) col_right_dist = dist_right;
                    ++col_right;
                }
            }
            else{
                if(col_top_dist < dist_top) col_top_dist = dist_top;
                ++col_top;
            }
        }
    }
        //quart quadrant
    else if(center1.x >= center2.x && center1.y > center2.y){
        //std::cout << "quart quadrant " << std::endl;
        float dist_left = right2-left1;
        float dist_top = bottom2 - top1;
        if(dist_left <= dist_top){
            //Chunk* c = map->getChunk(center2.x + Settings::TILE_SIZE, center2.y);
            Tile* t = map->getTile(center2.x + Settings::TILE_SIZE, center2.y, 1);
            if(t->id !="0"){
                //Chunk* c2 = map->getChunk(center2.x, center2.y + Settings::TILE_SIZE);
                Tile* t2 = map->getTile(center2.x, center2.y + Settings::TILE_SIZE, 1);
                if(t2->id !="0"){
                    if(col_left_dist < dist_left) col_left_dist = dist_left;
                    if(col_top_dist < dist_top) col_top_dist = dist_top;
                    ++col_top;
                    ++col_left;
                }
                else{
                    if(col_top_dist < dist_top) col_top_dist = dist_top;
                    ++col_top;
                }
            }else{
                if(col_left_dist < dist_left) col_left_dist = dist_left;
                ++col_left;
            }
        }
        else {
            //Chunk* c = map->getChunk(center2.x, center2.y + Settings::TILE_SIZE);
            Tile* t = map->getTile(center2.x, center2.y + Settings::TILE_SIZE, 1);
            if(t->id !="0"){
                //Chunk* c2 = map->getChunk(center2.x + Settings::TILE_SIZE, center2.y);
                Tile* t2 = map->getTile(center2.x + Settings::TILE_SIZE, center2.y, 1);

                if(t2->id !="0"){
                    if(col_left_dist < dist_left) col_left_dist = dist_left;
                    if(col_top_dist < dist_top) col_top_dist = dist_top;
                    ++col_left;
                    ++col_top;
                }
                else{
                    if(col_left_dist < dist_left) col_left_dist = dist_left;
                    ++col_left;
                }
            }else{
                if(col_top_dist < dist_top) col_top_dist = dist_top;
                ++col_top;

            }
        }
    }
}