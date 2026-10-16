/* Nem_Decode - decompress Nemesis tile data into RAM, into VRAM, or into VRAM through a tile mask.
 *
 * src:   the compressed data (its first two bytes start the bit buffer).
 * table: the code lookup table Nem_BuildCodeTable built for it.
 * rows:  rows to decode, 8 per tile.
 * mode:  0: to RAM at dest; 1: to VRAM at byte address dest; 2: to VRAM at dest, only the tiles
 *        whose bit is set in mask (the others are decoded and dropped; the kept ones are packed).
 * mask:  the tile bitmap for mode 2 (Level_LoadUsedTiles); not read otherwise.
 * Mode 2 starts the masked writer's counters (g_NemTileRows 8, g_NemTileIndex 0: tile 0 is
 * written). Hand-written 68000 with a C frame: it picks the row writer for Nem_DecodeLoop
 * (Nem_PutRowRam / Nem_PutRowVdp / Nem_PutRowMasked), sets the VDP write address, primes the bit
 * buffer with 16 bits and an empty row, and runs the loop.
 * Two jump-table entries reach it: $4102 (the mode 1 callers, 5 arguments) and $410A
 * (Level_LoadUsedTiles, mode 2, with the mask as a 6th): the masked writer reads the mask from
 * this frame ($1C(a6)). */
#include "xmen2.h"

#define g_NemTileRows	(*(s16 *)g_SceneVar3B04)	/* scene variable: rows left in the tile */
#define g_NemTileIndex	(*(s16 *)g_SceneVar3B06)	/* scene variable: the tile being decoded */

void Nem_Decode(void *src, NemCodeTable *table, long rows, char mode, long dest, NemTileMask *mask)
{
	u8 *s = (u8 *)src;
	unsigned short bits;
	unsigned long fill = 0;		/* the loop's d7: not read before it is set */

	if (mode != 0) {
		if (mode != 1) {
			g_NemTileRows = 8;
			g_NemTileIndex = 0;
		}
		VDP_CTRL32 = ((unsigned long)dest & 0x3FFF) << 16 | 0x40000000 | ((unsigned long)dest >> 14 & 3);
	}
	bits = (unsigned short)(s[0] << 8 | s[1]);
	Nem_DecodeLoop(s + 2, (u16 *)table->code, (short)rows, mode, mode == 0 ? AT68(u32, dest) : 0, (u8 *)mask,
		bits, 16, 8, 0, &fill);
}
