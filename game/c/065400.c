/* Cerebro_ClearTextWindow - fill the Cerebro text window (rows g_TextWinTop..g_TextWinBottom,
 * columns g_TextWinLeft..g_TextWinRight of the 64-column plane at VRAM $E000) with the blank
 * tile $86C0, then home the text cursor. The original swaps the two: the cursor column gets the
 * window's top row and the cursor row its left column (kept). */
#include "xmen2.h"

#define g_TextCol	(*(s16 *)g_SceneVar3B04)	/* scene variable: Cerebro text cursor column */
#define g_TextRow	(*(s16 *)g_SceneVar3B06)	/* scene variable: Cerebro text cursor row */

/* The VDP control word for a VRAM write at byte address a, as the original summed it. */
#define VRAM_WRITE(a)	((((a) & 0x3FFF) << 16) + 0x40000000 + ((unsigned long)((a) & 0xC000) >> 14))
/* The byte address of a cell of the 64-column plane at $E000. */
#define TEXT_CELL(row, col)	(((row) << 7) + 0xE000 + ((col) << 1))

void Cerebro_ClearTextWindow(void)
{
	register short row;
	register short col;

	for (row = g_TextWinTop; row <= g_TextWinBottom; ++row) {
		VDP_CTRL32 = VRAM_WRITE(TEXT_CELL(row, g_TextWinLeft));
		for (col = g_TextWinLeft; col <= g_TextWinRight; ++col)
			VDP_DATA = 0x86C0;
	}
	g_TextCol = g_TextWinTop;
	g_TextRow = g_TextWinLeft;
}
