#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface *surface;
bool stop;

void gameLoop(void);
void draw(void);

int main(void)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                    "SDL_Init failed %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Maze Game", 640, 480, 0);
    if (!window)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
                    "Window creation failed %s\n", SDL_GetError());
        return 1;
    }

    surface = SDL_GetWindowSurface(window);
    renderer = SDL_CreateSoftwareRenderer(surface);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Render creation for surface fail : %s\n", SDL_GetError());
        return 1;
    }

    stop = false;

    while (!stop)
    {
        gameLoop();
    }

    SDL_Quit();
    return 0;
}

void draw(void)
{
    SDL_FRect obj;
    SDL_Rect s_area;

    SDL_GetRenderViewport(renderer, &s_area);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0x0F, 0x0F, 0x0F, 0xFF);

    obj.w = s_area.w/2.0;
    obj.h = s_area.h/2.0;
    obj.x = 0;
    obj.y = 0;

    SDL_RenderFillRect(renderer, &obj);

    SDL_RenderPresent(renderer);

}

void gameLoop(void)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_EVENT_QUIT)
        {
            stop = true;
            return;
        }

        if ((e.type == SDL_EVENT_KEY_DOWN) && (e.key.key == SDLK_ESCAPE)) 
        {
            stop = true;
            return;
        }
    }

    // Render
    draw();

    // Update window
    SDL_UpdateWindowSurface(window);
}