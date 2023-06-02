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
#define SDL_INIT_CHIP8 ( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS )

// ------------------------------------------------------------------------
// Structs to set up SDL for video output, and to map keypresses to fit 
// CHIP8's keypad.
// ------------------------------------------------------------------------

typedef struct {
    SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
} chip8_screen;

// ------------------------------------------------------------------------
// Function prototypes.
// ------------------------------------------------------------------------

// Initialize SDL window and texture surface.
int sdl_init (chip8_screen *chip8_screen);

// Update texture with pixels and render the texture to the screen.
void sdl_draw (uint32_t *, chip8_screen *chip8_screen);

// Free the memory allocated to SDL components.
void sdl_teardown(chip8_screen *chip8_screen);

#endif

