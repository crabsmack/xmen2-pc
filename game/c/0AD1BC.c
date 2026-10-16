/* SavageA2_SetBlockTiles - the Perimeter's breakable blocks (SavageA2/A3 crack hooks): give the
 * map blocks of the solid block (a type-$14 floor segment) at a map cell a new tile, and with
 * remove, take the block away.
 *
 * col, row: the map cell hit (FG block map).
 * tile:     the block word to write (tile and attribute bits).
 * remove:   take the block away: everything alive standing on it falls (Obj_Detach), the segment
 *           is freed (Seg_Free), the tile loses its priority and solid bits ($8000, $3800), each
 *           cell gets a puff (Explosion_Spawn) and the top row gets edge (with the tile's upper
 *           bits) instead.
 * edge:     the top row's tile when removing.
 * The cells are the 32-px blocks the segment covers (its corners + 16, / 32), redrawn at once
 * (Map_RedrawBlock). Returns 0 (also when there is no block there).
 * MPW: 4 bytes differ: the ROM keeps the object index in d7 (with y), this C gets d6 (with x). */
#include "xmen2.h"

long SavageA2_SetBlockTiles(short col, short row, register short tile, register char remove, short edge)
{
	long ex;
	long ey;
	short top;
	short bottom;
	short left;
	short right;
	register Seg *seg;
	register s16 *p;
	register Actor *obj;
	register long i;
	register short x;
	register short y;

	if (!(seg = Seg_FindInBlock(0x14, col, row)))
		return 0;
	top = (seg->y1 + 0x10) >> 5;
	bottom = (seg->y2 + 0x10) >> 5;
	left = (seg->x1 + 0x10) >> 5;
	right = (seg->x2 + 0x10) >> 5;
	if (remove) {
		for (i = 0, obj = g_ObjTable[0]; i < g_ObjCount; obj = g_ObjTable[++i]) {
			if (IS_ALIVE(obj) && obj->seg == seg)
				Obj_Detach(obj);
		}
		Seg_Free(seg);
		tile &= 0x7FFF;
		tile &= 0xC7FF;
	}
	for (x = left; x < right; x++) {
		for (y = top; y < bottom; y++) {
			p = &g_FgBlockMap[g_FgRowTable[y] + x];
			if (remove) {
				ex = ((x << 5) + 0x10) << 8;
				ey = ((y << 5) + 0x10) << 8;
				Explosion_Spawn(ex, ey, 0);
			}
			if (remove && y == top)
				*p = tile & ~0x1FF | edge;
			else
				*p = tile;
			Map_RedrawBlock(x, y, 0);
		}
	}
	return 0;
}
