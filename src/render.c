#include "render.h"
#include "texture.h"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_surface.h>
#include <stdint.h>
#include <stdlib.h>

static inline uint32_t *rendererPixelAt(Renderer *renderer, int x, int y)
{
    return &(renderer->pixels[x + y*renderer->width]);
}

Renderer *createRenderer(int width, int height, const Window* window_target)
{

    Renderer *renderer = malloc(sizeof(*renderer)); // TODO Arena

    if (renderer == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
            "createRenderer: Failed to allocate memory for renderer");
        return NULL;
    }

    renderer->width = width;
    renderer->height = height;    
    
    renderer->target_surface = getSurface(window_target);
    renderer->src_surface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);

    if (renderer->src_surface == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
            "Failed to create renderer surface: %s", SDL_GetError());
        return NULL;
    }

    renderer->pixels = (Color*)(renderer->src_surface->pixels);

    return renderer;

}

void destroyRenderer(Renderer *renderer)
{
    SDL_DestroySurface(renderer->src_surface);
    free(renderer);
}

void clearRenderer(Renderer *renderer)
{
    SDL_ClearSurface(renderer->src_surface, 0, 0, 0, 1.0);
}

void drawVertLine(Renderer *renderer, Color color, int y1, int y2, int x)
{

    if (y1 > y2)
    {
        int temp = y1;
        y1 = y2;
        y2 = temp;
    }

    for (int y = y1; y <= y2; y ++)
    {
        // Only start drawing when on the rendering surface
        if (y < 0 || y >= renderer->height)
            continue;

        *rendererPixelAt(renderer, x, y) = color;
    }
}

void drawTexturedVertLine(Renderer *renderer, int texture_id, int texture_xoffset, int y1, int y2, int x)
{

    if (y1 == y2)
        return;

    if (y1 > y2)
    {
        int temp = y1;
        y1 = y2;
        y2 = temp;
    }

    // Return false if line will never enter the screen
    if (y2 < 0 || y1 > renderer->height)
        return;

    for (int y = y1; y <= y2; y++)
    {
        // Only start drawing when on the rendering surface
        if (y < 0 || y >= renderer->height)
            continue;

        // Texture y-coord
        Texture *texture = getTexture(texture_id);
        int tex_max_y = texture->height - 1;
        int tex_y = (y-y1) * ((float)(tex_max_y)/(y2-y1));

        /*
            TODO Sort out cache miss on getting texture pixel.
            We are looping through each row, as textures are stored as y rows of x. 
            By reversing this, We may speed up the renderer.
            Actually, there may be a cache miss in both of these functions.
        */
        *rendererPixelAt(renderer, x, y) = getTexturePixel(texture, texture_xoffset, tex_y);
    }
}

void drawHorLine(Renderer *renderer, Color color, int x1, int x2, int y)
{

    if (x1 > x2)
    {
        int temp = x1;
        x1 = x2;
        x2 = temp;
    }

    for (int x = x1; x <= x2; x ++)
    {
        // Only start drawing when on the rendering surface
        if (x < 0 || x >= renderer->width)
            continue;

        *rendererPixelAt(renderer, x, y) = color;
    }
}

// TODO allow to choose which direction the texture is sampled
void drawTexturedHorLine(Renderer *renderer, int texture_id, int texture_xoffset, int x1, int x2, int y)
{

    if (x1 == x2)
        return;

    if (x1 > x2)
    {
        int temp = x1;
        x1 = x2;
        x2 = temp;
    }

    // Return false if line will never enter the screen
    if (x2 < 0 || x1 > renderer->width)
        return;

    for (int x = x1; x <= x2; x++)
    {
        // Only start drawing when on the rendering surface
        if (x < 0 || x >= renderer->width)
            continue;

        // Texture y-coord (Flipped)
        Texture *texture = getTexture(texture_id);
        int tex_max_y = texture->height - 1;
        int tex_y = (x-x1) * ((float)(tex_max_y)/(x2-x1));

        *rendererPixelAt(renderer, x, y) = getTexturePixel(texture, texture_xoffset, tex_y);
    }
}

void drawTextureSampledPixel(Renderer *renderer, int texture_id, 
    int tex_x, int tex_y, int x, int y)
{
    *rendererPixelAt(renderer, x, y) = 
        getTexturePixel(getTexture(texture_id), tex_x, tex_y);
}

void drawRect(  Renderer *renderer, Color color,
                int x, int y, 
                int width, int height)
{

    // TODO handle negative volume

    // If rect is never in view, do not draw it
    if 
    ( 
        (x + width < 0 || y + height < 0) &&
        (x > renderer->width || y > renderer->height)
    )
        return;

    for (int i = y; i < y+height; i++)
    {
        // Only draw pixels in view
        if (i < 0)
            continue;

        for (int j = x; j < x+width; j++)
        {
            if (j < 0)
                continue;

            *rendererPixelAt(renderer, j, i) = color;
        }
    }
}

void updateRenderer(Renderer *renderer, Window *window)
{
    if (renderer == NULL || window == NULL)
        return;


    if (!SDL_BlitSurface(renderer->src_surface, 0, 
        renderer->target_surface, 0))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
            "updateRenderer: Error blitting surface: %s", SDL_GetError());
    }

    updateWindow(window);
}