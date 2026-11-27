#ifndef MG_TEXTURE_H
#define MG_TEXTURE_H

#include "color.h"
#include <stdbool.h>

typedef struct
{
    int width, height, channels;
    Color *pixels;
} Texture;

Texture *registerTexture(const char *filepath, int id, int format);
void freeTexture(Texture *texture);
Color getTexturePixel(const Texture *texture, int x, int y);
Texture *getTexture(int id);

#endif