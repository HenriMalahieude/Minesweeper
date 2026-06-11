#include "stdio.h"
#include "stdlib.h"

#include "globals.h"
#include "interface.h"
#include "board.h"

void InterfaceDraw() {
	uint16_t margin = (win_width - board_size);
	int button_sz = margin / 3;
	int icon_sz = 2; //each square in the 16^2 icon is given this pixel sz
	int icon_pad = (button_sz - (icon_sz*16)) / 2;

	//Change Board
	Rectangle rstrt_btn = {win_width-icon_pad-button_sz, icon_pad, button_sz, button_sz};
	bool restart = GuiButton(rstrt_btn, "");
	GuiDrawIcon(ICON_RESTART, rstrt_btn.x+icon_pad, rstrt_btn.y+icon_pad, icon_sz, BLACK);

	int sz_easy = MeasureText("Easy", font_size);
	Rectangle dfc = {1, win_heigh-button_sz-icon_pad, sz_easy*1.2+icon_pad, button_sz};
	bool btn_easy = GuiButton(dfc, "Easy");

	int sz_medi = MeasureText("Medium", font_size);
	dfc.x += dfc.width+icon_pad; dfc.width = sz_medi*1.2+icon_pad;
	bool btn_medi = GuiButton(dfc, "Medium");

	int sz_hard = MeasureText("Hard", font_size);
	dfc.x += dfc.width+icon_pad; dfc.width = sz_hard*1.2+icon_pad;
	bool btn_hard = GuiButton(dfc, "Hard");

	int sz_impo = MeasureText("Impossible", font_size);
	dfc.x += dfc.width+icon_pad; dfc.width = sz_impo*1.2+icon_pad;
	bool btn_impo = GuiButton(dfc, "Impossible");

	if 		(btn_easy) difficulty = DFC_EASY;
	else if (btn_medi) difficulty = DFC_MEDI;
	else if (btn_hard) difficulty = DFC_HARD;
	else if (btn_impo) difficulty = DFC_IMPO;

	if (btn_easy || btn_medi || btn_hard || btn_impo || restart) {
		BoardGenerate(&board, DifficultySquare[difficulty], DifficultyMines[difficulty]);
		reload = true;
	}

	//Board Info
	char txt[20];
	snprintf(txt, sizeof(txt), "%d / %d", (board.state != ST_CLEAR) ? board.flags : DifficultyMines[difficulty], DifficultyMines[difficulty]);
	int txt_width = MeasureText(txt, font_size*2)*1.2 + icon_pad;
	Color info_col = (board.state != ST_CLEAR) ? RED : GREEN;
	DrawText(txt, (win_width/2) - (txt_width/2), icon_pad, font_size*2, info_col);


	char *dfc_txt;
	dfc_txt = "Impossible";
	if (difficulty == DFC_EASY) dfc_txt = "Easy";
	else if (difficulty == DFC_MEDI) dfc_txt = "Medium";
	else if (difficulty == DFC_HARD) dfc_txt = "Hard";
	DrawText(dfc_txt, icon_pad, icon_pad, font_size*2, info_col);

}

/*
void InterfaceInteract(int mx, int my) {
	LOG_ONCE("TODO\n");
}
*/
