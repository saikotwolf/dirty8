#include "screen.h"

void chip8_screen_set(chip8_screen *screen, int x, int y)
{
    screen->pixels[x][y] = true;
}

bool chip8_screen_is_set(chip8_screen *screen, int x, int y)
{
    return screen->pixels[x][y];
}

bool chip8_screen_draw_sprite(chip8_screen *screen, int x, int y, const char *sprite, int num)
{
    bool pixel_collision = false;

    for (int pixel_y = 0; pixel_y < num; pixel_y++)
    {
        char current_pixel = sprite[pixel_y];
        for (int pixel_x = 0; pixel_x < 8; pixel_x++)
        {
            if ((current_pixel & (0b10000000 >> pixel_x)) == 0)
                continue;
            if (screen->pixels[(pixel_x + x) % CHIP8_WIDTH][(pixel_y + y) % CHIP8_HEIGHT])
            {
                pixel_collision = true;
            }
            screen->pixels[(pixel_x + x) % CHIP8_WIDTH][(pixel_y + y) % CHIP8_HEIGHT] ^= true;
        }
    }

    return pixel_collision;
}
