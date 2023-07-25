#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include "chip8.h"
#include <unistd.h>

const char keyboard_map[CHIP8_TOTAL_KEYS] = {
    SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5,
    SDLK_6, SDLK_7, SDLK_8, SDLK_9, SDLK_a, SDLK_b,
    SDLK_c, SDLK_d, SDLK_e, SDLK_f};

void beep(){
    // Load Wav file.
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;

    SDL_LoadWAV("sounds/beep.wav", &wavSpec, &wavBuffer, &wavLength);

    // Open audio device.
    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

    // Play audio.
    int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    SDL_PauseAudioDevice(deviceId, 0);

    // Keep program running.
    SDL_Delay(120);

    // Clean up.
    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer);
}

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
    chip8_keyboard_set_map(&chip8.keyboard, keyboard_map);

    bool quit = false;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
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

            case SDL_KEYDOWN:
            {
                char key = event.key.keysym.sym;
                int vkey = chip8_keyboard_map(&chip8.keyboard, key);
                if (vkey != -1)
                {
                    chip8_keyboard_down(&chip8.keyboard, vkey);
                }
            }
            break;

            case SDL_KEYUP:
            {
                char key = event.key.keysym.sym;
                int vkey = chip8_keyboard_map(&chip8.keyboard, key);
                if (vkey != -1)
                {
                    chip8_keyboard_up(&chip8.keyboard, vkey);
                }
            }
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

        usleep(475);

        if(chip8.registers.sound_timer > 0)
        {
            beep();
            chip8.registers.sound_timer = 0;
        }
    }
    
    SDL_DestroyWindow(window);

    return 0;
}
