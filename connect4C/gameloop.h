#pragma once
#include "ezsdl.h"
#include "shapedraw.h"
#include "connect4.h"

void runGameLoop(EZ_SDL_Context* context, Connect4* game) {

    gameState = S_START;
    nextState = S_START;

    while (true){

        bool mousePressed = false;
        while (SDL_PollEvent(&context->event)) {

            if (context->event.type == SDL_QUIT) {
                EZ_CleanUpSDL(context);
                exit(0);
            }

            if (context->event.type == SDL_MOUSEBUTTONDOWN) {
                if (context->event.button.button == SDL_BUTTON_LEFT) {
                    mousePressed = true;
                }
            }
        }

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        int ballOver = getMouseOverColumn(game, mouseX, mouseY);

        EZ_SetContextColor(context, 226, 220, 205, 255);
        EZ_Clear(context);

        switch (gameState) {

            case S_START: {
                connect4Init(game);
                nextState = S_SELECTING_HOLE;
                break;
            }

            case S_SELECTING_HOLE: {
                drawBoard(context, &game->board);
                drawHoles(context, game);
                drawBallMoveableState(context, game, ballOver);
                drawBalls(context, game);
                if (game->usedBalls == 41) {
                    nextState = S_START;
                }
                if (mousePressed && ballOver != -1) {
                    SetBallInitial(game, ballOver);
                    nextState = S_FALLING;
                }
                break;
            }

            case S_FALLING: {

                drawBoard(context, &game->board);
                drawBalls(context, game);
                drawHoles(context, game);
                int toDo = ballFall(game);

                if (toDo == 0){
                    nextState = S_SELECTING_HOLE;
                }
                else if (toDo == 1) {
                    game->turn = !game->turn;
                    nextState = S_SELECTING_HOLE;
                }
                else if (toDo == 2) {
                    nextState = S_START;
                }
 
                break;
            }
        }

        EZ_UpdateWindow(context);
        gameState = nextState;
    }
}
