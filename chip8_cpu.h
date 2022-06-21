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

/*------------------------------------------------------------------------/
/ * Skip next instruction if condition.
/------------------------------------------------------------------------*/

// 3XXX: If VX != NN
void eval_eq_uchar (chip8_mem *memory, chip8_opcode *opcode);

// 4XXX: If VX == NN
void eval_ne (chip8_mem *memory, chip8_opcode *opcode);

// 5XXX: If VX != VY
void eval_eq_reg (chip8_mem *memory, chip8_opcode *opcode);

// 6XXX: If VX == VY
void set_val_to_reg (chip8_mem *memory, chip8_opcode *opcode);

/*------------------------------------------------------------------------/
/ * Register Manipulation functions.
/------------------------------------------------------------------------*/

// Add NN to VX.
void add_val_to_reg (chip8_mem *memory, chip8_opcode *opcode);

/* 8XYN
*   Multi Case Instruction, base on current value of N.
*  -------------------------
*   0x0: Set VX to the value of VY.
*   0x1: Set VX to the value of VX | VY (bitwise OR).
*   0x2: Set VX to the value of VX & VY (bitwise AND).
*   0x3: Set VX to the value of VX ^ VY (bitwise XOR).
*   0x4: Set VX 
*/
void reg_sub_func (chip8_mem *memory, chip8_opcode *opcode);

void eval_ne_reg (chip8_mem *memory, chip8_opcode *opcode);

void set_index_reg (chip8_mem *memory, chip8_opcode *opcode);

void jump_add_offset (chip8_mem *memory, chip8_opcode *opcode);

void random_val (chip8_mem *memory, chip8_opcode *opcode);

void draw (chip8_mem *memory, chip8_opcode *opcode);

void keypress (chip8_mem *memory, chip8_opcode *opcode);

void subfunc_ex (chip8_mem *memory, chip8_opcode *opcode);

void tick (chip8_mem *memory);

// Stack Management, size is set to 15 to prevent overwriting reserved register.
#define STACK_SIZE 15
#define STACK_EMPTY 0
#define FLAG (0xF)

int push(chip8_mem *);
uint16_t pop(chip8_mem *);



#endif