#ifndef __BOARD_H__
#define __BOARD_H__
#include <stdint.h>

#define BOARD_MAX 25

static const uint16_t board_size = 600; //in pixels, both sides
static const uint16_t border = 4;

enum SQR_TYP { //Square type in grid
	SQR_VOID 		= -2,
	SQR_UNREVEALED 	= -1,
	SQR_EMPTY 		= 0,
	SQR_I			= 1,
	SQR_II			= 2,
	SQR_III			= 3,
	SQR_IV			= 4,
	SQR_V			= 5,
	SQR_VI			= 6,
	SQR_VII			= 7,
	SQR_VIII		= 8,
	SQR_MINE		= 9,
	SQR_FLAG		= 10
};

enum GME_DFC { //Game Difficulty
	DFC_EASY = 0,
	DFC_MEDI,
	DFC_HARD,
	DFC_IMPO,
};
#define DFC_AM 4

enum BRD_ST {
	ST_RUNNING	= 0,
	ST_FAILED 	= 1,
	ST_CLEAR 	= 2
};

struct Board {
	//-1 = non-revealed, 0 = empty, 1-8 = number square, 9 = mine
	enum SQR_TYP  grid[BOARD_MAX][BOARD_MAX];
	enum SQR_TYP rgrid[BOARD_MAX][BOARD_MAX]; //grid of revealed squares
	enum BRD_ST state;
	uint16_t square;
	uint16_t flags;
};

static const uint16_t DifficultyMines[DFC_AM] = {
	[DFC_EASY] = 10,	// 10%
	[DFC_MEDI] = 29,	// 20%
	[DFC_HARD] = 100,	// 25%
	[DFC_IMPO] = 250 	// 40%
};

static const uint16_t DifficultySquare[DFC_AM] = {
	[DFC_EASY] = 10,
	[DFC_MEDI] = 12,
	[DFC_HARD] = 20,
	[DFC_IMPO] = 25
};

//The Board, length of one side, amount of mines to spawn
void BoardGenerate(struct Board *board, uint16_t square, uint16_t mine_cnt);
void BoardDraw(struct Board *board, float frametime, bool intro_sgn);
void BoardInteract(struct Board *board);

#endif
