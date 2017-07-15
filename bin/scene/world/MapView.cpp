//
// Created by arnito on 11/07/17.
//

#include "MapView.h"
#include "../../Settings.h"
#include "../../MagicView.h"
#include "../../Inputs.h"

MapViewer::MapViewer(){
    _texture.create(Chunk::N_TILES_X*10, Chunk::N_TILES_Y);
    _displace = 0;
    _visible = false;
    _currentChunk =0;
    _currentTile = sf::Vector2i(0,0);
}
MapViewer::~MapViewer(){

}
void MapViewer::update(int currentChunk, sf::Vector2i tilePlayer){
    _currentChunk = currentChunk;
    _currentTile = tilePlayer;
    if(_visible){
        if(Inputs::KeyBreak(Inputs::LEFT)){
            _displace -=1;
            drawOnTexture(1);
        }
        if(Inputs::KeyBreak(Inputs::RIGHT)){
            _displace +=1;
            drawOnTexture(1);
        }
        if(Inputs::KeyBreak(Inputs::M)){
            _visible=false;
        }
    } else if(Inputs::KeyBreak(Inputs::M)) {
        _visible = true;
        int firstChunkId=0;
        if(_negative_map.size() >0) firstChunkId = _negative_map[_negative_map.size()-1].idChunk;
        else if(_positive_map.size() >0) firstChunkId = _positive_map[0].idChunk;

        _displace = _currentChunk-firstChunkId-4;
        drawOnTexture(1);
    }
}
void MapViewer::draw(sf::RenderTarget &render){
    if(_visible){
        sf::View currentView = render.getView();
        MagicView *view = new MagicView(&render,MagicView::expanded,sf::Vector2i(_texture.getSize().x,_texture.getSize().y));
        render.setView(*view);
        _texture.display();
        sf::Sprite sprite(_texture.getTexture());
        render.draw(sprite);

        sf::RectangleShape rectangle;
        rectangle.setFillColor(sf::Color(255,255,255,0));
        rectangle.setOutlineColor(sf::Color::Red);
        rectangle.setOutlineThickness(1);
        rectangle.setSize(sf::Vector2f(Chunk::N_TILES_X*3, Chunk::N_TILES_Y));
        int firstChunkId=0;
        if(_negative_map.size() >0) firstChunkId = _negative_map[_negative_map.size()-1].idChunk;
        else if(_positive_map.size() >0) firstChunkId = _positive_map[0].idChunk;
        firstChunkId = firstChunkId +_displace;
        int chunkInTexture = _currentChunk-firstChunkId;
        rectangle.setPosition(sf::Vector2f(Chunk::N_TILES_X*(chunkInTexture-1),0));
        render.draw(rectangle);

        rectangle.setFillColor(sf::Color(255,0,0,255));
        rectangle.setSize(sf::Vector2f(2, 2));
        rectangle.setPosition(sf::Vector2f(Chunk::N_TILES_X*(chunkInTexture-1)+_currentTile.y+Chunk::N_TILES_X,_currentTile.x));
        render.draw(rectangle);
        render.setView(currentView);
    }

}
void MapViewer::addChunk(Chunk &c){
    int idChunk = c.chunk_id;
    if(idChunk>=0){
        if(idChunk<_positive_map.size()){
            MapChunk *mc = &_positive_map[idChunk];
            mc->idChunk = idChunk;
            for(int i=0; i<mc->values.size(); i++){
                for(int j =0; j<mc->values[i].size(); j++){
                    std::string value = c.getTileByIndex(j,i,1)->id;
                    if(value =="0") value = c.getTileByIndex(j,i,0)->id;
                    mc->values[i][j] = value;
                }
            }
        }
        else{
            int size = _positive_map.size();
            while(size < idChunk){
                MapChunk mc;
                mc.values = std::vector<std::vector<std::string> >(Chunk::N_TILES_Y,std::vector<std::string>(Chunk::N_TILES_X,"0"));
                mc.idChunk = size;
                _positive_map.push_back(mc);
                ++size;
            }
            MapChunk mc2;
            mc2.values = std::vector<std::vector<std::string> >(Chunk::N_TILES_Y, std::vector<std::string>(Chunk::N_TILES_X,"0"));
            for(int i=0; i<mc2.values.size(); i++){
                for(int j =0; j<mc2.values[i].size(); j++){
                    std::string value = c.getTileByIndex(j,i,1)->id;
                    if(value =="0") value = c.getTileByIndex(j,i,0)->id;
                    mc2.values[i][j] = value;
                }
            }
            mc2.idChunk = idChunk;
            _positive_map.push_back(mc2);

        }
    } else{
        idChunk = idChunk*(-1)-1;
        if(idChunk<_negative_map.size()){
            MapChunk *mc = &_negative_map[idChunk];
            mc->idChunk = idChunk;
            for(int i=0; i<mc->values.size(); i++){
                for(int j =0; j<mc->values[i].size(); j++){
                    std::string value = c.getTileByIndex(j,i,1)->id;
                    if(value =="0") value = c.getTileByIndex(j,i,0)->id;
                    mc->values[i][j] = value;
                }
            }
        }
        else{
            int size = _negative_map.size();
            while(size < idChunk){
                MapChunk mc;
                mc.values = std::vector<std::vector<std::string> >(Chunk::N_TILES_Y, std::vector<std::string>(Chunk::N_TILES_X,"0"));
                mc.idChunk = size;
                _negative_map.push_back(mc);
                ++size;
            }
            MapChunk mc2;
            mc2.values = std::vector<std::vector<std::string> >(Chunk::N_TILES_Y, std::vector<std::string>(Chunk::N_TILES_X,"0"));
            for(int i=0; i<mc2.values.size(); i++){
                for(int j =0; j<mc2.values[i].size(); j++){
                    std::string value = c.getTileByIndex(j,i,1)->id;
                    if(value =="0") value = c.getTileByIndex(j,i,0)->id;
                    mc2.values[i][j] = value;
                }
            }
            mc2.idChunk = -1*(idChunk+1);
            _negative_map.push_back(mc2);
        }
    }
    drawOnTexture(1);
}
void MapViewer::drawOnTexture(int size) {
    int num_elems = _texture.getSize().x/(size*Chunk::N_TILES_X);
    int x_tex =0;
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(size, size));
    _texture.clear(sf::Color(255,255,255,0));
    for(int p = _displace; p<_displace+num_elems; p++){
        if(p< _negative_map.size()){
            MapChunk mc = _negative_map[_negative_map.size()-1-p];
            for(int i=0; i<mc.values.size(); i++){
                for(int j =0; j<mc.values[i].size(); j++){
                    std::string value = mc.values[i][j];
                    rectangle.setPosition(j*size+x_tex*size*Chunk::N_TILES_X, i*size);
                    sf::Color color = getColorByTileId(value);
                    rectangle.setFillColor(color);
                    if(color !=sf::Color(255,255,255,255)) _texture.draw(rectangle);
                }
            }

        } else if(p-_negative_map.size() < _positive_map.size()){
            MapChunk mc = _positive_map[p-_negative_map.size()];
            for(int i=0; i<mc.values.size(); i++){
                for(int j =0; j<mc.values[i].size(); j++){
                    std::string value = mc.values[i][j];
                    rectangle.setPosition(j*size+x_tex*size*Chunk::N_TILES_X, i*size);
                    sf::Color color = getColorByTileId(value);
                    rectangle.setFillColor(color);
                    if(color !=sf::Color(255,255,255,255)) _texture.draw(rectangle);

                }
            }
        }
        x_tex++;
    }

}
sf::Color MapViewer::getColorByTileId(std::string s){
    if(s == "d") return sf::Color(76,58,34,150);
    else if(s == "D") return sf::Color(135,102,60,150);
    else if(s == "c") return sf::Color(99,97,95,150);
    else if(s == "C") return sf::Color(145,142,139,150);
    else return sf::Color(255,255,255,255);
}