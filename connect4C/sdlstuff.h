#pragma once
#include "SDL.h"
#include <stdbool.h>

bool EZ_CreateWindow(SDL_Window** window, const char* name, uint32_t width, uint32_t height) {
    *window = SDL_CreateWindow
    (
        name,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN
    );

    return window != NULL;
}

bool EZ_CreateRenderer(SDL_Renderer** renderer, SDL_Window* window, bool blendMode) {
    *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer != NULL && blendMode)
        SDL_SetRenderDrawBlendMode(*renderer, SDL_BLENDMODE_BLEND);
    return renderer != NULL;
}


