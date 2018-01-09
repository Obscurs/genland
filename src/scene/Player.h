#pragma once
#include "world/Map.h"
#include "world/Background.h"
#include "Inventory.h"
#include "Colisionable.h"
#include <vector>
class Player: public Colisionable, public Entity
{
public:
	Player();
	~Player();
	const static int PLAYER_SPEED_X = 150;
	const static int PLAYER_SPEED_Y = 400;
    const static int PLAYER_SPRITE_SIZE = 64;
    const static int PLAYER_SPRITE_MAX_TIME = 1;
    const static int PLAYER_WIDTH = 30;
    const static int PLAYER_HEIGHT = 46;
    const static int MAX_HEALTH = 100;
	const static int MAX_HUNGER = 100;
    const static int MAX_TEMP_BASE = 35;
    const static int MIN_TEMP_BASE = 10;
    const static int DIST_ACTION = 65;

    const static int BASE_DAMAGE = 10;
    enum ActionState{IDLE, WALKING,ATTACKING,STAIRS, FALLING,DEAD};
    enum AnimationDirection{LEFT,RIGHT};
    enum EquipTool {W_PICKAXE,W_SWORD,I_PICKAXE,HAMMER, NONE_T};
    enum EquipArmor {NONE_A,ARMOR1,ARMOR2,ARMOR3};
	void Draw(sf::RenderWindow & window);
	void Draw2(sf::RenderTexture & tex);
	void DrawInventory(sf::RenderWindow & window);
    void DrawStats(sf::RenderTarget &target);
	void Update(float delta, Map &map, sf::RenderWindow &window);
    int id;
    Inventory* getInventory();
    void hurt(float amount);
	void SetPosition(float x, float y);
	void SetSize(float x);
	void saveStats(std::string pathGame);
	void loadStats(std::string pathGame);
	sf::Vector2f GetPosition() const;
    bool giveItem(std::string id, int amount);

private:
    void setAnimation(ActionState act);
    void updateSprite(float delta);
    void updateToolsAndArmors();
    void updateStats(float delta);
    void mine(Tile *t, float delta);
    void build(Tile *t, float delta, int position_tile);
	sf::Sprite& GetSprite();

    float _damage;
    //STATS
    int _resPhysics;
    float _health;
	float _hunger;
    int _maxTemperatureSafe;
    int _minTemperatureSafe;
	//PHYSICS
	float vx;
	float vy;
    bool _attacking;
    bool _mining;
	//INVENTORY
	Inventory* inventory;
    float _attackColdown;
	//REMOVE AND PUT TILES;
	Tile* tile_being_removed;


	//SHOW
	sf::Sprite _sprite;
    sf::Sprite _spriteTool;
	sf::Texture _image;
	std::string _filename;
    float _spriteTime;
    float _dieTime;
    bool _dead;
    ActionState _animationId;
    AnimationDirection _playerDirection;
    EquipTool _tool;
    EquipArmor _armor;
    int _animationFrame;
    int _numFramesAnimation;
    float _toolFactor;

    float _spriteTimeHurt;
    bool _hurted;
    bool _drawSquareTile;
    sf::Vector2f _squarePosition;
    sf::Sprite _spriteHurt;
    int _currHurtAnim;
    int _keyframeHurt;


};
