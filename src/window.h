#ifndef MG_WINDOW_H
#define MG_WINDOW_H

#include "color.h"
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <stdbool.h>

typedef struct
{
    int width, height;
    SDL_Window *sdl_window;
    void *pixels;
} Window;

Window *createWindow(int width, int height, const char* title);
void freeWindow(Window* window);
void resizeWindow(Window* window, int n_width, int n_height);
void updateWindow(Window *window);
void putPixel(Window *window, ARGB32 pixel_color, int x, int y);

#endif