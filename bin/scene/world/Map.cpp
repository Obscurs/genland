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
#include "../../Debuger.h"


//#include "Game.h"



Map::Map():
        _mapViewer()
{
    _initialized = false;
}
void Map::init(int pos)
{

    _initialized = true;
    Light l1(sf::Vector2f(+500.0,2000.0),95.0,105.0,100.0, sf::Color::Green);
    Light l2(sf::Vector2f(+550.0,2000.0),95.0,105.0,100.0, sf::Color::Red);
    Light l3(sf::Vector2f(0,0),78.0,80.0,100.0, sf::Color::Yellow);
    lights.push_back(l1);
    lights.push_back(l2);
    lights.push_back(l3);


    _posMap = pos;
    createMap(0, pos);
    createMap(1, pos+1);
    createMap(2, pos+2);
    _chunk_mat[0]->neighbors[1] = _chunk_mat[1];
    _chunk_mat[1]->neighbors[0] = _chunk_mat[0];
    _chunk_mat[1]->neighbors[1] = _chunk_mat[2];
    _chunk_mat[2]->neighbors[0] = _chunk_mat[1];

    _chunk_mat[0]->calcLateralNeighborsTiles(0);
    _chunk_mat[0]->calcLateralNeighborsTiles(1);
    _chunk_mat[1]->calcLateralNeighborsTiles(0);
    _chunk_mat[1]->calcLateralNeighborsTiles(1);
    _chunk_mat[2]->calcLateralNeighborsTiles(0);
    _chunk_mat[2]->calcLateralNeighborsTiles(1);

    //_chunk_mat[0]->recalcReachFloor();
    //_chunk_mat[1]->recalcReachFloor();
    //_chunk_mat[2]->recalcReachFloor();

}
void Map::searchDeserts(bool left, int pos){
    Scene *scene = Scene::getScene();
    std::string path = scene->getGamePath();
    std::mt19937 generator = scene->getGenerator();
    int seed = std::stoi(scene->getSeed());

    generator.seed(seed);
    Simplex2d* escarp = new Simplex2d(&generator, 1000.0f, 0.0f, 0.1f);
    generator.seed(seed+1);
    Simplex2d* altitud = new Simplex2d(&generator, 20000.0f, 0.2f, 0.5f);
    generator.seed(seed+2);
    Simplex2d* escarp_factor = new Simplex2d(&generator, 6000.0f, 0.0f, 1.0f);
    Simplex2d* noise_stone_to_dirt = new Simplex2d(&generator, 50.0f, -0.01f, 0.01f);
    Simplex2d* noise_stone_to_dirt2 = new Simplex2d(&generator, 500.0f, -0.02f, 0.02f);
    Simplex2d* noise_transition_materials3 = new Simplex2d(&generator, 200.0f, -1.0f, 1.0f);

    generator.seed(seed+3);
    Simplex2d* mount_factor = new Simplex2d(&generator, 10000.0f, -1.2f, 1.0f);
    generator.seed(seed+4);
    Simplex2d* mountains = new Simplex2d(&generator, 500.0f, 0.4f, 0.5f);

    generator.seed(seed+5);
    Simplex2d* base_noise_temperature = new Simplex2d(&generator, 25000.0f, -10, 20);
    generator.seed(seed+6);
    Simplex2d* noise_humidity = new Simplex2d(&generator, 25000.0f, 0.0f, 100.0f);

    sf::Vector2i lims = scene->getLimsBiome();
    sf::Vector2i old_lims = lims;


    int iter = pos;
    while(old_lims == lims){
        std::vector<sf::Vector2i> grassTiles;
        bool has_desert = false;
        for(int j = 0; j<Chunk::N_TILES_X; j = j +1){
            bool ant_terrain = true;
            for(int i = 0; i<Chunk::N_TILES_Y; i = i +1){
                int y_pos = Chunk::N_TILES_Y-1-i;
                float current_global_x = iter*Chunk::N_TILES_X*Settings::TILE_SIZE_HIGH+j*Settings::TILE_SIZE_HIGH;
                float current_global_y = i*Settings::TILE_SIZE_HIGH;
                float height_factor = float(y_pos)/float(Chunk::N_TILES_Y);
                float valFloor = altitud->valSimplex2D(0, current_global_x);
                float valEscarpAux = escarp->valSimplex2D(0, current_global_x);
                float valEscarpFact = escarp_factor->valSimplex2D(0, current_global_x);
                float valPlains = valFloor+(valEscarpAux*valEscarpFact);
                float valTransition = noise_transition_materials3->valSimplex2D(0, current_global_x);
                float mountains_noise = mountains->valSimplex2D(0, current_global_x);
                float mountains_factor = mount_factor->valSimplex2D(0, current_global_x);
                mountains_factor = std::max(float(0.0), mountains_factor);
                float valMountains = mountains_factor*mountains_noise;
                float valTerrain = valMountains+valPlains;
                bool valHeightMax = height_factor < valTerrain;
                float valNoiseStoneDirt = noise_stone_to_dirt->valSimplex2D(0, current_global_x)+noise_stone_to_dirt2->valSimplex2D(0, current_global_x);
                float valHeightStone = (height_factor < (valTerrain+valNoiseStoneDirt+-0.04*(1-(mountains_factor*1.6+(1-height_factor)/2)))? 1 : 0);
                int valHumidity = int(noise_humidity->valSimplex2D(0, current_global_x));
                float heightTemp = (1-height_factor)*(Settings::MAX_TEMPERATURE-Settings::MIN_TEMPERATURE)+Settings::MIN_TEMPERATURE;
                int valTemperature = int(heightTemp)+int(base_noise_temperature->valSimplex2D(0, current_global_x));
                Tile::Bioma bioma = Tile::STANDARD;
                if(mountains_factor<=0) {
                    if(valTemperature+valTransition*2 >30){
                        if(valHumidity+valTransition*2 >70) bioma = Tile::JUNGLE;
                        else if(valHumidity+valTransition*2 <40) bioma = Tile::DESERT;
                    }
                }
                if(ant_terrain && valHeightMax){
                    ant_terrain = false;
                    if(bioma == Tile::DESERT) has_desert = true;
                    else if(!valHeightStone) grassTiles.push_back(sf::Vector2i(current_global_x,current_global_y-Settings::TILE_SIZE));
                }
            }
        }
        if(has_desert){
            sf::Vector2i limitsBio = scene->getLimsBiome();
            if((limitsBio.x == limitsBio.y) || (limitsBio.x -10 > iter) || (limitsBio.y +10 < iter)) scene->addLimit(iter);
            std::string filenameEnt = path;
            filenameEnt.append("/entities/");
            filenameEnt.append(std::to_string(iter));
            filenameEnt.append(".txt");
            std::cout << filenameEnt << std::endl;
            std::ofstream myfile;
            myfile.open(filenameEnt);
            myfile << "END";
            myfile.close();
        } else {
            std::vector<Tree> trees;
            if(grassTiles.size()>0){
                Tree *t = new Tree(iter, grassTiles[rand() % grassTiles.size()],2,3,1,0.8,0.6,0.9,2,2,3);
                trees.push_back(*t);
            }
            std::string filenameEnt = path;
            filenameEnt.append("/entities/");
            filenameEnt.append(std::to_string(iter));
            filenameEnt.append(".txt");
            std::cout << filenameEnt << std::endl;
            std::ofstream myfile;
            myfile.open(filenameEnt);
            for(int i = 0; i< trees.size(); i++){
                trees[i].saveToFile(iter, myfile);
            }
            myfile << "END";
            myfile.close();
            //std::cout << "lsalasldasldasl" << std::endl;
        }
        if(iter==0){
                std::cout << "hola" << std::endl;
        }

        lims = scene->getLimsBiome();
        if(left) iter = iter-1;
        else iter = iter +1;
    }


}
void Map::saveMap(){
    if(_initialized){
        Scene *scene = Scene::getScene();
        std::string path = scene->getGamePath();
        for(int i=0; i<N_CHUNKS_X; i++){
            _chunk_mat[i]->saveToFile(path);
        }
    } std::cout << "canot save map, map not _initialized" << std::endl;
}
void Map::createMap(int map_index, int chunk_index){
    if(_initialized) {
        Scene *scene = Scene::getScene();
        std::string path = scene->getGamePath();
        std::string filenameMap = path;
        filenameMap.append("/map/");
        filenameMap.append(std::to_string(chunk_index));
        filenameMap.append(".txt");

        Chunk *c;
        if (exists_file(filenameMap)) {
            std::ifstream myfile(filenameMap);
            myfile.open(filenameMap);
            std::cout << map_index << " map to " << chunk_index << " " << 0 << std::endl;
            c = new Chunk(chunk_index, myfile);
            _chunk_mat[map_index] = c;
            myfile.close();
        }
        else {
            std::ofstream myfile;
            myfile.open(filenameMap);
            c = new Chunk(chunk_index, myfile);
            _chunk_mat[map_index] = c;
            myfile.close();
        }




        if (map_index == 0 && _chunk_mat[1] != nullptr) {
            _chunk_mat[0]->neighbors[1] = _chunk_mat[1];
            _chunk_mat[1]->neighbors[0] = _chunk_mat[0];
            _chunk_mat[0]->calcLateralNeighborsTiles(1);
            _chunk_mat[1]->calcLateralNeighborsTiles(0);
            //_chunk_mat[0]->recalcReachSun();
        } else if (map_index == 1) {
            if (_chunk_mat[0] != nullptr) {
                _chunk_mat[0]->neighbors[1] = _chunk_mat[1];
                _chunk_mat[1]->neighbors[0] = _chunk_mat[0];
                _chunk_mat[0]->calcLateralNeighborsTiles(1);
                _chunk_mat[1]->calcLateralNeighborsTiles(0);
            }
            if (_chunk_mat[2] != nullptr) {
                _chunk_mat[1]->neighbors[1] = _chunk_mat[2];
                _chunk_mat[2]->neighbors[0] = _chunk_mat[1];
                _chunk_mat[1]->calcLateralNeighborsTiles(1);
                _chunk_mat[2]->calcLateralNeighborsTiles(0);
            }
            //_chunk_mat[1]->recalcReachSun();
        } else if (map_index == 2 && _chunk_mat[1] != nullptr) {
            _chunk_mat[2]->neighbors[0] = _chunk_mat[1];
            _chunk_mat[1]->neighbors[1] = _chunk_mat[2];
            _chunk_mat[1]->calcLateralNeighborsTiles(1);
            _chunk_mat[2]->calcLateralNeighborsTiles(0);
            //_chunk_mat[2]->recalcReachSun();
        }
        sf::Vector2i ecoLimits = scene->getLimsBiome();
        if(ecoLimits.x != ecoLimits.y){
            if(chunk_index > ecoLimits.y){
                searchDeserts(0, chunk_index);
            }
            else if(chunk_index < ecoLimits.x){
                searchDeserts(1, chunk_index);
            }
        }
    } else std::cout << "canot create map, map not _initialized" << std::endl;
}
void Map::syncEntitiesToChunk(int chunk){
    _mapViewer.addChunk(*_chunk_mat[chunk]);
    Scene *s = Scene::getScene();
    s->syncTreesWithChunk(_chunk_mat[chunk], chunk);
    s->syncNotRenderedTrees(_chunk_mat[1]);


}
Map::~Map() {
}

