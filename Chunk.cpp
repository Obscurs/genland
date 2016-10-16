#include <stdio.h>


// TODO: reference additional headers your program requires here
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>
#include <queue>
#include "Chunk.h"



void Chunk::saveToFile(){
    std::string filename = "map/";
    filename.append(std::to_string(chunk_pos.x));
    filename.append(".txt");
    //std::ofstream myfile(filename, std::ios::trunc);
    std::ofstream myfile;
    myfile.open (filename);

    for(int i = 0; i<N_TILES_Y; ++i){
        for(int j = 0; j<N_TILES_X; ++j){
            Tile* t0 = tile_mat[i][j][0];
            Tile* t1 = tile_mat[i][j][1];
            //std::cout << t0->id << std::endl;
            myfile << t0->id;
            myfile << t1->id;
        }
    }

    myfile.close();

}
void Chunk::setTileNeighbors(int index_x, int index_y){
    Tile* t0 = tile_mat[index_y][index_x][0];
    Tile* t1 = tile_mat[index_y][index_x][1];
    if(t0->id_temp==19900){
        int cactus = 3;

    }
    t0->neighbors[8] = t1;
    t1->neighbors[8] = t0;
    if(index_x-1<0){
        //ESQUERRA
        if(neighbors[0] != nullptr){
            if(index_y-1<0){
                t0->neighbors[0] = nullptr;
                t1->neighbors[0] = nullptr;
            } else {
                t0->neighbors[0] = neighbors[0]->tile_mat[index_y-1][Chunk::N_TILES_X-1][0];
                t1->neighbors[0] = neighbors[0]->tile_mat[index_y-1][Chunk::N_TILES_X-1][1];
            }
            if(index_y+1>=Chunk::N_TILES_Y){
                t0->neighbors[6] = nullptr;
                t1->neighbors[6] = nullptr;
            }
            else{
                t0->neighbors[6] = neighbors[0]->tile_mat[index_y+1][Chunk::N_TILES_X-1][0];
                t1->neighbors[6] = neighbors[0]->tile_mat[index_y+1][Chunk::N_TILES_X-1][1];
            }
            t0->neighbors[7] = neighbors[0]->tile_mat[index_y][Chunk::N_TILES_X-1][0];
            t1->neighbors[7] = neighbors[0]->tile_mat[index_y][Chunk::N_TILES_X-1][1];
        } else{
            t0->neighbors[0] = nullptr;
            t0->neighbors[6] = nullptr;
            t0->neighbors[7] = nullptr;
            t1->neighbors[0] = nullptr;
            t1->neighbors[6] = nullptr;
            t1->neighbors[7] = nullptr;
        }
        //CENTRE I DRETA
        if(index_y-1<0){
            t0->neighbors[1] = nullptr;
            t1->neighbors[1] = nullptr;
            t0->neighbors[2] = nullptr;
            t1->neighbors[2] = nullptr;
        } else {
            t0->neighbors[1] = tile_mat[index_y-1][index_x][0];
            t1->neighbors[1] = tile_mat[index_y-1][index_x][1];
            t0->neighbors[2] = tile_mat[index_y-1][index_x+1][0];
            t1->neighbors[2] = tile_mat[index_y-1][index_x+1][1];
        }
        if(index_y+1>=Chunk::N_TILES_Y){
            t0->neighbors[4] = nullptr;
            t1->neighbors[4] = nullptr;
            t0->neighbors[5] = nullptr;
            t1->neighbors[5] = nullptr;
        }
        else{
            t0->neighbors[4] = tile_mat[index_y+1][index_x+1][0];
            t1->neighbors[4] = tile_mat[index_y+1][index_x+1][1];
            t0->neighbors[5] = tile_mat[index_y+1][index_x][0];
            t1->neighbors[5] = tile_mat[index_y+1][index_x][1];
        }
        t0->neighbors[3] = tile_mat[index_y][index_x+1][0];
        t1->neighbors[3] = tile_mat[index_y][index_x+1][1];

    } else if(index_x+1>=Chunk::N_TILES_X){
        //DRETA
        if(neighbors[1] != nullptr){
            if(index_y-1<0){
                t0->neighbors[2] = nullptr;
                t1->neighbors[2] = nullptr;
            } else {
                t0->neighbors[2] = neighbors[1]->tile_mat[index_y-1][0][0];
                t1->neighbors[2] = neighbors[1]->tile_mat[index_y-1][0][1];
            }
            if(index_y+1>=Chunk::N_TILES_Y){
                t0->neighbors[4] = nullptr;
                t1->neighbors[4] = nullptr;
            }
            else{
                t0->neighbors[4] = neighbors[1]->tile_mat[index_y+1][0][0];
                t1->neighbors[4] = neighbors[1]->tile_mat[index_y+1][0][1];
            }
            t0->neighbors[3] = neighbors[1]->tile_mat[index_y][0][0];
            t1->neighbors[3] = neighbors[1]->tile_mat[index_y][0][1];
        } else{
            t0->neighbors[2] = nullptr;
            t0->neighbors[3] = nullptr;
            t0->neighbors[4] = nullptr;
            t1->neighbors[2] = nullptr;
            t1->neighbors[3] = nullptr;
            t1->neighbors[4] = nullptr;
        }
        //CENTRE I ESQUERRA
        if(index_y-1<0){
            t0->neighbors[1] = nullptr;
            t1->neighbors[1] = nullptr;
            t0->neighbors[0] = nullptr;
            t1->neighbors[0] = nullptr;
        } else {
            t0->neighbors[1] = tile_mat[index_y-1][index_x][0];
            t1->neighbors[1] = tile_mat[index_y-1][index_x][1];
            t0->neighbors[0] = tile_mat[index_y-1][index_x-1][0];
            t1->neighbors[0] = tile_mat[index_y-1][index_x-1][1];
        }
        if(index_y+1>=Chunk::N_TILES_Y){
            t0->neighbors[6] = nullptr;
            t1->neighbors[6] = nullptr;
            t0->neighbors[5] = nullptr;
            t1->neighbors[5] = nullptr;
        }
        else{
            t0->neighbors[6] = tile_mat[index_y+1][index_x-1][0];
            t1->neighbors[6] = tile_mat[index_y+1][index_x-1][1];
            t0->neighbors[5] = tile_mat[index_y+1][index_x][0];
            t1->neighbors[5] = tile_mat[index_y+1][index_x][1];
        }
        t0->neighbors[7] = tile_mat[index_y][index_x-1][0];
        t1->neighbors[7] = tile_mat[index_y][index_x-1][1];
    } else {

        if(index_y-1<0){
            t0->neighbors[2] = nullptr;
            t1->neighbors[2] = nullptr;
            t0->neighbors[1] = nullptr;
            t1->neighbors[1] = nullptr;
            t0->neighbors[0] = nullptr;
            t1->neighbors[0] = nullptr;
        } else {
            t0->neighbors[2] = tile_mat[index_y-1][index_x+1][0];
            t1->neighbors[2] = tile_mat[index_y-1][index_x+1][1];
            t0->neighbors[1] = tile_mat[index_y-1][index_x][0];
            t1->neighbors[1] = tile_mat[index_y-1][index_x][1];
            t0->neighbors[0] = tile_mat[index_y-1][index_x-1][0];
            t1->neighbors[0] = tile_mat[index_y-1][index_x-1][1];
        }
        if(index_y+1>=Chunk::N_TILES_Y){
            t0->neighbors[6] = nullptr;
            t1->neighbors[6] = nullptr;
            t0->neighbors[5] = nullptr;
            t1->neighbors[5] = nullptr;
            t0->neighbors[4] = nullptr;
            t1->neighbors[4] = nullptr;
        }
        else{
            t0->neighbors[6] = tile_mat[index_y+1][index_x-1][0];
            t1->neighbors[6] = tile_mat[index_y+1][index_x-1][1];
            t0->neighbors[5] = tile_mat[index_y+1][index_x][0];
            t1->neighbors[5] = tile_mat[index_y+1][index_x][1];
            t0->neighbors[4] = tile_mat[index_y+1][index_x+1][0];
            t1->neighbors[4] = tile_mat[index_y+1][index_x+1][1];
        }
        t0->neighbors[7] = tile_mat[index_y][index_x-1][0];
        t1->neighbors[7] = tile_mat[index_y][index_x-1][1];
        t0->neighbors[3] = tile_mat[index_y][index_x+1][0];
        t1->neighbors[3] = tile_mat[index_y][index_x+1][1];
    }
    //posem a visible els bordes de la llum
    if(t0->id=="0" && t1->id=="0"){
        for(int i = 0; i<9; i++){
            if(t0->neighbors[i] != nullptr && t0->neighbors[i]->id != "0") t0->neighbors[i]->reach_sun=true;
            if(t1->neighbors[i] != nullptr && t1->neighbors[i]->id != "0") t1->neighbors[i]->reach_sun=true;
        }
    }

}
Chunk::Chunk(sf::Vector2i pos, std::mt19937 *generator, std::ofstream &myfile)
{

	chunk_pos = pos;
    //std::cout  << chunk_pos.x*N_TILES_X*TILE_SIZE << " " << chunk_pos.y*N_TILES_Y*TILE_SIZE << std::endl;
    Simplex2d* sim1 = new Simplex2d(generator, 1000.0f, 0.0f, 0.1f);
    Simplex2d* sim2 = new Simplex2d(generator, 16000.0f, 0.0f, 0.9f);
    Simplex2d* simCueva = new Simplex2d(generator, 300.0f, 0.0f, 1.0f);
    Simplex2d* simStone = new Simplex2d(generator, 300.0f, 0.0f, 1.0f);
    for(int i = 0; i<N_TILES_Y; ++i){
        for(int j = 0; j<N_TILES_X; ++j){
            float current_global_x = chunk_pos.x*N_TILES_X*TILE_SIZE+j*TILE_SIZE;
            float current_global_y = chunk_pos.y*N_TILES_Y*TILE_SIZE+i*TILE_SIZE;
            float valFloor = sim1->valSimplex2D(0, current_global_x);
            float valFloor2 = sim2->valSimplex2D(0, current_global_x);
            float valReal1 = ((float) current_global_y/3000.0f > (valFloor+valFloor2)? 1 : 0);

            float valCueva = simCueva->valSimplex2D(current_global_y, current_global_x)*2;

            float valStone = simStone->valSimplex2D(current_global_y, current_global_x);

            float valReal2 = valReal1 - valCueva;

            //float valReal = ((float) current_global_y/3000.0f > valFloor? 1 : 0);
            //std::cout << valFloor << std::endl;

            Tile* t = new Tile(0, 0);
            Tile* t2 = new Tile(0, 1);
            if(i==N_TILES_Y-1){
                t->rigid=true;
                t2->rigid=true;
                t->reach_floor = true;
                t2->reach_floor = true;
            }
            if(valReal1 >0){
                if(valStone > 0.8){
                    t->Reload("c");
                    t->reach_floor = true;
                } else{
                    t->Reload("d");
                    t->reach_floor = true;
                }


            } else{
                t->Reload("0");
                t->reach_sun=true;
                t->reach_floor = false;
            }

            if(valReal2 >0){
                if(valStone > 0.8){
                    t2->Reload("C");
                    t2->reach_floor = true;
                } else {
                    t2->Reload("D");
                    t2->reach_floor = true;
                }

            } else{
                t2->Reload("0");
                t2->reach_floor = false;

            }


            t->SetPosition(chunk_pos.x*TILE_SIZE*N_TILES_X+j*TILE_SIZE, chunk_pos.y*TILE_SIZE*N_TILES_Y+i*TILE_SIZE);
            t->SetSize(TILE_SIZE);
            t2->SetPosition(chunk_pos.x*TILE_SIZE*N_TILES_X+j*TILE_SIZE, chunk_pos.y*TILE_SIZE*N_TILES_Y+i*TILE_SIZE);
            t2->SetSize(TILE_SIZE);
            tile_mat[i][j][0] = t;
            tile_mat[i][j][1] = t2;
            myfile << t->id;
            myfile << t2->id;
            //std::cout << (valReal > 0 ? "X " : "  ");
            //SETEJEM ELS VEINS DE CADA TILE DEL CHUNK MENYS LES COLUMNES EXTERIORS
            if(j > 1 && i > 0){
                setTileNeighbors(j-1,i-1);
            }
            if(i==Chunk::N_TILES_Y-1){
                setTileNeighbors(j-1,i);
            }
        }
    }
	
}

