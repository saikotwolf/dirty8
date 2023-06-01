#include "stack.h"
#include "chip8.h"

void chip8_stack_push(struct chip8_context *ctx, unsigned short value)
{
    ctx->registers.SP += 1;
    ctx->stack.stack[ctx->registers.SP] = value;
}

unsigned short chip8_stack_pop(struct chip8_context *ctx)
{
    unsigned short value = ctx->stack.stack[ctx->registers.SP];
    ctx->registers.SP -= 1;
    return value;
}
