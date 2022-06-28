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

// 7XXX: Add NN to VX.
void add_val_to_reg (chip8_mem *memory, chip8_opcode *opcode);

/* 8XYN
*   Multi Case Instruction, base on current value of N.
*  -------------------------
*   0x0: Set VX to the value of VY.
*   0x1: Set VX to the value of VX | VY (bitwise OR).
*   0x2: Set VX to the value of VX & VY (bitwise AND).
*   0x3: Set VX to the value of VX ^ VY (bitwise XOR).
*   0x4: Add the value of VY to VX.
*   0x5: Subtract the value of VY from VX. Set VF = 0 if no borrow.
*   0x6: Shift VX one bit to the right. Set VF = 1 if least significant bit is set.
*   0x7: Set VX to VY - VX. Set VF = 0 if borrow.
*   0xE: Shift VX one bit to the left. Set VF = 1 if most significant bit is set.
*/
void reg_sub_func (chip8_mem *memory, chip8_opcode *opcode);

// 9XY0: Skip if VX != VY.
void eval_ne_reg (chip8_mem *memory, chip8_opcode *opcode);

// ANNN: Set I to NNN.
void set_index_reg (chip8_mem *memory, chip8_opcode *opcode);

// BXNN: Jump to NNN + the value of VX.
void jump_add_offset (chip8_mem *memory, chip8_opcode *opcode);

// Set VX to a random number with offset of NN.
void random_val (chip8_mem *memory, chip8_opcode *opcode);

/* DXYN
*   Draw a sprite at position (VX,VY) with N bytes of sprite data starting at I.
*   Check value of I for sprite information to draw, eight bits at a time.
*   If current pixel is on, set VF = 1 then turn pixel off.
*   Sets draw flag for main loop.
*/
void draw (chip8_mem *memory, chip8_opcode *opcode);

/* 
* EX9E: Skip if key corresponding to value in VX is pressed.
* EXA1: Skip if key corresponding to value in VX is not pressed.
*/
void keypress (chip8_mem *memory, chip8_opcode *opcode);

/* 
*   FX07: Store delay timer in VX.
*   FX0A: Wait for a keypress and store result in VX.
*   FX15: Set delay timer to VX.
*   FX18: Set sound timer to VX.
*   FX1E: Add VX to I.
*   FX29: Set I to the address of sprite data corresponding to the value stored in VX.
*   FX33: Store the digits of VX at addresses I, I + 1, I + 2 starting from the most significant digit.
*   FX55: Starting at VX[0] store each value in memory at I incrementing I for each register.
*   FX65: Inverse of 55, store each value of I starting at I[0] in the registers VX.
*/
void subfunc_ex (chip8_mem *memory, chip8_opcode *opcode);

// Timer manipulation, decrements timers if > 0. Runs independently of main processing.
void tick (chip8_mem *memory);

/*------------------------------------------------------------------------/
/ * Stack handling functions.
/------------------------------------------------------------------------*/
#define STACK_SIZE 15
#define STACK_EMPTY 0

int push(chip8_mem *memory);
uint16_t pop(chip8_mem *memory);

#endif