Chunk::Chunk(sf::Vector2i pos, std::ifstream &myfile, int &id_temp)
{
    chunk_pos = pos;
    std::cout << "creat" << chunk_pos.x << std::endl;
    typedef std::istreambuf_iterator<char> buf_iter;
    buf_iter k(myfile), e;

    for(int i = 0; i<N_TILES_Y; ++i){
        for(int j = 0; j<N_TILES_X; ++j){

            Tile* t = new Tile(id_temp, 0);
            Tile* t2 = new Tile(id_temp, 1);
            id_temp++;
            char c1 = *k;
            ++k;
            char c2 = *k;
            ++k;
            //myfile.get(c1);
            //myfile.get(c2);
            t->Reload(std::string(1, c1));
            t2->Reload(std::string(1, c2));
            t->reach_floor = true;
            t2->reach_floor = true;
            if(t->id == "0") t->reach_sun=true;


            t->SetPosition(chunk_pos.x*TILE_SIZE*N_TILES_X+j*TILE_SIZE, chunk_pos.y*TILE_SIZE*N_TILES_Y+i*TILE_SIZE);
            t->SetSize(TILE_SIZE);
            t2->SetPosition(chunk_pos.x*TILE_SIZE*N_TILES_X+j*TILE_SIZE, chunk_pos.y*TILE_SIZE*N_TILES_Y+i*TILE_SIZE);
            t2->SetSize(TILE_SIZE);
            tile_mat[i][j][0] = t;
            tile_mat[i][j][1] = t2;
            //std::cout << (valReal > 0 ? "X " : "  ");
            //SETEJEM ELS VEINS DE CADA TILE DEL CHUNK MENYS LES COLUMNES EXTERIORS
            if(j > 1 && i > 0){
                setTileNeighbors(j-1,i-1);
            }
            if(i==Chunk::N_TILES_Y-1){
                setTileNeighbors(j-1,i);
            }
        }
    }


}
Chunk::~Chunk()
{
	//std::for_each(_gameObjects.begin(),_gameObjects.end(),GameObjectDeallocator());
}
void Chunk::recalcReachFloor(){
    for(int i= N_TILES_Y-1; i>=0; --i){
        for(int j=0; j<N_TILES_X; j++){
            if(i == (N_TILES_Y-1)){
                tile_mat[i][j][0]->rigid=true;
                tile_mat[i][j][1]->rigid=true;
                tile_mat[i][j][0]->reach_floor=true;
                tile_mat[i][j][1]->reach_floor=true;
            } else {
                Tile* t0 = tile_mat[i][j][0];
                Tile* t1 = tile_mat[i][j][1];
                tile_mat[i][j][0]->reach_floor = (t0->id!="0" &&(t0->neighbors[5]->reach_floor || t0->neighbors[8]->reach_floor));
                tile_mat[i][j][1]->reach_floor = (t1->id!="0" &&(t1->neighbors[5]->reach_floor || t1->neighbors[8]->reach_floor));

            }
        }
    }
}
void Chunk::recalcReachSun(){
    std::queue<Tile*> queue_bfs;
    for(int i= 0; i<N_TILES_Y; ++i){
        for(int j=0; j<N_TILES_X; j++){
            //std::cout << i << " " << j << std::endl;
            if(tile_mat[i][j][0]->id != "0" && tile_mat[i][j][0]->reach_sun){
                if(tile_mat[i][j][1]->id=="0") queue_bfs.push(tile_mat[i][j][0]);
            }
        }
    }
    while(!queue_bfs.empty()){
        Tile* t0 = queue_bfs.front();
        Tile* t1 = t0->neighbors[8];
        queue_bfs.pop();
        for(int i = 0; i<8; i++){
            if(t0->neighbors[i] !=nullptr){
                Tile* t0_aux = t0->neighbors[i];
                Tile* t1_aux = t1->neighbors[i];
                if(!t0_aux->reach_sun && t0_aux->id != "0" && t1_aux->id == "0"){
                    queue_bfs.push(t0_aux);
                }
                t0_aux->reach_sun=true;
                t1_aux->reach_sun=true;
            }
        }

    }
}
void Chunk::calcLateralNeighborsTiles(int lateral){
    int x;
    if(lateral==1) x=Chunk::N_TILES_X-1;
    else x = 0;
    for(int y=0; y<Chunk::N_TILES_Y; y++){
        std::cout << tile_mat[y][x][1]->id_temp << " set"<< std::endl;
        setTileNeighbors(x,y);
    }
}

