#include "globals.h"
#include "board.h"

#include "raylib.h"
#include "raygui.h"

static const float intro_max = 2.0f;
float intro_time = 0.0f;

void BoardGenerate(struct Board *board, uint16_t square, uint16_t mine_cnt) {
	board->square = square;
	LOG_ONCE("TODO\n");
}

void SquareDraw(enum SQR_TYP type, int px, int py, int sz) {
	switch (type) {
		case SQR_VOID:
			DrawRectangle(px, py, sz, sz, RED);
			break;

		default:
			DrawRectangle(px, py, sz, sz, GRAY);
	};
	LOG_ONCE("TODO\n");
}

void BoardDraw(struct Board *board, float frametime, bool intro_sgn) {
	float margin = (win_width - board_size) / 2.f;
	float square_size = (board_size - ((board->square+1)*border)) / (float)board->square;

	//the borders, optimized into one thing
	DrawRectangleV(
		(Vector2){margin, margin},
		(Vector2){board_size, board_size},
		BLACK
	);

	if (intro_sgn) intro_time = intro_max;

	if (intro_time > 0.0f) {
		intro_time -= frametime;
		float intro_perc = 1 - (intro_time / intro_max); //0 -> 1 how much is revealed

		for (int x = 0; x < board->square; x++) {
			for (int y = 0; y < board->square; y++) {

				float reduction = ((x / (float)board->square) + (y / (float)board->square)) / 2.f;
				if (reduction < intro_perc) {
					float rx = x * (square_size+border) + margin;
					float ry = y * (square_size+border) + margin;

					SquareDraw(SQR_VOID, rx+border, ry+border, square_size); //the square
				}
			}
		}

		return;
	}

	for (int x = 0; x < board->square; x++) {
		uint16_t rx = x * (square_size + border) + margin;

		for (int y = 0; y < board->square; y++) {
			uint16_t ry = y * (square_size + border) + margin;
			SquareDraw(SQR_UNREVEALED, rx+border, ry+border, square_size);
		}
	}
}

void BoardInteract(struct Board *board, uint32_t mx, uint32_t my) {
	LOG_ONCE("TODO\n");
}
