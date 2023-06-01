#include "memory.h"

unsigned char chip8_memory_get(chip8_memory *ctx, int index)
{
    return ctx->memory[index];
}

void chip8_memory_set(chip8_memory *ctx, int index, unsigned char value)
{
    ctx->memory[index] = value;
}

unsigned short chip8_memory_get_opcode(chip8_memory *ctx, int index)
{
    unsigned char byte1 = chip8_memory_get(ctx, index);
    unsigned char byte2 = chip8_memory_get(ctx, index+1);
    return byte1 << 8 | byte2;
}