Tile* Chunk::getTile(float x, float y, int z){
	//std::cout << 100.0 /64.0 << std::endl;
	//if(x<0) x = 0;
	//if(y<0) y = 0;

    if(x>=0){
        int size_chunk_x = Chunk::N_TILES_X*Chunk::TILE_SIZE;
        int size_chunk_y = Chunk::N_TILES_Y*Chunk::TILE_SIZE;
        int tile_x = ((int)x%size_chunk_x)/Chunk::TILE_SIZE;
        int tile_y = ((int)y%size_chunk_y)/Chunk::TILE_SIZE;
        return tile_mat[tile_y][abs(tile_x)][z];
    }
    else{
        int size_chunk_x = Chunk::N_TILES_X*Chunk::TILE_SIZE;
        int size_chunk_y = Chunk::N_TILES_Y*Chunk::TILE_SIZE;
        int tile_x = (((int)floorf(x)%size_chunk_x)+size_chunk_x)/Chunk::TILE_SIZE;
        int tile_y = ((int)floorf(y)%size_chunk_y)/Chunk::TILE_SIZE;
        //std::cout << "t" << x <<" "<< floorf(x) << " " << (int)floorf(x) << std::endl;
        //tile_x = Chunk::N_TILES_X-1-tile_x;
        return tile_mat[abs(tile_y)][abs(tile_x)][z];
    }

	//std::cout << ((int)x) % size_chunk_x << std::endl;
	//std::cout << 641 % 640 << std::endl;
    if(x<0){
        //tile_x = Chunk::N_TILES_X-1-tile_x;
        //std::cout << tile_x <<" "<< tile_y<< std::endl;

    }

}
Tile* Chunk::getTileByIndex(int x, int y, int z){
    return tile_mat[y][x][z];
}

