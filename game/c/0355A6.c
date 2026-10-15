/* AvalonA4_CrackFloorHook - map tile hook 4 of the Throne Room: a player character (its class's
 * parent is XCLIMBER or XPLAYER), or no object, cracks the cracked-floor column it touches one
 * stage ($86 -> $87 -> $88 -> broken $85), sound $1A: every block of the type-$14 floor segment
 * in the touched block (rows from its y1 + 8 to its y2 - 8, in column col) becomes the next
 * stage; the broken stage also frees the segment. Then one explosion at the touched block's
 * centre.
 *
 * col, row: the block touched.  obj: the object (0: none).
 * The new stage is set "animated" (Map_SetBlockFG's last argument 1) except the broken one.
 * Another tile than $86-$88 leaves the new stage unset (the 68000 passes whatever d4 held; the
 * hook only runs on its own tiles). A y for the explosion is stepped per row (fy) but never used:
 * the explosion's y is the touched row's. Without a segment, sound only. Returns 0.
 * MPW: obj and col are copied into register locals (the ROM loads a4 before d6). */
#include "xmen2.h"

long AvalonA4_CrackFloorHook(short col, short row, Actor *obj)
{
	short first, last;
	long fx, fy;
	register Actor *o = obj;
	register short c = col;
	register Class *parent;
	register short tile, stage;
	register long r;
	register Seg *seg;
	register char broken, ok;

	ok = 0;
	if (o) {
		parent = o->cls->parent;
		if (parent == CLASS(Tbl_XClimber_vtable) || parent == CLASS(Tbl_XPlayer_vtable))
			ok = 1;
	} else
		ok = 1;
	if (ok) {
		tile = g_FgBlockMap[g_FgRowTable[row] + c] & 0x1FF;
		broken = 0;
		seg = Seg_FindInBlock(0x14, c, row);
		Snd_Stop(0x1A);
		Snd_Play(0x1A);
		if (tile == 0x86)
			stage = 0x87;
		else if (tile == 0x87)
			stage = 0x88;
		else if (tile == 0x88) {
			stage = 0x85;
			broken = 1;
		}
		if (seg) {
			first = (seg->y1 + 8) >> 5;
			last = (seg->y2 - 8) >> 5;
			fx = ((c << 5) + 16) << 8;
			fy = ((first << 5) + 8) << 8;
			for (r = first; r <= last; r++, fy += 0x2000)
				Map_SetBlockFG(c, r, stage, !broken);
			if (broken)
				Seg_Free(seg);
			Explosion_SpawnSetTimerWord(fx, ((row << 5) + 16) << 8, 0, 0);
		}
	}
	return 0;
}
