//
// Created by ant on 06/11/16.
//

#ifndef GENLAND_RUNNINGGAME_H
#define GENLAND_RUNNINGGAME_H
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include "Drawer.h"
#include "Player.h"
#include "world/Map.h"
#include "../menu_interface/MenuMain.h"
#include "../menu_interface/NewGameMenu.h"
#include "world/WorldBackground.h"
#include "Clock.h"
#include "../MagicView.h"

class Scene {
public:

    static Scene* getScene(){
        static Scene* scenePointer;
        if(! scenePointer) scenePointer = new Scene();
        return scenePointer;
    }
    void init(std::string path, sf::RenderWindow &window, std::string seed);
    bool isInit();

    Scene();
    ~Scene();

    Map &getMap();
    Player &getPlayer();
    Drawer &getDrawer();
    WorldBackground &getBackgrounds();
    Clock &getClock();
    float getZoom();
    std::string getGamePath();
    std::string getSeed();
    std::mt19937 &getGenerator();

    void updateView();
    void update(sf::RenderWindow &window,float delta);
    void saveGame();
    void draw(sf::RenderWindow &window);

    int getTemperature(sf::Vector2f pos);
    int getHumidity(sf::Vector2f pos);
    float getMountFactor(sf::Vector2f pos);

private:
    std::mt19937 _generator;
    std::string _seed;
    std::string _pathGame;
    MagicView _viewGame;
    bool _initialized;
    float _zoom;
    Map _map_curr;
    Player _player;
    Drawer _drawer;
    WorldBackground _backgrounds;
    Clock _clock;
};


#endif //GENLAND_RUNNINGGAME_H
