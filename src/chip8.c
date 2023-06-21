#include "chip8.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

const char chip8_character_set[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    0x90, 0x90, 0xF0, 0x10, 0x10,
    0xF0, 0x80, 0xF0, 0x10, 0xF0,
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    0xF0, 0x10, 0x20, 0x40, 0x40,
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    0xF0, 0x90, 0xF0, 0x10, 0xF0,
    0xF0, 0x90, 0xF0, 0x90, 0x90,
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    0xF0, 0x80, 0x80, 0x80, 0xF0,
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    0xF0, 0x80, 0xF0, 0x80, 0xF0,
    0xF0, 0x80, 0xF0, 0x80, 0x80
};

void chip8_init(struct chip8_context *chip8)
{
    memset(chip8, 0, sizeof(struct chip8_context));
    memcpy(chip8->memory.memory, chip8_character_set, sizeof(chip8_character_set));
}

void chip8_load(struct chip8_context *chip8, const char* buf, size_t size)
{
    memcpy(&chip8->memory.memory[CHIP8_PROGRAM_LOAD_ADDRESS], buf, size);
    chip8->registers.PC = CHIP8_PROGRAM_LOAD_ADDRESS;
}

static void chip8_exec_extended_F(struct chip8_context *chip8, unsigned short opcode)
{
    unsigned char x = (opcode >> 8) & 0x000f;
    switch (opcode & 0x00ff)
    {
        // Fx07: LD Vx, DT | Set Vx = delay timer value.
        case 0x07:
            chip8->registers.V[x] = chip8->registers.delay_timer;
        break;

        // Fx0A: LD Vx, K | Wait for a key press, store the value of the key in Vx.
        case 0x0A:
            printf("NOT IMPLEMENTED YET\n");
        break;

        // Fx15: LD DT, Vx | Set delay timer = Vx.
        case 0x15:
            chip8->registers.delay_timer = chip8->registers.V[x];
        break;

        // Fx18: LD ST, Vx | Set sound timer = Vx.
        case 0x18:
            chip8->registers.sound_timer = chip8->registers.V[x];
        break;

        // Fx1E: ADD I, Vx | Set I = I + Vx.
        case 0x1E:
            chip8->registers.I += chip8->registers.V[x];
        break;

        // Fx29: LD F, Vx | Set I = location of sprite for digit Vx.
        case 0x29:
            chip8->registers.I = chip8->registers.V[x] * CHIP8_DEFAULT_SPRITE_HEIGHT;
        break;

        // Fx33: LD B, Vx | Store BCD representation of Vx in memory locations I, I+1, and I+2.
        case 0x33:
        {
            unsigned char hundreds = chip8->registers.V[x] / 100;
            unsigned char tens = chip8->registers.V[x] / 10 % 10;
            unsigned char units = chip8->registers.V[x] % 10;
            chip8_memory_set(&chip8->memory, chip8->registers.I, hundreds);
            chip8_memory_set(&chip8->memory, chip8->registers.I+1, tens);
            chip8_memory_set(&chip8->memory, chip8->registers.I+2, units);
        }
        break;

        // Fx55: LD [I], Vx | Store registers V0 through Vx in memory starting at location I.
        case 0x55:
        {
            for (int i = 0; i <= x; i++)
            {
                chip8_memory_set(&chip8->memory, chip8->registers.I+i, chip8->registers.V[i]);
            }
        }
        break;

        // Fx65: LD Vx, [I] | Read registers V0 through Vx from memory starting at location I.
        case 0x65:
        {
            for (int i = 0; i <= x; i++)
            {
                chip8->registers.V[i] = chip8_memory_get(&chip8->memory, chip8->registers.I+i);
            }
        }
        break;
    }
}

