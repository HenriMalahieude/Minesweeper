#include "globals.h"
#include "board.h"

#include "raylib.h"
#include "raygui.h"

const uint16_t margin = (win_width - board_size) / 2.f;

const float intro_max = 0.5f;
float intro_time = 0.0f;

Vector2 kernel[8] = {
	{-1,  0}, //mid-left
	{-1,  1}, //top-left
	{-1, -1}, //bot-left
	{ 1,  0}, //mid-right
	{ 1,  1}, //top-right
	{ 1, -1}, //bot-right
	{ 0, -1}, //str-up
	{ 0,  1}  //str-down
};

void BoardReveal(struct Board *board) {
	for (int x = 0; x < BOARD_MAX; x++)
		for (int y = 0; y < BOARD_MAX; y++)
			board->rgrid[x][y] = SQR_VOID; //reveal everything
}

void BoardFail(struct Board *board) {
	board->state = ST_FAILED;
	BoardReveal(board);
}

void BoardCheckClear(struct Board *board) {
	if (board->state != ST_RUNNING) return;

	for (int x = 0; x < board->square; x++) {
		for (int y = 0; y < board->square; y++) {
			if ((board->rgrid[x][y] == SQR_UNREVEALED || board->rgrid[x][y] == SQR_FLAG)
					&& board->grid[x][y] != SQR_MINE) return; //haven't fully revealed the board
		}
	}

	board->state = ST_CLEAR;
	BoardReveal(board);
}

void BoardGenerate(struct Board *board, uint16_t square, uint16_t mine_cnt) {
	board->square = square;
	board->state = ST_RUNNING;

	//Reset
	for (size_t x = 0; x < BOARD_MAX; x++) {
		for (size_t y = 0; y < BOARD_MAX; y++) {
			if (x >= square || y >= square) {
				board->grid[x][y] = SQR_VOID;
				board->rgrid[x][y] = SQR_UNREVEALED;
				continue;
			}

			board->grid[x][y] = SQR_EMPTY;
			board->rgrid[x][y] = SQR_UNREVEALED;
		}
	}

	//Place Mines, and Increment Numbers Around
	for (int i = 0; i < mine_cnt; i++) {
		int x = GetRandomValue(0, square-1);
		int y = GetRandomValue(0, square-1);
		if (board->grid[x][y] == SQR_MINE) { //retry
			i -= 1;
			continue;
		}

		board->grid[x][y] = SQR_MINE;
		for (int j = 0; j < 8; j++) {
			Vector2 nloc = {x + kernel[j].x, y + kernel[j].y};
			if (nloc.x < 0 || nloc.x >= square || nloc.y < 0 || nloc.y >= square) continue;

			if (board->grid[(int)nloc.x][(int)nloc.y] != SQR_MINE) {
				board->grid[(int)nloc.x][(int)nloc.y] += 1;
			}
		}
	}
}

void SquareDraw(struct Board *board, enum SQR_TYP type, uint16_t px, uint16_t py, uint16_t sz) {
	Color exploded = {255, 0, 0, 255};
	Color unrevealed = {192, 192, 192, 255};
	Color revealed = {75, 75, 75, 255};
	uint16_t txt_sz = (sz+border-2);
	uint16_t icon_sz = ((sz-border) > 32) ? 2 : 1;
	uint16_t empty_space = (sz-border) - 16*icon_sz; //for icons


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
			DrawText("1", px+border+txt_sz/4, py+border, txt_sz, BLUE);
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
			DrawText("4", px+border+txt_sz/4, py+border, txt_sz, PURPLE);
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
			GuiDrawIcon(ICON_GEAR, px+border+(empty_space/2), py+border+(empty_space/2), icon_sz, BLACK);
			break;

		case SQR_UNREVEALED:
			DrawRectangle(px+border, py+border, sz-border, sz-border, unrevealed);
			break;

		case SQR_FLAG: //only used in the revealed grid
			DrawRectangle(px+border, py+border, sz-border, sz-border, unrevealed);
			GuiDrawIcon(ICON_CROSS, px+border+(empty_space/2), py+border+(empty_space/2), icon_sz, BLACK);
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
			//if (type != SQR_UNREVEALED) LOG_ONCE("Bruh? %d %d\n", x, y);
			if (type == SQR_VOID) type = board->grid[x][y];

			SquareDraw(board, type, rx, ry, square_size);
		}
	}
}

