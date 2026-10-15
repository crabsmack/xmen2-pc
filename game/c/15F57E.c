/* Nem_PutRowMasked - Nemesis row writer for VRAM through a tile mask: write one 8-pixel row to
 * the VDP data port, or leave it out when its tile isn't wanted.
 *
 * row: the row (8 pixels, 4 bits each).  rowsLeft: the rows still to come, counted down.
 * skip: whether the current tile is left out; at each tile's end (8 rows: g_NemTileRows) the
 * next tile's bit in mask decides it.  mask: one bit per tile (bit n & 7 of byte n >> 3) of the
 * decoded tile set, set for the tiles to write: the written ones land in VRAM one after the
 * other (Level_LoadUsedTiles loads only the tiles a level uses).
 * Returns 1 when that was the last row: the decode is over (the tile counters are left as
 * they are then). Hand-written 68000 called by Nem_DecodeLoop through a3 (Nem_Decode mode 2,
 * which sets the counters to 8 rows, tile 0, and writes tile 0), with registers (d4 the row, d2
 * the count, a4 = the VDP data port) and the mask at $1C(a6), Nem_Decode's last argument. The
 * choice for the next tile is the a3 it leaves: this entry, or the inner one after the write
 * ($15F580) for a skipped tile (skip here). On the last row it drops its own return address
 * and returns from the decode loop to Nem_Decode (game/runtime/asm_conv.cpp adapts it). */
#include "xmen2.h"

#define g_NemTileRows	(*(s16 *)g_SceneVar3B04)	/* scene variable: rows left in the tile */
#define g_NemTileIndex	(*(s16 *)g_SceneVar3B06)	/* scene variable: the tile being decoded */

char Nem_PutRowMasked(unsigned long row, short *rowsLeft, char *skip, u8 *mask)
{
	unsigned short tile;

	if (!*skip)
		VDP_DATA32 = row;
	/* $15F580: the entry for a skipped tile */
	if (--*rowsLeft == 0)
		return 1;
	if (--g_NemTileRows == 0) {
		g_NemTileIndex++;
		g_NemTileRows = 8;
		tile = g_NemTileIndex;
		*skip = !(mask[(unsigned short)((short)tile >> 3)] & 1 << (tile & 7));
	}
	return 0;
}
