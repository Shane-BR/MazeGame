#include "render.h"
#include <SDL3/SDL_surface.h>
#include <stdint.h>
#include <stdlib.h>

struct Renderer
{
    int width, height;
    uint32_t *pixels;
    SDL_Surface *target_surface;
    SDL_Surface *src_surface;
};

static inline uint32_t *getPixel(Renderer *renderer, int x, int y)
{
    return &(renderer->pixels[x + y*renderer->width]);
}

Renderer *createRenderer(int width, int height, Window* window_target)
{

    if (window_target == NULL)
        return NULL;

    Renderer *renderer = malloc(sizeof(*renderer)); // TODO Arena
    renderer->width = width;
    renderer->height = height;    
    
    renderer->target_surface = getSurface(window_target);
    renderer->src_surface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);
    renderer->pixels = (uint32_t*)(renderer->src_surface->pixels);

    return renderer;

}

bool destroyRenderer(Renderer *renderer)
{
    if (renderer == NULL)
        return false;

    SDL_DestroySurface(renderer->src_surface);
    free(renderer);
    return true;
}

bool clearRenderer(Renderer *renderer)
{
    if (renderer == NULL)
        return false;

    SDL_ClearSurface(renderer->src_surface, 0, 0, 0, 1.0);

    return true;
    
}

bool drawVertLine(Renderer *renderer, Color color, int y1, int y2, int x)
{
    if (renderer == NULL)
        return false;

    int iteration_dir = 1;
    if (y1 > y2) iteration_dir = -1;

    // Return false if line will never enter the screen
    if (iteration_dir == -1 && y1 < 0)
        return false;
    
    if (iteration_dir == 1 && y1 > renderer->height)
        return false;

    for (int y = y1; y != y2; y += iteration_dir)
    {
        // Only start drawing when on the rendering surface
        if (y < 0 || y > renderer->height)
            continue;

        *getPixel(renderer, x, y) = color;
    }

    return true;
}

bool drawRect(  Renderer *renderer, Color color,
                int x, int y, 
                int width, int height)
{
    if (renderer == NULL)
        return false;

    // TODO handle negative volume

    // If rect is never in view, do not draw it
    if 
    ( 
        (x + width < 0 || y + height < 0) &&
        (x > renderer->width || y > renderer->height)
    )
        return false;

    for (int i = y; i < y+height; i++)
    {
        // Only draw pixels in view
        if (i < 0)
            continue;

        for (int j = x; j < x+width; j++)
        {
            if (j < 0)
                continue;

            *getPixel(renderer, j, i) = color;
        }
    }

    return true;
}

void updateRenderer(Renderer *renderer, Window *window)
{
    if (renderer == NULL || window == NULL)
        return;

    SDL_BlitSurface(renderer->src_surface, 0, renderer->target_surface, 0);
    updateWindow(window);
}