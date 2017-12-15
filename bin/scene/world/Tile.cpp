#include <stdio.h>


// TODO: reference additional headers your program requires here
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <map>
#include <iostream>
#include <cassert>

#include "Chunk.h"
#include "../../Settings.h"
#include "../../Resources.h"
#include "../../SoundManager.hpp"


Tile::Tile(int l){
    layer = l;
    being_removed = false;
    last_tension_debug =0;
    _temperature = 0;
    _mountain_factor =0;
    _humidity = 0;
    _bio = STANDARD;
    _leaveType = "0" ;
    _leaveDensity = "";
    _reach_sun = false;
    _isTree = false;
    _soundRemove = "";
}


Tile::~Tile()
{
	
}
void Tile::setReachFloorCascade(){
    Tile* tB = neighbors[8];
    if(id!="0")reach_floor = true;
    if(tB->id!="0")tB->reach_floor = true;
    if(neighbors[1] != nullptr && (neighbors[1]->id != "0" || neighbors[1]->neighbors[8]->id != "0")) neighbors[1]->setReachFloorCascade();
}
void Tile::removeReachFloorCascade(){

    Tile* tB = neighbors[8];
    reach_floor = false;
    tB->reach_floor = false;
    if(neighbors[1] != nullptr && (neighbors[1]->reach_floor || neighbors[1]->neighbors[8]->reach_floor)) neighbors[1]->removeReachFloorCascade();
}

