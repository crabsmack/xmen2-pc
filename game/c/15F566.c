/* Nem_PutRowVdp - Nemesis row writer for VRAM: one 8-pixel row to the VDP data port.
 *
 * row: the row (8 pixels, 4 bits each).  rowsLeft: the rows still to come, counted down.
 * Returns 1 when that was the last row: the decode is over. Hand-written 68000 called by
 * Nem_DecodeLoop through a3 (Nem_Decode mode 1), with registers (d4 the row, d2 the count, a4 =
 * the VDP data port): on the last row it drops its own return address and returns from the
 * decode loop to Nem_Decode (game/runtime/asm_conv.cpp adapts it). */
#include "xmen2.h"

char Nem_PutRowVdp(unsigned long row, short *rowsLeft)
{
	VDP_DATA32 = row;
	if (--*rowsLeft == 0)
		return 1;
	return 0;
}
