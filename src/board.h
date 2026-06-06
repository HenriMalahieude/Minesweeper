#ifndef __BOARD_H__
#define __BOARD_H__
#include <stdint.h>

#define BOARD_MAX 32

static const uint16_t board_size = 512; //in pixels, both sides
static const uint16_t border = 5;

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
	SQR_MINE		= 9
};

enum GME_DFC { //Game Difficulty
	DFC_EASY = 0,
	DFC_MEDI,
	DFC_HARD,
	DFC_IMPO,
};
#define DFC_AM 4

struct Board {
	//-1 = non-revealed, 0 = empty, 1-8 = number square, 9 = mine
	enum SQR_TYP grid[BOARD_MAX][BOARD_MAX];
	uint16_t square;
};

static const uint16_t DifficultyMines[DFC_AM] = {
	[DFC_EASY] = 10,  //~10%
	[DFC_MEDI] = 64,  //~25%
	[DFC_HARD] = 200, //~40%
	[DFC_IMPO] = 614  //~60%
};
static const uint16_t DifficultySquare[DFC_AM] = {
	[DFC_EASY] = 10,
	[DFC_MEDI] = 16,
	[DFC_HARD] = 24,
	[DFC_IMPO] = 32
};

//The Board, length of one side, amount of mines to spawn
void BoardGenerate(struct Board *board, uint16_t square, uint16_t mine_cnt);

void BoardDraw(struct Board *board, float frametime, bool intro_sgn);

void BoardInteract(struct Board *board, uint32_t mx, uint32_t my);

#endif