void Tile::reloadLeave(std::string id,std::string density, std::string type){
    reload(id);
    _leaveDensity = density;
    _leaveType = type;
}
void Tile::reproduceSoundRemove(){
    if(_soundRemove != ""){
        SoundManager::playSound(_soundRemove);
    }
}
void Tile::reload(std::string new_id)
{
    _isTree = false;
    last_tension_debug =0;
	if(new_id == "0"){
		weight = 0;
		max_tension = 0;
		rigid = false;
		reach_floor = false;
        id_pick="0";
        ms_to_remove = 100;
        if(neighbors[8] != nullptr && !neighbors[8]->reach_floor) removeReachFloorCascade();
	}
    else {
        reach_floor = true;
        rigid = false;
        if (new_id == "D" || new_id == "d") {
            weight = 10;
            max_tension = 50;
            id_pick = "D";
            ms_to_remove = 100;
            _soundRemove = "dirt";
        }else if (new_id == "C" || new_id == "c") {
            weight = 20;
            max_tension = 300;
            id_pick = "C";
            ms_to_remove = 200;
            _soundRemove = "stones";
        }else if (new_id == "K" || new_id == "k") {
            weight = 20;
            max_tension = 300;
            id_pick = "K";
            ms_to_remove = 400;
            _soundRemove = "stones";
        }else if (new_id == "G" || new_id == "g") {
            weight = 20;
            max_tension = 300;
            id_pick = "G";
            ms_to_remove = 400;
            _soundRemove = "stones";
        }else if (new_id == "I" || new_id == "i") {
            weight = 20;
            max_tension = 300;
            id_pick = "I";
            ms_to_remove = 400;
            _soundRemove = "stones";
        }else if (new_id == "Y" || new_id == "y") {
            weight = 20;
            max_tension = 300;
            id_pick = "Y";
            ms_to_remove = 500;
            _soundRemove = "stones";
        }else if (new_id == "B" || new_id == "b") {
            weight = 20;
            max_tension = 300;
            rigid = true;
            id_pick = "B";
            ms_to_remove = 500;
            _soundRemove = "stones";
        }else if (new_id == "N" || new_id == "n") {
            weight = 10;
            max_tension = 5;
            id_pick = "N";
            ms_to_remove = 50;
            _soundRemove = "stones";
        }else if (new_id == "W" || new_id == "w") {
            weight = 5;
            max_tension = 7;
            id_pick = "D";
            ms_to_remove = 50;
            _soundRemove = "dirt";
        }else if (new_id == "R" || new_id == "r") {
            weight = 20;
            max_tension = 300;
            id_pick = "C";
            ms_to_remove = 100;
            _soundRemove = "stones";
        }else if (new_id == "J" || new_id == "j") {
            weight = 10;
            max_tension = 50;
            id_pick = "D";
            ms_to_remove = 100;
            _soundRemove = "dirt";
        }
        else if (new_id == "T" || new_id == "t") {
            weight = 7;
            max_tension = 60;
            id_pick = "T";
            ms_to_remove = 100;
            _isTree = true;
            _soundRemove = "dirt";
        }
        else if (new_id == "F" || new_id == "f") {
            weight = 0;
            max_tension = 0;
            id_pick = "F";
            ms_to_remove = 20;
            _soundRemove = "leaf";
        }
        else if (new_id == "A" || new_id == "a") {
            weight = 7;
            max_tension = 80;
            id_pick = "A";
            ms_to_remove = 20;
            _soundRemove = "dirt";
        }
        else if (new_id == "L" || new_id == "l") {
            weight = 7;
            max_tension = 80;
            id_pick = "L";
            ms_to_remove = 20;
            _soundRemove = "stones";
        }
        else {
            reload("0");
        }
        if(neighbors[8] != nullptr){
            reach_floor = (neighbors[8]->reach_floor || (neighbors[5] != nullptr &&
                                                         (neighbors[5]->reach_floor ||
                                                          neighbors[5]->neighbors[8]->reach_floor)));
            if(reach_floor) setReachFloorCascade();
        }

    }
    id = new_id;
	
}
bool Tile::drawable(){
	bool res =false;
    if(layer==1 && id=="0") return true;
	for(int i=0; i<8; i++){
		if(neighbors[i] !=nullptr && (neighbors[i]->id=="0" || neighbors[i]->id=="F" || neighbors[i]->id=="f")){
			res=true;
			break;
		}
	}
	return res;
}
bool Tile::isVisibleSun(){
    return (id =="0" || id =="F" || id == "f" || id == "L1" || id == "L2" || id == "L3"|| id == "l1" || id == "l2" || id == "l3");
}
void Tile::drawFadeOut(sf::VertexArray &vertexArray){
    if(neighbors[0] != nullptr && neighbors[0]->neighbors[8]->id != "0" && !neighbors[0]->drawable()  && neighbors[7]->drawable() && neighbors[1]->drawable()){
        appendSpriteToArray( vertexArray, -1, "S2", 90, sf::Vector2i(0,0));
    }
    if(neighbors[1] != nullptr && neighbors[1]->neighbors[8]->id != "0" && !neighbors[1]->drawable() ){
        appendSpriteToArray( vertexArray, -1, "S", 180, sf::Vector2i(0,0));
    }
    if(neighbors[2] != nullptr && neighbors[2]->neighbors[8]->id != "0" && !neighbors[2]->drawable() && neighbors[3]->drawable() && neighbors[1]->drawable()){
        appendSpriteToArray( vertexArray, -1, "S2", 180, sf::Vector2i(0,0));

    }
    if(neighbors[3] != nullptr && neighbors[3]->neighbors[8]->id != "0" && !neighbors[3]->drawable()){
        appendSpriteToArray( vertexArray, -1, "S", -90, sf::Vector2i(0,0));
    }
    if(neighbors[4] != nullptr && neighbors[4]->neighbors[8]->id != "0" && !neighbors[4]->drawable() && neighbors[3]->drawable() && neighbors[5]->drawable()){
        appendSpriteToArray( vertexArray, -1, "S2", -90, sf::Vector2i(0,0));
    }
    if(neighbors[5] != nullptr && neighbors[5]->neighbors[8]->id != "0" && !neighbors[5]->drawable()){
        appendSpriteToArray( vertexArray, -1, "S", 0, sf::Vector2i(0,0));

    }
    if(neighbors[6] != nullptr && neighbors[6]->neighbors[8]->id != "0" && !neighbors[6]->drawable() && neighbors[5]->drawable() && neighbors[7]->drawable() ){
        appendSpriteToArray( vertexArray, -1, "S2", 0, sf::Vector2i(0,0));

    }
    if(neighbors[7] != nullptr && (neighbors[7]->neighbors[8]->id != "0") && !neighbors[7]->drawable() ){
        appendSpriteToArray( vertexArray, -1, "S", 90, sf::Vector2i(0,0));
    }
}
void Tile::drawAmbientOclusion(sf::VertexArray &vertexArray){
    Tile* uper = neighbors[8];
    if(neighbors[0] != nullptr && !neighbors[0]->drawable() && neighbors[7]->drawable() && neighbors[1]->drawable()){

    }
    if(neighbors[1] != nullptr && uper->neighbors[1]->id!="0"){
        appendSpriteToArray( vertexArray, -1, "Swall", 180, sf::Vector2i(0,0));
    }
    if(neighbors[2] != nullptr && !neighbors[2]->drawable() && neighbors[3]->drawable() && neighbors[1]->drawable()){

    }
    if(neighbors[3] != nullptr && uper->neighbors[3]->id!="0"){
        appendSpriteToArray( vertexArray, -1, "Swall", -90, sf::Vector2i(0,0));
    }
    if(neighbors[4] != nullptr && !neighbors[4]->drawable() && neighbors[3]->drawable() && neighbors[5]->drawable()){

    }
    if(neighbors[5] != nullptr && uper->neighbors[5]->id!="0"){
        appendSpriteToArray( vertexArray, -1, "Swall", 0, sf::Vector2i(0,0));
        //appendSpriteToArray( vertexArray, -1, "Scorner", 0, sf::Vector2i(16,0));

    }
    if(neighbors[6] != nullptr && !neighbors[6]->drawable() && neighbors[5]->drawable() && neighbors[7]->drawable()){

    }
    if(neighbors[7] != nullptr && uper->neighbors[7]->id!="0"){
        appendSpriteToArray( vertexArray, -1, "Swall", 90, sf::Vector2i(0,0));
    }
}
void Tile::drawOuts(sf::VertexArray &vertexArray)
{
    Tile* t1 = neighbors[1];
    Tile* t3 = neighbors[3];
    Tile* t5 = neighbors[5];
    Tile* t7 = neighbors[7];
    bool valid_t1= (t1 != nullptr && (t1->id !="0"));
    bool valid_t3= (t3 != nullptr && (t3->id !="0"));
    bool valid_t5= (t5 != nullptr && (t5->id !="0"));
    bool valid_t7 = (t7 != nullptr && (t7->id !="0"));
    if(valid_t1 && valid_t3 && t1->id==t3->id){
        std::string id_mini = t1->id;
        id_mini.append("_out");
        appendSpriteToArray( vertexArray, 1, id_mini, 0, sf::Vector2i(0,0));
    }
    if(valid_t1 && valid_t7 && t1->id==t7->id){
        std::string id_mini = t1->id;
        id_mini.append("_out");
        appendSpriteToArray( vertexArray, 0, id_mini, 0, sf::Vector2i(0,0));
    }
    if(valid_t3 && valid_t5 && t3->id==t5->id){
        std::string id_mini = t3->id;
        id_mini.append("_out");
        appendSpriteToArray( vertexArray, 3, id_mini, 0, sf::Vector2i(0,0));
    }
    if(valid_t7 && valid_t5 && t5->id==t7->id){
        std::string id_mini = t5->id;
        id_mini.append("_out");
        appendSpriteToArray( vertexArray, 2, id_mini, 0, sf::Vector2i(0,0));
    }
}
void Tile::appendSpriteToArray(sf::VertexArray &vertexArray, int mini_pos, std::string id_in_tex,int rotation, sf::Vector2i displacement){
    TextureManager *texMan = Resources::getTextureManager("tileMap");
    float increment_x=displacement.x;
    float increment_y=displacement.y;
    float increment_x_tex=0;
    float increment_y_tex=0;
    int divisor=2;
    if(mini_pos==1) {
        increment_x+=Settings::TILE_SIZE/2;
        increment_x_tex+=texMan->size_sprite.x/2;
    }
    else if(mini_pos==2) {
        increment_y+=Settings::TILE_SIZE/2;
        increment_y_tex+=texMan->size_sprite.y/2;
    }
    else if(mini_pos==3){
        increment_x+=Settings::TILE_SIZE/2;
        increment_y+=Settings::TILE_SIZE/2;
        increment_y_tex+=texMan->size_sprite.y/2;
        increment_x_tex+=texMan->size_sprite.x/2;
    }
    if(mini_pos==-1) divisor=1;
    sf::Vector2i position_sprite = texMan->getPositionSprite(id_in_tex);
    sf::Vector2f pos_tex1, pos_tex2, pos_tex3, pos_tex4;
    if(rotation==0){
        pos_tex1 = sf::Vector2f(position_sprite.x+increment_x_tex, position_sprite.y+increment_y_tex);
        pos_tex2 = sf::Vector2f(position_sprite.x+texMan->size_sprite.x/divisor+increment_x_tex, position_sprite.y+increment_y_tex);
        pos_tex3 = sf::Vector2f(position_sprite.x+texMan->size_sprite.x/divisor+increment_x_tex, position_sprite.y+texMan->size_sprite.y/divisor+increment_y_tex);
        pos_tex4 = sf::Vector2f(position_sprite.x+increment_x_tex, position_sprite.y+texMan->size_sprite.y/divisor+increment_y_tex);
    } else if(rotation==90){
        pos_tex2 = sf::Vector2f(position_sprite.x+increment_x_tex, position_sprite.y+increment_y_tex);
        pos_tex3 = sf::Vector2f(position_sprite.x+texMan->size_sprite.x/divisor+increment_x_tex, position_sprite.y+increment_y_tex);
        pos_tex4 = sf::Vector2f(position_sprite.x+texMan->size_sprite.x/divisor+increment_x_tex, position_sprite.y+texMan->size_sprite.y/divisor+increment_y_tex);
        pos_tex1 = sf::Vector2f(position_sprite.x+increment_x_tex, position_sprite.y+texMan->size_sprite.y/divisor+increment_y_tex);
    }
    else if(rotation==-90){
        pos_tex4 = sf::Vector2f(position_sprite.x+increment_x_tex, position_sprite.y+increment_y_tex);
        pos_tex1 = sf::Vector2f(position_sprite.x+texMan->size_sprite.x/divisor+increment_x_tex, position_sprite.y+increment_y_tex);
        pos_tex2 = sf::Vector2f(position_sprite.x+texMan->size_sprite.x/divisor+increment_x_tex, position_sprite.y+texMan->size_sprite.y/divisor+increment_y_tex);
        pos_tex3 = sf::Vector2f(position_sprite.x+increment_x_tex, position_sprite.y+texMan->size_sprite.y/divisor+increment_y_tex);
    }
    else if(rotation==180){
        pos_tex3 = sf::Vector2f(position_sprite.x+increment_x_tex, position_sprite.y+increment_y_tex);
        pos_tex4 = sf::Vector2f(position_sprite.x+texMan->size_sprite.x/divisor+increment_x_tex, position_sprite.y+increment_y_tex);
        pos_tex1 = sf::Vector2f(position_sprite.x+texMan->size_sprite.x/divisor+increment_x_tex, position_sprite.y+texMan->size_sprite.y/divisor+increment_y_tex);
        pos_tex2 = sf::Vector2f(position_sprite.x+increment_x_tex, position_sprite.y+texMan->size_sprite.y/divisor+increment_y_tex);
    }


    vertexArray.append(sf::Vertex(sf::Vector2f(position.x+increment_x,position.y+increment_y), pos_tex1));
    vertexArray.append(sf::Vertex(sf::Vector2f(position.x+increment_x+Settings::TILE_SIZE/divisor,position.y+increment_y), pos_tex2));
    vertexArray.append(sf::Vertex(sf::Vector2f(position.x+increment_x+Settings::TILE_SIZE/divisor,position.y+increment_y+Settings::TILE_SIZE/divisor), pos_tex3));
    vertexArray.append(sf::Vertex(sf::Vector2f(position.x+increment_x,position.y+Settings::TILE_SIZE/divisor+increment_y), pos_tex4));
}
void Tile::drawLeaves(sf::VertexArray &vertexArray){
    if(layer) appendSpriteToArray( vertexArray, -1, "L"+_leaveType+"_"+_leaveDensity, 0, sf::Vector2i(0,0));
    else appendSpriteToArray( vertexArray, -1, "l"+_leaveType+"_"+_leaveDensity, 0, sf::Vector2i(0,0));
}
void Tile::drawIns(sf::VertexArray &vertexArray){
    bool is_mini[4] ={0,0,0,0};
    if(neighbors[1] != nullptr && neighbors[1]->id=="0" && neighbors[2] != nullptr && neighbors[2]->id=="0" && neighbors[3] != nullptr && neighbors[3]->id=="0"){
        std::string id_mini = id;
        id_mini.append("_in");
        is_mini[0]=true;
        appendSpriteToArray( vertexArray, 1, id_mini, 0, sf::Vector2i(0,0));
    }
    if(neighbors[3] != nullptr && neighbors[3]->id=="0" && neighbors[4] != nullptr && neighbors[4]->id=="0" && neighbors[5] != nullptr && neighbors[5]->id=="0"){
        std::string id_mini = id;
        id_mini.append("_in");
        is_mini[1]=true;
        appendSpriteToArray( vertexArray, 3, id_mini, 0, sf::Vector2i(0,0));
    }
    if(neighbors[5] != nullptr && neighbors[5]->id=="0" && neighbors[6] != nullptr && neighbors[6]->id=="0" && neighbors[7] != nullptr && neighbors[7]->id=="0"){
        std::string id_mini = id;
        id_mini.append("_in");
        is_mini[2]=true;
        appendSpriteToArray( vertexArray, 2, id_mini, 0, sf::Vector2i(0,0));
    }
    if(neighbors[7] != nullptr && neighbors[7]->id=="0" && neighbors[1] != nullptr && neighbors[1]->id=="0" && neighbors[0] != nullptr && neighbors[0]->id=="0"){
        std::string id_mini = id;
        id_mini.append("_in");
        is_mini[3]=true;
        appendSpriteToArray( vertexArray, 0, id_mini, 0, sf::Vector2i(0,0));
    }
    if(!is_mini[0] && !is_mini[1] && !is_mini[2] && !is_mini[3]){
        appendSpriteToArray( vertexArray, -1, id, 0, sf::Vector2i(0,0));
    } else{
        if(!is_mini[0]) appendSpriteToArray( vertexArray, 1, id, 0, sf::Vector2i(0,0));
        if(!is_mini[1]) appendSpriteToArray( vertexArray, 3, id, 0, sf::Vector2i(0,0));
        if(!is_mini[2]) appendSpriteToArray( vertexArray, 2, id, 0, sf::Vector2i(0,0));
        if(!is_mini[3]) appendSpriteToArray( vertexArray, 0, id, 0, sf::Vector2i(0,0));
    }
    //Draw sombra full
    Tile *uper = neighbors[8];
    if(uper->layer && uper->id!="0" && uper->neighbors[7] != nullptr && uper->neighbors[1] != nullptr && uper->neighbors[3] != nullptr && uper->neighbors[5] != nullptr){
        if(uper->neighbors[7]->id == "0" && uper->neighbors[1]->id == "0" && (neighbors[7]->id !="0" || neighbors[1]->id !="0")){
            appendSpriteToArray( vertexArray, 0, "Sfull", 0, sf::Vector2i(0,0));
        }
        if(uper->neighbors[1]->id == "0" && uper->neighbors[3]->id == "0" && (neighbors[1]->id !="0" || neighbors[3]->id !="0")){
            appendSpriteToArray( vertexArray, 1, "Sfull", 0, sf::Vector2i(0,0));
            //appendSpriteToArray( vertexArray, -1, "Scorner", 0, sf::Vector2i(16,-16));
        }
        if(uper->neighbors[3]->id == "0" && uper->neighbors[5]->id == "0" && (neighbors[3]->id !="0" || neighbors[5]->id !="0")){
            appendSpriteToArray( vertexArray, 3, "Sfull", 0, sf::Vector2i(0,0));
            //appendSpriteToArray( vertexArray, -1, "Scorner", 90, sf::Vector2i(16,16));
        }
        if(uper->neighbors[5]->id == "0" && uper->neighbors[7]->id == "0" && (neighbors[5]->id !="0" || neighbors[7]->id !="0")){
            appendSpriteToArray( vertexArray, 2, "Sfull", 0, sf::Vector2i(0,0));

        }
    }
    else if(uper->layer && uper->id =="0"  && uper->neighbors[0] != nullptr && uper->neighbors[2] != nullptr && uper->neighbors[4] != nullptr && uper->neighbors[6] != nullptr){
        if(uper->neighbors[0]->id !="0"){
            appendSpriteToArray( vertexArray, -1, "Scorner", 90, sf::Vector2i(0,0));
        }
        if(uper->neighbors[2]->id !="0"){
            appendSpriteToArray( vertexArray, -1, "Scorner", 180, sf::Vector2i(0,0));
        }
        if(uper->neighbors[4]->id !="0"){
            appendSpriteToArray( vertexArray, -1, "Scorner", -90, sf::Vector2i(0,0));
        }
        if(uper->neighbors[6]->id !="0"){
            appendSpriteToArray( vertexArray, -1, "Scorner", 0, sf::Vector2i(0,0));
        }
    }
}
void Tile::drawSkyArray(sf::VertexArray &skyArray,bool isTree){
    sf::Color c;
    if(isTree){
        c = sf::Color(255,0,0,255);
    } else{
        c = sf::Color(0,0,255,255);
    }
    skyArray.append(sf::Vertex(sf::Vector2f(position.x,position.y), c));
    skyArray.append(sf::Vertex(sf::Vector2f(position.x+Settings::TILE_SIZE,position.y), c));
    skyArray.append(sf::Vertex(sf::Vector2f(position.x+Settings::TILE_SIZE,position.y+Settings::TILE_SIZE), c));
    skyArray.append(sf::Vertex(sf::Vector2f(position.x,position.y+Settings::TILE_SIZE), c));

}
void Tile::drawBorderSkyArray(sf::VertexArray &skyArray){
    sf::Vector2f pos_tex1, pos_tex2, pos_tex3, pos_tex4;
    pos_tex1 = sf::Vector2f(96, 0);
    pos_tex2 = sf::Vector2f(128, 0);
    pos_tex3 = sf::Vector2f(128, 32);
    pos_tex4 = sf::Vector2f(96, 32);
    skyArray.append(sf::Vertex(sf::Vector2f(position.x-Settings::TILE_SIZE*3,position.y-Settings::TILE_SIZE*3), pos_tex1));
    skyArray.append(sf::Vertex(sf::Vector2f(position.x+Settings::TILE_SIZE*4,position.y-Settings::TILE_SIZE*3), pos_tex2));
    skyArray.append(sf::Vertex(sf::Vector2f(position.x+Settings::TILE_SIZE*4,position.y+Settings::TILE_SIZE*4), pos_tex3));
    skyArray.append(sf::Vertex(sf::Vector2f(position.x-Settings::TILE_SIZE*3,position.y+Settings::TILE_SIZE*4), pos_tex4));

}
void Tile::drawGrass(sf::VertexArray &vertexArray){
    TextureManager *texMan = Resources::getTextureManager("tileMap");
    std::string grass0;
    std::string grass1;
    if(id=="D"){
        grass0 = "grass0";
        grass1 = "grass1";
    } else if(id=="J"){
        grass0 = "grassJungle0";
        grass1 = "grassJungle1";
    } else{
        grass0 = "grassIce0";
        grass1 = "grassIce1";
    }
    sf::Vector2f pos_tex1, pos_tex2, pos_tex3, pos_tex4;
    sf::Vector2i position_sprite = texMan->getPositionSprite(grass0);
    pos_tex1 = sf::Vector2f(position_sprite.x, position_sprite.y);
    pos_tex2 = sf::Vector2f(position_sprite.x+texMan->size_sprite.x, position_sprite.y);
    pos_tex3 = sf::Vector2f(position_sprite.x+texMan->size_sprite.x, position_sprite.y+texMan->size_sprite.y);
    pos_tex4 = sf::Vector2f(position_sprite.x, position_sprite.y+texMan->size_sprite.y);
    vertexArray.append(sf::Vertex(sf::Vector2f(position.x,position.y- getHeight()/2), pos_tex1));
    vertexArray.append(sf::Vertex(sf::Vector2f(position.x+Settings::TILE_SIZE,position.y- getHeight()/2), pos_tex2));
    vertexArray.append(sf::Vertex(sf::Vector2f(position.x+Settings::TILE_SIZE,position.y- getHeight()/2+Settings::TILE_SIZE), pos_tex3));
    vertexArray.append(sf::Vertex(sf::Vector2f(position.x,position.y+Settings::TILE_SIZE- getHeight()/2),
                                  pos_tex4));
    if(neighbors[3]!=nullptr && ((neighbors[3]->id !="D" && neighbors[3]->id !="J" && neighbors[3]->id !="W") || (neighbors[2] !=nullptr && neighbors[2]->neighbors[8]->id !="0"))){
        sf::Vector2i position_sprite = texMan->getPositionSprite(grass1);
        pos_tex1 = sf::Vector2f(position_sprite.x, position_sprite.y);
        pos_tex2 = sf::Vector2f(position_sprite.x+texMan->size_sprite.x, position_sprite.y);
        pos_tex3 = sf::Vector2f(position_sprite.x+texMan->size_sprite.x, position_sprite.y+texMan->size_sprite.y);
        pos_tex4 = sf::Vector2f(position_sprite.x, position_sprite.y+texMan->size_sprite.y);
        vertexArray.append(sf::Vertex(sf::Vector2f(position.x+ getWidth(),position.y- getHeight()/2), pos_tex1));
        vertexArray.append(sf::Vertex(sf::Vector2f(position.x+ getWidth()+Settings::TILE_SIZE,position.y- getHeight()/2), pos_tex2));
        vertexArray.append(sf::Vertex(sf::Vector2f(position.x+ getWidth()+Settings::TILE_SIZE,position.y- getHeight()/2+Settings::TILE_SIZE), pos_tex3));
        vertexArray.append(sf::Vertex(sf::Vector2f(position.x+ getWidth(),position.y+Settings::TILE_SIZE- getHeight()/2), pos_tex4));
    }
    if(neighbors[7]!=nullptr && ((neighbors[7]->id !="D" && neighbors[7]->id !="J" && neighbors[7]->id !="W") || (neighbors[0] !=nullptr && neighbors[0]->neighbors[8]->id !="0"))){
        sf::Vector2i position_sprite = texMan->getPositionSprite(grass1);
        pos_tex3 = sf::Vector2f(position_sprite.x, position_sprite.y);
        pos_tex4 = sf::Vector2f(position_sprite.x+texMan->size_sprite.x, position_sprite.y);
        pos_tex1 = sf::Vector2f(position_sprite.x+texMan->size_sprite.x, position_sprite.y+texMan->size_sprite.y);
        pos_tex2 = sf::Vector2f(position_sprite.x, position_sprite.y+texMan->size_sprite.y);
        vertexArray.append(sf::Vertex(sf::Vector2f(position.x- getWidth(),position.y- getHeight()/2+1), pos_tex1));
        vertexArray.append(sf::Vertex(sf::Vector2f(position.x- getWidth()+Settings::TILE_SIZE,position.y- getHeight()/2+1), pos_tex2));
        vertexArray.append(sf::Vertex(sf::Vector2f(position.x- getWidth()+Settings::TILE_SIZE,position.y- getHeight()/2+1+Settings::TILE_SIZE), pos_tex3));
        vertexArray.append(sf::Vertex(sf::Vector2f(position.x- getWidth(),position.y+Settings::TILE_SIZE- getHeight()/2+1), pos_tex4));
    }
}
void Tile::draw(sf::VertexArray &vertexArray)
{


    if (layer == 1) {
            if(drawable()) {
                if (neighbors[8] != nullptr && neighbors[8]->id != "0") {
                    neighbors[8]->draw(vertexArray);
                    //appendSpriteToArray( vertexArray, -1, "Sfull", 0, sf::Vector2i(0,0));
                }
                drawIns(vertexArray);
                if(_leaveType != "0") drawLeaves(vertexArray);
                drawFadeOut(vertexArray);

            }
            else {
                sf::Vector2f pos= GetPosition();
                vertexArray.append(sf::Vertex(sf::Vector2f(pos.x,pos.y), sf::Color::Black));
                vertexArray.append(sf::Vertex(sf::Vector2f(pos.x+Settings::TILE_SIZE,pos.y), sf::Color::Black));
                vertexArray.append(sf::Vertex(sf::Vector2f(pos.x+Settings::TILE_SIZE,pos.y+Settings::TILE_SIZE), sf::Color::Black));
                vertexArray.append(sf::Vertex(sf::Vector2f(pos.x,pos.y+Settings::TILE_SIZE), sf::Color::Black));

            }
    }
    else {
            drawIns(vertexArray);
            if(_leaveType != "0") drawLeaves(vertexArray);
            drawAmbientOclusion(vertexArray);

        }

}


