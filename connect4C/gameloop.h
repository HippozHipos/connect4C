#pragma once
#include "ezsdl.h"
#include "shapedraw.h"
#include "connect4.h"

void runGameLoop(EZ_SDL_Context* context, Connect4* game) {

    while (true) {
        SDL_WaitEvent(&context->event);
        if (context->event.type == SDL_QUIT) {
            EZ_CleanUpSDL(context);
            exit(0);
        }

        int mouseX, mouseY;
        uint32_t mousePressed = SDL_GetMouseState(&mouseX, &mouseY);

        getMouseOverColumn(&game->board, mouseX, mouseY);

        //DRAW STUFF FROM HERE
        EZ_SetContextColor(context, 226, 220, 205, 255);
        EZ_Clear(context);

        Board board = game->board;
        EZ_SetContextColor(context, 
            board.color.r, board.color.g, board.color.b, board.color.a);
        EZ_FillRect(context,
            board.inScreen.x, board.inScreen.y, board.inScreen.w, board.inScreen.h);

        for (size_t i = 0; i < 42; i++) {
            EZ_SetContextColor(context, 0, 0, 100, 255);
            EZ_DrawCircle(context, game->holes[i].x, game->holes[i].y, game->holeRadius + 1);
            EZ_SetContextColor(context, 226, 220, 205, 255);
            EZ_FillCircle(context, game->holes[i].x, game->holes[i].y, game->holeRadius);
        }

        EZ_UpdateWindow(context);
    }
}
