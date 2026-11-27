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
static Player *initPlayer(void);

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
    game_state->player = initPlayer();

    game_state->map_width = map_w;
    game_state->map_height = map_h;

    return game_state;
}

void freeGameState(GameState *game_state)
{
    free(game_state->map);
    free(game_state->player);
    free(game_state);
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

// TODO some of these should be configurable
static Player *initPlayer(void)
{
    Player *player = malloc(sizeof(*player)); // TODO Arena
    if (player == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
            "initGame: Failed to allocate memory for player.");

        return NULL;
    }

    player->height = 32;
    player->direction.x = 1;
    player->direction.y = 0;
    player->fov_deg = 60.0f;
    player->position.x = 2;
    player->position.y = 2;
    player->movement_speed = 0.01f;

    return player;
}