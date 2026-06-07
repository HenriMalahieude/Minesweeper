#include "globals.h"
#include "board.h"

#include "raylib.h"
#include "raygui.h"

const uint16_t margin = (win_width - board_size) / 2.f;

const float intro_max = 2.0f;
float intro_time = 0.0f;

void BoardGenerate(struct Board *board, uint16_t square, uint16_t mine_cnt) {
	board->square = square;
	board->state = ST_RUNNING;

	for (size_t x = 0; x < BOARD_MAX; x++) {
		for (size_t y = 0; y < BOARD_MAX; y++) {
			if (x > square || y > square) {
				board->grid[x][y] = SQR_VOID;
				board->rgrid[x][y] = SQR_VOID;
			}

			board->grid[x][y] = SQR_EMPTY;
			board->rgrid[x][y] = SQR_UNREVEALED;
		}
	}

	LOG_ONCE("TODO\n");
}

void SquareDraw(struct Board *board, enum SQR_TYP type, uint16_t px, uint16_t py, uint16_t sz) {
	Color exploded = {255, 0, 0, 255};
	Color unrevealed = {192, 192, 192, 255};
	Color revealed = {75, 75, 75, 255};
	uint16_t txt_sz = (sz+border-2);

	DrawRectangle(px, py, sz+border, sz+border, BLACK);
	switch (type) {
		case SQR_VOID:
			LOG_ONCE("Error, Why are we rendering void squares?\n");
			break;

		case SQR_EMPTY:
			DrawRectangle(px+border, py+border, sz-border, sz-border, revealed);
			break;

		case SQR_I:
			DrawRectangle(px+border, py+border, sz-border, sz-border, revealed);
			DrawText("1", px+border+txt_sz/4, py+border, txt_sz, DARKBLUE);
			break;

		case SQR_II:
			DrawRectangle(px+border, py+border, sz-border, sz-border, revealed);
			DrawText("2", px+border+txt_sz/4, py+border, txt_sz, GREEN);
			break;

		case SQR_III:
			DrawRectangle(px+border, py+border, sz-border, sz-border, revealed);
			DrawText("3", px+border+txt_sz/4, py+border, txt_sz, RED);
			break;

		case SQR_IV:
			DrawRectangle(px+border, py+border, sz-border, sz-border, revealed);
			DrawText("4", px+border+txt_sz/4, py+border, txt_sz, DARKPURPLE);
			break;

		case SQR_V:
			DrawRectangle(px+border, py+border, sz-border, sz-border, revealed);
			DrawText("5", px+border+txt_sz/4, py+border, txt_sz, BEIGE);
			break;

		case SQR_VI:
			DrawRectangle(px+border, py+border, sz-border, sz-border, revealed);
			DrawText("6", px+border+txt_sz/4, py+border, txt_sz, SKYBLUE);
			break;

		case SQR_VII:
			DrawRectangle(px+border, py+border, sz-border, sz-border, revealed);
			DrawText("7", px+border+txt_sz/4, py+border, txt_sz, BLACK);
			break;

		case SQR_VIII:
			DrawRectangle(px+border, py+border, sz-border, sz-border, revealed);
			DrawText("8", px+border+txt_sz/4, py+border, txt_sz, YELLOW);
			break;

		case SQR_MINE:
			Color toUse = (board->state == ST_FAILED) ? exploded : GREEN;

			DrawRectangle(px+border, py+border, sz-border, sz-border, toUse);
			GuiDrawIcon(ICON_GEAR, px+border, py+border, (sz-border)/16, BLACK);
			break;

		case SQR_UNREVEALED:
			DrawRectangle(px+border, py+border, sz-border, sz-border, unrevealed);
			break;

		case SQR_FLAG: //only used in the revealed grid
			DrawRectangle(px+border, py+border, sz-border, sz-border, unrevealed);
			GuiDrawIcon(ICON_CRACK, px+border, py+border, (sz-border)/16, BLACK);
			break;

		default:
			LOG("Error, attempted to draw an unknown square type?\n");
	};
}

void BoardDraw(struct Board *board, float frametime, bool intro_sgn) {
	uint16_t square_size = (board_size) / ((float)board->square);

	//the borders, optimized into one thing
	/*
	DrawRectangleV(
		(Vector2){margin, margin},
		(Vector2){board_size, board_size},
		BLACK
	); // */
	//TODO: For some reason this makes the borders not consistent across mines
	//		I'll need to figure this out at some point

	if (intro_sgn) intro_time = intro_max;

	if (intro_time > 0.0f) {
		intro_time -= frametime;
		float intro_perc = 1 - (intro_time / intro_max); //0 -> 1 how much is revealed

		for (int x = 0; x < board->square; x++) {
			for (int y = 0; y < board->square; y++) {

				float reduction = ((x / (float)board->square) + (y / (float)board->square)) / 2.f;
				if (reduction < intro_perc) {
					uint16_t rx = x * (square_size) + margin;
					uint16_t ry = y * (square_size) + margin;

					SquareDraw(board, SQR_UNREVEALED, rx, ry, square_size); //the square
				}
			}
		}

		return;
	}

	for (int x = 0; x < board->square; x++) {
		uint16_t rx = x * (square_size) + margin;

		for (int y = 0; y < board->square; y++) {
			uint16_t ry = y * (square_size) + margin;

			enum SQR_TYP type = board->rgrid[x][y];
			if (type == SQR_VOID) type = board->grid[x][y];

			SquareDraw(board, type, rx, ry, square_size);
		}
	}
}

void BoardInteract(struct Board *board) {
	uint16_t square_size = (board_size) / ((float)board->square);
	uint16_t mx = GetMouseX();
	uint16_t my = GetMouseY();

	if (   mx > margin && mx < (win_width-margin)
		&& my > margin && my < (win_heigh-margin)) { //within bounds
		int sx = ((mx-margin) / square_size); sx = min(sx, board->square-1);
		int sy = ((my-margin) / square_size); sy = min(sy, board->square-1);
		//LOG("%d, %d\n", sx, sy);

		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			if (board->rgrid[sx][sy] != SQR_FLAG) {
				board->rgrid[sx][sy] = SQR_VOID;

				if (board->grid[sx][sy] == SQR_MINE) {
					board->state = ST_FAILED;
				}
			}
		}

		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
			if (board->rgrid[sx][sy] == SQR_FLAG) {
				board->rgrid[sx][sy] = SQR_UNREVEALED;
			} else if (board->rgrid[sx][sy] == SQR_UNREVEALED) {
				board->rgrid[sx][sy] = SQR_FLAG;
			}
		}
	}
}
