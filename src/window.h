#ifndef MG_WINDOW_H
#define MG_WINDOW_H

#include <SDL3/SDL_surface.h>
#include <stdbool.h>

typedef struct Window Window;

Window *createWindow(int width, int height, const char* title);
void destroyWindow(Window* window);
int getWindowWidth(Window* window);
int getWindowHeight(Window* window);
bool resizeWindow(Window* window, int n_width, int n_height);
SDL_Surface *getSurface(Window *window);
void updateWindow(Window *window);

#endif