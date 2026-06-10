#include <stdbool.h>

#define RAYGUI_IMPLEMENTATION
#include "raylib.h"

#include "globals.h"
#include "board.h"
#include "interface.h"

enum GME_DFC difficulty = DFC_EASY;
struct Board board = {0};

int main(int argc, char **argv) {
	InitWindow(win_heigh, win_width, "Minesweeper");
	SetTargetFPS(target_fps);

	BoardGenerate(&board, DifficultySquare[difficulty], DifficultyMines[difficulty]);
	bool reload = true;

	while (!WindowShouldClose()) {
		BeginDrawing();
			ClearBackground(WHITE);

			BoardDraw(&board, GetFrameTime(), reload);
			reload = false;

			InterfaceDraw();
		EndDrawing();

		BoardInteract(&board);
	};

	return 0;
}
