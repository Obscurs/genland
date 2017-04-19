#include <stdio.h>


// TODO: reference additional headers your program requires here
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <map>
#include <iostream>
#include <cassert>
#include "Map.h"
#include "../../Game.h"
#include "../../Settings.h"


//#include "Game.h"



Map::Map(){
    initialized = false;
}
void Map::init(int pos, std::string path, std::string s, TextureManager& t)
{
    initialized = true;
    seed = s;
    save_path=path;
    Light l1(sf::Vector2f(+500.0,2000.0),95.0,105.0,100.0, sf::Color::Green);
    Light l2(sf::Vector2f(+550.0,2000.0),95.0,105.0,100.0, sf::Color::Red);
    Light l3(sf::Vector2f(0,0),78.0,80.0,100.0, sf::Color::Yellow);
    lights.push_back(l1);
    lights.push_back(l2);
    lights.push_back(l3);
    texMan = &t;


    int id_temp = 0;
    posMap = pos;
    createMap(0, pos, id_temp);
    createMap(1, pos+1, id_temp);
    createMap(2, pos+2, id_temp);
    chunk_mat[0]->neighbors[1] = chunk_mat[1];
    chunk_mat[1]->neighbors[0] = chunk_mat[0];
    chunk_mat[1]->neighbors[1] = chunk_mat[2];
    chunk_mat[2]->neighbors[0] = chunk_mat[1];

    std::cout << "chunk 00000000000000" << std::endl;
    chunk_mat[0]->calcLateralNeighborsTiles(0);
    chunk_mat[0]->calcLateralNeighborsTiles(1);

    std::cout << "chunk 111111111111111" << std::endl;
    chunk_mat[1]->calcLateralNeighborsTiles(0);
    chunk_mat[1]->calcLateralNeighborsTiles(1);
    std::cout << "chunk 222222222222222" << std::endl;
    chunk_mat[2]->calcLateralNeighborsTiles(0);
    chunk_mat[2]->calcLateralNeighborsTiles(1);


    std::cout << "chunk 0 recalc" << std::endl;
    chunk_mat[0]->recalcReachFloor();
    std::cout << "chunk 1 recalc" << std::endl;
    chunk_mat[1]->recalcReachFloor();
    std::cout << "chunk 2 recalc" << std::endl;
    chunk_mat[2]->recalcReachFloor();

}

void Map::saveMap(){
    if(initialized){
        for(int i=0; i<N_CHUNKS_X; i++){
            chunk_mat[i]->saveToFile(save_path);
        }
    } std::cout << "canot save map, map not initialized" << std::endl;
}
void Map::createMap(int map_index, int chunk_index, int &id_temp){
    if(initialized) {
        std::string filename = save_path;
        filename.append("/map/");
        filename.append(std::to_string(chunk_index));
        filename.append(".txt");

        if (exists_file(filename)) {
            std::ifstream myfile(filename);
            myfile.open(filename);
            std::cout << map_index << " map to " << chunk_index << " " << 0 << std::endl;
            sf::Vector2i chunk_pos(chunk_index, 0);
            Chunk *c = new Chunk(chunk_pos, myfile, id_temp, *texMan);
            chunk_mat[map_index] = c;
            myfile.close();
        }
        else {

            std::ofstream myfile;
            myfile.open(filename);
            generator.seed(std::stoi(seed));

            sf::Vector2i chunk_pos(chunk_index, 0);
            Chunk *c = new Chunk(chunk_pos, &generator, myfile, *texMan);
            chunk_mat[map_index] = c;

            myfile.close();
        }
        if (map_index == 0 && chunk_mat[1] != nullptr) {
            chunk_mat[0]->neighbors[1] = chunk_mat[1];
            chunk_mat[1]->neighbors[0] = chunk_mat[0];
            chunk_mat[0]->calcLateralNeighborsTiles(1);
            chunk_mat[1]->calcLateralNeighborsTiles(0);
            //chunk_mat[0]->recalcReachSun();
        } else if (map_index == 1) {
            if (chunk_mat[0] != nullptr) {
                chunk_mat[0]->neighbors[1] = chunk_mat[1];
                chunk_mat[1]->neighbors[0] = chunk_mat[0];
                chunk_mat[0]->calcLateralNeighborsTiles(1);
                chunk_mat[1]->calcLateralNeighborsTiles(0);
            }
            if (chunk_mat[2] != nullptr) {
                chunk_mat[1]->neighbors[1] = chunk_mat[2];
                chunk_mat[2]->neighbors[0] = chunk_mat[1];
                chunk_mat[1]->calcLateralNeighborsTiles(1);
                chunk_mat[2]->calcLateralNeighborsTiles(0);
            }
            //chunk_mat[1]->recalcReachSun();
        } else if (map_index == 2 && chunk_mat[1] != nullptr) {
            chunk_mat[2]->neighbors[0] = chunk_mat[1];
            chunk_mat[1]->neighbors[1] = chunk_mat[2];
            chunk_mat[1]->calcLateralNeighborsTiles(1);
            chunk_mat[2]->calcLateralNeighborsTiles(0);
            //chunk_mat[2]->recalcReachSun();
        }
    } else std::cout << "canot create map, map not initialized" << std::endl;
}

