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
        }
    }
    initializeLights();
	
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



            t->SetPosition(chunk_pos.x*TILE_SIZE*N_TILES_X+j*TILE_SIZE, chunk_pos.y*TILE_SIZE*N_TILES_Y+i*TILE_SIZE);
            t->SetSize(TILE_SIZE);
            t2->SetPosition(chunk_pos.x*TILE_SIZE*N_TILES_X+j*TILE_SIZE, chunk_pos.y*TILE_SIZE*N_TILES_Y+i*TILE_SIZE);
            t2->SetSize(TILE_SIZE);
            tile_mat[i][j][0] = t;
            tile_mat[i][j][1] = t2;
            //std::cout << (valReal > 0 ? "X " : "  ");
        }
    }
    initializeLights();

}
Chunk::~Chunk()
{
	//std::for_each(_gameObjects.begin(),_gameObjects.end(),GameObjectDeallocator());
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
void Chunk::initializeLights(){
    for(int i = 0; i<Chunk::N_TILES_Y; i++){
        for(int j = 0; j < Chunk::N_TILES_X; j++){
            bool is_light = tile_mat[i][j][1]->id=="0";
            //if(is_light){
                if(i-1>=0 && j-1>=0){
                    tile_mat[i-1][j-1][1]->lights[0]=is_light;
                }
                if(i-1>=0){
                    tile_mat[i-1][j][1]->lights[1]=is_light;
                }
                if(i-1>=0 && j+1<Chunk::N_TILES_X){
                    tile_mat[i-1][j+1][1]->lights[2]=is_light;
                }
                if(j+1<Chunk::N_TILES_X){
                    tile_mat[i][j+1][1]->lights[3]=is_light;
                }
                if(i+1<Chunk::N_TILES_Y && j+1<Chunk::N_TILES_X){
                    tile_mat[i+1][j+1][1]->lights[4]=is_light;
                }
                if(i+1<Chunk::N_TILES_Y){
                    tile_mat[i+1][j][1]->lights[5]=is_light;
                }
                if(i+1<Chunk::N_TILES_Y && j-1>=0){
                    tile_mat[i+1][j-1][1]->lights[6]=is_light;
                }
                if(j-1>=0){
                    tile_mat[i][j-1][1]->lights[7]=is_light;
                }
            //}
        }
    }
}


void Chunk::DrawChunk(sf::RenderWindow& renderWindow, sf::Vector2f pos1, sf::Vector2f pos2, TextureManager &t)
{
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
            if(t1->visible)t1->Draw(renderWindow, t);

            else{
                Tile* t0 = tile_mat[i][j][0];


                if(t0->visible)t0->Draw(renderWindow, t);
                if(t0->id!="0"){
                    if(i+1<Chunk::N_TILES_Y){
                        Tile* t_shadow = tile_mat[i+1][j][1];
                        if(t_shadow->id !="0") {
                            sf::Sprite s;
                            t.generateSprite("s", t0->GetPosition(), s, sf::Vector2f(t0->GetWidth(),t0->GetHeight()));
                            renderWindow.draw(s);
                        }
                    }
                    if(i-1>=0){
                        Tile* t_shadow = tile_mat[i-1][j][1];
                        if(t_shadow->id !="0") {
                            sf::Sprite s;
                            sf::Vector2f shadow_pos = t0->GetPosition();
                            shadow_pos.y += t0->GetHeight();
                            shadow_pos.x += t0->GetWidth();
                            t.generateSprite("s", shadow_pos, s, sf::Vector2f(t0->GetWidth(),t0->GetHeight()));
                            s.setRotation(180);
                            renderWindow.draw(s);
                        }
                    }
                    if(j+1<Chunk::N_TILES_X){
                        Tile* t_shadow = tile_mat[i][j+1][1];
                        if(t_shadow->id !="0") {
                            sf::Sprite s;
                            sf::Vector2f shadow_pos = t0->GetPosition();
                            shadow_pos.y += t0->GetHeight();
                            t.generateSprite("s", shadow_pos, s, sf::Vector2f(t0->GetWidth(),t0->GetHeight()));
                            s.setRotation(-90);
                            renderWindow.draw(s);
                        }
                    }
                    if(j-1>=0){
                        Tile* t_shadow = tile_mat[i][j-1][1];
                        if(t_shadow->id !="0") {
                            sf::Sprite s;
                            sf::Vector2f shadow_pos = t0->GetPosition();
                            shadow_pos.x += t0->GetWidth();
                            t.generateSprite("s", shadow_pos, s, sf::Vector2f(t0->GetWidth(),t0->GetHeight()));
                            s.setRotation(90);
                            renderWindow.draw(s);
                        }
                    }
                }
            }
            //DEBUG
            int test = t1->id_temp;
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



