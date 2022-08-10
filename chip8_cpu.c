/*------------------------------------------------------------------------/
/ * Contains functions which process CHIP8 OPCODEs.
/------------------------------------------------------------------------*/

#include <time.h>
#include "chip8_mem.h"
#include "chip8_cpu.h"
#include "chip8_sdl.h"

#define PIXEL_ON 0xFFFFFF
#define KEYPRESSED 0

void subroutine (chip8_mem *memory, chip8_opcode *opcode) 
{ 
    if (opcode->OPCODE == 0x00E0) {
		memset(memory->gfx, 0, sizeof memory->gfx );
    } else if (opcode->OPCODE == 0x00EE){
        memory->PC = pop(memory);
    }

 }

void jump_to (chip8_mem *memory, chip8_opcode *opcode) 
{ 
    memory->PC = opcode->ADDR;
}

void call (chip8_mem *memory, chip8_opcode *opcode) 
{
    int stk_rtn = 0;
    stk_rtn = push(memory);
    if(stk_rtn < 0){
        printf("STACK OVERFLOW\n");
        exit(-1);
    }

    memory->PC = opcode->ADDR;
}

void eval_eq_uchar (chip8_mem *memory, chip8_opcode *opcode) 
{
    if (memory->V[ opcode->VX ] == opcode->NN) {
        memory->PC += 2;
    }
}

void eval_ne (chip8_mem *memory, chip8_opcode *opcode) 
{
    if (memory->V[ opcode->VX ] != opcode->NN) {
        memory->PC += 2;
    }
}

void eval_eq_reg (chip8_mem *memory, chip8_opcode *opcode) 
{
    if (memory->V[ opcode->VX ] == memory->V[ opcode-> VY]) {
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
        memory->V[0xF] = 0;
        break;

    case 0x1: 
        memory->V[ opcode->VX ] |= memory->V[ opcode->VY ];
        memory->V[0xF] = 0;
        break;
    
    case 0x2: 
        memory->V[ opcode->VX ] &= memory->V[ opcode->VY ];
        memory->V[0xF] = 0;
        break;
    
    case 0x3: 
        memory->V[ opcode->VX ] ^= memory->V[ opcode->VY ]; 
        memory->V[0xF] = 0;
        break;
    
    case 0x4:
        overflow_chk = memory->V[ opcode->VX ] + memory->V[ opcode->VY ];

        if (overflow_chk > 255){
            memory->V[0xF] = 1;
        } else {
            memory->V[0xF] = 0;
        }

        memory->V[ opcode->VX ] = (uint8_t)overflow_chk;
        break;
    
    case 0x5:
        memory->V[ opcode->VX ] -= memory->V[ opcode->VY ];

        if(memory->V[ opcode->VX ] > memory->V[ opcode->VY ]){
            memory->V[0xF] = 1;
        } else {
            memory->V[0xF] = 0;
        }
          
        break;

    case 0x6: 
        memory->V[ opcode->VX ] = (memory->V[ opcode->VY ] >> 1);
        memory->V[0xF] = memory->V[ opcode->VY ] & 0x01 ? 1 : 0;

        break;

    case 0x7:
        memory->V[ opcode->VX ] = memory->V[ opcode->VY ] - memory->V[ opcode->VX ];

        if(memory->V[ opcode->VX ] < memory->V[ opcode->VY ]) {
            memory->V[0xF] = 1;
        } else {
            memory->V[0xF] = 0;
        }
        
        break;
    
    case 0xE:
        memory->V[ opcode->VX ] = (memory->V[ opcode->VY ] << 1);
        memory->V[0xF] = (memory->V[ opcode->VY ] >> 7) & 0x01 ? 1 : 0;

        break;
    
    default:
        break;
    }
}

void eval_ne_reg (chip8_mem *memory, chip8_opcode *opcode) 
{
    if ( memory->V[ opcode->VX ] != memory->V[opcode->VY] )
        memory->PC += 2; 
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
    memory->V[ opcode->VX ] = (rand() % 256) & opcode->NN;
}

