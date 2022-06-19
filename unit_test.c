#include "chip8_sdl.h"
#include "chip8_mem.h"
#include "chip8_cpu.h"

int main(int argc, char **argv)
{
	SDL_Event event;

	int keypressed;
	int debugFlag = 0;
	int file_err = 0;
	int sdl_err = 0;
	int quit = 0;

	// Library Function testing.

	chip8_screen *screen = (chip8_screen *) malloc(sizeof(chip8_screen));

	sdl_init(screen);

	chip8_mem *mem;    
	chip8_opcode *opcode;
	mem = (chip8_mem *) malloc(sizeof(chip8_mem));
	opcode = (chip8_opcode *)(malloc(sizeof(chip8_opcode)));


	if (mem != NULL)
		init_memory(debugFlag, mem);

	file_err = load_game(argv[1], mem);
	if(file_err)
		exit(-1);

	/* SDL Test data.
	uint8_t pixel_junk[WIDTH * HEIGHT * SCALE] = {0};

	memset(pixel_junk, 0xFF, sizeof pixel_junk);
	*/

	while (!quit){
    	while (SDL_PollEvent(&event)){
    	    if (event.type == SDL_QUIT){
    	        quit = 1;
    	    }
			//debugFlag = 1;
			cpu_cycle(debugFlag, mem, opcode);

			if(draw_flag) {
				sdl_draw(gfx, screen);
			}

			draw_flag = 0;
		}
	}

	sdl_teardown(screen);
	free(mem);
	free(opcode);

	return 0;
}

