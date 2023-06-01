#pragma once
#ifndef REGISTERS_H
#define REGISTERS_H

typedef struct
{
    unsigned char V[16];
    unsigned short I;
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned short PC;
    unsigned char SP;
} chip8_registers;

#endif
