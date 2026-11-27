#ifndef MG_GAME_H
#define MG_GAME_H

#include "player.h"

#define TILE_SIZE 32

typedef struct 
{
    int map_width, map_height;
    char *map;
    Player *player;
    
} GameState;

GameState *initGame(int map_w, int map_h);
void freeGameState(GameState *game_state);
char getMapTile(const GameState *game_state, Vec2i at);

#endif