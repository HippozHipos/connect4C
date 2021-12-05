#pragma once
#include "sdlstuff.h"

typedef struct EZ_SDL_Context
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Color c;
    SDL_Event event;

} EZ_SDL_Context;

void EZ_InitContext(EZ_SDL_Context* context) {
    SDL_Init(SDL_INIT_EVERYTHING);
    *context = (EZ_SDL_Context){ NULL, NULL, {0, 0, 0, 0} };
}

void EZ_SetContextColor(EZ_SDL_Context* context, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    context->c.r = r;
    context->c.g = g;
    context->c.b = b;
    context->c.a = a;
}

void EZ_Clear(EZ_SDL_Context* context) {
    SDL_SetRenderDrawColor(context->renderer, 
        context->c.r, context->c.g, context->c.b, context->c.a);
    SDL_RenderClear(context->renderer);
}

void EZ_UpdateWindow(EZ_SDL_Context* context) {
    SDL_RenderPresent(context->renderer);
}

void EZ_CleanUpSDL(EZ_SDL_Context* context) {
    SDL_DestroyWindow(context->window);
    SDL_DestroyRenderer(context->renderer);
}