sf::Vector2i Chunk::getTileIndex(float x, float y){
	//if(x<0) x = 0;
	//if(y<0) y = 0;

    if(x>=0) {
        int size_chunk_x = Chunk::N_TILES_X * Chunk::TILE_SIZE;
        int size_chunk_y = Chunk::N_TILES_Y * Chunk::TILE_SIZE;
        int tile_x = ((int) x % size_chunk_x) / Chunk::TILE_SIZE;
        int tile_y = ((int) y % size_chunk_y) / Chunk::TILE_SIZE;
        if(tile_x == Chunk::N_TILES_X) tile_x = 0;
        return sf::Vector2i(abs(tile_y), abs(tile_x));
    }
    else{
        int size_chunk_x = Chunk::N_TILES_X * Chunk::TILE_SIZE;
        int size_chunk_y = Chunk::N_TILES_Y * Chunk::TILE_SIZE;
        int tile_x = (((int) floorf(x) % size_chunk_x)+size_chunk_x) / Chunk::TILE_SIZE;
        int tile_y = (((int) floorf(y) % size_chunk_y)) / Chunk::TILE_SIZE;
        if(tile_x == Chunk::N_TILES_X) tile_x = 0;
        return sf::Vector2i(abs(tile_y), abs(tile_x));
    }

}