inline bool Map::exists_file (const std::string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}
void Map::drawViewMap(sf::RenderTarget &render){
    _mapViewer.draw(render);
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
    if(_initialized){
        std::cout << "calculating removed tile " << std::endl;
        bool debug = Debuger::activated;
        std::vector<Tile *> tilesWallDebug;
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
                ////////////////////////////////////////////////////////////////
                //////////////////////////DEBUG/////////////////////////////////
                ////////////////////////////////////////////////////////////////
                std::vector<Tile *> tilesLimitsDebug;
                if(debug){
                    std::queue<sf::Vector2i> queueAux;

                    while (!queue_bfs_next_left.empty()) {
                        queueAux.push(queue_bfs_next_left.front());
                        queue_bfs_next_left.pop();
                        sf::Vector2i u = queueAux.front();
                        Tile *tileEval0 = getTile(first_tile_global_position.x + u.x * Settings::TILE_SIZE,
                                                 first_tile_global_position.y + u.y * Settings::TILE_SIZE, 0);
                        Tile *tileEval1 = tileEval0->neighbors[8];
                        if(tileEval0->id !="0") tilesLimitsDebug.push_back(tileEval0);
                        if(tileEval1->id !="0") tilesLimitsDebug.push_back(tileEval1);
                    }
                    while (!queueAux.empty()) {
                        queue_bfs_next_left.push(queueAux.front());
                        queueAux.pop();
                    }

                    while (!queue_bfs_next_right.empty()) {
                        queueAux.push(queue_bfs_next_right.front());
                        queue_bfs_next_right.pop();
                        sf::Vector2i u = queueAux.front();
                        Tile *tileEval0 = getTile(first_tile_global_position.x + u.x * Settings::TILE_SIZE,
                                                  first_tile_global_position.y + u.y * Settings::TILE_SIZE, 0);
                        Tile *tileEval1 = tileEval0->neighbors[8];
                        if(tileEval0->id !="0") tilesLimitsDebug.push_back(tileEval0);
                        if(tileEval1->id !="0") tilesLimitsDebug.push_back(tileEval1);
                    }
                    while (!queueAux.empty()) {
                        queue_bfs_next_right.push(queueAux.front());
                        queueAux.pop();
                    }

                    while (!queue_bfs_next_top.empty()) {
                        queueAux.push(queue_bfs_next_top.front());
                        queue_bfs_next_top.pop();
                        sf::Vector2i u = queueAux.front();
                        Tile *tileEval0 = getTile(first_tile_global_position.x + u.x * Settings::TILE_SIZE,
                                                  first_tile_global_position.y + u.y * Settings::TILE_SIZE, 0);
                        Tile *tileEval1 = tileEval0->neighbors[8];
                        if(tileEval0->id !="0") tilesLimitsDebug.push_back(tileEval0);
                        if(tileEval1->id !="0") tilesLimitsDebug.push_back(tileEval1);
                    }
                    while (!queueAux.empty()) {
                        queue_bfs_next_top.push(queueAux.front());
                        queueAux.pop();
                    }
                }

                ////////////////////////////////////////////////////////////////
                //////////////////////////ENDEBUG///////////////////////////////
                ////////////////////////////////////////////////////////////////
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
                            //Mirem si la tile ja estaba visitada
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
                                    //////////////////////////////////////////
                                    /////////////////DEBUG////////////////////
                                    //////////////////////////////////////////
                                    if(debug){
                                        if(tile_actual0->id != "0") tilesWallDebug.push_back(tile_actual0);
                                        if(tile_actual1->id != "0") tilesWallDebug.push_back(tile_actual1);
                                    }
                                    ///////////////////////////////////////////
                                    /////////////////ENDEBUG///////////////////
                                    ///////////////////////////////////////////
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
                double total_tension =pow((total_tension_no_rigid_left + total_tension_no_rigid_right + total_tension_no_rigid_top +
                                        total_tension_rigid), 1.1);
                //////////////////////////////////////////
                /////////////DEBUG////////////////////////
                //////////////////////////////////////////
                if(tilesWallDebug.size() + tilesLimitsDebug.size() > 0){
                    float weight_per_limit_tile = total_weight/(tilesWallDebug.size() + tilesLimitsDebug.size());
                    float tension_per_limit_tile = total_tension/(tilesWallDebug.size() + tilesLimitsDebug.size());
                    for(int i = 0; i< tilesLimitsDebug.size(); i++){
                        tilesLimitsDebug[i]->last_tension_debug=std::min(weight_per_limit_tile/tension_per_limit_tile,tension_per_limit_tile);
                    }
                    for(int i = 0; i< tilesWallDebug.size(); i++){
                        tilesWallDebug[i]->last_tension_debug=std::min(weight_per_limit_tile/tension_per_limit_tile,tension_per_limit_tile);
                    }
                }

                //////////////////////////////////////////
                /////////////ENDEBUG//////////////////////
                //////////////////////////////////////////
                if (total_weight >total_tension) {
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
                            falling_t->SetSize(t->getWidth());
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
                            falling_t->SetSize(t1->getWidth());
                            falling_t->setFactor(dist_x, dist_y);
                            falling_tiles.push_back(falling_t);
                        }
                        else {
                            std::cout << "WHHHHHHHHHHHHAAAAAAAAAAAAT" << std::endl;
                        }
                        t->reload("0");
                        t1->reload("0");
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
    } else std::cout << "canot calc phisics, map no _initialized " <<  std::endl;

}
void Map::dirtyChunks(){
    for(int i = 0 ; i<N_CHUNKS_X ; ++i){
        _chunk_mat[i]->_is_dirty = true;
    }
}
void Map::removeTile2(Tile* removed_tile){
    //bool removed_reach_sun=removed_tile->reach_sun;
    //if(removed_tile->layer==0) removed_reach_sun = true;
    Tile* otherLayerRemovedTile= removed_tile->neighbors[8];
    //if(!otherLayerRemovedTile->reach_floor) removeReachFloorCascade2(removed_tile->neighbors[1]);
    removed_tile->reload("0");
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


Tile* Map::getTile(float x, float y, int z){
	if(y<0) y = 0;
	int size_chunk_x = Chunk::N_TILES_X*Settings::TILE_SIZE;
	int chunk_x = (x-size_chunk_x*_posMap)/size_chunk_x;
    //std::cout << chunk_x << std::endl;
    if(chunk_x>= N_CHUNKS_X || chunk_x <0) return nullptr;
    else{
        Chunk* c = _chunk_mat[chunk_x];
        return c->getTile(x, y, z);
    }

}
Chunk* Map::getChunk(float x, float y, float z){
    if(y<0) y = 0;
    int size_chunk_x = Chunk::N_TILES_X*Settings::TILE_SIZE;
    int chunk_x = (x-size_chunk_x*_posMap)/size_chunk_x;
    //std::cout << chunk_x << std::endl;
    if(chunk_x>= N_CHUNKS_X || chunk_x <0) return nullptr;
    else{
        Chunk* c = _chunk_mat[chunk_x];
        return c;
    }
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
        int pos_x = _chunk_mat[i]->_chunk_id;
        if(pos_x == x){
            final_x = i;
        }
    }

    return final_x;
}

