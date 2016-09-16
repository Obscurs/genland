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


//#include "Game.h"




Map::Map(int pos)
{
	texMan = new TextureManager("resources/tiles2.png", 16, 16);
	texMan->insert_map_value("D",sf::Vector2i(0,0));
	texMan->insert_map_value("d",sf::Vector2i(16,0));
	texMan->insert_map_value("C",sf::Vector2i(0,16));
	texMan->insert_map_value("c",sf::Vector2i(16,16));

    texMan->insert_map_value("D_in",sf::Vector2i(32,0));
    texMan->insert_map_value("d_in",sf::Vector2i(64,0));
    texMan->insert_map_value("C_in",sf::Vector2i(32,16));
    texMan->insert_map_value("c_in",sf::Vector2i(64,16));

    texMan->insert_map_value("D_out",sf::Vector2i(48,0));
    texMan->insert_map_value("d_out",sf::Vector2i(80,0));
    texMan->insert_map_value("C_out",sf::Vector2i(48,16));
    texMan->insert_map_value("c_out",sf::Vector2i(80,16));

	texMan->insert_map_value("r",sf::Vector2i(0,32));
	texMan->insert_map_value("0",sf::Vector2i(64,32));
    texMan->insert_map_value("s",sf::Vector2i(16,32));
    texMan->insert_map_value("S",sf::Vector2i(32,32));
    texMan->insert_map_value("S2",sf::Vector2i(48,32));

    texMan->insert_map_value("grass0",sf::Vector2i(0,48));
    texMan->insert_map_value("grass1",sf::Vector2i(16,48));

    int id_temp = 0;
    posMap = pos;
    createMap(0, -1, id_temp);
    createMap(1, 0, id_temp);
    createMap(2, 1, id_temp);
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

    chunk_mat[0]->recalcReachFloor();
    chunk_mat[1]->recalcReachFloor();
    chunk_mat[2]->recalcReachFloor();


}
void Map::createMap(int map_index, int chunk_index, int &id_temp){

    std::string filename = "map/";
    filename.append(std::to_string(chunk_index));
    filename.append(".txt");

    if(exists_file(filename)) {
        std::ifstream myfile(filename);
        myfile.open (filename);
                std::cout << map_index << " map to " << chunk_index << " " << 0 << std::endl;
                sf::Vector2i chunk_pos(chunk_index,0);
                Chunk* c = new Chunk(chunk_pos, myfile, id_temp);
                chunk_mat[map_index] = c;
        myfile.close();
    }
    else{

        std::ofstream myfile;
        myfile.open (filename);
        generator.seed(234);

        sf::Vector2i chunk_pos(chunk_index,0);
        Chunk* c = new Chunk(chunk_pos, &generator, myfile);
        chunk_mat[map_index] = c;

        myfile.close();
    }
    if(map_index==0 && chunk_mat[1] != nullptr){
        chunk_mat[0]->neighbors[1] = chunk_mat[1];
        chunk_mat[1]->neighbors[0] = chunk_mat[0];
        chunk_mat[0]->calcLateralNeighborsTiles(1);
        chunk_mat[1]->calcLateralNeighborsTiles(0);
    } else if(map_index==1){
        if(chunk_mat[0] != nullptr){
            chunk_mat[0]->neighbors[1] = chunk_mat[1];
            chunk_mat[1]->neighbors[0] = chunk_mat[0];
            chunk_mat[0]->calcLateralNeighborsTiles(1);
            chunk_mat[1]->calcLateralNeighborsTiles(0);
        }
        if(chunk_mat[2] != nullptr){
            chunk_mat[1]->neighbors[1] = chunk_mat[2];
            chunk_mat[2]->neighbors[0] = chunk_mat[1];
            chunk_mat[1]->calcLateralNeighborsTiles(1);
            chunk_mat[2]->calcLateralNeighborsTiles(0);
        }
    } else if(map_index==2 && chunk_mat[1] != nullptr){
        chunk_mat[2]->neighbors[0] = chunk_mat[1];
        chunk_mat[1]->neighbors[1] = chunk_mat[2];
        chunk_mat[1]->calcLateralNeighborsTiles(1);
        chunk_mat[2]->calcLateralNeighborsTiles(0);
    }

}


