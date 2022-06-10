#include "chip8_mem.h"
#include "chip8_cpu.h"

#include <unistd.h>

// Chip 8 fontset. Each number or character is 4 pixels wide and 5 pixels high.
uint8_t chip8_fontset[80] = { 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

// Array to hold function pointers for opcode processing functions.
opcode_func subroutines[16] = {
    &subroutine,                //0XXX
    &jump_to,                   //1XXX
    &call,                      //2XXX
    &eval_ne_uchar,             //3XXX
    &eval_eq,                   //4XXX
    &eval_ne_reg,               //5XXX
    &set_val_to_reg,            //6XXX
    &add_val_to_reg,            //7XXX
    &reg_sub_func,              //8XXX
    &eval_eq_reg,               //9XXX
    &set_index_reg,             //AXXX
    &jump_add_offset,           //BXXX
    &random_val,                //CXXX
    &draw,                      //DXXX
    &keypress,                  //EXXX
    &subfunc_ex                 //FXXX
};

int initialize(int debug, chip8_mem *memory)
{
    memory->PC      = 0x200;              // Program counter starts at 0x200.
    memory->I       = 0x000;                  // Reset index register.
    memory->SP      = STACK_EMPTY;        // Reset stack pointer.

    // TODO: Clear display
    memset(memory->STACK, 0, sizeof memory->STACK);     // Clear stack
    memset(memory->V, 0, sizeof memory->V);             // Clear registers V0-VF
    memset(memory->RAM, 0, sizeof memory->RAM);         // Clear memory


    // Load fontset
    for (int i = 0; i < 0x50; ++i)
        memory->RAM[i] = chip8_fontset[i];

    // Reset timers
    memory->delay_timer = 0;
    memory->sound_timer = 0;

    if (debug == 1) {
        // Verify the fontset was properly loaded.
        for (int i = 0; i < 0x50; i++) {
            printf("Memory[%i]: %02x\n", i, memory->RAM[i]);
        }

    }

    return 0;
}

int loadGame(char *game, chip8_mem *memory)
{
    FILE * fp;

    printf("Loading file: %s\n", game);
    fp = fopen(game, "rb");
    if (fp == NULL) {
        printf("Error loading file\n");
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    int fSize = ftell(fp);
    rewind(fp);

    // Load our program into memory starting at 0x200 == 512.
    uint8_t *buffer = malloc(fSize);

    if (buffer != NULL){
        fread(buffer, fSize, 1, fp);

        for(int i = 0; i < fSize; ++i) {
            memory->RAM[i + 512] = buffer[i];
        }
    }

    fclose(fp);
    free(buffer);

    return 0;
}

int emulateCycle(chip8_mem *memory)
{
    chip8_opcode *opcode_digits;

    opcode_digits = (chip8_opcode *)(malloc(sizeof(chip8_opcode)));

    if (opcode_digits != NULL) {
        for (;;){
            opcode_digits->OPCODE = memory->RAM[memory->PC];
            opcode_digits->OPCODE <<= 8;
            opcode_digits->OPCODE |= memory->RAM[memory->PC + 1];

            // Split up our OPCODE into usable values.
            opcode_digits->ADDR = (opcode_digits->OPCODE) & 0x0FFF;
            opcode_digits->VX = (opcode_digits->OPCODE >> 8) & 0xF; 
            opcode_digits->VY = (opcode_digits->OPCODE >> 4) & 0xF;
            opcode_digits->N = (opcode_digits->OPCODE) & 0xF;
            opcode_digits->NN = (opcode_digits->N) | (opcode_digits->VY) << 4;
            opcode_digits->DECODED = ((opcode_digits->OPCODE) & 0xF000) >> 12;

            memory->PC += 2;

            subroutines[opcode_digits->DECODED](memory, opcode_digits);
        }

    } else {
        printf("failed to initialized memory for opcode_digits.");
        return 1;
    }

    free(opcode_digits);
    return 0;
}

void Debug (chip8_mem *memory, chip8_opcode *opcode_digits) {
    printf("----------OPCODES------------\n");
    printf("OPCODE: %04x \n", opcode_digits->OPCODE);
    printf("DECODED: %04x \n", opcode_digits->DECODED);
    printf("PC: %04x \n", memory->PC);
    printf("\n---------REGISTERS-----------\n");
    printf("V[0]: %04x\n", memory->V[0]);
    printf("V[1]: %04x\n", memory->V[1]);
    printf("V[2]: %04x\n", memory->V[2]);
    printf("V[3]: %04x\n", memory->V[3]);
    printf("V[4]: %04x\n", memory->V[4]);
    printf("V[15]: %04x\n", memory->V[15]);
    printf("I: %04x\n", memory->I);
    printf("\n-----------STACK------------\n");
    printf("STACK[0]: %04x\n", memory->STACK[0]);
    printf("SP: %04x\n", memory->SP);

    // ANSI escape sequence to update in place.
    printf("\033[17F");
    usleep(1000000);
}