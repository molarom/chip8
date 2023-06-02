/*
*   Simple CHIP-8 emulator
*/
#include <stdlib.h>
#include <strings.h>
#include <SDL2/SDL_events.h>

#include "chip8_sdl.h"
#include "chip8_mem.h"
#include "chip8_cpu.h"

int main(int argc, char **argv)
{
    SDL_Event event; 
	int debug_flag = 0;
	int quit = 0;

	if (argc > 2) {
		if (strncmp(argv[2], "--debug", 7) == 0) {
			debug_flag = 1;
		} else {
			printf("usage: ./chip8 <rom.ch8> --debug\n");
			exit(-1);
		}
	}

	chip8_screen *screen = (chip8_screen *) malloc(sizeof(chip8_screen));
	chip8_mem mem = malloc(sizeof(struct memory));    
	chip8_opcode opcode = malloc(sizeof(struct opcode));

	sdl_init(screen);
	init_memory(debug_flag, mem);

	int file_err = load_game(argv[1], mem);
	if(file_err) {
		sdl_teardown(screen);
		exit(-1);
	}

	while (!quit) {
        cpu_cycle(debug_flag, mem, opcode);
        tick(mem);

        if (mem->draw_flag) {
            sdl_draw(mem->gfx, screen);
        }
        mem->draw_flag = 0;
        while ( SDL_PollEvent(&event) != 0 ) { 
            if (event.type == SDL_KEYDOWN) {
                mem->key = event.key.keysym.sym;
            }
            if (event.type == SDL_KEYUP) {
                mem->key = 0;
            }
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
        SDL_Delay(10);
	}

	sdl_teardown(screen);
	free(mem);
	free(opcode);

	return 0;
}

