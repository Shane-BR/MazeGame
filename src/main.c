#include "game.h"
#include "raycast.h"
#define STB_IMAGE_IMPLEMENTATION

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_timer.h>
#include <float.h>
#include "render.h"
#include "stb_image.h"
#include "texture.h"
#include "window.h"

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

    box_tex = createTexture("/home/shane/Documents/repos/MazeGame/res/bricksx64.png");
    floor_tex = createTexture("/home/shane/Documents/repos/MazeGame/res/stein2.png");

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

        }
    }


    // Keyboard
    const bool *key_states = SDL_GetKeyboardState(NULL);

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