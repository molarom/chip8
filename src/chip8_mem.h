#ifndef CHIP8_MEM_H_
#define CHIP8_MEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define WIDTH 64
#define HEIGHT 32

/*------------------------------------------------------------------------/
/ * Struct to establish values for CHIP 8's memory.
/------------------------------------------------------------------------*/

typedef struct {
    uint8_t RAM[4096];
    uint16_t STACK[16];
    uint8_t SP;
    uint8_t V[16];
    uint16_t I;
    uint16_t PC;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint32_t gfx[WIDTH * HEIGHT];
    uint8_t draw_flag;

} chip8_mem;

/*------------------------------------------------------------------------/
/ * Struct to assist with OPCODE processing.
/------------------------------------------------------------------------*/

typedef struct {
    uint16_t OPCODE;
    uint16_t DECODED;
    uint16_t ADDR;
    uint16_t VX;
    uint16_t VY;
    uint16_t NN;
    uint16_t N;
} chip8_opcode;

/*------------------------------------------------------------------------/
/ * Function prototypes and related definitions.
/------------------------------------------------------------------------*/

// Initialize our memory values on "boot".
int init_memory(int, chip8_mem *);

// Load our ROM into the memory buffer. Must run after initialize.
int load_game(char *, chip8_mem *);

// Fetch, Decode, Execute every opcode from the ROM loaded into memory.
int cpu_cycle(int, chip8_mem *, chip8_opcode *);

// Print live memory information, suppresses all other output currently.
void debugMem(chip8_mem *, chip8_opcode *);

#endif

