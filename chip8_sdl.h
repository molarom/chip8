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
#define SDL_INIT_CHIP8 ( SDL_INIT_VIDEO | SDL_INIT_AUDIO )

// ------------------------------------------------------------------------
// Structs to set up SDL for video output, and to map keypresses to fit 
// CHIP8's keypad.
// ------------------------------------------------------------------------

typedef struct {
    SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
} chip8_screen;

static SDL_Scancode keypad[16] = {
	SDL_SCANCODE_1,
	SDL_SCANCODE_2,
	SDL_SCANCODE_3,
	SDL_SCANCODE_4,
	SDL_SCANCODE_Q,
	SDL_SCANCODE_W,
	SDL_SCANCODE_E,
	SDL_SCANCODE_R,
	SDL_SCANCODE_A,
	SDL_SCANCODE_S,
	SDL_SCANCODE_D,
	SDL_SCANCODE_F,
	SDL_SCANCODE_Z,
	SDL_SCANCODE_X,
	SDL_SCANCODE_C,
	SDL_SCANCODE_V
};

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