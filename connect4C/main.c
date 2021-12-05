#include <stdio.h>
#include <stdbool.h>

#include "ezsdl.h"
#include "gameloop.h"
#include "connect4.h"

int main(int argc, char** argv)
{
    EZ_SDL_Context context;
    EZ_InitContext(&context);
    EZ_CreateWindow(&context.window, "Connect 4", 600, 600);
    EZ_CreateRenderer(&context.renderer, context.window, true);

    Connect4 game;
    game.holeRadius = 30;
    MakeBoard(&game.board);
    MakeHoles(&game);
  
    runGameLoop(&context, &game);

    return 1;
}

