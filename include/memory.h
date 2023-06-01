#pragma once
#ifndef MEMORY_H
#define MEMORY_H

#include "config.h"

typedef struct
{
    unsigned char memory[CHIP8_MEMORY_SIZE];
} chip8_memory;

unsigned char chip8_memory_get(chip8_memory *ctx, int index);
void chip8_memory_set(chip8_memory *ctx, int index, unsigned char value);
unsigned short chip8_memory_get_opcode(chip8_memory *ctx, int index);

#endif
