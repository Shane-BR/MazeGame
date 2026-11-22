#ifndef MG_RENDER_H
#define MG_RENDER_H

#include "window.h"
#include "color.h"
#include <stdbool.h>

typedef struct
{
    int width, height;
    uint32_t *pixels;
    SDL_Surface *target_surface;
    SDL_Surface *src_surface;
} Renderer;

Renderer *createRenderer(int width, int height, const Window* window_target);
void destroyRenderer(Renderer *renderer);
void clearRenderer(Renderer *renderer);
void drawVertLine(Renderer *renderer, Color color, int y1, int y2, int x);
void drawTexturedVertLine(  Renderer *renderer, int texture_id, int texture_offset, 
                            int y1, int y2, int x);
void drawTextureSampledPixel(Renderer *renderer, int texture_id, 
    int tex_x, int tex_y, int x, int y);
void drawRect(  Renderer *renderer, Color color,
                int x, int y, 
                int width, int height);

void updateRenderer(Renderer *renderer, Window *window);


#endif