void Chunk::DrawGrassTiles(sf::RenderWindow& renderWindow,  TextureManager &t, sf::VertexArray &vertexArray)
{
    for(int i=0; i<grass_tiles.size(); i++){
        grass_tiles[i]->DrawGrass(renderWindow, t,vertexArray);
    }
}
void Chunk::DrawChunk(sf::RenderWindow& renderWindow, sf::Vector2f pos1, sf::Vector2f pos2, TextureManager &t, sf::Shader &tile_shader, sf::VertexArray &vertexArray, sf::VertexArray &skyArray)
{
    grass_tiles.clear();
    //std::cout << "Drawing chunk " << chunk_pos.x<< std::endl;
    sf::Text text;
    sf::Font font;
    if (!font.loadFromFile("resources/font1.ttf"))
    {
        std::cout << "font error" << std::endl;
    }
    text.setCharacterSize(9);
    text.setColor(sf::Color::Red);
    //text.setStyle(sf::Text::Bold);
    text.setFont(font); // font is a sf::Font
    char c[10];

    //std::cout << pos1.x << " " << pos1.y << " " << pos2.x << " " << pos2.y << " " << chunk_pos.x << std::endl;
    float pos1x = pos1.x;
    float pos1y = pos1.y;
    float pos2x = pos2.x;
    float pos2y = pos2.y;
    float golbal_chunk_x = Chunk::N_TILES_X*Chunk::TILE_SIZE*chunk_pos.x;
    float golbal_chunk_y = Chunk::N_TILES_Y*Chunk::TILE_SIZE*chunk_pos.y;
    if(pos1x < golbal_chunk_x) pos1x = golbal_chunk_x;
    if(pos1y < golbal_chunk_y) pos1y = golbal_chunk_y;
    if(pos2x >= golbal_chunk_x + Chunk::N_TILES_X*Chunk::TILE_SIZE) pos2x = golbal_chunk_x + (Chunk::N_TILES_X-1)*Chunk::TILE_SIZE;
    if(pos2y >= golbal_chunk_y + Chunk::N_TILES_Y*Chunk::TILE_SIZE) pos2y = golbal_chunk_y + (Chunk::N_TILES_Y-1)*Chunk::TILE_SIZE;
    //std::cout << pos1x << " " << pos1y << " " << pos2x << " " << pos2y<< " " << chunk_pos.x << std::endl;
    sf::Vector2i first_index = getTileIndex(pos1x, pos1y);
    sf::Vector2i last_index = getTileIndex(pos2x, pos2y);
    //std::cout << "hiii" << std::endl;
    //std::cout << first_index.x << " " << first_index.y << " " << last_index.x << " " << last_index.y << " " << chunk_pos.x << std::endl;
    for(int i = first_index.x; i<=last_index.x; ++i){
        for(int j = first_index.y; j<=last_index.y; ++j){
            Tile* t1 = tile_mat[i][j][1];
            if(t1->id !="0"){
                t1->Draw(renderWindow, t, tile_shader,vertexArray);
                if(t1->neighbors[1] != nullptr && t1->neighbors[1]->neighbors[8] != nullptr){
                    if(t1->neighbors[1]->id=="0" && t1->neighbors[1]->neighbors[8]->id =="0" && t1->id=="D") grass_tiles.push_back(t1);
                }
            }

            else{
                Tile* t0 = tile_mat[i][j][0];

                if(t0->id=="0"){
                    if(t0->neighbors[1]!=nullptr && t0->neighbors[1]->id=="0" &&
                            t0->neighbors[3]!=nullptr && t0->neighbors[3]->id=="0" &&
                            t0->neighbors[5]!=nullptr && t0->neighbors[5]->id=="0" &&
                            t0->neighbors[7]!=nullptr && t0->neighbors[7]->id=="0"){
                            t0->drawSkyArray(skyArray);
                        //vertexArray.append(sf::Vertex(sf::Vector2f(t0->position.x+t0->GetWidth(),position.y-GetHeight()/2), pos_tex1));
                        //vertexArray.append(sf::Vertex(sf::Vector2f(position.x+GetWidth()+Chunk::TILE_SIZE,position.y-GetHeight()/2), pos_tex2));
                        //vertexArray.append(sf::Vertex(sf::Vector2f(position.x+GetWidth()+Chunk::TILE_SIZE,position.y-GetHeight()/2+Chunk::TILE_SIZE), pos_tex3));
                        //vertexArray.append(sf::Vertex(sf::Vector2f(position.x+GetWidth(),position.y+Chunk::TILE_SIZE-GetHeight()/2), pos_tex4));
                    }
                    else t0->drawBorderSkyArray(skyArray,t);
                }
                if(t0->id != "0")t0->Draw(renderWindow, t, tile_shader,vertexArray);
                else if(t0->reach_sun)t0->DrawOuts(renderWindow, t,vertexArray);

                if(t1->reach_sun)t1->DrawOuts(renderWindow, t,vertexArray);

            }
            //DEBUG

            Tile* t0 = tile_mat[i][j][0];
            int test = t0->reach_sun;
            //int test = -1;
            //if(t0->reach_floor & t1->reach_floor) test=11;
            //else if(!t0->reach_floor & t1->reach_floor) test = 1;
            //else if(t0->reach_floor & !t1->reach_floor) test = 10;
            //else test =0;
            sf::Vector2f test_pos = t1->GetPosition();
            sprintf(c, "%i", test);
            std::string string(c);
            sf::String str(string);
            text.setString(str);
            text.setPosition(test_pos.x, test_pos.y);
            text.setScale(sf::Vector2f(1.5,1.5));
            //renderWindow.draw(text);
            //END DEBUG
        }
    }






}



