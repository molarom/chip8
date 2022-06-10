#include "chip8_sdl.h"
#include "chip8_mem.h"
#include "chip8_cpu.h"

int main(int argc, char **argv)
{
	int debug = 0;
	int fileLoaded = 0;

	// SDL setup functions.
	/*
    InitSetup();
	GameLoop();
	FinishOff();
	*/

	/* Library Function testing.
	*/
	chip8_mem *mem;

	mem = (chip8_mem *) malloc(sizeof(chip8_mem));

	if (mem != NULL)
		initialize(debug, mem);

	fileLoaded = loadGame(argv[1], mem);

	if (fileLoaded == 0)
		emulateCycle(mem);
	

	free(mem);

	return 0;
}