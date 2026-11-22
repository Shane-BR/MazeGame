#include "game.h"
#include <SDL3/SDL_log.h>
#include <stdlib.h>

static const char tmp_level[] =
{
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
    1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1,
    1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static char *initMap(int map_w, int map_h);

GameState *initGame(int map_w, int map_h)
{
    GameState *game_state = malloc(sizeof(*game_state)); // TODO Arena

    if (game_state == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
            "initGame: Failed to allocate memory for game state.");

        return NULL;
    }

    game_state->map = initMap(map_w, map_h);

    game_state->player = malloc(sizeof(*game_state->player)); // TODO Arena
    if (game_state->player == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
            "initGame: Failed to allocate memory for player.");

        return NULL;
    }

    game_state->map_width = map_w;
    game_state->map_height = map_h;

    game_state->player->height = 32;
    game_state->player->direction.x = 1;
    game_state->player->direction.y = 0;
    game_state->player->fov_deg = 60.0f;
    game_state->player->position.x = 2;
    game_state->player->position.y = 2;
    game_state->player->movement_speed = 1.0f;

    return game_state;
}

char getMapTile(const GameState *game_state, Vec2i at)
{
    if (at.x < 0 || at.x >= game_state->map_width ||
        at.y < 0 || at.y >= game_state->map_height)
    {
        return -1;
    }

    return game_state->map[at.x + at.y * game_state->map_width];
}

static char *initMap(int map_w, int map_h)
{
    char *map = malloc(sizeof(*map)*map_w*map_h); // TODO Arena
    if (map == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
            "initGame: Failed to allocate memory for game map.");

        return NULL;
    }

    // TODO Replace with maze generator
    memcpy(map, tmp_level, sizeof(tmp_level));

    return map;
}