#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include "chip8.h"

int main(int argc, char *argv)
{
    printf("IN PROGRESS!\n");

    bool quit = false;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow(
        "dirty8 emulator",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        CHIP8_WIDTH * 10,
        CHIP8_HEIGHT * 10,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window,-1, SDL_TEXTUREACCESS_TARGET);

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
    }
    
    SDL_DestroyWindow(window);
    return 0;
}
