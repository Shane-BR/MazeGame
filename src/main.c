#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <float.h>
#include <math.h>
#include "stb_image.h"

#define MAP_WIDTH 16
#define MAP_HEIGHT 16
#define TILE_SIZE 32
#define PI 3.14159265358979323846
#define RAD(x) ((x)*(PI/180.0))

SDL_Window *window;
SDL_Surface *window_surface;
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

int windowWidth = 1920;
int windowHeight = 1080;

SDL_Surface *raycast_surface;
uint32_t *raycast_pixels;
uint32_t color;

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

    window = SDL_CreateWindow("Maze Game", windowWidth, windowHeight, 0);
    if (!window)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                    "Window creation failed %s\n", SDL_GetError());
        return 1;
    }

    window_surface = SDL_GetWindowSurface(window);

    stop = false;

    // TODO allow for window scaling
    distToProjPlane = (windowWidth/2.0)/tan(RAD(player.fov_deg/2.0));
    angle_between_rays = (float)player.fov_deg / windowWidth;
    raycast_surface = SDL_CreateSurface(windowWidth, windowHeight, window_surface->format);
    raycast_pixels = raycast_surface->pixels;
    color = SDL_MapRGBA(SDL_GetPixelFormatDetails(window_surface->format), 0, 0x99, 0x99, 0x99, 0xFF);

    while (!stop)
    {
        gameLoop();
    }

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

    SDL_ClearSurface(raycast_surface, 0, 0, 0, 0xFF);

    float start = player.angle_deg - player.fov_deg/2.0;
    float intersection_x;
    float intersection_y;
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

        if (!skip)
        {
            distance = fabs((player.x - intersection_x)/cosTheta);
        }

        intersection_x = (int)(player.x/TILE_SIZE) * TILE_SIZE;

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
            distance = temp < distance ? temp : distance;
        }

        // Correct fisheye distortion
        distance = distance * cos(RAD(ray_angle_deg - player.angle_deg));

        // Draw
        float projDrawHeight = (TILE_SIZE / distance) * distToProjPlane;
        float middle = windowHeight/2.0;

        float bottom = middle - (projDrawHeight/2.0);
        float top = middle + (projDrawHeight/2.0);

        bottom = bottom < 0 ? 0 : bottom;
        bottom = bottom > windowHeight ? windowHeight : bottom;

        top = top < 0 ? 0 : top;
        top = top > windowHeight ? windowHeight : top;
        
        for (int y = bottom; y < top; y++)
        {
            raycast_pixels[c + (y * raycast_surface->w)] = color;
        }
                
    }

    SDL_BlitSurface(raycast_surface, 0, window_surface, 0);
    SDL_UpdateWindowSurface(window);

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