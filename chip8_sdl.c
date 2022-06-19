#include "chip8_sdl.h"

int sdl_init (chip8_screen *chip8_screen) 
{
    // Initialize SDL video components only for now.
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        printf("Error initializing SDL: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
   
    // Create SDL window, multiplied range by 4 for testing.
    chip8_screen->window = SDL_CreateWindow("Chip8 Emulator", 
                                            SDL_WINDOWPOS_CENTERED, 
                                            SDL_WINDOWPOS_CENTERED, 
                                            WIDTH * SCALE, HEIGHT * SCALE, 0);
    if(!chip8_screen->window){
        printf("Couldn't create window: %s\n", SDL_GetError());
        sdl_teardown(chip8_screen);
    }

    // Create SDL renderer with hardware access and enable VSYNC.
    uint8_t render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    chip8_screen->renderer = SDL_CreateRenderer(chip8_screen->window, -1, render_flags);
    if(!chip8_screen->renderer){
        printf("Couldn't create renderer: %s\n", SDL_GetError());
        sdl_teardown(chip8_screen);
    }

    // Create texture with streaming access to modify pixels.
    chip8_screen->texture = SDL_CreateTexture(chip8_screen->renderer,
                                            SDL_PIXELFORMAT_ARGB8888,           
                                            SDL_TEXTUREACCESS_STREAMING,        
                                            WIDTH, HEIGHT);
    if (!chip8_screen->texture) {
        printf("Couldn't create texture: %s", SDL_GetError());
        sdl_teardown(chip8_screen);
    }

    return 0;
    
}

void sdl_draw (uint8_t *pixels, chip8_screen *chip8_screen)
{
    // texture_pitch: the height of the window to lock for writing.
    // texture_pixels: The pixel array we write to the screen.

    int texture_pitch = 0;
    void *texture_pixels = NULL;

    if(SDL_LockTexture(chip8_screen->texture, NULL, &texture_pixels, &texture_pitch) != 0){
        printf("Unable to lock texture for writing: %s\n", SDL_GetError());
    } else {
        memcpy (texture_pixels, pixels, texture_pitch * HEIGHT);
    }
    SDL_UnlockTexture(chip8_screen->texture);

    // Update the renderer with our new texture.
    SDL_RenderClear(chip8_screen->renderer);
    SDL_RenderCopy(chip8_screen->renderer, chip8_screen->texture, NULL, NULL);
    SDL_RenderPresent(chip8_screen->renderer);
}

void sdl_teardown(chip8_screen *chip8_screen)
{
    SDL_DestroyTexture(chip8_screen->texture);
    SDL_DestroyRenderer(chip8_screen->renderer);
    SDL_DestroyWindow(chip8_screen->window);

    SDL_Quit();
}

