#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include "chip8.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return -1;
    }

    const char *filename = argv[1];
    printf("The filename to load is: %s\n", filename);

    FILE * f = fopen(filename, "rb");
    if (!f)
    {
        printf("Failed to open the file");
        return -1;
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char buf[size];
    fread(buf, size, 1, f);
    fclose(f);

    struct chip8_context chip8;
    chip8_init(&chip8);
    chip8_load(&chip8, buf, size);

    bool quit = false;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow(
        CHIP8_WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        CHIP8_WIDTH * CHIP8_WINDOW_MULTIPLIER,
        CHIP8_HEIGHT * CHIP8_WINDOW_MULTIPLIER,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);

    SDL_Event event;
    while (!quit)
    {
        SDL_PollEvent(&event);
        switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
            break;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

        for (int x = 0; x < CHIP8_WIDTH; x++)
        {
            for (int y = 0; y < CHIP8_WIDTH; y++)
            {
                if (chip8_screen_is_set(&chip8.screen, x, y))
                {
                    SDL_Rect r;
                    r.x = x * CHIP8_WINDOW_MULTIPLIER;
                    r.y = y * CHIP8_WINDOW_MULTIPLIER;
                    r.w = CHIP8_WINDOW_MULTIPLIER;
                    r.h = CHIP8_WINDOW_MULTIPLIER;
                    SDL_RenderFillRect(renderer, &r);
                }
            }
        }
        
        SDL_RenderPresent(renderer);

        unsigned short opcode = chip8_memory_get_opcode(&chip8.memory, chip8.registers.PC);

        chip8.registers.PC +=2;
        
        chip8_exec(&chip8, opcode);

        if(chip8.registers.delay_timer > 0)
        {
            chip8.registers.delay_timer -= 1;
        }
        if(chip8.registers.sound_timer > 0)
        {
            chip8.registers.sound_timer = 0;
        }
    }
    
    SDL_DestroyWindow(window);

    return 0;
}
