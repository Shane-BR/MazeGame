#ifndef MG_TEXTURE_H
#define MG_TEXTURE_H

#include "color.h"
#include <stdbool.h>

typedef struct Texture Texture;

Texture *createTexture(const char *filepath);
bool destroyTexture(Texture *texture);
int getTextureWidth(const Texture *texture);
int getTextureHeight(const Texture *texture);
Color getTexturePixel(const Texture *texture, int x, int y);

#endif