Map::~Map()
{
	//std::for_each(_gameObjects.begin(),_gameObjects.end(),GameObjectDeallocator());
}
inline bool Map::exists_file (const std::string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

sf::Vector2i Map::getCordinatesRespectTile(sf::Vector2f pos_origen, sf::Vector2f pos_goal){
	//std::cout << "pos_origen_x " << pos_origen.x << " pos_origen_y " << pos_origen.y << std::endl;
	//std::cout << "pos_goal_x " << pos_goal.x << " pos_goal_y " << pos_goal.y << std::endl;
	pos_origen.x = pos_origen.x/Chunk::TILE_SIZE;
	pos_origen.y = pos_origen.y/Chunk::TILE_SIZE;
	pos_goal.x = pos_goal.x/Chunk::TILE_SIZE;
	pos_goal.y = pos_goal.y/Chunk::TILE_SIZE;
	sf::Vector2i result;
	result.x = ((int)pos_goal.x)-((int)pos_origen.x);
	result.y = ((int)pos_goal.y)-((int)pos_origen.y);
	return result;
}


bool Map::calcPhysics(sf::Vector2f r_tile_pos_global, bool &conex_dreta, bool &conex_esquerra, bool &conex_abaix, sf::Vector2f eval_tile_pos, std::queue<Tile*> &queue_final_tiles, int position_case, std::queue<Tile*> &extension_tiles){
	int left = 0;
	int right = 0;
	int total_weight = 0;
	int total_tension_no_rigid = 0;
	int total_tension_rigid = 0;
	bool limit_tension_reached = false;
	std::vector<std::vector<int> > visitats_dreta;
	std::vector<std::vector<int> > visitats_esquerra;
	std::queue<sf::Vector2i> queue_bfs;
	std::queue<sf::Vector2i> queue_bfs_next_left;
	std::queue<sf::Vector2i> queue_bfs_next_right;

	Tile* first_tile0 = getTile(eval_tile_pos.x, eval_tile_pos.y, 0);
	Tile* first_tile1 = getTile(eval_tile_pos.x, eval_tile_pos.y, 1);

	//std::cout << "prehappening " << first_tile0->reach_floor << " " << first_tile1->reach_floor << std::endl;
	if((first_tile0->visible || first_tile1->visible) && !first_tile1->rigid && !first_tile0->rigid && !first_tile1->reach_floor && !first_tile0->reach_floor){
		
		std::vector<int> new_list;
		std::vector<int> new_list0;
		std::vector<int> new_list1;

		if(position_case==0){
			new_list.push_back(-1);
			visitats_dreta.push_back(new_list);	//hem afegit com a visitat el primer node.
			visitats_dreta.push_back(new_list0);	//donem espai per mirar els adjacents
			visitats_esquerra.push_back(new_list1);	//donem espai per mirar els adjacents
			queue_bfs.push(sf::Vector2i(0,-1));	//push del primer node del bfs
		}else if(position_case==1){
			std::vector<int> new_list2;
			new_list0.push_back(0);
			visitats_dreta.push_back(new_list);	//hem afegit com a visitat el primer node.
			visitats_dreta.push_back(new_list0);	//donem espai per mirar els adjacents
			visitats_dreta.push_back(new_list1);	//donem espai per mirar els adjacents
			visitats_esquerra.push_back(new_list2);	//donem espai per mirar els adjacents
			queue_bfs.push(sf::Vector2i(1,0));	//push del primer node del bfs
		}else if(position_case==2){
			std::vector<int> new_list2;
			new_list1.push_back(0);
			visitats_dreta.push_back(new_list);	//hem afegit com a visitat el primer node.
			visitats_dreta.push_back(new_list0);	//donem espai per mirar els adjacents
			visitats_esquerra.push_back(new_list1);	//donem espai per mirar els adjacents
			visitats_esquerra.push_back(new_list2);	//donem espai per mirar els adjacents
			queue_bfs.push(sf::Vector2i(-1,0));	//push del primer node del bfs
		} else{
			new_list.push_back(1);
			visitats_dreta.push_back(new_list);	//hem afegit com a visitat el primer node.
			visitats_dreta.push_back(new_list0);	//donem espai per mirar els adjacents
			visitats_esquerra.push_back(new_list1);	//donem espai per mirar els adjacents
			queue_bfs.push(sf::Vector2i(0, 1));	//push del primer node del bfs
		}
		

		queue_final_tiles.push(first_tile0); //fem push a laltre queue (interior)
		//queue_final_tiles.push(first_tile1); //fem push a laltre queue (exterior)
		bool end_left = false;
		bool end_right = false;
		bool first_it =true;
		int turn = 0;
		while(first_it || (!limit_tension_reached && (!queue_bfs_next_left.empty() || !queue_bfs_next_right.empty()))){
			first_it = false;
			//std::cout << "happening " << end_left << " " << end_right<< std::endl;
			total_tension_no_rigid = 0;
			if(turn == 0){	//si es el turn de l'esquerra
				while(!queue_bfs_next_left.empty()){ 
					sf::Vector2i u = queue_bfs_next_left.front();
					queue_bfs.push(u);
					queue_bfs_next_left.pop();
				}
			}
			else if(turn == 1){	//si es el turn de la dreta
				while(!queue_bfs_next_right.empty()){ 
					sf::Vector2i u = queue_bfs_next_right.front();
					queue_bfs.push(u);
					queue_bfs_next_right.pop();
				}
			}
			while(!queue_bfs.empty()){
				
				sf::Vector2i u = queue_bfs.front();
				//std::cout << "bfs_it " << u.x << " " << u.y << std::endl;
				Tile* tile_actual0 = getTile(r_tile_pos_global.x + u.x*Chunk::TILE_SIZE, r_tile_pos_global.y + u.y*Chunk::TILE_SIZE, 0);
				Tile* tile_actual1 = getTile(r_tile_pos_global.x + u.x*Chunk::TILE_SIZE, r_tile_pos_global.y + u.y*Chunk::TILE_SIZE, 1);

				if(tile_actual0->visible){
					total_weight += tile_actual0->weight; //sumem el pes
					queue_final_tiles.push(tile_actual0); //fem push a laltre queue (interior)
					//std::cout << "sumem pes " << u.x << " " << u.y << std::endl;
				}
				if(tile_actual1->visible){
					total_weight += tile_actual1->weight; //sumem el pes
					//queue_final_tiles.push(tile_actual1); //fem push a laltre queue (exterior)

				}
				
				queue_bfs.pop();

				Tile* adj_0[4];
				adj_0[0] = getTile(r_tile_pos_global.x + u.x*Chunk::TILE_SIZE + Chunk::TILE_SIZE, r_tile_pos_global.y + u.y*Chunk::TILE_SIZE, 0);
				adj_0[1] = getTile(r_tile_pos_global.x + u.x*Chunk::TILE_SIZE, r_tile_pos_global.y + u.y*Chunk::TILE_SIZE - Chunk::TILE_SIZE, 0);
				adj_0[2] = getTile(r_tile_pos_global.x + u.x*Chunk::TILE_SIZE - Chunk::TILE_SIZE, r_tile_pos_global.y + u.y*Chunk::TILE_SIZE, 0);
				adj_0[3] = getTile(r_tile_pos_global.x + u.x*Chunk::TILE_SIZE, r_tile_pos_global.y + u.y*Chunk::TILE_SIZE + Chunk::TILE_SIZE, 0);
				Tile* adj_1[4]; 
				adj_1[0] = getTile(r_tile_pos_global.x + u.x*Chunk::TILE_SIZE + Chunk::TILE_SIZE, r_tile_pos_global.y + u.y*Chunk::TILE_SIZE, 1);
				adj_1[1] = getTile(r_tile_pos_global.x + u.x*Chunk::TILE_SIZE, r_tile_pos_global.y + u.y*Chunk::TILE_SIZE - Chunk::TILE_SIZE, 1);
				adj_1[2] = getTile(r_tile_pos_global.x + u.x*Chunk::TILE_SIZE - Chunk::TILE_SIZE, r_tile_pos_global.y + u.y*Chunk::TILE_SIZE, 1);
				adj_1[3] = getTile(r_tile_pos_global.x + u.x*Chunk::TILE_SIZE, r_tile_pos_global.y + u.y*Chunk::TILE_SIZE + Chunk::TILE_SIZE, 1);

				for(int i = 0; i<4; i++){
					//std::cout << "adj_it" << std::endl;
					//agafem els dos tiles adjacents
					Tile* tile_adj0 = adj_0[i];
					Tile* tile_adj1 = adj_1[i];

					//comprobacio de si es visitat
					bool visitat = false;
					sf::Vector2i coord_respect = getCordinatesRespectTile(r_tile_pos_global, tile_adj0->GetPosition());
					
					if(coord_respect.x >=0){
						int j=0;
						while(j<visitats_dreta[coord_respect.x].size() && !visitat){
							//std::cout << "visitor_it_dreta " << coord_respect.x << " "<<  coord_respect.y << std::endl;
							if(visitats_dreta[coord_respect.x][j] == coord_respect.y) visitat = true;
							++j;
						}
					}
					else {
						int j=0;
						while(j<visitats_esquerra[coord_respect.x*(-1)-1].size() && !visitat){
							//std::cout << "visitor_it_esq " << coord_respect.x << " "<<  coord_respect.y << std::endl;
							if(visitats_esquerra[coord_respect.x*(-1)-1][j] == coord_respect.y) visitat = true;
							++j;
						}
					}
					//std::cout << "coordenades adj " << coord_respect.x << " "<<  coord_respect.y << " visitat "<< visitat << " visible " << tile_adj0->visible << " " << tile_adj1->visible << std::endl;
					//si existeix una tile i no l'hem visitat
					if((tile_adj0->visible || tile_adj1->visible )&& !visitat){
						//afageix a visitats
						if(coord_respect.x >=0) visitats_dreta[coord_respect.x].push_back(coord_respect.y);
						else visitats_esquerra[coord_respect.x*(-1)-1].push_back(coord_respect.y);
						//mirem si ex conex amb la dreta o esquerra del inici
						if(coord_respect.x == -1 && coord_respect.y == 0) conex_esquerra = true;
						else if(coord_respect.x == 1 && coord_respect.y == 0) conex_dreta = true;
						else if(coord_respect.x == 0 && coord_respect.y == 1) conex_abaix = true;
						//calculem la tensio
						int t0 = std::min(tile_actual0->max_tension, tile_adj0->max_tension);
						int t1 = std::min(tile_actual1->max_tension, tile_adj1->max_tension);
						int tension = t0 + t1;

						//si arriba al terra guardem en tension_rigid i no fem push
						if(tile_adj0->reach_floor || tile_adj0->rigid || tile_adj1->reach_floor || tile_adj1->rigid) total_tension_rigid += tension; 
						else{
							//si estem al limit per la dreta posem tensio no rigida i fem push al next dreta
							if(coord_respect.x > right){
								//std::cout << "toca dreta" << std::endl;
								total_tension_no_rigid += tension;
								queue_bfs_next_right.push(coord_respect);
							}

							//si estem al limit per la esquerra posem tensio no rigida i fem push al next esquerra
							else if(coord_respect.x < left){
								//std::cout << "toca esquerra" << std::endl;
								total_tension_no_rigid += tension;	
								queue_bfs_next_left.push(coord_respect);
							}

							//en el cas contrari, el node no esta en un extrem i fem push en el bfs actual
							else{
								//std::cout << "bfs_push " << coord_respect.x << " " << coord_respect.y << std::endl;
								queue_bfs.push(coord_respect); 
							}
						}

					}
				}
				//if(visitats_esquerra[-1][0] == 1) end_right = true;

			}
			if(queue_bfs_next_right.empty()) turn = 1;
			if(queue_bfs_next_left.empty()) turn = 0;
			if(turn == 0){
				turn = 1;
				++right;
				std::vector<int> new_list;
				visitats_dreta.push_back(new_list);	//hem afegit una nova columna a la dreta de visitats.
			}
			else{
				turn = 0;
				--left;
				std::vector<int> new_list;
				visitats_esquerra.push_back(new_list);	//hem afegit una nova columna a l'esquerra de visitats.
			}
			std::cout << "weight " << total_weight << " tension" << total_tension_no_rigid << " " << total_tension_rigid << std::endl;
			if(total_weight > total_tension_no_rigid + total_tension_rigid) {
				limit_tension_reached = true;
				while(!queue_bfs_next_right.empty()){
					sf::Vector2i act_pos_ext = queue_bfs_next_right.front();
					queue_bfs_next_right.pop();
					Tile* ext_tile0 = getTile(act_pos_ext.x*Chunk::TILE_SIZE+r_tile_pos_global.x - Chunk::TILE_SIZE ,act_pos_ext.y*Chunk::TILE_SIZE+r_tile_pos_global.y, 0);
					//Tile* ext_tile1 = getTile(act_pos_ext.x*Chunk::TILE_SIZE+r_tile_pos_global.x - Chunk::TILE_SIZE ,act_pos_ext.y*Chunk::TILE_SIZE+r_tile_pos_global.y, 1);
					extension_tiles.push(ext_tile0);
					//extension_tiles.push(ext_tile1);
				}
				while(!queue_bfs_next_left.empty()){
					sf::Vector2i act_pos_ext = queue_bfs_next_left.front();
					queue_bfs_next_left.pop();
					Tile* ext_tile0 = getTile(act_pos_ext.x*Chunk::TILE_SIZE+r_tile_pos_global.x + Chunk::TILE_SIZE ,act_pos_ext.y*Chunk::TILE_SIZE+r_tile_pos_global.y, 0);
					//Tile* ext_tile1 = getTile(act_pos_ext.x*Chunk::TILE_SIZE+r_tile_pos_global.x + Chunk::TILE_SIZE ,act_pos_ext.y*Chunk::TILE_SIZE+r_tile_pos_global.y, 1);
					extension_tiles.push(ext_tile0);
					//extension_tiles.push(ext_tile1);
				}
			}
		}
	}
	return limit_tension_reached;
}
void Map::removeTile2(Tile* removed_tile){
    Tile* otherLayerRemovedTile= removed_tile->neighbors[8];
    if(!otherLayerRemovedTile->reach_floor) removeReachFloorCascade2(removed_tile->neighbors[1]);
    removed_tile->Reload("0");

    std::map<Tile*,bool> leftUpRight_evaluatedTiles;
    leftUpRight_evaluatedTiles[removed_tile->neighbors[7]] = (removed_tile->neighbors[7] == nullptr);
    leftUpRight_evaluatedTiles[removed_tile->neighbors[1]] = (removed_tile->neighbors[1] == nullptr);
    leftUpRight_evaluatedTiles[removed_tile->neighbors[3]] = (removed_tile->neighbors[3] == nullptr);


    std::queue<Tile*> extension_tiles;
    std::queue<Tile*> queue_final_tilesTotal;
    std::queue<Tile*> queue_final_tilesUp;
    std::queue<Tile*> queue_final_tilesLeft;
    std::queue<Tile*> queue_final_tilesRight;
    /*
    for (auto& m : leftUpRight_evaluatedTiles) {
        if(!m.second){

        }
        k = m.first;
        v = m.second;
    }
    while(!queue_final_tiles1.empty()) {
        Tile *t = queue_final_tiles1.front();
        Tile *t1 = t->neighbors[8];
        queue_final_tiles1.pop();
    }

    if(!leftUpRight_evaluatedTiles[removed_tile->neighbors[7]]){

    }
     */
}
void Map::removeTile(Tile* r_tile, int z_removed){


	//cas up
	sf::Vector2f r_tile_pos_global = r_tile->GetPosition();
	int z_other_tile;
	if(z_removed==0) z_other_tile = 1;
	else z_other_tile = 0;
	Tile* other_tile = getTile(r_tile_pos_global.x, r_tile_pos_global.y, z_other_tile);
	Tile* tile_down0 = getTile(r_tile_pos_global.x, r_tile_pos_global.y-Chunk::TILE_SIZE, 0);
	Tile* tile_down1 = getTile(r_tile_pos_global.x, r_tile_pos_global.y-Chunk::TILE_SIZE, 1);
	if(!other_tile->rigid && !other_tile->reach_floor && r_tile->reach_floor) removeReachFloorCascade(r_tile_pos_global.x, r_tile_pos_global.y - Chunk::TILE_SIZE);
	if(!other_tile->rigid && r_tile->reach_floor && !tile_down1->reach_floor && !tile_down0->reach_floor) other_tile->reach_floor = false;
	r_tile->Reload("0");

	bool conex_dreta = false;
	bool conex_esquerra = false;
	bool conex_abaix = false;
	std::queue<Tile*> queue_final_tiles1;
	std::queue<Tile*> extension_tiles;
	//up
	sf::Vector2f eval_tile_pos;
	eval_tile_pos.x = r_tile_pos_global.x;
	eval_tile_pos.y = r_tile_pos_global.y - Chunk::TILE_SIZE;
	//std::cout << r_tile_pos_global.x << " " << r_tile_pos_global.y << " "<< eval_tile_pos.x << " "<< eval_tile_pos.y << " "<< std::endl;
	bool falls1 = calcPhysics(r_tile_pos_global, conex_dreta, conex_esquerra,conex_abaix, eval_tile_pos, queue_final_tiles1, 0, extension_tiles);
	if(falls1){
		while(!queue_final_tiles1.empty()){
			Tile* t = queue_final_tiles1.front();
            Tile* t1 = t->neighbors[8];
			queue_final_tiles1.pop();
            if(t->id != "0" && t1->id=="0"){
                AnimatedTile* falling_t = new AnimatedTile();
                falling_t->Reload(t->id);
                sf::Vector2f tpos = t->GetPosition();
                falling_t->SetPosition(tpos.x, tpos.y);
                falling_t->SetSize(t->GetWidth());
                falling_tiles.push_back(falling_t);
            } else if(t1->id !="0") {
                AnimatedTile* falling_t = new AnimatedTile();
                falling_t->Reload(t1->id);
                sf::Vector2f tpos = t1->GetPosition();
                falling_t->SetPosition(tpos.x, tpos.y);
                falling_t->SetSize(t1->GetWidth());
                falling_tiles.push_back(falling_t);
            }
            else{
                std::cout <<"WHHHHHHHHHHHHAAAAAAAAAAAAT" << std::endl;
            }
            t->Reload("0");
            t1->Reload("0");
		}
		while(!extension_tiles.empty()){
			Tile* act_ext_tile = extension_tiles.front();
			extension_tiles.pop();
			removeTile(act_ext_tile, 0);
		}
	}
	//right


	if(!conex_dreta){
		std::queue<Tile*> queue_final_tiles2;
		eval_tile_pos.x = r_tile_pos_global.x + Chunk::TILE_SIZE;
		eval_tile_pos.y = r_tile_pos_global.y;
		bool falls2 = calcPhysics(r_tile_pos_global, conex_dreta, conex_esquerra, conex_abaix, eval_tile_pos, queue_final_tiles2, 1, extension_tiles);
		if(falls2){
			while(!queue_final_tiles2.empty()){
                Tile* t = queue_final_tiles2.front();
                Tile* t1 = t->neighbors[8];
                queue_final_tiles2.pop();
                if(t->id != "0" && t1->id=="0"){
                    AnimatedTile* falling_t = new AnimatedTile();
                    falling_t->Reload(t->id);
                    sf::Vector2f tpos = t->GetPosition();
                    falling_t->SetPosition(tpos.x, tpos.y);
                    falling_t->SetSize(t->GetWidth());
                    falling_tiles.push_back(falling_t);
                } else if(t1->id !="0") {
                    AnimatedTile* falling_t = new AnimatedTile();
                    falling_t->Reload(t1->id);
                    sf::Vector2f tpos = t1->GetPosition();
                    falling_t->SetPosition(tpos.x, tpos.y);
                    falling_t->SetSize(t1->GetWidth());
                    falling_tiles.push_back(falling_t);
                }
                else{
                    std::cout <<"WHHHHHHHHHHHHAAAAAAAAAAAAT" << std::endl;
                }
                t->Reload("0");
                t1->Reload("0");
			}
			while(!extension_tiles.empty()){
				Tile* act_ext_tile = extension_tiles.front();
				extension_tiles.pop();
				removeTile(act_ext_tile, 0);
			}
		}
	}
	//left
	if(!conex_esquerra){
		std::queue<Tile*> queue_final_tiles3;
		eval_tile_pos.x = r_tile_pos_global.x - Chunk::TILE_SIZE;
		eval_tile_pos.y = r_tile_pos_global.y;
		bool falls3 = calcPhysics(r_tile_pos_global, conex_dreta, conex_esquerra, conex_abaix, eval_tile_pos, queue_final_tiles3, 2, extension_tiles);
		if(falls3){
			while(!queue_final_tiles3.empty()){
                Tile* t = queue_final_tiles3.front();
                Tile* t1 = t->neighbors[8];
                queue_final_tiles3.pop();
                if(t->id != "0" && t1->id=="0"){
                    AnimatedTile* falling_t = new AnimatedTile();
                    falling_t->Reload(t->id);
                    sf::Vector2f tpos = t->GetPosition();
                    falling_t->SetPosition(tpos.x, tpos.y);
                    falling_t->SetSize(t->GetWidth());
                    falling_tiles.push_back(falling_t);
                } else if(t1->id !="0") {
                    AnimatedTile* falling_t = new AnimatedTile();
                    falling_t->Reload(t1->id);
                    sf::Vector2f tpos = t1->GetPosition();
                    falling_t->SetPosition(tpos.x, tpos.y);
                    falling_t->SetSize(t1->GetWidth());
                    falling_tiles.push_back(falling_t);
                }
                else{
                    std::cout <<"WHHHHHHHHHHHHAAAAAAAAAAAAT" << std::endl;
                }
                t->Reload("0");
                t1->Reload("0");
			}
			while(!extension_tiles.empty()){
				Tile* act_ext_tile = extension_tiles.front();
				extension_tiles.pop();
				removeTile(act_ext_tile, 0);
			}
		}
	}
	if(!conex_abaix){
		std::queue<Tile*> queue_final_tiles4;
		eval_tile_pos.x = r_tile_pos_global.x ;
		eval_tile_pos.y = r_tile_pos_global.y + Chunk::TILE_SIZE;
		bool falls4 = calcPhysics(r_tile_pos_global, conex_dreta, conex_esquerra, conex_abaix, eval_tile_pos, queue_final_tiles4, 3, extension_tiles);
		if(falls4){
			while(!queue_final_tiles4.empty()){
                Tile* t = queue_final_tiles4.front();
                Tile* t1 = t->neighbors[8];
                queue_final_tiles4.pop();
                if(t->id != "0" && t1->id=="0"){
                    AnimatedTile* falling_t = new AnimatedTile();
                    falling_t->Reload(t->id);
                    sf::Vector2f tpos = t->GetPosition();
                    falling_t->SetPosition(tpos.x, tpos.y);
                    falling_t->SetSize(t->GetWidth());
                    falling_tiles.push_back(falling_t);
                } else if(t1->id !="0") {
                    AnimatedTile* falling_t = new AnimatedTile();
                    falling_t->Reload(t1->id);
                    sf::Vector2f tpos = t1->GetPosition();
                    falling_t->SetPosition(tpos.x, tpos.y);
                    falling_t->SetSize(t1->GetWidth());
                    falling_tiles.push_back(falling_t);
                }
                else{
                    int aaa=0;
                    std::cout <<"WHHHHHHHHHHHHAAAAAAAAAAAAT" << t->id <<" & " << t1->id << std::endl;
                    int jj;
                }
                t->Reload("0");
                t1->Reload("0");
			}
			while(!extension_tiles.empty()){
				Tile* act_ext_tile = extension_tiles.front();
				extension_tiles.pop();
				removeTile(act_ext_tile, 0);
			}
		}
	}
	//std::cout << falls1 << " " << falls2 << " " << falls3 << " " << falls4 << std::endl;
}

void Map::removeReachFloorCascade(float x, float y){
	bool finished = false;
	while(!finished){

		std::cout << y << std::endl;
		Tile* t0 = getTile(x, y, 0);
		Tile* t1 = getTile(x, y, 1);
		if((!t0->rigid && !t1->rigid) && (t0->visible || t1->visible)){
			t0->reach_floor = false;
			t1->reach_floor = false;
			y -= Chunk::TILE_SIZE;
		}
		else finished = true;
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
Chunk* Map::getChunk(float x, float y){
	int size_chunk_x = Chunk::N_TILES_X*Chunk::TILE_SIZE;
	int chunk_x = x/size_chunk_x;
	return chunk_mat[chunk_x];
}


Tile* Map::getTile(float x, float y, int z){
	if(y<0) y = 0;
	int size_chunk_x = Chunk::N_TILES_X*Chunk::TILE_SIZE;
	int chunk_x = (x-size_chunk_x*posMap)/size_chunk_x;

	Chunk* c = chunk_mat[chunk_x];

	return c->getTile(x, y, z);
}

int Map::getChunkIndex(float x){
	int size_chunk_x = Chunk::N_TILES_X*Chunk::TILE_SIZE;
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

        Chunk *c1 = chunk_mat[0];
        Chunk *c2 = chunk_mat[N_CHUNKS_X - 1];
        Tile *t1 = c1->getTileByIndex(0, 0, 0);
        Tile *t2 = c2->getTileByIndex(Chunk::N_TILES_X - 1, 0, 0);
        sf::Vector2f p1 = t1->GetPosition();
        sf::Vector2f p2 = t2->GetPosition();
        float distance_1 = sqrt((x - p1.x) * (x - p1.x));
        float distance_2 = sqrt((x - p2.x) * (x - p2.x));


        if (distance_1 < Chunk::N_TILES_X / 2 * Chunk::TILE_SIZE) {
            //#pragma omp task
            {

                c2->saveToFile();
                int current_pos = c1->chunk_pos.x;
                int id_temp = 0;
                Chunk *chunk_mid = chunk_mat[1];


                chunk_mat[2] = chunk_mid;
                chunk_mat[1] = c1;
                --posMap;
                createMap(0, current_pos - 1, id_temp);
                chunk_mat[2]->neighbors[1] = nullptr;
                chunk_mat[2]->calcLateralNeighborsTiles(1);
                chunk_mat[2]->recalcReachFloor();
                delete c2;
            }

            //std::cout << distance_1 << " " << distance_2 << std::endl;
        }
        if (distance_2 < Chunk::N_TILES_X / 2 * Chunk::TILE_SIZE) {
            //#pragma omp task
            {

                c1->saveToFile();
                int current_pos = c2->chunk_pos.x;
                int id_temp = 0;
                Chunk *chunk_mid = chunk_mat[1];

                chunk_mat[0] = chunk_mid;
                chunk_mat[1] = c2;
                ++posMap;
                createMap(2, current_pos + 1, id_temp);
                chunk_mat[0]->neighbors[1] = nullptr;
                chunk_mat[0]->calcLateralNeighborsTiles(0);
                chunk_mat[0]->recalcReachFloor();
                delete c1;
            }
            //std::cout << distance_1 << " " << distance_2 << std::endl;
        }


}

std::vector<Tile*> Map::getTilesCol(sf::Vector2f pos, sf::Vector2f size){
    std::vector<Tile*> result;
    float num_ite_x = size.x/Chunk::TILE_SIZE;
    float num_ite_y = size.y/Chunk::TILE_SIZE;
    for(int j = 0; j <= num_ite_y; ++j){
            for(int i = 0; i <=num_ite_x; i++){
                Tile* t = getTile(pos.x+i*Chunk::TILE_SIZE, pos.y+j*Chunk::TILE_SIZE, 1);
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


void Map::DrawFrontItems(sf::RenderWindow& renderWindow)
{
    sf::View currentView = renderWindow.getView();
    sf::Vector2f centerView = currentView.getCenter();
    sf::Vector2f sizeView = currentView.getSize();
    float first_x = centerView.x-(sizeView.x/2)-1;
    float last_x = centerView.x+(sizeView.x/2)+1;
    int first_chunk = getChunkIndex(first_x);
    int last_chunk = getChunkIndex(last_x+Chunk::TILE_SIZE);
    for(int i = first_chunk ; i<=last_chunk ; ++i) {
        int index_mat = getIndexMatChunk(i);
        chunk_mat[index_mat]->DrawGrassTiles(renderWindow, *texMan);
    }
}
void Map::DrawMap(sf::RenderWindow& renderWindow)
{
    sf::View currentView = renderWindow.getView();
    sf::Vector2f centerView = currentView.getCenter();
    sf::Vector2f sizeView = currentView.getSize();
    float first_x = centerView.x-(sizeView.x/2)-1;
    float first_y = centerView.y-(sizeView.y/2)-1;
    float last_x = centerView.x+(sizeView.x/2)+1;
    float last_y = centerView.y+(sizeView.y/2)+1;
    //if(first_x<0) first_x = 0;
    //if(first_y<0) first_y = 0;

    sf::Vector2f firstPos(first_x, first_y);
    sf::Vector2f lastPos(last_x+Chunk::TILE_SIZE, last_y+Chunk::TILE_SIZE);
    //std::cout << first_x << " " << first_y << " " << last_x << " " << last_y << std::endl;
    int first_chunk = getChunkIndex(first_x);
    int last_chunk = getChunkIndex(last_x+Chunk::TILE_SIZE);
    //std::cout << "first " << first_chunk.x << "last " << last_chunk.x << std::endl;
    //std::cout << "last " << last_chunk.x << " " << last_chunk.y << std::endl;
    //std::cout << first_chunk <<  " " << last_chunk << " " << first_x << std::endl;

    for(int i = first_chunk ; i<=last_chunk ; ++i) {
            //if(i>0) std::cout << "heeyy" << std::endl;
        int index_mat = getIndexMatChunk(i);

            //std::cout << firstPos.x << " " << firstPos.y << " " << lastPos.x << " " << lastPos.y << std::endl;
            //std::cout << "draw chunk " << index_mat.x << " " << index_mat.y << std::endl;
            //#pragma omp task shared(renderWindow)
        chunk_mat[index_mat]->DrawChunk(renderWindow, firstPos, lastPos, *texMan);
    }

    for(int i = 0; i<falling_tiles.size(); i++){

        falling_tiles[i]->Draw(renderWindow, *texMan);
    }
    //vector<int> v
    /*
    std::map<std::string,VisibleGameObject*>::const_iterator itr = _gameObjects.begin();
    while(itr != _gameObjects.end())
    {
        itr->second->Draw(renderWindow);
        itr++;
    } */
}

void Map::UpdateAll(float delta)
{
	for(int i=0; i<falling_tiles.size(); ++i){
		falling_tiles[i]->Update(delta);
	}

}