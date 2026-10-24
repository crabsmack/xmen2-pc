/* Tile_RunHooksAtPoints - run the map-tile hooks (g_TileHooks) under an object's hit points
 * (XPlayer_Update: a player's attack hitting breakable map tiles).
 *
 * obj:    the object; its sprite's flips mirror the points, its frame's hitFlags bit 0 says
 *         whether the list is one rectangle.
 * points: the frame's hit-point list: (dx, dy) byte pairs ended by (0, 0), or the rectangle
 *         (x1, y1, x2, y2), from the object's position.
 * x, y:   the object's position in pixels (for the pairs; the rectangle uses its own x/y).
 * Pairs: each point's 32-px block (in the solid map: g_SolidMapIsBg) with a hook type runs
 * hook(column, row, obj, dx, dy). Rectangle: every block it covers runs hook(column, row, obj,
 * -1, -1), each hook type until one returns 0 (the result says go on).
 * Returns 1 if any hook ran.
 *
 * MPW: the ROM keeps its copy of py on the stack in the v-flipped arm of the rectangle, in the
 * h-flip arms' stack temp for px (-$1C), and in d7 in the other arm: x1 (d7) is kept live
 * through the v-flipped arm by the dead use there, so its copy of py has no free register
 * (xmen2disasm docs/COMPILER.md, "Copies of stack variables"). The dead use also names y1 and y2,
 * which keeps the ROM's register ranking. */
#include "xmen2.h"

/* 1 if any of bits m of v are set (the original's materialized form). */
#define BIT(v, m)	((v) & (m) ? 1 : 0)
/* The hook type (map bits $3800) of the solid map's block (col, row). */
#define HOOK_TYPE(col, row) (g_SolidMapIsBg \
	? (g_BgBlockMap[g_BgRowTable[row] + (col)] & 0x3800) >> 11 \
	: (g_FgBlockMap[g_FgRowTable[row] + (col)] & 0x3800) >> 11)
#define SWAP(a, b)	((a) ^= (b), (b) ^= (a), (a) ^= (b))

char Tile_RunHooksAtPoints(void *obj, char *points, short x, short y)
{
	short w;
	short h;
	char hflip;
	char vflip;
	char any;
	char more[8];
	register unsigned char i;
	register short col;
	register short row;
	register short type;
	register SprInst *spr;
	register SprFrame *fr;

	w = g_SolidMapIsBg ? g_BgMapW : g_FgMapW;
	h = g_SolidMapIsBg ? g_BgMapH : g_FgMapH;
	spr = &((Actor *)obj)->spr;
	i = spr->flags;
	hflip = BIT(i, 0x10);
	vflip = BIT(i, 8);
	any = 0;
	i = 0;
	while (i < 8) {
		more[i] = 1;
		i++;
	}
	fr = SPR_FRAME(((Actor *)obj)->spr.def, ((Actor *)obj)->spr.frame);
	if (BIT(fr->hitFlags, 1)) {
		short left;
		short top;
		short right;
		short px;
		short py;
		register short x1;
		register short y1;
		register short x2;
		register short y2;
		register short bottom;
		register TileHookFn hook;

		px = ((Actor *)obj)->x >> 8;
		py = ((Actor *)obj)->y >> 8;
		x1 = *points++;
		y1 = *points++;
		x2 = *points++;
		y2 = *points++;
		if (x1 > x2)
			SWAP(x1, x2);
		if (y1 > y2)
			SWAP(y1, y2);
		if (hflip) {
			left = px - x2;
			right = px - x1;
		} else {
			left = px + x1;
			right = px + x2;
		}
		if (vflip) {
			top = py - y2;
			bottom = py - y1;
			if (0) (void)(x1 + y1 + y2);
		} else {
			top = py + y1;
			bottom = py + y2;
		}
		left >>= 5;
		right >>= 5;
		top >>= 5;
		bottom >>= 5;
		for (col = left; col <= right; col++)
			for (row = top; row <= bottom; row++)
				if (col >= 0 && col < w && row >= 0 && row < h)
					if ((type = HOOK_TYPE(col, row)) && (hook = g_TileHooks[type]) && more[type]) {
						more[type] = (char)(*hook)(col, row, obj, -1, -1);
						any = 1;
					}
	} else {
		short dx;
		register short dy;
		register TileHookFn hook;

		for (;;) {
			dx = *points++;
			dy = *points++;
			if (!dx && !dy)
				break;
			if (hflip)
				dx = -dx;
			if (vflip)
				dy = -dy;
			col = (x + dx) >> 5;
			row = (y + dy) >> 5;
			if (col < 0 || col >= w || row < 0 || row >= h)
				continue;
			if (!(type = HOOK_TYPE(col, row)))
				continue;
			if (!(hook = g_TileHooks[type]))
				continue;
			(*hook)(col, row, obj, dx, dy);
			any = 1;
		}
	}
	return any;
}