void Tile::update(float elapsedTime)
{
}

void Tile::setPosition(float x, float y)
{

	position.x=x;
	position.y=y;
}
void Tile::setSize(float x)
{

	size.x=x;
	size.y=x;
}

sf::Vector2f Tile::GetPosition() const
{

	return position;
}

float Tile::getHeight() const
{
	return size.y;
}

float Tile::getWidth() const
{
	return size.x;
}
bool Tile::isDirt(){
    return (id == "n" || id == "N" || id == "j" || id == "J" || id == "w" || id == "W" || id == "d" || id == "D");
}
void Tile::debugTile(sf::RenderTarget &target,const std::string keyDebug, sf::Text &text, int chunk_id, sf::Vector2i posTile){
    text.setPosition(position.x,position.y);
    if(keyDebug == "id"){
        if(layer==0){
            int mapIdy = posTile.x;
            int mapIdx = chunk_id*Chunk::N_TILES_X+posTile.y;
            if(mapIdx%5==0 && mapIdy%5==0){
                sf::String str(std::to_string(mapIdx));
                sf::String str2(std::to_string(mapIdy));
                text.setString(str+" "+str2);
                target.draw(text);
            }
        }


    }
    else if(keyDebug == "reachFloor"){

        sf::RectangleShape rectangle;
        rectangle.setFillColor(sf::Color(255-_reach_sun*255,_reach_sun*255,0,255/3));

        if(layer==1){
            rectangle.setSize(sf::Vector2f(size.x/2, size.y/2));
            rectangle.setPosition(position.x+size.x/4,position.y+size.y/4);
        }
        else{
            rectangle.setSize(sf::Vector2f(size.x, size.y));
            rectangle.setPosition(position.x,position.y);
        }
        target.draw(rectangle);
    } else if(keyDebug == "tension"){
        if(layer==0){
            float factor = (last_tension_debug+neighbors[8]->last_tension_debug)/2;

            sf::RectangleShape rectangle;
            rectangle.setFillColor(sf::Color(factor*255,255-factor*255,0,255/3));
            rectangle.setSize(sf::Vector2f(size.x, size.y));
            rectangle.setPosition(position.x,position.y);
            target.draw(rectangle);
        }
    }else if(keyDebug == "humidity"){
        if(layer==0){
            float factor = (float)_humidity/100;

            sf::RectangleShape rectangle;
            rectangle.setFillColor(sf::Color(factor*255,0,255-factor*255,255/3));
            rectangle.setSize(sf::Vector2f(size.x, size.y));
            rectangle.setPosition(position.x,position.y);
            target.draw(rectangle);
        }
    }else if(keyDebug == "temperature"){
        if(layer==0){
            float factor = float(_temperature+Settings::MIN_TEMPERATURE)/float(Settings::MAX_TEMPERATURE-Settings::MIN_TEMPERATURE);

            sf::RectangleShape rectangle;
            rectangle.setFillColor(sf::Color(factor*255,0,255-factor*255,255/3));
            rectangle.setSize(sf::Vector2f(size.x, size.y));
            rectangle.setPosition(position.x,position.y);
            target.draw(rectangle);
        }
    }



}

