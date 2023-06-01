#pragma once
#ifndef CHIP8_H
#define CHIP8_H

#include "registers.h"
#include "stack.h"

struct chip8_context
{
    chip8_registers registers;
    chip8_stack stack;
};

#endif
