/* Nem_PutRowRam - Nemesis row writer for RAM: store one 8-pixel row and advance.
 *
 * row: the row (8 pixels, 4 bits each).  rowsLeft: the rows still to come, counted down.
 * dest: where it goes; moves past it.
 * Returns 1 when that was the last row: the decode is over. Hand-written 68000 called by
 * Nem_DecodeLoop through a3 (Nem_Decode mode 0; no caller in the game uses that mode), with
 * registers (d4 the row, d2 the count, a4 the destination): on the last row it drops its own
 * return address and returns from the decode loop to Nem_Decode (game/runtime/asm_conv.cpp
 * adapts it). */
#include "xmen2.h"

char Nem_PutRowRam(unsigned long row, short *rowsLeft, u32 **dest)
{
	*(*dest)++ = row;
	if (--*rowsLeft == 0)
		return 1;
	return 0;
}
