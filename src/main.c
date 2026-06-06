#include <stdbool.h>

#define RAYGUI_IMPLEMENTATION
#include "raylib.h"

#include "globals.h"
#include "board.h"
#include "interface.h"

enum GME_DFC difficulty = DFC_MEDI;
struct Board board = {0};

int main(int argc, char **argc) {
	InitWindow(win_heigh, win_width, "Minesweeper");
	SetTargetFPS(target_fps);

	BoardGenerate(&board, DifficultySquare[difficulty], DifficultyMines[difficulty]);
	bool reload = true;

	while (!WindowShouldClose()) {
		BeginDrawing();
			BoardDraw(&board, GetFrameTime(), reload);
			reload = false;

			InterfaceDraw();
		EndDrawing();

		if (IsMouseButtonRelease(0)) {
			int mx = GetMouseX();
			int my = GetMouseY();
			if (!InterfaceInteract(mx, my)) BoardInteract(&board, mx, my);
		}
	};

	return 0;
}
