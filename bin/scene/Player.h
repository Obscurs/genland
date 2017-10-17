#pragma once
#include "world/Map.h"
#include "world/Background.h"
#include "Inventory.h"
#include "Colisionable.h"
#include <vector>
class Player: public Colisionable
{
public:
	Player();
	~Player();
	const static int PLAYER_SPEED_X = 150;
	const static int PLAYER_SPEED_Y = 400;
    const static int PLAYER_SPRITE_SIZE = 64;
    const static int PLAYER_SPRITE_MAX_TIME = 1;
    const static int PLAYER_WIDTH = 32;
    const static int PLAYER_HEIGHT = 48;
    const static int MAX_HEALTH = 100;
    const static int MAX_TEMP_BASE = 35;
    const static int MIN_TEMP_BASE = 10;
    enum ActionState{IDLE, WALKING,ATTACKING,STAIRS, FALLING};
    enum AnimationDirection{LEFT,RIGHT};
    enum EquipTool {W_PICKAXE,W_SWORD, NONE_T};
    enum EquipArmor {NONE_A,ARMOR1,ARMOR2,ARMOR3};
	void Load(std::string filename);
	void Draw(sf::RenderWindow & window);
	void Draw2(sf::RenderTexture & tex);
	void DrawInventory(sf::RenderWindow & window);
    void DrawStats(sf::RenderTarget &target);
	void Update(float delta, Map &map, sf::RenderWindow &window);
    int id;
	void SetPosition(float x, float y);
	void SetSize(float x);
	void saveStats(std::string pathGame);
	void loadStats(std::string pathGame);
	sf::Vector2f GetPosition() const;


private:
    void setAnimation(ActionState act);
    void updateSprite(float delta);
    void updateToolsAndArmors();
    void updateHealth(float delta);

	sf::Sprite& GetSprite();
	bool giveItem(std::string id, int amount);

    //STATS
    int _resPhysics;
    float _health;
    int _maxTemperatureSafe;
    int _minTemperatureSafe;
	//PHYSICS
	float vx;
	float vy;
    bool _attacking;
    bool _mining;
	//INVENTORY
	Inventory* inventory;

	//REMOVE AND PUT TILES;
	Tile* tile_being_removed;


	//SHOW
	sf::Sprite _sprite;
    sf::Sprite _spriteTool;
	sf::Texture _image;
	std::string _filename;
    sf::Vector2f _colPosition;
    sf::Vector2f _colSize;
    float _spriteTime;
    ActionState _animationId;
    AnimationDirection _playerDirection;
    EquipTool _tool;
    EquipArmor _armor;
    int _animationFrame;
    int _numFramesAnimation;
    float _toolFactor;

};