Map::~Map() {
}

inline bool Map::exists_file (const std::string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

sf::Vector2i Map::getCordinatesRespectTile(sf::Vector2f pos_origen, sf::Vector2f pos_goal){
	//std::cout << "pos_origen_x " << pos_origen.x << " pos_origen_y " << pos_origen.y << std::endl;
	//std::cout << "pos_goal_x " << pos_goal.x << " pos_goal_y " << pos_goal.y << std::endl;
	pos_origen.x = pos_origen.x/Settings::TILE_SIZE;
	pos_origen.y = pos_origen.y/Settings::TILE_SIZE;
	pos_goal.x = pos_goal.x/Settings::TILE_SIZE;
	pos_goal.y = pos_goal.y/Settings::TILE_SIZE;
	sf::Vector2i result;
	result.x = ((int)pos_goal.x)-((int)pos_origen.x);
	result.y = ((int)pos_goal.y)-((int)pos_origen.y);
	return result;
}

void Map::calcPhysics2(Tile* first_tile, std::map<Tile*,bool> conected_bfs) {
    if(initialized){
        std::cout << "calculating removed tile " << first_tile->id_temp << std::endl;
        int max_left = 0;
        int max_right = 0;
        int max_up = 0;
        int total_weight = 0;
        int total_tension_no_rigid_left = 0;
        int total_tension_no_rigid_right = 0;
        int total_tension_no_rigid_top = 0;
        int total_tension_rigid = 0;
        bool limit_tension_reached = false;
        std::vector<std::vector<int> > visitats_dreta;
        std::vector<std::vector<int> > visitats_esquerra;
        std::queue<sf::Vector2i> queue_bfs;
        std::queue<sf::Vector2i> queue_bfs_next_left;
        std::queue<sf::Vector2i> queue_bfs_next_right;
        std::queue<sf::Vector2i> queue_bfs_next_top;
        Tile *first_tile0;
        Tile *first_tile1;
        if (first_tile->layer == 1) {
            first_tile1 = first_tile;
            first_tile0 = first_tile->neighbors[8];
        } else {
            first_tile0 = first_tile;
            first_tile1 = first_tile->neighbors[8];
        }


        if ((first_tile0->id != "0" || first_tile1->id != "0") && !first_tile1->reach_floor && !first_tile0->reach_floor) {

            sf::Vector2f first_tile_global_position = first_tile0->GetPosition();
            sf::Vector2f min_pos = first_tile_global_position;
            sf::Vector2f max_pos = first_tile_global_position;
            std::vector<int> esquerra0;
            std::vector<int> dreta0;
            std::vector<int> dreta1;
            dreta0.push_back(0);
            std::queue<Tile *> queue_final_tiles;

            visitats_dreta.push_back(dreta0);    //hem afegit com a visitat el primer node.
            visitats_dreta.push_back(dreta1);    //donem espai per mirar els adjacents
            visitats_esquerra.push_back(esquerra0);    //donem espai per mirar els adjacents
            queue_bfs.push(sf::Vector2i(0, 0));    //push del primer node del bfs
            //bool end_left = false;
            //bool end_right = false;
            bool first_iteration = true;
            int turn = 0;
            while (first_iteration || (!limit_tension_reached &&
                                       (!queue_bfs_next_left.empty() || !queue_bfs_next_right.empty() ||
                                        !queue_bfs_next_top.empty()))) {
                first_iteration = false;
                bool wall_left = false;
                bool wall_right = false;
                //std::cout << "happening " <<  (!queue_bfs_next_left.empty()) << std::endl;
                //std::cout << "happening " <<  (!queue_bfs_next_right.empty()) << std::endl;
                //std::cout << "happening " <<  (!queue_bfs_next_top.empty()) << std::endl;

                if (turn == 0 && queue_bfs_next_left.empty()) turn = 1;
                if (turn == 1 && queue_bfs_next_right.empty()) turn = 2;
                if (turn == 2 && queue_bfs_next_top.empty()) turn = 0;
                if (turn == 0 && queue_bfs_next_left.empty()) turn = 1;
                if (turn == 1 && queue_bfs_next_right.empty()) turn = 2;
                if (turn == 2 && queue_bfs_next_top.empty()) turn = 0;

                if (turn == 2 && total_tension_no_rigid_top < total_tension_no_rigid_left &&
                    !queue_bfs_next_left.empty())
                    turn = 0;
                else if (turn == 2 && total_tension_no_rigid_top < total_tension_no_rigid_right &&
                         !queue_bfs_next_right.empty())
                    turn = 1;
                if (turn == 0) {    //si es el turn de l'esquerra
                    while (!queue_bfs_next_left.empty()) {
                        queue_bfs.push(queue_bfs_next_left.front());
                        queue_bfs_next_left.pop();
                        std::vector<int> new_list;
                        visitats_esquerra.push_back(new_list);    //hem afegit una nova columna a l'esquerra de visitats.
                        --max_left;
                        total_tension_no_rigid_left = 0;
                    }
                    turn = 1;
                }
                else if (turn == 1) {    //si es el turn de la dreta
                    while (!queue_bfs_next_right.empty()) {
                        queue_bfs.push(queue_bfs_next_right.front());
                        queue_bfs_next_right.pop();
                        std::vector<int> new_list;
                        visitats_dreta.push_back(new_list);    //hem afegit una nova columna a la dreta de visitats.
                        ++max_right;
                        total_tension_no_rigid_right = 0;
                    }
                    turn = 2;
                }
                else if (turn == 2) {    //si es el turn de amunt
                    while (!queue_bfs_next_top.empty()) {
                        queue_bfs.push(queue_bfs_next_top.front());
                        queue_bfs_next_top.pop();
                        --max_up;
                        total_tension_no_rigid_top = 0;
                    }
                    turn = 0;
                }

                while (!queue_bfs.empty()) {

                    sf::Vector2i u = queue_bfs.front();
                    queue_bfs.pop();
                    //std::cout << "bfs_it " << u.x << " " << u.y << std::endl;
                    Tile *tile_actual0 = getTile(first_tile_global_position.x + u.x * Settings::TILE_SIZE,
                                                 first_tile_global_position.y + u.y * Settings::TILE_SIZE, 0);
                    Tile *tile_actual1 = tile_actual0->neighbors[8];
                    sf::Vector2f tile_actual_global_position = tile_actual0->GetPosition();
                    if (tile_actual_global_position.x > max_pos.x) max_pos.x = tile_actual_global_position.x;
                    if (tile_actual_global_position.y > max_pos.y) max_pos.y = tile_actual_global_position.y;
                    if (tile_actual_global_position.x < min_pos.x) min_pos.x = tile_actual_global_position.x;
                    if (tile_actual_global_position.y < min_pos.y) min_pos.y = tile_actual_global_position.y;
                    if (tile_actual0->id != "0") {

                        total_weight += tile_actual0->weight; //sumem el pes
                        queue_final_tiles.push(tile_actual0); //fem push a laltre queue (interior)
                        //std::cout << "sumem pes back" << tile_actual0->id_temp << std::endl;
                    }
                    if (tile_actual1->id != "0") {
                        total_weight += tile_actual1->weight; //sumem el pes
                        //std::cout << "sumem pes front " << tile_actual1->id_temp << std::endl;

                    }


                    Tile *adj_0[4];
                    adj_0[0] = tile_actual0->neighbors[3];
                    adj_0[1] = tile_actual0->neighbors[5];
                    adj_0[2] = tile_actual0->neighbors[7];
                    adj_0[3] = tile_actual0->neighbors[1];
                    Tile *adj_1[4];
                    adj_1[0] = tile_actual1->neighbors[3];
                    adj_1[1] = tile_actual1->neighbors[5];
                    adj_1[2] = tile_actual1->neighbors[7];
                    adj_1[3] = tile_actual1->neighbors[1];
                    for (int i = 0; i < 4; i++) {

                        //std::cout << "adj_it" << std::endl;
                        //agafem els dos tiles adjacents
                        Tile *tile_adj0 = adj_0[i];
                        Tile *tile_adj1 = adj_1[i];

                        if (tile_adj0 != nullptr && tile_adj1 != nullptr) {
                            //comprobacio de si es visitat
                            bool visitat = false;
                            sf::Vector2i coord_respect = getCordinatesRespectTile(first_tile_global_position,
                                                                                  tile_adj0->GetPosition());

                            if (coord_respect.x >= 0) {
                                int j = 0;
                                while (j < visitats_dreta[coord_respect.x].size() && !visitat) {
                                    //std::cout << "visitor_it_dreta " << coord_respect.x << " "<<  coord_respect.y << std::endl;
                                    if (visitats_dreta[coord_respect.x][j] == coord_respect.y) visitat = true;
                                    ++j;
                                }
                            } else {
                                int j = 0;
                                while (j < visitats_esquerra[coord_respect.x * (-1) - 1].size() && !visitat) {
                                    //std::cout << "visitor_it_esq " << coord_respect.x << " "<<  coord_respect.y << std::endl;
                                    if (visitats_esquerra[coord_respect.x * (-1) - 1][j] == coord_respect.y) visitat = true;
                                    ++j;
                                }
                            }
                            //std::cout << "coordenades adj " << coord_respect.x << " "<<  coord_respect.y << " visitat "<< visitat << " visible " << tile_adj0->visible << " " << tile_adj1->visible << std::endl;
                            //si existeix una tile i no l'hem visitat
                            if ((tile_adj0->id != "0" || tile_adj1->id != "0") && !visitat) {
                                //afageix a visitats
                                if (coord_respect.x >= 0) visitats_dreta[coord_respect.x].push_back(coord_respect.y);
                                else visitats_esquerra[coord_respect.x * (-1) - 1].push_back(coord_respect.y);
                                //mirem si ex conex amb la dreta o esquerra del inici
                                /*
                                if (coord_respect.x <= 2 && coord_respect.x >= -2 && coord_respect.y <= 2 &&
                                    coord_respect.y >= -2) {
                                    if (!(conected_bfs.find(tile_adj0) == conected_bfs.end())) {
                                        conected_bfs[tile_actual0] = true;
                                    }
                                }
                                */

                                //calculem la tensio
                                int t0 = std::min(tile_actual0->max_tension, tile_adj0->max_tension);
                                int t1 = std::min(tile_actual1->max_tension, tile_adj1->max_tension);
                                int tension = t0 + t1;

                                //si arriba al terra guardem en tension_rigid i no fem push
                                if (tile_adj0->reach_floor || tile_adj0->rigid || tile_adj1->reach_floor ||
                                    tile_adj1->rigid) {
                                    total_tension_rigid += tension;
                                    if (coord_respect.x > 0) {
                                        wall_right = true;
                                    }
                                    else if (coord_respect.x < 0) {
                                        wall_left = true;
                                    }

                                }

                                else {
                                    //si estem al limit per la dreta posem tensio no rigida i fem push al next dreta
                                    if (coord_respect.x > max_right) {
                                        //std::cout << "toca dreta" << std::endl;
                                        total_tension_no_rigid_right += tension;
                                        queue_bfs_next_right.push(coord_respect);
                                    }

                                        //si estem al limit per la esquerra posem tensio no rigida i fem push al next esquerra
                                    else if (coord_respect.x < max_left) {
                                        //std::cout << "toca esquerra" << std::endl;
                                        total_tension_no_rigid_left += tension;
                                        queue_bfs_next_left.push(coord_respect);
                                    }
                                        //si estem al limit per amunt posem tensio no rigida i fem push al next amunt
                                    else if (coord_respect.y < max_up) {
                                        //std::cout << "toca up" << std::endl;
                                        total_tension_no_rigid_top += tension;
                                        queue_bfs_next_top.push(coord_respect);
                                    }

                                        //en el cas contrari, el node no esta en un extrem i fem push en el bfs actual
                                    else {
                                        //std::cout << "bfs_push " << coord_respect.x << " " << coord_respect.y << std::endl;
                                        queue_bfs.push(coord_respect);
                                    }
                                }
                            }
                        }
                    }
                }


                std::cout << "weight " << total_weight << " tension no " << total_tension_no_rigid_left << " "
                          << total_tension_no_rigid_right << " " << total_tension_no_rigid_top << " si "
                          << total_tension_rigid << " left/right/top " << max_left << " " << max_right << " " << max_up
                          << std::endl;
                if (total_weight >
                    pow((total_tension_no_rigid_left + total_tension_no_rigid_right + total_tension_no_rigid_top +
                         total_tension_rigid), 1.1)) {
                    limit_tension_reached = true;
                    std::queue<Tile *> border_tiles;
                    while (!queue_bfs_next_right.empty()) {
                        wall_right = true;
                        sf::Vector2i act_pos_ext = queue_bfs_next_right.front();
                        queue_bfs_next_right.pop();
                        Tile *ext_tile0 = getTile(
                                act_pos_ext.x * Settings::TILE_SIZE + first_tile_global_position.x - Settings::TILE_SIZE,
                                act_pos_ext.y * Settings::TILE_SIZE + first_tile_global_position.y, 0);

                        border_tiles.push(ext_tile0);
                    }
                    while (!queue_bfs_next_left.empty()) {
                        wall_left = true;
                        sf::Vector2i act_pos_ext = queue_bfs_next_left.front();
                        queue_bfs_next_left.pop();
                        Tile *ext_tile0 = getTile(
                                act_pos_ext.x * Settings::TILE_SIZE + first_tile_global_position.x + Settings::TILE_SIZE,
                                act_pos_ext.y * Settings::TILE_SIZE + first_tile_global_position.y, 0);
                        border_tiles.push(ext_tile0);
                    }
                    while (!queue_bfs_next_top.empty()) {
                        sf::Vector2i act_pos_ext = queue_bfs_next_top.front();
                        queue_bfs_next_top.pop();
                        Tile *ext_tile0 = getTile(act_pos_ext.x * Settings::TILE_SIZE + first_tile_global_position.x,
                                                  act_pos_ext.y * Settings::TILE_SIZE + first_tile_global_position.y +
                                                          Settings::TILE_SIZE, 0);
                        border_tiles.push(ext_tile0);
                        //extension_tiles.push(ext_tile1);
                    }
                    //add falling tiles and remove tiles
                    float center_falling_x = (max_pos.x - min_pos.x) / 2 + min_pos.x;
                    while (!queue_final_tiles.empty()) {
                        Tile *t = queue_final_tiles.front();
                        Tile *t1 = t->neighbors[8];
                        queue_final_tiles.pop();

                        if (t->id != "0" && t1->id == "0") {
                            AnimatedTile *falling_t = new AnimatedTile();
                            falling_t->Reload(t->id);
                            falling_t->wall_left = wall_left;
                            falling_t->wall_right = wall_right;
                            sf::Vector2f tpos = t->GetPosition();
                            float dist_x = tpos.x - center_falling_x;
                            float dist_y = max_pos.y - tpos.y;
                            falling_t->SetPosition(tpos.x, tpos.y + Settings::TILE_SIZE / 2);
                            falling_t->SetSize(t->GetWidth());
                            falling_t->setFactor(dist_x, dist_y);

                            falling_tiles.push_back(falling_t);
                        } else if (t1->id != "0") {
                            AnimatedTile *falling_t = new AnimatedTile();
                            falling_t->Reload(t1->id);
                            falling_t->wall_left = wall_left;
                            falling_t->wall_right = wall_right;
                            sf::Vector2f tpos = t1->GetPosition();
                            float dist_x = tpos.x - center_falling_x;
                            float dist_y = max_pos.y - tpos.y;
                            falling_t->SetPosition(tpos.x, tpos.y + Settings::TILE_SIZE / 2);
                            falling_t->SetSize(t1->GetWidth());
                            falling_t->setFactor(dist_x, dist_y);
                            falling_tiles.push_back(falling_t);
                        }
                        else {
                            std::cout << "WHHHHHHHHHHHHAAAAAAAAAAAAT" << std::endl;
                        }
                        t->Reload("0");
                        t1->Reload("0");
                    }

                    //compute border tiles

                    while (!border_tiles.empty()) {
                        Tile *act_ext_tile = border_tiles.front();
                        border_tiles.pop();
                        removeTile2(act_ext_tile);
                    }

                }
            }
        }
    } else std::cout << "canot calc phisics, map no initialized " <<  std::endl;

}
void Map::dirtyChunks(){
    for(int i = 0 ; i<N_CHUNKS_X ; ++i){
        chunk_mat[i]->is_dirty = true;
    }
}
void Map::removeTile2(Tile* removed_tile){
    //bool removed_reach_sun=removed_tile->reach_sun;
    //if(removed_tile->layer==0) removed_reach_sun = true;
    Tile* otherLayerRemovedTile= removed_tile->neighbors[8];
    if(!otherLayerRemovedTile->reach_floor) removeReachFloorCascade2(removed_tile->neighbors[1]);
    removed_tile->Reload("0");
    //if(removed_reach_sun) removed_tile->reach_sun = true;
    Tile* removed_tile0;
    if(removed_tile->layer==0) removed_tile0 = removed_tile;
    else removed_tile0 = removed_tile->neighbors[8];

    std::map<Tile*,bool> leftUpRight_evaluatedTiles;
    leftUpRight_evaluatedTiles[removed_tile0->neighbors[7]] = (removed_tile0->neighbors[7] == nullptr);
    leftUpRight_evaluatedTiles[removed_tile0->neighbors[1]] = (removed_tile0->neighbors[1] == nullptr);
    leftUpRight_evaluatedTiles[removed_tile0->neighbors[3]] = (removed_tile0->neighbors[3] == nullptr);
    leftUpRight_evaluatedTiles[removed_tile0->neighbors[5]] = (removed_tile0->neighbors[5] == nullptr);


    for (auto& m : leftUpRight_evaluatedTiles) {
        if(!m.second){
            calcPhysics2(m.first, leftUpRight_evaluatedTiles);
        }
    }
}

void Map::removeReachFloorCascade2(Tile* t_first){
    if(t_first == nullptr || (t_first->neighbors[8]->id=="0" && t_first->id=="0") || t_first->neighbors[8]->rigid || t_first->rigid) return;
    else {
        Tile* tB = t_first->neighbors[8];
        t_first->reach_floor = false;
        tB->reach_floor = false;
        removeReachFloorCascade2(t_first->neighbors[1]);
    }
}

Tile* Map::getTile(float x, float y, int z){
	if(y<0) y = 0;
	int size_chunk_x = Chunk::N_TILES_X*Settings::TILE_SIZE;
	int chunk_x = (x-size_chunk_x*posMap)/size_chunk_x;

	Chunk* c = chunk_mat[chunk_x];

	return c->getTile(x, y, z);
}

int Map::getChunkIndex(float x){
	int size_chunk_x = Chunk::N_TILES_X*Settings::TILE_SIZE;
	int chunk_x = x/size_chunk_x;
    if(x <0) --chunk_x;
	return chunk_x;
}

int Map::getIndexMatChunk(int x){
    int final_x = -1;
    for(int i = 0 ; i<N_CHUNKS_X ; ++i){
        int pos_x = chunk_mat[i]->chunk_pos.x;
        if(pos_x == x){
            final_x = i;
        }
    }

    return final_x;
}

void Map::checkLoadedChunks(float x, float y){
    if(initialized){
        Chunk *c1 = chunk_mat[0];
        Chunk *c2 = chunk_mat[N_CHUNKS_X - 1];
        Tile *t1 = c1->getTileByIndex(0, 0, 0);
        Tile *t2 = c2->getTileByIndex(Chunk::N_TILES_X - 1, 0, 0);
        sf::Vector2f p1 = t1->GetPosition();
        sf::Vector2f p2 = t2->GetPosition();
        float distance_1 = sqrt((x - p1.x) * (x - p1.x));
        float distance_2 = sqrt((x - p2.x) * (x - p2.x));


        if (distance_1 < Chunk::N_TILES_X / 1.8 * Settings::TILE_SIZE) {
            //#pragma omp task
            {

                c2->saveToFile(save_path);
                int current_pos = c1->chunk_pos.x;
                int id_temp = 0;
                Chunk *chunk_mid = chunk_mat[1];


                chunk_mat[2] = chunk_mid;
                chunk_mat[1] = c1;
                --posMap;
                createMap(0, current_pos - 1, id_temp);
                chunk_mat[2]->neighbors[1] = nullptr;
                chunk_mat[2]->calcLateralNeighborsTiles(1);
                chunk_mat[0]->calcLateralNeighborsTiles(0);
                chunk_mat[0]->recalcReachFloor();
                delete c2;
            }

            //std::cout << distance_1 << " " << distance_2 << std::endl;
        }
        if (distance_2 < Chunk::N_TILES_X / 1.8 * Settings::TILE_SIZE) {
            //#pragma omp task
            {

                c1->saveToFile(save_path);
                int current_pos = c2->chunk_pos.x;
                int id_temp = 0;
                Chunk *chunk_mid = chunk_mat[1];

                chunk_mat[0] = chunk_mid;
                chunk_mat[1] = c2;
                ++posMap;
                createMap(2, current_pos + 1, id_temp);
                chunk_mat[0]->neighbors[1] = nullptr;
                chunk_mat[0]->calcLateralNeighborsTiles(0);
                chunk_mat[2]->calcLateralNeighborsTiles(1);
                chunk_mat[2]->recalcReachFloor();
                delete c1;
            }
            //std::cout << distance_1 << " " << distance_2 << std::endl;
        }
    } else std::cout << "canot check loaded, map no initialized " <<  std::endl;

}

std::vector<Tile*> Map::getTilesCol(sf::Vector2f pos, sf::Vector2f size){
    std::vector<Tile*> result;
    float num_ite_x = size.x/Settings::TILE_SIZE;
    float num_ite_y = size.y/Settings::TILE_SIZE;
    for(int j = 0; j <= num_ite_y; ++j){
            for(int i = 0; i <=num_ite_x; i++){
                Tile* t = getTile(pos.x+i*Settings::TILE_SIZE, pos.y+j*Settings::TILE_SIZE, 1);
                if(t->id !="0") {

                    bool trobat = false;
                    for(int k = 0; k < result.size(); k++){
                        if(result[k]==t) trobat = true;
                    }
                    if(!trobat)result.push_back(t);
                }
            }
    }


    //DEBUG
    /*
    for(int i= 0; i<result.size(); i++){
        sf::Vector2f pos = result[i]->GetPosition();
        int aux_id = result[i]->id_temp;
        //std::cout << "pos x " << pos.x << " pos y " << pos.y << std::endl;
        std::cout << aux_id << " ";

    }

    std::cout << std::endl;
    */

    return result;
}


void Map::UpdateAll(float delta, sf::Vector2f player_pos)
{
    if(initialized){
        //BACKGROUNDS
        for(int i = 0; i<N_CHUNKS_X; i++){
            chunk_mat[i]->update(delta);
        }
        for(int i=0; i<lights.size(); i++){
            lights[i].Update(delta);
        }
        player_pos.x+=16;
        player_pos.y+=16;
        lights[2].position=player_pos;

        for(int i=0; i<falling_tiles.size(); ++i){
            falling_tiles[i]->Update(delta, chunk_mat[0], chunk_mat[1], chunk_mat[2], posMap);
            if(falling_tiles[i]->deleted==1){
                delete falling_tiles[i];
                falling_tiles.erase(falling_tiles.begin()+i);
            }
        }
    } else std::cout << "canot update, map no initialized " <<  std::endl;

}