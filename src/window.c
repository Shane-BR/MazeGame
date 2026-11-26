#include "window.h"
#include "color.h"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_mouse.h>
#include <stdint.h>
#include <stdlib.h>

static inline bool in_window(Window *window, int x, int y)
{
    return x >= 0 && x < window->width && y >= 0 && y < window->height;
}

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

    window->pixels = SDL_GetWindowSurface(window->sdl_window)->pixels;

    // TODO temp: make an option
    SDL_SetWindowRelativeMouseMode(window->sdl_window, true);

    return window;
}

void destroyWindow(Window* window)
{
    SDL_DestroyWindow(window->sdl_window);
    free(window); // TODO Arena
}

void resizeWindow(Window* window, int n_width, int n_height)
{
    // TODO
}

void putPixel(Window *window, Color pixel_color, int x, int y)
{
    ((Color*)window->pixels)[x + y*window->width] = pixel_color;
}

void updateWindow(Window *window)
{
    SDL_UpdateWindowSurface(window->sdl_window);
}