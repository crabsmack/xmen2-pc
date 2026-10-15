/* AvalonA4_GlobCrackColumnHook - map tile hook 3 of the Throne Room: a MAGNETICGLOB (Magneto's
 * globe) hitting a pillar cracks its three blocks one stage, each with an explosion: series
 * $8C -> $A9 -> $AA -> $AB -> $AC -> $11 (gone) and $9F -> $A3 -> $A4 -> $A5 -> $A6 -> $A7
 * (broken). When a block breaks, the pillar's floor segment (type $14 in the hit block) is
 * freed. Sound $1A.
 *
 * col, row: the block hit.  obj: the object that hit it.
 * The pillar's top: up to 3 blocks up from row while their bits 11-13 are 3 (row itself
 * included; if row isn't one, the column starts at row). Returns 0; other objects do nothing.
 *
 * MPW: the class is compared with the long at +2 of MAGNETICGLOB's jump-table entry, an external
 * data record (AvalonA4_GlobCrackBlockHook). The explosion's coordinates are in a block of their
 * own (do/while (0)): in one basic block with the tile lookup, MPW keeps (long)col and r in
 * temporaries the ROM doesn't have (and a goto label instead stops the hoisting of the first
 * loop's bound into d3). The sound pair's do/while puts its argument pop before the return. */
#include "xmen2.h"

#define PASTE(a, b)		a##b
#ifdef XMEN2_MPW
extern struct { short jmp; Class *cls; } PASTE(Tbl_MagneticGlob, _vtable);
#define MAGNETICGLOB_CLASS	(PASTE(Tbl_MagneticGlob, _vtable).cls)
#else
#define MAGNETICGLOB_CLASS	CLASS(PASTE(Tbl_MagneticGlob, _vtable))
#endif

long AvalonA4_GlobCrackColumnHook(short col, short row, Actor *obj)
{
	char broke;
	register long top, i, r;
	register Seg *seg;

	if (obj->cls != MAGNETICGLOB_CLASS)
		return 0;
	broke = 0;
	top = row;
	for (i = 0, r = row; i < 3; i++, r--) {
		short kind;

		kind = (g_FgBlockMap[g_FgRowTable[r] + col] & 0x3800) >> 11;
		if (kind != 3)
			break;
		top = r;
	}
	for (i = 0, r = top; i < 3; i++, r++) {
		short tile;
		long x, y;

		tile = g_FgBlockMap[g_FgRowTable[r] + col] & 0x1FF;
		do {
			x = ((col << 5) + 16) << 8;
			y = ((r << 5) + 16) << 8;
		} while (0);
		Explosion_SpawnSetTimerWord(x, y, 0, 0);
		if (tile == 0x8C)
			Map_SetBlockFG(col, r, 0xA9, 1);
		else if (tile == 0xA9)
			Map_SetBlockFG(col, r, 0xAA, 1);
		else if (tile == 0xAA)
			Map_SetBlockFG(col, r, 0xAB, 1);
		else if (tile == 0xAB)
			Map_SetBlockFG(col, r, 0xAC, 1);
		else if (tile == 0xAC) {
			Map_SetBlockFG(col, r, 0x11, 0);
			broke = 1;
		} else if (tile == 0x9F)
			Map_SetBlockFG(col, r, 0xA3, 1);
		else if (tile == 0xA3)
			Map_SetBlockFG(col, r, 0xA4, 1);
		else if (tile == 0xA4)
			Map_SetBlockFG(col, r, 0xA5, 1);
		else if (tile == 0xA5)
			Map_SetBlockFG(col, r, 0xA6, 1);
		else if (tile == 0xA6) {
			Map_SetBlockFG(col, r, 0xA7, 0);
			broke = 1;
		}
	}
	if (broke) {
		if (seg = Seg_FindInBlock(0x14, col, row))
			Seg_Free(seg);
	}
	do {
		Snd_Stop(0x1A);
		Snd_Play(0x1A);
	} while (0);
	return 0;
}
