#pragma once
#include "ezsdl.h"
#include "shapedraw.h"
 
enum GameState
{
	S_START,
	S_SELECTING_HOLE,
	S_FALLING

} gameState, nextState;

typedef struct Hole {

	int x;
	int y;
	bool empty;

} Hole;

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
	SDL_Rect r; r.x = 0; r.y = 200; r.w = 600; r.h = 400;
	SDL_Color c; c.r = 100; c.g = 100; c.b = 255, c.a = 255;
	InitBoard(board, r, c, 7, 6);
}

typedef struct Ball
{
	float x;
	float y;
	bool turn;

} Ball;

typedef struct Connect4
{
	Board board;
	uint8_t holeRadius;
	Hole holes[42];
	Ball balls[42];
	int usedBalls;
	bool turn;
	float xHoleOffset;
	float holeSpacing;

} Connect4;

float holeToScreenAcross(Connect4* game, int x) {
	return (float)(x * game->holeSpacing + game->xHoleOffset);
}

int screenToHoleAcross(Connect4* game, float x) {
	return (int)((x - game->xHoleOffset + game->holeRadius) / game->holeSpacing);
}

float holeToScreenDown(Connect4* game, int y) {
	return (float)(y * game->holeSpacing + game->board.inScreen.y + game->holeRadius);
}

int screenToHoleDown(Connect4* game, float y) {
	return (int)((y - game->board.inScreen.y - game->holeRadius) / game->holeSpacing);
}

void SetBallInitial(Connect4* game, int ballOver) {
	if (game->usedBalls < 41) {
		game->usedBalls++;
		game->balls[game->usedBalls].x = holeToScreenAcross(game, ballOver);
		game->balls[game->usedBalls].y = (float)game->board.inScreen.y - game->holeRadius;
		game->balls[game->usedBalls].turn = game->turn;
	}
}

void MakeHoles(Connect4* game) {
	for (size_t x = 0; x < game->board.rows; x++) {
		for (size_t y = 0; y < game->board.cols; y++) {
			game->holes[y * game->board.rows + x] = (Hole){
				(int)(x * game->holeSpacing + game->xHoleOffset),
				(int)(y * game->holeSpacing + game->board.inScreen.y + game->holeRadius),
				true
			};
		}
	}
}

void connect4Init(Connect4* game) {
	MakeBoard(&game->board);
	game->holeRadius = 30;
	game->xHoleOffset = 105;
	game->holeSpacing = game->holeRadius * 2.1f;
	MakeHoles(game);
	game->turn = false; //turn : player1, !turn : player2
	game->usedBalls = -1;
}

bool mouseInRect(SDL_Rect r, int mx, int my) {
	return mx > r.x && mx < r.x + r.w && my > r.y && my < r.y + r.h;
}

int getMouseOverColumn(Connect4* game, int mx, int my) {
	SDL_Rect r; 
	r.x = (int)game->xHoleOffset - (int)(game->holeRadius);
	r.y = 0; 
	r.w = game->board.inScreen.w - (int)(game->xHoleOffset * 1.6f);
	r.h = game->board.inScreen.y;

	if (mouseInRect(r, mx, my)) {
		return screenToHoleAcross(game, (float)mx);
	}
	return -1;
}

int ballFall(Connect4* game) {
	int down = screenToHoleDown(game, game->balls[game->usedBalls].y);
	int across = screenToHoleAcross(game, game->balls[game->usedBalls].x);

	bool columnFull = true;

	for (size_t i = 0; i < 6; i++) {
		if (game->holes[i * 7 + across].empty) {
			columnFull = false;
			break;
		}
	}

	if (down < 5) {		
		if (columnFull) {
			game->usedBalls--; //mission failed, we will get em next time
			return 0; //go to selecting state without toggling turn
		}
		if (!game->holes[(down + 1) * 7 + across].empty) {
			game->balls[game->usedBalls].y = holeToScreenDown(game, down);
			game->holes[down * 7 + across].empty = false;
			return 1; //toggle turn and go to selecting state
		}
	}

	if (down < 5) {
		game->balls[game->usedBalls].y += 3.0f;
		return 3; //continue the current state
	}

	game->balls[game->usedBalls].y = holeToScreenDown(game, down);
	game->holes[down * 7 + across].empty = false;
	return 1;
}

//////////////DRAWWW//////////////////////

void drawBoard(EZ_SDL_Context* context, const Board* board) {
	EZ_SetContextColor(context,
		board->color.r, board->color.g, board->color.b, board->color.a);
	EZ_FillRect(context,
		board->inScreen.x, board->inScreen.y, board->inScreen.w, board->inScreen.h);
}

void drawHoles(EZ_SDL_Context* context, const Connect4* game) {
	for (size_t i = 0; i < 42; i++) {
		EZ_SetContextColor(context, 0, 0, 100, 255);
		EZ_DrawCircle(context, game->holes[i].x, game->holes[i].y, game->holeRadius);
	}
}

void drawBallMoveableState(EZ_SDL_Context* context, Connect4* game, int ballOver) {
	if (ballOver != -1) {
		if (game->turn) {
			EZ_SetContextColor(context, 255, 0, 0, 255);
		}
		else {
			EZ_SetContextColor(context, 0, 255, 0, 255);
		}
		EZ_FillCircle(context,
			(int)holeToScreenAcross(game, ballOver),
			game->board.inScreen.y - game->holeRadius, 
			game->holeRadius - 3);
	}
}

void drawBalls(EZ_SDL_Context* context, const Connect4* game) {

	if (game->usedBalls != -1) {
		for (size_t i = 0; i < (size_t)game->usedBalls + 1; i++) {
			if (game->balls[i].turn) {
				EZ_SetContextColor(context, 255, 0, 0, 255);
			}
			else {
				EZ_SetContextColor(context, 0, 255, 0, 255);
			}

			EZ_FillCircle(context,
				(int)game->balls[i].x,
				(int)game->balls[i].y,
				game->holeRadius - 3);
		}
	}
}

