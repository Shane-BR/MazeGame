#include "window.h"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_video.h>
#include <stdint.h>
#include <stdlib.h>

struct Window
{
    int width, height;
    SDL_Window* sdl_window;
};

Window *createWindow(int width, int height, const char* title)
{
    Window* window = malloc(sizeof(*window)); // TODO Arena
    window->sdl_window = SDL_CreateWindow(title, width, height, 0);
    window->width = width;
    window->height = height;
    if (!window->sdl_window)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                    "Window creation failed %s\n", SDL_GetError());
        return NULL;
    }

    return window;
}

void destroyWindow(Window* window)
{
    if (window == NULL)
        return;

    SDL_DestroyWindow(window->sdl_window);
    free(window); // TODO Arena
}

int getWindowWidth(Window* window)
{
    if (window == NULL)
        return 0;

    return window->width;
}

int getWindowHeight(Window* window)
{
    if (window == NULL)
        return 0;

    return window->height;
}

SDL_Surface *getSurface(Window *window)
{
    return SDL_GetWindowSurface(window->sdl_window);
}

bool resizeWindow(Window* window, int n_width, int n_height)
{
    if (window == NULL)
        return false;

    return true; // TODO
}

void updateWindow(Window *window)
{
    SDL_UpdateWindowSurface(window->sdl_window);
}