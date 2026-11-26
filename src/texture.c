#include "texture.h"
#include "stb_image.h"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_surface.h>
#include <stdlib.h>

#define MAX_TEXTURES 32

typedef struct
{
    int count;
    Texture *textures[MAX_TEXTURES];
} TextureTable;

TextureTable table;

static void registerTexture(Texture *texture, int id);

Texture *createTexture(const char *filepath, int id, int format)
{
    Texture *texture = malloc(sizeof(*texture)); // TODO Arena

    if (texture == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                    "createTexture: Failed to allocate memory for texture struct at %s", filepath);
        return NULL;
    }

    Color *data = (Color *)stbi_load(filepath, 
        &texture->width, &texture->height, &texture->channels, STBI_rgb_alpha);

    if (data == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "createTexture: stb_image failed to load texture.");
        return NULL;
    }

    texture->pixels = malloc(sizeof(*texture->pixels)*texture->width*texture->height); // TODO Arena

    if (texture->pixels == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                    "createTexture: Failed to allocate memory for texture pixels %s", filepath);
        return NULL;
    }

    int src_pitch = texture->width * SDL_BYTESPERPIXEL(SDL_PIXELFORMAT_RGBA32);
    int dst_pitch = texture->width * SDL_BYTESPERPIXEL(format);

    SDL_ConvertPixels(texture->width, texture->height, SDL_PIXELFORMAT_RGBA32, 
        data, src_pitch, format, texture->pixels, dst_pitch);

    stbi_image_free(data);

    registerTexture(texture, id);

    return texture;
}

void destroyTexture(Texture *texture)
{
    free(texture->pixels);
    free(texture);
}

Color getTexturePixel(const Texture *texture, int x, int y)
{
    return texture->pixels[x + y*texture->width];
}

Texture *getTexture(int id)
{
    return table.textures[id];
}

static void registerTexture(Texture *texture, int id)
{
    table.textures[id] = texture;
    table.count++;
}