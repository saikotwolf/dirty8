#include "keyboard.h"

void chip8_keyboard_set_map(chip8_keyboard *keyboard, const char *map)
{
    keyboard->keyboard_map = map;
}

int chip8_keyboard_map(chip8_keyboard *keyboard, char key)
{
    for (int i = 0; i < CHIP8_TOTAL_KEYS; i++)
    {
        if (keyboard->keyboard_map[i] == key)
        {
            return i;
        }
    }
    return -1;
}

void chip8_keyboard_down(chip8_keyboard *keyboard, int key)
{
    keyboard->keys[key] = true;
}

void chip8_keyboard_up(chip8_keyboard *keyboard, int key)
{
    keyboard->keys[key] = false;
}

bool chip8_keyboard_is_down(chip8_keyboard *keyboard, int key)
{
    return keyboard->keys[key];
}