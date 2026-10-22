/* Scaler_EmitSprites - show the scaled picture (the scaler's VRAM tiles, g_ScalerVram) as a grid
 * of hardware sprites of up to 4x4 tiles (32x32 px), the tiles column by column.
 *
 * x, y: the picture's sprite coordinates less 128 (the grid starts at x + 128, y + 128).
 * pal: the palette line (low 2 bits).  prio: high priority.
 * Each piece takes the next free slot of the sprite table (the link byte at g_SprWritePtr; it
 * links to the slot after it), unless it is outside 96..448 x 96..352. The tile index advances by
 * each piece's tiles (Tbl_SprSizeTilesGrid) whether it is shown or not
 * (Scaler_EmitSprites_Loop is the piece loop). Returns the sprites written, -1 (and stops) when
 * the table is full (80).
 * Hand-written 68000 behind a C frame (ror, bset, pc-relative tables): it can't byte-match. */
#include "xmen2.h"

short Scaler_EmitSprites(short x, short y, short pal, char prio)
{
	short count = 0;
	unsigned short attr;
	short rowsLeft, colsLeft;
	short dx, dy;

	attr = (unsigned short)((pal & 3) << 13) + ((unsigned short)g_ScalerVram >> 5);
	if (prio)
		attr |= 0x8000;
	rowsLeft = g_ScalerParams.tilesH;
	dy = 0x80;
	do {
		short h = (rowsLeft > 4 ? 4 : rowsLeft) - 1;

		colsLeft = g_ScalerParams.tilesW;
		dx = 0x80;
		do {
			short w = (colsLeft > 4 ? 4 : colsLeft) - 1;
			short size = h | ((u8 *)CODEPTR(Tbl_GridWidthBits))[w];
			short tiles = ((u8 *)CODEPTR(Tbl_SprSizeTilesGrid))[size];
			short sx = x + dx;
			short sy = y + dy;

			if (sx >= VIEW_L(0x60) && sx <= VIEW_R(0x1C0) && sy >= 0x60 && sy <= 0x160) {	/* Scaler_EmitSprites_Loop */
				unsigned short slot = (u8)g_SprWritePtr->link;
				HwSprite *s;

#if defined(XMEN2_NATIVE)
				if (slot == SPR_STOP) {	/* phase 7: the raised stop; the piece it leaves out counted */
					plat_spr_dropped(1);
					return -1;
				}
#else
				if (slot == 0x50)
					return -1;
#endif
				g_SprIndex = slot;
				s = &g_SpriteTable[slot];
				g_SprWritePtr = s;
				s->y = sy;
				s->size = (u8)size;
				s->link = (s8)(slot + 1);
				s->attr = attr;
				s->x = sx;
				count++;
			}
			attr += tiles;
			dx += 0x20;
			colsLeft -= 4;
		} while (colsLeft > 0);
		dy += 0x20;
		rowsLeft -= 4;
	} while (rowsLeft > 0);
	return count;
}
