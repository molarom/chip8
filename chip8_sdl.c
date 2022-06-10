#include "chip8_sdl.h"

int keypressed;

int sdl_main (int x, int y) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WIDTH,
        HEIGHT,
        0,
        &window,
        &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderDrawPoint(renderer, 0, 0);
    SDL_RenderPresent(renderer);

    if (SDL_PollEvent(&event) && (keypressed == QUITKEY))
        FinishOff();
    
}

void SetCaption(char *msg)
{
    SDL_SetWindowTitle(window, msg);
}

void FinishOff()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    exit(0);
}