//When they left click on empty
void CascadeReveal(struct Board *board, int x, int y) {
	Vector2 unchecked[4*BOARD_MAX];
	int queue_pos = 0;

	unchecked[queue_pos++] = (Vector2){x, y};

	while (queue_pos > 0) {
		Vector2 local_queue[4*BOARD_MAX];
		int local_queue_pos = 0;

		for (int i = 0; i < queue_pos; i++) { //Breadth-first search
			Vector2 *cur = unchecked + i;
			int cx = (int)cur->x;
			int cy = (int)cur->y;
			if (board->rgrid[cx][cy] != SQR_VOID) {
				if (board->grid[cx][cy] != SQR_MINE) {
					board->rgrid[cx][cy] = SQR_VOID; //reveal it
				}

				if (board->grid[cx][cy] == SQR_EMPTY) {
					for (int j = 0; j < 8; j++) {
						Vector2 nloc = {cx + kernel[j].x, cy + kernel[j].y};
						if (nloc.x < 0 || nloc.x >= BOARD_MAX || nloc.y < 0 || nloc.y >= BOARD_MAX
								|| nloc.x > board->square || nloc.y > board->square) {
							continue;
						}
						if (board->rgrid[(int)nloc.x][(int)nloc.y] == SQR_VOID) continue;
						local_queue[local_queue_pos++] = nloc;
					}
				}
			}
		}

		//unchecked = local_queue;
		for (int i = 0; i < local_queue_pos; i++) unchecked[i] = local_queue[i];
		queue_pos = local_queue_pos;
	}
}

//When they left click on a number
void NumberReveal(struct Board *board, int x, int y) {
	int numeric = -1;
	if (board->rgrid[x][y] == SQR_VOID) numeric = (int)board->grid[x][y];
	if (numeric < 0) return;

	int flag_count = 0;
	for (int i = 0; i < 8; i++) {
		Vector2 nloc = {x - kernel[i].x, y - kernel[i].y};
		if (nloc.x < 0 || nloc.x >= board->square || nloc.y < 0 || nloc.y >= board->square) continue; //no existo

		if (board->rgrid[(int)nloc.x][(int)nloc.y] == SQR_FLAG) {
			flag_count += 1;
		}
	}

	if (flag_count >= numeric) { //supposedly we are chillin
		for (int i = 0; i < 8; i++) {
			Vector2 nloc = {x - kernel[i].x, y - kernel[i].y};
			if (nloc.x < 0 || nloc.x >= board->square || nloc.y < 0 || nloc.y >= board->square) continue; //no existo

			int nlx = (int)nloc.x;
			int nly = (int)nloc.y;
			if (board->rgrid[nlx][nly] == SQR_UNREVEALED) {
				if (board->grid[nlx][nly] != SQR_MINE) CascadeReveal(board, nlx, nly);
				else BoardFail(board);
			}
		}
	}
}

void BoardInteract(struct Board *board) {
	if (board->state != ST_RUNNING) return;
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

				if (board->grid[sx][sy] == SQR_MINE) { //Oops
					BoardFail(board);
				} else if (board->grid[sx][sy] == SQR_EMPTY) { //Reveal section
					CascadeReveal(board, sx, sy);
				} else if (board->rgrid[sx][sy] == SQR_VOID && board->grid[sx][sy] != SQR_EMPTY) { //QoL
					NumberReveal(board, sx, sy);
				} else { //twas an unrevealed numba
					board->rgrid[sx][sy] = SQR_VOID;
				}
			}
			BoardCheckClear(board);
		}

		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
			if (board->rgrid[sx][sy] == SQR_FLAG) {
				board->rgrid[sx][sy] = SQR_UNREVEALED;
				board->flags -= 1;
			} else if (board->rgrid[sx][sy] == SQR_UNREVEALED) {
				board->rgrid[sx][sy] = SQR_FLAG;
				board->flags += 1;
			}
		}
	}
}
