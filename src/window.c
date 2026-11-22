#include "window.h"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_mouse.h>
#include <stdint.h>
#include <stdlib.h>

Window *createWindow(int width, int height, const char* title)
{
    Window* window = malloc(sizeof(*window)); // TODO Arena

    if (window == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to allocate memory for window");
        return NULL;
    }

    window->sdl_window = SDL_CreateWindow(title, width, height, 0);

    if (!window->sdl_window)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                    "Window creation failed %s\n", SDL_GetError());
        return NULL;
    }

    window->width = width;
    window->height = height;

    // TODO temp: make an option
    SDL_HideCursor();
    SDL_SetWindowMouseGrab(window->sdl_window, true);

    return window;
}

void destroyWindow(Window* window)
{
    SDL_DestroyWindow(window->sdl_window);
    free(window); // TODO Arena
}

SDL_Surface *getSurface(const Window *window)
{
    return SDL_GetWindowSurface(window->sdl_window);
}

void resizeWindow(Window* window, int n_width, int n_height)
{
    // TODO
}

void updateWindow(Window *window)
{
    SDL_UpdateWindowSurface(window->sdl_window);
}