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
#include "world/Ecosystem.h"
#include "../Utils.h"
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
    void syncEntitiesWithLoadedChunk(Chunk *c,int index_in_mat_chunks);
    std::vector<Date*>* getEcosystemLastUpdateList();
    sf::Vector2i searchIntervalEcosystem(int i);
    sf::Vector2i getIntervalEcosystem(int i);
    std::vector<std::vector<std::pair<int, bool> > >* getSurface(sf::Vector2i interval);
    std::vector<std::vector<std::vector<int> > >* getUnderground(sf::Vector2i interval);
    bool ecosystemsReady();
    void addEntity(Entity *e);
    Entity *getEntity(sf::FloatRect pos);
    Ecosystem* getEcosystem(int index);
    void getMobsOnArea(std::vector<Mob*> &mobs, sf::Vector2i position, int radius, int indexEcosystem);
    void getTreesOnArea(std::vector<Tree*> &trees, sf::Vector2i position, int radius, int indexEcosystem);
    void getEntityesArea(std::vector<Entity*> &entities, sf::Vector2i position, int radius, int indexEcosystem);
    void getPositionsOnArea(std::vector<sf::Vector2i> &positions,sf::Vector2i position, int radius, int indexEcosystem);
    void getFallingTilesArea(std::vector<AnimatedTile*> &entities, sf::Vector2i position, int radius);
    sf::RenderWindow* getWindowDebug();
private:
    sf::RenderWindow* _winForDebug;
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
    std::pair<Ecosystem*,Ecosystem*> _ecosystems;
    bool betweenInts(sf::Vector2i interval, int i);
    std::vector<Date*> _entitiesLastUpdate;
    void checkEcosystems();
};


#endif //GENLAND_RUNNINGGAME_H
