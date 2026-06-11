#include <stdbool.h>

#define RAYGUI_IMPLEMENTATION
#include "raylib.h"

#include "globals.h"
#include "board.h"
#include "interface.h"

#if defined(PLATFORM_WEB)
	#include <emscripten/emscripten.h>
#endif

enum GME_DFC difficulty = DFC_EASY;
struct Board board = {0};
bool reload = true;

void DrawFunction() {
	BoardInteract(&board); //place mouse interaction before the draw step
	BeginDrawing();
		InterfaceDraw();
		ClearBackground(WHITE);

		BoardDraw(&board, GetFrameTime(), reload);
		reload = false;
	EndDrawing();
}

int main(int argc, char **argv) {
	InitWindow(win_heigh, win_width, "Minesweeper");
	SetTargetFPS(target_fps);

	GuiSetStyle(DEFAULT, TEXT_SIZE, font_size);
	GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
	GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, ColorToInt(GRAY));
	GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(BLACK));
	//GuiSetStyle(BUTTON, TEXT_COLOR_DISABLED, ColorToInt(GRAY));

	BoardGenerate(&board, DifficultySquare[difficulty], DifficultyMines[difficulty]);

#if defined(PLATFORM_WEB)
	emscripten_set_main_loop(DrawFunction, 0, 1);
#else
	SetTargetFPS(60);
	while (!WindowShouldClose()) DrawFunction();
#endif

	return 0;
}
