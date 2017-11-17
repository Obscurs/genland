//
// Created by arnito on 16/12/16.
//

#ifndef GENLAND_SETTINGS_H
#define GENLAND_SETTINGS_H

#endif //GENLAND_SETTINGS_H
class Settings{
public:

    static int TILE_SIZE;
    static int SCREEN_WIDTH;
    static int SCREEN_HEIGHT;
    static int GAME_WIDTH;
    static int GAME_HEIGHT;

    static const int DISTANCE_DAMAGE_TREE = 32;
    static constexpr float MIN_MOBS_ECO = 1.f;
    static constexpr float MIN_TREES_ECO = 0.5f;
    static constexpr float MAX_MOBS_ECO = 8.f;
    static constexpr float MAX_TREES_ECO = 4.f;
static const int TILE_SIZE_HIGH = 16;
static const int TILE_SIZE_MED = 8;
static const int TILE_SIZE_LOW = 4;
static const int RADIUS_MOB_MULTIPLYER = 8;
    static const int MIN_RADIUS_MOB = 256;
static const int GAME_WIDTH_HIGH = 1536;
static const int GAME_WIDTH_MED = 768;
static const int GAME_WIDTH_LOW = 432;

static const int GAME_HEIGHT_HIGH = 864;
static const int GAME_HEIGHT_MED = 384;
static const int GAME_HEIGHT_LOW = 216;

    static const int MAX_TEMPERATURE = 40;
    static const int MIN_TEMPERATURE = -30;
    static const int SYNC_UPDATE_SPEED = 35;
    static const int GEN_SPEED = 1;
};