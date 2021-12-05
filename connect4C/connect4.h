#pragma once
#include "ezsdl.h"
#include "shapedraw.h"

typedef enum HoleState
{
	EMPTY,
	PLAYER1,
	PLAYER2
} HoleState;

typedef struct Hole {

	int x;
	int y;
	HoleState state;

} Hole;

void InitHole(Hole* hole, int x, int y, uint8_t r, HoleState state) {
	*hole = (Hole){ x, y, state };
}

typedef struct Board
{
	SDL_Rect inScreen;
	SDL_Color color;
	uint8_t rows;
	uint8_t cols;
} Board;

void InitBoard(Board* board, SDL_Rect r, SDL_Color c, uint8_t rows, uint8_t cols) {
	*board = (Board){ r, c, rows, cols };
}

void MakeBoard(Board* board) {
	SDL_Rect r; r.x = 0; r.y = 220; r.w = 600; r.h = 380;
	SDL_Color c; c.r = 100; c.g = 100; c.b = 255, c.a = 255;
	InitBoard(board, r, c, 7, 6);
}

void DrawBoard(EZ_SDL_Context* context, const Board* board) {
	EZ_SetContextColor(context,
		board->color.r, board->color.g, board->color.b, board->color.a);
	EZ_FillRect(context,
		board->inScreen.x, board->inScreen.y, board->inScreen.w, board->inScreen.h);
}

typedef struct Connect4
{
	Board board;
	uint8_t holeRadius;
	Hole holes[42];
	
} Connect4;

void MakeHoles(Connect4* game) {
	float offset = 100;
	float spaceAcross = ((float)game->board.inScreen.w - offset) / game->board.rows;
	float spaceDown = (float)game->holeRadius * 2;

	for (size_t x = 0; x < game->board.rows; x++) {
		for (size_t y = 0; y < game->board.cols; y++) {
			game->holes[y * game->board.rows + x] = (Hole){
				(int)(x * spaceAcross) + (int)(0.8f * offset),
				(int)((y * spaceDown) + game->board.inScreen.y) + (int)(0.3f * offset),
				EMPTY };
		}
	}
}

bool mouseInRect(SDL_Rect r, int mx, int my) {
	return mx > r.x && mx < r.x + r.w && my > r.y && my < r.y + r.h;
}

void getMouseOverColumn(Board* board, int mx, int my) {
	if (mouseInRect(board->inScreen, mx, my)) {

		printf("%d\n", mx / (board->inScreen.w / board->rows));
	}
}