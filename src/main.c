#define STB_IMAGE_IMPLEMENTATION

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_timer.h>
#include <float.h>
#include <math.h>
#include "render.h"
#include "stb_image.h"
#include "texture.h"
#include "window.h"

#define MAP_WIDTH 16
#define MAP_HEIGHT 16
#define TILE_SIZE 32
#define PI 3.14159265358979323846
#define RAD(x) ((x)*(PI/180.0))

Window *window;
Renderer *renderer;
bool stop;

char map[MAP_WIDTH][MAP_HEIGHT] = 
{
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1 },
    { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};

typedef struct
{
    uint8_t height;
    uint8_t fov_deg;
    float angle_deg;
    float x;
    float y;
} 
Player;

Player player = 
{
    .height = 32,
    .fov_deg = 60,
    .angle_deg = 60,
    (MAP_WIDTH/2.0)*TILE_SIZE, (MAP_HEIGHT/2.0)*TILE_SIZE
};

// TODO MOVE!!!
float distToProjPlane;
float angle_between_rays;

Texture *box_tex;

void gameLoop(void);
void draw(void);
bool inMap(float x, float y);

int main(void)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                    "SDL_Init failed %s\n", SDL_GetError());
        return 1;
    }

    window = createWindow(1920, 1080, "Maze Game");
    renderer = createRenderer(getWindowWidth(window), getWindowHeight(window), window);

    stop = false;

    // TODO allow for window scaling
    distToProjPlane = (getWindowWidth(window)/2.0)/tan(RAD(player.fov_deg/2.0));
    angle_between_rays = (float)player.fov_deg / getWindowWidth(window);
    box_tex = createTexture("/home/shane/Documents/repos/MazeGame/res/bricksx64.png");
    

    while (!stop)
    {
        gameLoop();
    }

    destroyRenderer(renderer);
    destroyWindow(window);
    SDL_Quit();
    return 0;
}

bool inMap(float x, float y)
{
    return  x > 0 &&
            x < MAP_WIDTH*TILE_SIZE &&
            y > 0 &&
            y < MAP_HEIGHT*TILE_SIZE;
}

void draw(void)
{

    clearRenderer(renderer);

    float start = player.angle_deg - player.fov_deg/2.0;
    float intersection_x;
    float intersection_y;
    int windowWidth = getWindowWidth(window);
    int windowHeight = getWindowHeight(window);

    for (int c = 0; c < windowWidth; c++)
    {
        float ray_angle_deg = start + c*angle_between_rays;

        // Undefined tan
        if(ray_angle_deg == 90.0f || ray_angle_deg == 270.0f)
            continue;

        if (ray_angle_deg > 360.0f)
        {
            ray_angle_deg -= 360.0f;
        }
        else if (ray_angle_deg < 0)
        {
            ray_angle_deg += 360.0f;
        }

        float rad = RAD(ray_angle_deg);
        float tanTheta = tan(rad);
        float cosTheta = cos(rad);
        float distance = FLT_MAX;
        float dy, dx;

        intersection_y = (int)(player.y/TILE_SIZE) * TILE_SIZE;

        // HORIZONTAL

        // Facing up
        if (ray_angle_deg <= 180.0f)
        {
            // -1 so that the intersection is inside of the tile
            intersection_y -= 1;
            dy = -TILE_SIZE;
        }
        else // Facing down
        {
            intersection_y += TILE_SIZE; // Intersected with lower tile
            dy = TILE_SIZE;
        }


        intersection_x = player.x + (player.y - intersection_y)/tanTheta;
        bool skip = !inMap(intersection_x, intersection_y);

        dx = (-dy) / tanTheta;

        while (!skip &&
                map [(unsigned int)intersection_y/TILE_SIZE]
                    [(unsigned int)intersection_x/TILE_SIZE] == 0)
        {
            intersection_x += dx;
            intersection_y += dy;

            skip = !inMap(intersection_x, intersection_y);
        }

        int texture_xoffset;
        if (!skip)
        {
            distance = fabs((player.x - intersection_x)/cosTheta);
            texture_xoffset = (unsigned)(intersection_x - (unsigned)intersection_x/TILE_SIZE) % getTextureWidth(box_tex);
        }

        intersection_x = (int)(player.x/TILE_SIZE) * TILE_SIZE;

        // VERTICAL

        // Facing left
        if (ray_angle_deg > 90.0f && ray_angle_deg < 270.0f)
        {
            intersection_x -= 1;
            dx = -TILE_SIZE;
        }
        else 
        {
            intersection_x += TILE_SIZE;
            dx = TILE_SIZE;
        }

        intersection_y = player.y + (player.x - intersection_x)*tanTheta;

        dy = (-dx)*tanTheta;

        skip = !inMap(intersection_x, intersection_y);

        while (!skip &&
                map [(unsigned int)intersection_y/TILE_SIZE]
                    [(unsigned int)intersection_x/TILE_SIZE] == 0)
        {
            intersection_x += dx;
            intersection_y += dy;

            skip = !inMap(intersection_x, intersection_y);
        }

        if (!skip)
        {
            float temp = fabs((player.x - intersection_x)/cosTheta);

            if (temp < distance)
            {
                distance =  temp;
                texture_xoffset = (unsigned)(intersection_y - (unsigned)intersection_y/TILE_SIZE) % getTextureWidth(box_tex);
            }

        }

        if (distance == FLT_MAX)
            continue;

        // Correct fisheye distortion FIXME
        distance = distance * cos(RAD(player.angle_deg - ray_angle_deg));

        // Draw
        float projDrawHeight = (TILE_SIZE / distance) * distToProjPlane;
        float middle = windowHeight/2.0;

        float y1 = middle - (projDrawHeight/2.0);
        float y2 = middle + (projDrawHeight/2.0);

        y1 = y1 < 0 ? 0 : y1;
        y1 = y1 > windowHeight ? windowHeight : y1;

        y2 = y2 < 0 ? 0 : y2;
        y2 = y2 > windowHeight ? windowHeight : y2;

        drawTexturedVertLine(renderer, box_tex, texture_xoffset, y1, y2, c);    
    }

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
    }

    player.angle_deg = player.angle_deg + 0.1;
    player.angle_deg = player.angle_deg > 360 ? player.angle_deg - 360 : player.angle_deg;

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