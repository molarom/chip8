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
                                            WIDTH * 10, HEIGHT * 10, 0);
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
/*
    // Create surface to write 8 bit values per pixel.
    chip8_screen->surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 8, 0, 0, 0, 0);
    if (!chip8_screen->surface) {
        printf("Couldn't create surface: %s", SDL_GetError());
        sdl_teardown(chip8_screen);
    }
*/
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

void sdl_draw (uint32_t pixels[], chip8_screen *chip8_screen)
{
    // Update the texture with the filled pixel array, then render the texture.
    SDL_UpdateTexture(chip8_screen->texture, NULL, pixels, WIDTH * sizeof(uint32_t));
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

