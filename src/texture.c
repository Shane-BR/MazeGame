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
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                    "createTexture: Failed to allocate memory for texture at %s", filepath);
        return NULL;
    }

    texture->pixels = (Color *)stbi_load(filepath, 
        &texture->width, &texture->height, &texture->channels, STBI_rgb_alpha);

    if (texture->pixels == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "stb_image failed to load texture.");
        return NULL;
    }

    return texture;
}

void destroyTexture(Texture *texture)
{
    stbi_image_free(texture->pixels);
    free(texture);
}

int getTextureWidth(const Texture *texture)
{

    return texture->width;
}

int getTextureHeight(const Texture *texture)
{
    return texture->height;
}

Color getTexturePixel(const Texture *texture, int x, int y)
{
    if (x < 0 || x >= texture->width || y < 0 || y >= texture->height)
    {
        SDL_LogError(
                    SDL_LOG_CATEGORY_APPLICATION, 
                    "getTexturePixel: Texture coordinate (%i, %i) is out of bounds for texture of size %ix%i", 
                    x, y, texture->width, texture->height
                    );
        return 0x0;
    }

    return texture->pixels[x + y*texture->width];
}