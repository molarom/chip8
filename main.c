/*
*   Simple CHIP-8 emulator
*/
#include "chip8_sdl.h"
#include "chip8_mem.h"
#include "chip8_cpu.h"

int main(int argc, char **argv)
{
	SDL_Event event;

	int debugFlag = 0;
	int file_err = 0;
	int quit = 0;

	chip8_screen *screen = (chip8_screen *) malloc(sizeof(chip8_screen));

	sdl_init(screen);

	chip8_mem *mem;    
	chip8_opcode *opcode;
	mem = (chip8_mem *) malloc(sizeof(chip8_mem));
	opcode = (chip8_opcode *)(malloc(sizeof(chip8_opcode)));


	if (mem != NULL)
		init_memory(debugFlag, mem);

	file_err = load_game(argv[1], mem);
	if(file_err) {
		sdl_teardown(screen);
		exit(-1);
	}

	while (!quit){
    	if (SDL_PollEvent(&event)){
    	    if (event.type == SDL_QUIT){
    	        quit = 1;
    	    }
		}

		cpu_cycle(debugFlag, mem, opcode);
		tick(mem);

		if(mem->draw_flag) {
			sdl_draw(mem->gfx, screen);
		}
		mem->draw_flag = 0;

		SDL_Delay(20);
	}

	sdl_teardown(screen);
	free(mem);
	free(opcode);

	return 0;
}