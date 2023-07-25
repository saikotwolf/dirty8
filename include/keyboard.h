#pragma once
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "config.h"
#include <stdbool.h>

typedef struct
{
    bool keys[CHIP8_TOTAL_KEYS];
    const char *keyboard_map;
} chip8_keyboard;

void chip8_keyboard_set_map(chip8_keyboard *keyboard, const char *map);
int chip8_keyboard_map(chip8_keyboard *keyboard, char key);
void chip8_keyboard_down(chip8_keyboard *keyboard, int key);
void chip8_keyboard_up(chip8_keyboard *keyboard, int key);
bool chip8_keyboard_is_down(chip8_keyboard *keyboard, int key);

#endif