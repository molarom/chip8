#ifndef CHIP8_SDL_H_
#define CHIP8_SDL_H_

#include <SDL2/SDL.h>

#define QUITKEY SDLK_ESCAPE
#define WIDTH 64
#define HEIGHT 32
/*------------------------------------------------------------------------/
/ * Struct to establish values for generic CHIP 8 display output.
/------------------------------------------------------------------------*/


// TODO

SDL_Event event;
SDL_Renderer *renderer;
SDL_Window *window;
/*
typedef struct {
    
    uint8_t gfx[64 * 32];
}
*/

// Output err to screen.
void LogError(char*);

// Can change the window caption based on state.
void SetCaption(char*);

// Clean up SDL.
void FinishOff();

#endif