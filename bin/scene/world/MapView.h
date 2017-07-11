//
// Created by arnito on 11/07/17.
//

#ifndef GENLAND_MAPVIEWER_H
#define GENLAND_MAPVIEWER_H
#include <iostream>
#include <vector>
#include "Chunk.h"

class MapViewer {
public:
    MapViewer();
    ~MapViewer();
    void addChunk(Chunk &c);
    void draw(sf::RenderTarget &render);
    void update(int currentChunk, sf::Vector2i tilePlayer);
private:
    int _currentChunk;
    sf::Vector2i _currentTile;
    int _displace;
    bool _visible;
    struct MapChunk {
        std::vector<std::vector<std::string> > values;
        int idChunk;
    };
    void drawOnTexture(int size);
    sf::RenderTexture _texture;
    sf::Color getColorByTileId(std::string s);
    std::vector<MapChunk> _positive_map;
    std::vector<MapChunk> _negative_map;
};


#endif //GENLAND_MAPVIEWER_H