void draw (chip8_mem *memory, chip8_opcode *opcode) 
{
    int x = (memory->V[ opcode->VX ]);
    int y = (memory->V[ opcode->VY ]);
    uint8_t pixel;
    memory->V[0xF] = 0;

    for (int y_line = 0; y_line < opcode->N ; y_line++) {
        pixel = memory->RAM[ memory->I + y_line ];

        for (int x_line = 0; x_line < 8; x_line++) {
            if((pixel & (0x80 >> x_line)) != 0) {
                int pixel_pos  = ((x + x_line) % WIDTH) + ((y + y_line) % HEIGHT) * WIDTH;
                
                if(memory->gfx[pixel_pos] == PIXEL_ON ){
                    memory->V[0xF] = 1;
                    memory->gfx[pixel_pos] = 0;
                } else {
                    memory->gfx[pixel_pos] = PIXEL_ON;
                }
            }
        }
    }
		
    memory->draw_flag = 1;
}

void keypress (chip8_mem *memory, chip8_opcode *opcode) {
    if (opcode->NN == 0x9E){
        if ( SDL_GetKeyboardState(NULL)[keypad[memory->V[ opcode->VX ]]] )
             memory->PC += 2;
    } else if (opcode->NN == 0xA1) {
        if ( !SDL_GetKeyboardState(NULL)[keypad[memory->V[ opcode->VX ]]] )
            memory->PC += 2;
    }
}

void subfunc_ex (chip8_mem *memory, chip8_opcode *opcode) {
    int result = 0;
    int i = 0;
    uint8_t key_flag = 0;

    switch (opcode->NN){
        case 0x07:
            memory->V[ opcode->VX ] = memory->delay_timer;
            break;
        case 0x0A:
            for (; i < 15; i++) {
                if (SDL_GetKeyboardState(NULL)[keypad[i]]){
                    key_flag = KEYPRESSED;
                    memory->V[ opcode->VX ] = i;
                    memory->PC += 2;
                } else {
                    key_flag = 1;
                }
            }

            if (key_flag == 1)
                memory->PC -= 2;
            
            break;
        case 0x15: memory->delay_timer = memory->V[ opcode->VX ]; break;
        case 0x18: memory->sound_timer = memory->V[ opcode->VX ]; break;
        case 0x1E:
            result = memory->I + (memory->V[ opcode->VX ]);
            if ( result > 0xFFF )
                memory->V[0xF] = 1;

            memory->I = (uint16_t)result;
            break;
        case 0x29: memory->I = 5 * memory->V[ opcode->VX ]; break;
        case 0x33: 
            memory->RAM[memory->I] = (memory->V[ opcode->VX ] % 1000) / 100;
            memory->RAM[memory->I + 1] = (memory->V[ opcode->VX ] % 100) / 10;
            memory->RAM[memory->I + 2] = (memory->V[ opcode->VX ] % 10);
            break;
        case 0x55: 
            for (uint16_t i = 0; i <= opcode->VX; i++) {
                memory->RAM[memory->I + i] = memory->V[i]; 
            }
            memory->I += opcode->VX + 1;

            break;
        case 0x65:
            for (uint16_t i = 0; i <= opcode->VX; i++) {
                memory->V[i] = memory->RAM[memory->I + i]; 
            }
            memory->I += opcode->VX + 1;

            break;
        default:
            break;
    }
}

void tick(chip8_mem *memory)
{
    if (memory->delay_timer > 0) {
        --memory->delay_timer;
    }

    if (memory->sound_timer > 0) {
        if (--memory->sound_timer == 0)
            fprintf(stdout,"\aBEEP!\n");
    }
}

/*------------------------------------------------------------------------/
/ * Functions for handling the stack, helps with readability.
/------------------------------------------------------------------------*/

int push(chip8_mem *memory)
{
    if (memory->SP > STACK_SIZE) return -1;

    memory->STACK[memory->SP] = memory->PC;
    memory->SP++;
    return 0;
}

uint16_t pop(chip8_mem *memory)
{
    if (memory->SP == STACK_EMPTY) return memory->PC;

    memory->SP--;
    uint16_t stk_inst = memory->STACK[memory->SP];
    return stk_inst;
}