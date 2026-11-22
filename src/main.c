#define STB_IMAGE_IMPLEMENTATION

#include "game.h"
#include "raycast.h"
#include "render.h"
#include "stb_image.h"
#include "texture.h"
#include "window.h"

#include <Tracy/tracy/TracyC.h>

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_timer.h>
#include <math.h>

#define MAP_WIDTH 16
#define MAP_HEIGHT 16
#define TILE_SIZE 32

Window *window;
Renderer *renderer;
bool stop;

// TODO MOVE!
GameState *game_state;
Texture *box_tex;
Texture *floor_tex;

Vec2f camera_plane;

void gameLoop(void);
void draw(void);

int main(void)
{

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                    "SDL_Init failed %s\n", SDL_GetError());
        return 1;
    }

    window = createWindow(1920, 1080, "Maze Game");
    renderer = createRenderer(window->width, window->height, window);
    game_state = initGame(MAP_WIDTH, MAP_HEIGHT);

    camera_plane.x = game_state->player->direction.y;
    camera_plane.y = -game_state->player->direction.x;

    float fov_tan = tan(RAD(game_state->player->fov_deg / 2.0f));
    vec2f_scale(&camera_plane, 2*fov_tan);

    stop = false;

    box_tex = createTexture("/home/shane/Documents/repos/MazeGame/res/bricksx64.png", 0);
    floor_tex = createTexture("/home/shane/Documents/repos/MazeGame/res/stein2.png", 1);

    while (!stop)
    {
        gameLoop();
    }

    destroyTexture(box_tex);
    destroyTexture(floor_tex);
    destroyRenderer(renderer);
    destroyWindow(window);
    SDL_Quit();
    return 0;
}

void draw(void)
{

    clearRenderer(renderer);

    raycast(renderer, game_state, camera_plane);

    updateRenderer(renderer, window);
}

void gameLoop(void)
{
    SDL_Event e;
    Player *player = game_state->player;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_EVENT_QUIT)
        {
            stop = true;
            return;
        }

        if ((e.type == SDL_EVENT_KEY_DOWN) && (e.key.key == SDLK_ESCAPE)) 
        {
            stop = true;
            return;
        }

        if (e.type == SDL_EVENT_MOUSE_MOTION)
        {
            float dir_x = player->direction.x;
            float dir_y = player->direction.y;
            float rad = -RAD(e.motion.xrel) * 0.2f;

            player->direction.x = dir_x * cos(rad) - dir_y * sin(rad);
            player->direction.y = dir_x * sin(rad) + dir_y * cos(rad);

            camera_plane.x = player->direction.y;
            camera_plane.y = -player->direction.x;
            float fov_tan = tan(RAD(player->fov_deg / 2.0f));
            vec2f_scale(&camera_plane, 2*fov_tan);
        }
    }


    // Keyboard
    const bool *key_states = SDL_GetKeyboardState(NULL);

    // TODO collisions
    if (key_states[SDL_SCANCODE_W])
    {
        player->position.x += player->direction.x * player->movement_speed;
        player->position.y += player->direction.y * player->movement_speed;
    }
    
    if (key_states[SDL_SCANCODE_S])
    {
        player->position.x -= player->direction.x * player->movement_speed;
        player->position.y -= player->direction.y * player->movement_speed;
    }

    Vec2f left_dir = {-player->direction.y, player->direction.x};
    if (key_states[SDL_SCANCODE_A])
    {
        player->position.x += left_dir.x * player->movement_speed;
        player->position.y += left_dir.y * player->movement_speed;
    }

    if (key_states[SDL_SCANCODE_D])
    {
        player->position.x -= left_dir.x * player->movement_speed;
        player->position.y -= left_dir.y * player->movement_speed;
    }

    // Render
    draw();

    static uint64_t last_ticks = 0;
    static int frames = 0;
    frames++;
    uint64_t now = SDL_GetTicks();
    if (now - last_ticks >= 1000)
    {
        SDL_Log("Maze Game - %d FPS", frames);
        frames = 0;
        last_ticks = now;
    }
}