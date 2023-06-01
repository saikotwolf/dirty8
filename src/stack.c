#include "stack.h"
#include "chip8.h"

void chip8_stack_push(struct chip8_context *chip8, unsigned short value)
{
    chip8->registers.SP += 1;
    chip8->stack.stack[chip8->registers.SP] = value;
}

unsigned short chip8_stack_pop(struct chip8_context *chip8)
{
    unsigned short value = chip8->stack.stack[chip8->registers.SP];
    chip8->registers.SP -= 1;
    return value;
}
