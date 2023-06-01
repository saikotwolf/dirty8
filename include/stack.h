#pragma once
#ifndef STACK_H
#define STACK_H

#include "config.h"

struct chip8_context;
typedef struct
{
    unsigned short stack[CHIP8_STACK_DEPTH_SIZE];
} chip8_stack;

void chip8_stack_push(struct chip8_context *chip8, unsigned short value);
unsigned short chip8_stack_pop(struct chip8_context *chip8);

#endif
