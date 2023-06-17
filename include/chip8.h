#pragma once
#ifndef CHIP8_H
#define CHIP8_H

#include "registers.h"
#include "memory.h"
#include "screen.h"
#include "stack.h"

struct chip8_context
{
    chip8_registers registers;
    chip8_stack stack;
    chip8_memory memory;
    chip8_screen screen;
};

void chip8_init(struct chip8_context *chip8);

#endif
