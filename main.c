/*
*   Simple CHIP-8 emulator
*/
#include "cpu_lib.h"
#include "chip8_sdl.h"

/*  TODO
*   Setup SDL and get graphics drawn to screen.
*   
*   
*   
*/

// Template for main loop.

int main (int argc, char **argv)
{
    // Set up render system and register input callback.
    setupGraphics(argv[1]);
    setupInput();

    // Initialize the Chip8 system and load the game.
    initialize();
    loadGame(argv[1]);

    // Emulation loop
    for(;;){
        // Emulate one cycle
        emulateCycle();

        // If the draw flag is set, update the screen
        if(drawFlag)
            drawGraphics();

        // Store key press state (Press and Release)
        setKeys();
        
    }

    return 0;
}