void Map::checkLoadedChunks(float x, float y){
    if(_initialized){
        Chunk *c1 = _chunk_mat[0];
        Chunk *c2 = _chunk_mat[N_CHUNKS_X - 1];
        Tile *t1 = c1->getTileByIndex(0, 0, 0);
        Tile *t2 = c2->getTileByIndex(Chunk::N_TILES_X - 1, 0, 0);
        sf::Vector2f p1 = t1->GetPosition();
        sf::Vector2f p2 = t2->GetPosition();
        float distance_1 = sqrt((x - p1.x) * (x - p1.x));
        float distance_2 = sqrt((x - p2.x) * (x - p2.x));
        Scene *scene = Scene::getScene();
        std::string path = scene->getGamePath();

        if (distance_1 < Chunk::N_TILES_X / 1.8 * Settings::TILE_SIZE) {
            //#pragma omp task
            {
                c2->saveToFile(path);
                int current_pos = c1->_chunk_id;
                Chunk *chunk_mid = _chunk_mat[1];


                _chunk_mat[2] = chunk_mid;
                _chunk_mat[1] = c1;
                --_posMap;
                createMap(0, current_pos - 1);
                _chunk_mat[2]->neighbors[1] = nullptr;
                _chunk_mat[2]->calcLateralNeighborsTiles(1);
                _chunk_mat[0]->calcLateralNeighborsTiles(0);
                syncEntitiesToChunk(0);
                //_chunk_mat[0]->recalcReachFloor();
                delete c2;
            }

            //std::cout << distance_1 << " " << distance_2 << std::endl;
        }
        if (distance_2 < Chunk::N_TILES_X / 1.8 * Settings::TILE_SIZE) {
            //#pragma omp task
            {

                c1->saveToFile(path);
                int current_pos = c2->_chunk_id;
                int id_temp = 0;
                Chunk *chunk_mid = _chunk_mat[1];

                _chunk_mat[0] = chunk_mid;
                _chunk_mat[1] = c2;
                ++_posMap;
                createMap(2, current_pos + 1);
                _chunk_mat[0]->neighbors[1] = nullptr;
                _chunk_mat[0]->calcLateralNeighborsTiles(0);
                _chunk_mat[2]->calcLateralNeighborsTiles(1);
                syncEntitiesToChunk(2);
                //_chunk_mat[2]->recalcReachFloor();
                delete c1;
            }
            //std::cout << distance_1 << " " << distance_2 << std::endl;
        }
    } else std::cout << "canot check loaded, map no _initialized " <<  std::endl;

}

