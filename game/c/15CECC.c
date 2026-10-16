/* Tile_TestAt - what the map holds at a point: a tile hook of the asked types (optionally run)
 * or, failing that, a solid block.
 *
 * obj:     the object asking (passed to the hook; may be 0).
 * x, y:    the point in world pixels (32-px blocks).
 * fgMap:   1: the FG block map, 0: the BG one.
 * mask:    bits $3800: the hook types to look for (the block's bits $3800 must be one of them,
 *          and g_TileHooks has a hook for it); $4000: test the block's solid bit.
 * runHook: run the hook found: hook(column, row, obj, -1, -1).
 * Returns 1 for a hook or a solid block, and outside the map; else 0. */
#include "xmen2.h"

char Tile_TestAt(void *obj, short x, short y, register char fgMap, short mask, char runHook)
{
	register short row;
	register char hit;
	register short found;
	register short h;
	register short type;
	short hooks;
	short w;
	short col;
	register s16 *cell;
	register TileHookFn hook;

	w = fgMap ? g_FgMapW : g_BgMapW;
	h = fgMap ? g_FgMapH : g_BgMapH;
	col = x >> 5;
	row = y >> 5;
	hit = 1;
	if (col >= 0 && col < w && row >= 0 && row < h) {
		cell = fgMap ? &g_FgBlockMap[g_FgRowTable[row] + col] : &g_BgBlockMap[g_BgRowTable[row] + col];
		if (0) {	/* MPW: weights h and fgMap to give them d7/d6 as in the ROM; no code */
			h = (short)(h + h + h + h + h + h);
			fgMap = (char)(fgMap + fgMap + fgMap + fgMap);
		}
		hooks = (mask & 0x3800) >> 11;
		hit = 0;
		if (hooks) {
			type = (*cell & 0x3800) >> 11;
			found = type & hooks;
			if ((found = type & hooks) && found == type) {
				hook = g_TileHooks[found];
				if (hook) {
					hit = 1;
					if (runHook)
						(*hook)(col, row, obj, -1, -1);
				}
			}
		}
		if (!hit && (mask & 0x4000))
			hit = *cell & 0x4000 ? 1 : 0;
	}
	return hit;
}
