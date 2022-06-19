#ifndef CHIP8_SDL_H_
#define CHIP8_SDL_H_

#include <SDL2/SDL.h>

/*------------------------------------------------------------------------/
/ * Establish globals for SDL constants, and graphics.
/------------------------------------------------------------------------*/

#define QUITKEY SDLK_ESCAPE
#define WIDTH 64
#define HEIGHT 32
#define SCALE 10
// #define SDL_INIT_CHIP8 ( SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_AUDIO )

typedef struct {
    SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
} chip8_screen;

uint8_t old_gfx[WIDTH * HEIGHT * SCALE];
uint8_t gfx[WIDTH * HEIGHT * SCALE];
uint8_t draw_flag;

// Initialize SDL window and texture surface.
int sdl_init (chip8_screen *chip8_screen);

void sdl_draw (uint8_t *, chip8_screen *chip8_screen);

// Can change the window caption based on state.
void SetCaption(char*);

// Clean up SDL.
void sdl_teardown(chip8_screen *chip8_screen);

#endif