std::vector<Tile*> Map::getTilesCol(sf::Vector2f pos, sf::Vector2f size){

    std::vector<Tile*> result;

    Tile* firstT = getTile(pos.x, pos.y, 1);
    Tile* lastT = getTile(pos.x+size.x, pos.y+size.y, 1);
    sf::Vector2i firstPos = sf::Vector2i(firstT->GetPosition().x/Settings::TILE_SIZE,firstT->GetPosition().y/Settings::TILE_SIZE);
    sf::Vector2i lastPos = sf::Vector2i(lastT->GetPosition().x/Settings::TILE_SIZE,lastT->GetPosition().y/Settings::TILE_SIZE);
    for(int i = firstPos.x; i<=lastPos.x; i++){
        for(int j = firstPos.y; j<=lastPos.y; j++){
            Tile* t = getTile(i*Settings::TILE_SIZE, j*Settings::TILE_SIZE, 1);
            if(t->id !="0") {
                result.push_back(t);
            }
        }
    }

    return result;
}

int Map::getPosMap(){
    return _posMap;
}
void Map::update(float delta, sf::Vector2f player_pos)
{
    if(_initialized){
        //BACKGROUNDS
        int x = player_pos.x;
        int y = std::max((int)player_pos.y, 0);
        int size_chunk_x = Chunk::N_TILES_X*Settings::TILE_SIZE;
        int chunk_player = (x-size_chunk_x*_posMap)/size_chunk_x;
        int idChunk = _chunk_mat[chunk_player]->_chunk_id;

        sf::Vector2i tile = _chunk_mat[chunk_player]->getTileIndex(x, y);
        _mapViewer.update(_chunk_mat[chunk_player]->_chunk_id, tile);
        for(int i = 0; i<N_CHUNKS_X; i++){
            _chunk_mat[i]->update(delta);
        }
        for(int i=0; i<lights.size(); i++){
            lights[i].Update(delta);
        }
        player_pos.x+=16;
        player_pos.y+=16;
        lights[2].position=player_pos;

        for(int i=0; i<falling_tiles.size(); ++i){
            falling_tiles[i]->Update(delta, _chunk_mat[0], _chunk_mat[1], _chunk_mat[2], _posMap);
            if(falling_tiles[i]->deleted==1){
                delete falling_tiles[i];
                falling_tiles.erase(falling_tiles.begin()+i);
            }
        }
    } else std::cout << "canot update, map no _initialized " <<  std::endl;

}