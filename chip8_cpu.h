#ifndef CHIP8_CPU_H_
#define CHIP8_CPU_H_

/*------------------------------------------------------------------------/
/ *  Typdef for function pointers for CHIP 8 OPCODE processing.
/------------------------------------------------------------------------*/

typedef void (*opcode_func)(chip8_mem *, chip8_opcode *);

/*------------------------------------------------------------------------/
/ * Function prototypes.
/------------------------------------------------------------------------*/

// 0XXX: CLR if 00E0. RTN from subroutine otherwise.
void subroutine (chip8_mem *memory, chip8_opcode *opcode);

// 1XXX: JMP to NNN.
void jump_to (chip8_mem *memory, chip8_opcode *opcode);

// 2XXX: Call subroutine at NNN.
void call (chip8_mem *memory, chip8_opcode *opcode);

// 3XXX: 
void eval_ne_uchar (chip8_mem *memory, chip8_opcode *opcode);

void eval_eq (chip8_mem *memory, chip8_opcode *opcode);

void eval_ne_reg (chip8_mem *memory, chip8_opcode *opcode);

void set_val_to_reg (chip8_mem *memory, chip8_opcode *opcode);

void add_val_to_reg (chip8_mem *memory, chip8_opcode *opcode);

void reg_sub_func (chip8_mem *memory, chip8_opcode *opcode);

void eval_eq_reg (chip8_mem *memory, chip8_opcode *opcode);

void set_index_reg (chip8_mem *memory, chip8_opcode *opcode);

void jump_add_offset (chip8_mem *memory, chip8_opcode *opcode);

void random_val (chip8_mem *memory, chip8_opcode *opcode);

void draw (chip8_mem *memory, chip8_opcode *opcode);

void keypress (chip8_mem *memory, chip8_opcode *opcode);

void subfunc_ex (chip8_mem *memory, chip8_opcode *opcode);

// Stack Management, size is set to 15 to prevent overwriting reserved register.
#define STACK_SIZE 15
#define STACK_EMPTY 0
#define FLAG (0xF)

int push(chip8_mem *);
uint16_t pop(chip8_mem *);



#endif