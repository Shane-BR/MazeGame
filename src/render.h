#ifndef MG_RENDER_H
#define MG_RENDER_H

#include "texture.h"
#include "window.h"
#include "color.h"
#include <stdbool.h>

typedef struct Renderer Renderer;

Renderer *createRenderer(int width, int height, const Window* window_target);
bool destroyRenderer(Renderer *renderer);
bool clearRenderer(Renderer *renderer);
bool drawVertLine(Renderer *renderer, Color color, int y1, int y2, int x);
bool drawTexturedVertLine(  Renderer *renderer, Texture *texture, int texture_offset, 
                            int y1, int y2, int x);
bool drawRect(  Renderer *renderer, Color color,
                int x, int y, 
                int width, int height);

void updateRenderer(Renderer *renderer, Window *window);


#endif