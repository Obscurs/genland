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

    Map *getMap();
    Player *getPlayer();
    Drawer *getDrawer();
    WorldBackground *getBackgrounds();
    Clock *getClock();
    float getZoom();
    std::string getGamePath();
    std::string getSeed();

    void updateView();
    void update(sf::RenderWindow &window,float delta);
    void saveGame();
    void draw(sf::RenderWindow &window);

    int getTemperature(sf::Vector2f pos);
    int getTemperatureGlobal(sf::Vector2f pos);
    int getHumidityGlobal(sf::Vector2f pos);
    int getHumidity(sf::Vector2f pos);
    float getMountFactor(sf::Vector2f pos);
    sf::Vector2i getLimsBiome();
    void addLimit(int newLimit);
    bool firstBiomeCreated();
    void syncTreesWithChunk(Chunk *c,int index_in_mat_chunks);
    void syncNotRenderedTrees(Chunk *c);
    void addTreeToEntities(Tree t, sf::Vector2i interval);
    sf::Vector2i searchIntervalEcosystem(int i);
    sf::Vector2i getIntervalEcosystem(int i);
    std::vector<std::vector<std::pair<int, bool> > >* getSurface(sf::Vector2i interval);
    std::vector<std::vector<std::vector<int> > >* getUnderground(sf::Vector2i interval);
    bool _eco1Ready;
    bool _eco2Ready;
    bool __auxEco;
    int __auxPos;
private:
    struct date {
        sf::Vector2i interval;
        int day;
        int hour;
        int min;
    } ;
    void changeEcosystem();
    void updateWithElapsedTime(std::vector<Tree*> &entities, date *d);
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
    std::vector<int> _biomeLimitsRight;
    std::vector<int> _biomeLimitsLeft;
    sf::Vector2i _currentEcosystem1;
    sf::Vector2i _currentEcosystem2;
    bool betweenInts(sf::Vector2i interval, int i);

    std::vector<Tree*> _entities1;
    std::vector<Tree*> _entities2;

    std::vector<date*> _entitiesLastUpdate;
    std::vector<std::vector<std::pair<int, bool> > > _surface1;
    std::vector<std::vector<std::pair<int, bool> > >_surface2;
    std::vector<std::vector<std::vector<int> > >_underground1;
    std::vector<std::vector<std::vector<int> > >_underground2;



    void updateEcosystems(float delta);
    void saveEntities(bool arrayChosen);
    void loadEntities(bool arrayChosen);

    void linkTrees(bool arrayChosen);
    sf::Thread _threadSaveLoad0;
    sf::Thread _threadSaveLoad1;
};


#endif //GENLAND_RUNNINGGAME_H
