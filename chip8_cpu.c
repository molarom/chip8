/*------------------------------------------------------------------------/
/ * Contains functions which process CHIP8 OPCODEs.
/------------------------------------------------------------------------*/

#include <time.h>
#include "chip8_mem.h"
#include "chip8_cpu.h"
#include "chip8_sdl.h" // old_gfx, gfx, and draw_flag defined here.

void subroutine (chip8_mem *memory, chip8_opcode *opcode) 
{ 
    if (opcode->OPCODE == 0x00E0) {
		memset(&gfx, 0, sizeof gfx );

    } else if (opcode->OPCODE == 0x00EE){
        memory->PC = pop(memory);
    }

 }

void jump_to (chip8_mem *memory, chip8_opcode *opcode) 
{ 
    memory->PC = opcode->ADDR;

    // Make sure our jmp to addr is correct.
    memory->PC -= 2; 
}

void call (chip8_mem *memory, chip8_opcode *opcode) 
{
    push(memory);
    memory->PC = opcode->ADDR;
}

void eval_ne_uchar (chip8_mem *memory, chip8_opcode *opcode) 
{
    if (memory->V[ opcode->VX ] != opcode->NN) {
        memory->PC += 2;
    }
}

void eval_eq (chip8_mem *memory, chip8_opcode *opcode) 
{
    if (memory->V[ opcode->VX ] == opcode->NN) {
        memory->PC += 2;
    }
}

void eval_ne_reg (chip8_mem *memory, chip8_opcode *opcode) 
{
    if (memory->V[ opcode->VX ] != memory->V[ opcode-> VY]) {
        memory->PC += 2;
    }

}

void set_val_to_reg (chip8_mem *memory, chip8_opcode *opcode) 
{
    memory->V[ opcode->VX ] = opcode->NN;
}

void add_val_to_reg (chip8_mem *memory, chip8_opcode *opcode) 
{
    memory->V[ opcode->VX ] += opcode->NN;
}

void reg_sub_func (chip8_mem *memory, chip8_opcode *opcode) 
{
    int overflow_chk = 0;

    switch (opcode->N)
    {
    case 0x0:
        memory->V[ opcode->VX ] = memory->V[ opcode->VY ];
        break;

    case 0x1:
        memory->V[ opcode->VX ] |= memory->V[ opcode->VY ];
        break;
    
    case 0x2:
        memory->V[ opcode->VX ] &= memory->V[ opcode->VY ];
        break;
    
    case 0x3:
        memory->V[ opcode->VX ] ^= memory->V[ opcode->VY ];
        break;
    
    case 0x4:
        overflow_chk = memory->V[ opcode->VX ] + memory->V[ opcode->VX ];

        if (overflow_chk > 255)
            memory->V[0xF] = 1;

        memory->V[ opcode->VX ] = (uint8_t)(overflow_chk & 0x000000FF);
        break;
    
    case 0x5:
        if(memory->V[ opcode->VX ] > memory->V[ opcode->VY ]){
            memory->V[0xF] = 1;
        } else {
            memory->V[0xF] = 0;
        }
        
        memory->V[ opcode->VX ] -= memory->V[ opcode->VY ];
        break;

    case 0x6:
        if( memory->V[ opcode->VX ] & 0x01 )
            memory->V[0xF] = 1;

        memory->V[ opcode->VX ] = (memory->V[ opcode->VX ]) >> 1;
        break;

    case 0x7:
        if(memory->V[ opcode->VX ] < memory->V[ opcode->VY ]) {
            memory->V[0xF] = 1;
        } else {
            memory->V[0xF] = 0;
        }
        
        memory->V[ opcode->VX ] = memory->V[ opcode->VY ] - memory->V[ opcode->VX ];
        break;
    
    case 0xE:
        if( memory->V[ opcode->VX ] & 0x01 )
            memory->V[0xF] = 1;

        memory->V[ opcode->VX ] = (memory->V[ opcode->VX ]) >> 1;
        break;
    
    default:
        break;
    }
}

void eval_eq_reg (chip8_mem *memory, chip8_opcode *opcode) 
{
    if ( memory->V[ opcode->VX ] == memory->V[opcode->VY] ) {
        memory->PC += 2;
    }  
}

void set_index_reg (chip8_mem *memory, chip8_opcode *opcode) 
{
    memory->I = opcode->ADDR;
}

void jump_add_offset (chip8_mem *memory, chip8_opcode *opcode) 
{
    memory->PC = opcode->ADDR + memory->V[opcode->VX];
    memory->PC--;
}

void random_val (chip8_mem *memory, chip8_opcode *opcode) 
{
    // Use current time as seed for random number.
    srand(time(0));

    opcode->NN = (rand() % 256) & 0xFF;
    memory->V[ opcode->VX ] = opcode->NN;
}

void draw (chip8_mem *memory, chip8_opcode *opcode) 
{
    int x = (opcode->VX) % 64;
    int y = (opcode->VY) % 32;
    uint8_t pixel;
    memory->V[0xF] = 0;

    for (int y_line = 0; y_line < opcode->N ; y_line++) {
        pixel = memory->RAM[ memory->I + y_line ];

        for (int x_line = 0; x_line < 8; x_line++) {
            int cur_pixel  = (x + x_line) + (y * y_line);
            if(pixel & (0x80 >> x)) { 
                if((gfx[cur_pixel] & 0xFF) == 1){
                    memory->V[0xF] = 1;
                    gfx[cur_pixel] ^= 0xFF;
                } else {
                    gfx[cur_pixel] = 0xFF;
                }
            }
        }
    }


		
    draw_flag = 1;
}

void keypress (chip8_mem *memory, chip8_opcode *opcode) {
    //TODO
}

void subfunc_ex (chip8_mem *memory, chip8_opcode *opcode) {
    //TODO
}

/*------------------------------------------------------------------------/
/ * Functions for handling the stack, helps with readability.
/------------------------------------------------------------------------*/

int push(chip8_mem *memory)
{
    if (memory->SP >= STACK_SIZE-1) return -1;

    memory->STACK[memory->SP] = memory->PC;
    memory->SP++;
    return 0;
}

uint16_t pop(chip8_mem *memory)
{
    if (memory->SP == STACK_EMPTY) return STACK_EMPTY;

    uint16_t stk_inst = memory->STACK[memory->SP];
    memory->SP--;
    return stk_inst;
}