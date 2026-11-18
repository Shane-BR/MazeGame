#include "texture.h"
#include "stb_image.h"
#include <SDL3/SDL_log.h>
#include <stdlib.h>

struct Texture
{
    int width, height, channels;
    Color *pixels;
};

Texture *createTexture(const char *filepath)
{
    Texture *texture = malloc(sizeof(*texture)); // TODO Arena

    if (texture == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to allocate memory for texture");
        return NULL;
    }

    texture->pixels = (Color *)stbi_load(filepath, &texture->width, &texture->height, &texture->channels, 4);

    if (texture->pixels == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "stb_image failed to load texture.");
        return NULL;
    }

    return texture;
}

bool destroyTexture(Texture *texture)
{
    if (texture == NULL)
        return false;

    stbi_image_free(texture->pixels);
    free(texture);
    return true;
}

int getTextureWidth(const Texture *texture)
{
    if (texture == NULL)
        return -1;

    return texture->width;
}

int getTextureHeight(const Texture *texture)
{
    if (texture == NULL)
        return -1;

    return texture->height;
}

Color getTexturePixel(const Texture *texture, int x, int y)
{
    return texture->pixels[x + y*texture->width];
}