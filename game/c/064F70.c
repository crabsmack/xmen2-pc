/* Cerebro_PrintText - print a string into the Cerebro text window (the 64-column plane at VRAM
 * $E000) at the text cursor, word-wrapped: a word that would pass the window's right column
 * starts a new row (the row stops at the window's bottom; leading blanks are then skipped), NEWLINE
 * starts a new row. The text cursor ends after the text.
 *
 * text: the string.  style: 0 white, 1 blue, 2, 3 the other font banks (Cerebro_SetupPalCycles'
 * recoloured fonts: tiles $6C0, $680, $640, $600); 4/5: each word appears in a flash font (tiles
 * $5C0, palette line 3 colour 7 stepping through green or blue shades, 2 frames each), then in
 * white / blue; 6/7: the same letter by letter (typed out, a frame per shade and one after each
 * letter). When the text state is 0 (a wait was skipped) 4-7 print at once as 0/1. Every tile
 * has priority and palette line 3. The flash and the typing run the frame hook and wait for
 * V-blank. Cases of the bank switch: Cerebro_PrintText_part1; its exit Cerebro_PrintText_part2. */
#include "xmen2.h"

#define g_TextState	(*(s16 *)g_SceneVar3B02)	/* scene variable: Cerebro text state (0: a wait was skipped) */
#define g_TextCol	(*(s16 *)g_SceneVar3B04)	/* scene variable: Cerebro text cursor column */
#define g_TextRow	(*(s16 *)g_SceneVar3B06)	/* scene variable: Cerebro text cursor row */

/* A line break in the text: '\n' in MPW C (the Mac's carriage return, $0D). */
#define NEWLINE	0x0D

/* The VDP control word for a VRAM write at byte address a, as the original summed it. */
#define VRAM_WRITE(a)	((((a) & 0x3FFF) << 16) + 0x40000000 + ((unsigned long)((a) & 0xC000) >> 14))
/* The byte address of a cell of the 64-column plane at $E000. */
#define TEXT_CELL(row, col)	(((row) << 7) + 0xE000 + ((col) << 1))

void Cerebro_PrintText(char *text, register short style)
{
	s16 green[6];
	s16 blue[6];
	s16 *flash;
	register short bank;
	register short i;
	register short col;
	register char *start;
	register char *end;

	green[5] = 0x2A2;
	green[4] = 0x4A4;
	green[3] = 0x6A6;
	green[2] = 0x8A8;
	green[1] = 0xAAA;
	green[0] = 0xCCC;
	blue[5] = 0x22A;
	blue[4] = 0x44A;
	blue[3] = 0x66A;
	blue[2] = 0x88A;
	blue[1] = 0xAAA;
	blue[0] = 0xCCC;
	bank = 0;
	flash = green;
	if (style == 1 || style == 5 || style == 7) {
		bank = 1;
		flash = blue;
	}
	if (style == 2)
		bank = 2;
	if (style == 3)
		bank = 3;
	while (*text) {
		if (!g_TextState) {
			if (style == 5 || style == 7) {
				style = 1;
				bank = 1;
			}
			if (style == 4 || style == 6) {
				style = 0;
				bank = 0;
			}
		}
		if (*text == NEWLINE) {
			if (g_TextRow < g_TextWinBottom)
				g_TextRow++;
			g_TextCol = g_TextWinLeft;
			text++;
		} else {
			start = text;
			while (*start == ' ' || *start == NEWLINE || *start == '\t')
				start++;
			end = start;
			while (*end != ' ' && *end != NEWLINE && *end != '\t' && *end)
				end++;
			if (g_TextCol + (end - text - 1) > g_TextWinRight) {
				if (g_TextRow < g_TextWinBottom)
					g_TextRow++;
				g_TextCol = g_TextWinLeft;
				text = start;
			}
			if (style == 6 || style == 7) {
				for (col = g_TextCol, start = text; start != end; ++col, start++) {
					VDP_CTRL32 = VRAM_WRITE(TEXT_CELL(g_TextRow, col));
					VDP_DATA = *start + 0xE5C0 - 0x20;
					for (i = 0; i < 6; i++) {
						g_PalBase[0x37] = flash[i];
						g_PalBaseDirty = 1;
						Cerebro_FrameHook();
						VDP_WaitVBlank();
					}
					VDP_CTRL32 = VRAM_WRITE(TEXT_CELL(g_TextRow, col));
					if (bank)
						VDP_DATA = *start + 0xE680 - 0x20;
					else
						VDP_DATA = *start + 0xE6C0 - 0x20;
					Cerebro_FrameHook();
					VDP_WaitVBlank();
				}
			} else {
				if (style == 4 || style == 5) {
					VDP_CTRL32 = VRAM_WRITE(TEXT_CELL(g_TextRow, g_TextCol));
					for (start = text; start != end; start++)
						VDP_DATA = *start + 0xE5C0 - 0x20;
					for (i = 0; i < 6; i++) {
						g_PalBase[0x37] = flash[i];
						g_PalBaseDirty = 1;
						Cerebro_FrameHook();
						VDP_WaitVBlank();
						Cerebro_FrameHook();
						VDP_WaitVBlank();
					}
				}
				VDP_CTRL32 = VRAM_WRITE(TEXT_CELL(g_TextRow, g_TextCol));
				for (start = text; start != end; start++) {
					switch (bank) {
					case 0:
						VDP_DATA = *start + 0xE6C0 - 0x20;
						break;
					case 1:
						VDP_DATA = *start + 0xE680 - 0x20;
						break;
					case 2:
						VDP_DATA = *start + 0xE640 - 0x20;
						break;
					case 3:
						VDP_DATA = *start + 0xE600 - 0x20;
						break;
					}
				}
			}
			g_TextCol += end - text;
			text = end;
		}
	}
}
