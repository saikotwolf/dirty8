#pragma once
#ifndef CHIP8_H
#define CHIP8_H

#include "registers.h"
#include "memory.h"
#include "screen.h"
#include "stack.h"
#include "keyboard.h"
#include <stddef.h>

struct chip8_context
{
    chip8_registers registers;
    chip8_stack stack;
    chip8_memory memory;
    chip8_screen screen;
    chip8_keyboard keyboard;
};

void chip8_init(struct chip8_context *chip8);
void chip8_load(struct chip8_context *chip8, const char* buf, size_t size);
static void chip8_exec_extended(struct chip8_context *chip8, unsigned short opcode);
static void chip8_exec_extended_eight(struct chip8_context *chip8, unsigned short opcode);
static void chip8_exec_extended_F(struct chip8_context *chip8, unsigned short opcode);
void chip8_exec(struct chip8_context *chip8, unsigned short opcode);

#endif
