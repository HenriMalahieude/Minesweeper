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

	//Global Options
	Rectangle heat_btn = {win_width - button_sz - icon_pad, win_heigh - button_sz - icon_pad, button_sz, button_sz};
	bool heat_see = GuiButton(heat_btn, "");
	int heat_icon = (heat != HT_NONE) ? ICON_EYE_ON : ICON_EYE_OFF;
	GuiDrawIcon(heat_icon, heat_btn.x+icon_pad, heat_btn.y+icon_pad, icon_sz, BLACK);

	if (heat_see) {
		//LOG("HEAT_SEE\n");
		if (heat == HT_NONE) {heat = HT_MAXIMUM; BoardHeatCalculate(&board); }
		else				 heat = HT_NONE;
	}

	if (heat != HT_NONE) {
		heat_btn.x -= button_sz + icon_sz;
		bool heat_type_change = GuiButton(heat_btn, "");
		heat_icon = (heat != HT_ADDITIVE) ? ICON_BREAKPOINT_ON : ICON_BREAKPOINT_OFF;
		GuiDrawIcon(heat_icon, heat_btn.x+icon_pad, heat_btn.y+icon_pad, icon_sz, BLACK);

		if (heat_type_change) {
			if (heat == HT_ADDITIVE) heat = HT_MAXIMUM;
			else					 heat = HT_ADDITIVE;

			BoardHeatCalculate(&board);
		}
	}
}

static const unsigned int flag_icon[8] = {
	0x00C00000,
	0x01B001E0,
	0x018C0198,
	0x018001FC,
	0x01800180,
	0x01800180,
	0x1FF80180,
	0x00003FFC,
};

static const unsigned int plus_icon[8] = {
	0x00000000,
	0x03C003C0,
	0x03C003C0,

	0x3FFC3FFC,

	0x3FFC3FFC,

	0x03C003C0,
	0x03C003C0,
	0x00000000,
};

static const unsigned int lt_icon[8] = {
	0x0E001C00,
	0x03800700,
	0x00E001C0,
	0x00380070,

	0x00700038,
	0x01C000E0,
	0x07000380,
	0x1C000E00,
};

void IconInit() {
	unsigned int *icons = GuiGetIcons();

	//Flag
	int offset = ICON_CROSS * 8;
	for (int i = 0; i < 8; i++)
		icons[offset + i] = flag_icon[i];

	//Plus
	offset = ICON_BREAKPOINT_OFF * 8;
	for (int i = 0; i < 8; i++)
		icons[offset+i] = plus_icon[i];

	//Less Than
	offset = ICON_BREAKPOINT_ON * 8;
	for (int i = 0; i < 8; i++)
		icons[offset+i] = lt_icon[i];
}

/*
void InterfaceInteract(int mx, int my) {
	LOG_ONCE("TODO\n");
}
*/
