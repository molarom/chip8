/*
*   Simple CHIP-8 emulator
*/
#include <strings.h>
#include "chip8_sdl.h"
#include "chip8_mem.h"
#include "chip8_cpu.h"

int main(int argc, char **argv)
{
	SDL_Event event;
	int debug_flag = 0;
	int quit = 0;

	uint32_t start_time = 0;
	uint32_t end_time = 0;
	uint32_t delta = 0;
	int time_per_frame = 8; // miliseconds. 1/60th ~= 16

	if (argc > 2) {
		if (strncmp(argv[2], "--debug", 7) == 0) {
			debug_flag = 1;
		} else {
			printf("usage: ./chip8 <rom.ch8> --debug\n");
			exit(-1);
		}
	}

	chip8_screen *screen = (chip8_screen *) malloc(sizeof(chip8_screen));

	sdl_init(screen);

	chip8_mem *mem;    
	chip8_opcode *opcode;
	mem = (chip8_mem *) malloc(sizeof(chip8_mem));
	opcode = (chip8_opcode *)(malloc(sizeof(chip8_opcode)));


	if (mem != NULL)
		init_memory(debug_flag, mem);

	int file_err = load_game(argv[1], mem);
	if(file_err) {
		sdl_teardown(screen);
		exit(-1);
	}

	while (!quit){
		// Get ms passed since start.
		if (!start_time){
			start_time = SDL_GetTicks();
		} else {
			delta = end_time - start_time;
		}

    	if (SDL_PollEvent(&event)){
    	    if (event.type == SDL_QUIT){
    	        quit = 1;
    	    }
		}

		cpu_cycle(debug_flag, mem, opcode);
		tick(mem);

		if(mem->draw_flag) {
			sdl_draw(mem->gfx, screen);
		}
		mem->draw_flag = 0;

		if (delta < time_per_frame) SDL_Delay(time_per_frame - delta);
		start_time = end_time;
		end_time = SDL_GetTicks();
	}

	sdl_teardown(screen);
	free(mem);
	free(opcode);

	return 0;
}

