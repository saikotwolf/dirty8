#pragma once
#ifndef SCREEN_H
#define SCREEN_H

#include <stdbool.h>
#include "config.h"

typedef struct {
    bool pixels[CHIP8_WIDTH][CHIP8_HEIGHT];
} chip8_screen;

void chip8_screen_set(chip8_screen *screen, int x, int y);
bool chip8_screen_is_set(chip8_screen *screen, int x, int y);
bool chip8_screen_draw_sprite(chip8_screen *screen, int x, int y, const char *sprite, int num);

#endif