static void chip8_exec_extended_eight(struct chip8_context *chip8, unsigned short opcode)
{
    unsigned char x = (opcode >> 8) & 0x000f;
    unsigned char y = (opcode >> 4) & 0x000f;
    unsigned short tmp;

    switch(opcode & 0x000f)
    {
        // 8xy0: LD Vx, Vy | Set Vx = Vy.
        case 0x00:
            chip8->registers.V[x] = chip8->registers.V[y];
        break;

        // 8xy1: OR Vx, Vy | Set Vx = Vx OR Vy.
        case 0x01:
            chip8->registers.V[x] |= chip8->registers.V[y];
        break;

        // 8xy2: AND Vx, Vy | Set Vx = Vx AND Vy.
        case 0x02:
            chip8->registers.V[x] &= chip8->registers.V[y];
        break;

        // 8xy3: XOR Vx, Vy | Set Vx = Vx XOR Xy.
        case 0x03:
            chip8->registers.V[x] ^= chip8->registers.V[y];
        break;

        // 8xy4: ADD Vx, Vy | Set Vx = Vx ADD Vy , set VF = carry.
        case 0x04:
            chip8->registers.V[0x0F] = false;
            tmp = chip8->registers.V[x] + chip8->registers.V[x];
            if(tmp > 0xFF)
            {
                chip8->registers.V[0x0F] = true;
            }
            chip8->registers.V[x] = tmp;
        break;

        // 8xy5: SUB Vx, Vy | Set Vx = Vx - Vy, set VF = NOT borrow.
        case 0x05:
            chip8->registers.V[0x0f] = chip8->registers.V[x] > chip8->registers.V[y];
            chip8->registers.V[x] -= chip8->registers.V[y];
        break;

        // 8xy6: SHR Vx {, Vy} | Set Vx = Vx SHR 1.
        case 0x06:
            chip8->registers.V[0x0F] = chip8->registers.V[x]& 0b00000001;
            chip8->registers.V[x] /= 2;
        break;

        // 8xy7: SUBN Vx, Vy | Set Vx = Vy - Vx, set VF = NOT borrow.
        case 0x07:
            chip8->registers.V[0x0f] = chip8->registers.V[y] > chip8->registers.V[x];
            chip8->registers.V[x] = chip8->registers.V[y] - chip8->registers.V[x];
        break;

        // 8xyE: SHL Vx {, Vy} | Set Vx = Vx SHL 1.
        case 0x0E:
            chip8->registers.V[0x0f] = chip8->registers.V[x] & 0b10000000;
            chip8->registers.V[x] *= 2;
        break;
    }

}

static void chip8_exec_extended(struct chip8_context *chip8, unsigned short opcode)
{
    unsigned short nnn = opcode & 0x0fff;
    unsigned char x = (opcode >> 8) & 0x000f;
    unsigned char y = (opcode >> 4) & 0x000f;
    unsigned char kk = opcode & 0x00ff;
    unsigned char n = opcode & 0x000f;

    switch(opcode & 0xf000)
    {
        // 1nnn: JP addr | Jump to location nnn.
        case 0x1000:
            chip8->registers.PC = nnn;
        break;

        // 2nnn: CALL addr | Call subroutine at nnn.
        case 0x2000:
            chip8_stack_push(chip8, chip8->registers.PC);
            chip8->registers.PC = nnn;
        break;

        // 3xkk: SE Vx, byte | Skip next instruction if Vx = kk.
        case 0x3000:
            if(chip8->registers.V[x] == kk)
            {
                chip8->registers.PC += 2;
            }
        break;

        // 4xkk: SNE Vx, byte | Skip next instruction if Vx != kk.
        case 0x4000:
            if(chip8->registers.V[x] != kk)
            {
                chip8->registers.PC += 2;
            }
        break;

        // 5xy0: SE Vx, Vy | Skip next instruction if Vx = Vy.
        case 0x5000:
            if(chip8->registers.V[x] == chip8->registers.V[y])
            {
                chip8->registers.PC += 2;
            }
        break;

        // 6xkk: LD Vx, byte | Set Vx = kk.
        case 0x6000:
            chip8->registers.V[x] = kk;
        break;

        // 7xkk: ADD Vx, byte | Set Vx = Vx + kk.
        case 0x7000:
            chip8->registers.V[x] += kk;
        break;

        // Extended eight instructions.
        case 0x8000:
            chip8_exec_extended_eight(chip8, opcode);
        break;

        // 9xy0: SNE Vx, Vy | Skip next instruction if Vx != Vy.
        case 0x9000:
            if(chip8->registers.V[x] != chip8->registers.V[y])
            {
                chip8->registers.PC += 2;
            }
        break;

        // Annn: LD I, addr | Set I = nnn.
        case 0xA000:
            chip8->registers.I = nnn;
        break;

        // Bnnn: JP V0, addr | Jump to location nnn + V0.
        case 0xB000:
            chip8->registers.PC = nnn + chip8->registers.V[0x00];
        break;

        // Cxkk: RND Vx, byte | Set Vx = random byte AND kk.
        case 0xC000:
            srand(time(NULL));
            chip8->registers.V[x] = (rand() % 256) & kk;
        break;

        // Dxyn: DRW Vx, Vy, nibble | Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
        case 0xD000:
        {
            const char* sprite = (const char*) &chip8->memory.memory[chip8->registers.I];
            chip8_screen_draw_sprite(&chip8->screen, chip8->registers.V[x], chip8->registers.V[y], sprite, n);
        }
        break;

        // Extended F instructions.
        case 0xF000:
            chip8_exec_extended_F(chip8, opcode);
        break;
    }
}

void chip8_exec(struct chip8_context *chip8, unsigned short opcode)
{
    switch(opcode)
    {
        // CLS: Clear the display.
        case 0x00E0:
            chip8_screen_clear(&chip8->screen);
        break;

        // RET: Return from a subroutine.
        case 0x00EE:
            chip8->registers.PC = chip8_stack_pop(chip8);
        break;

        default:
        chip8_exec_extended(chip8, opcode);
    }
}