/* FluxBomb_HitTilesInArea - run the map-tile hooks (g_TileHooks) of the 32-px blocks in a
 * rectangle around a flux bomb (FluxBomb_DamageArea), spiralling out from the bomb's block.
 *
 * self:  the bomb (passed to the hooks).
 * left, top, right, bottom: the rectangle (world pixels); its block range is cut to the blocks
 *        on screen (12 x 9 from the camera's).
 * The walk is a square spiral of growing rings around the bomb's block, as far as the farther
 * edge of the range (twice its distance in blocks); each block of the solid map (g_SolidMapIsBg)
 * inside the range with a hook type runs hook(column, row, self, -1, -1), each hook type until one
 * returns 0. Hook types 1-7 only (type 0's flag is never set). */
#include "xmen2.h"

/* The map word of the solid map's block (col, row) */
#define CELL(col, row) (g_SolidMapIsBg \
	? &g_BgBlockMap[g_BgRowTable[row] + (col)] \
	: &g_FgBlockMap[g_FgRowTable[row] + (col)])

void FluxBomb_HitTilesInArea(FluxBomb *self, short left, short top, short right, short bottom)
{
	unsigned short side;
	short c0 = left >> 5;
	short r0 = top >> 5;
	short c1 = (right >> 5) + 1;
	short r1 = (bottom >> 5) + 1;
	register short camC = g_CameraX >> 5;
	register short camR = g_CameraY >> 5;
	short cmax = camC + 11;
	short rmax = camR + 8;
	register short col = self->x >> 13;
	register short row = self->y >> 13;
	char more[8];
	short ring = 0;
	short rings = 0;
	register TileHookFn hook;
	register s16 *cell;

	side = 7;
	while (side) {
		more[side] = 1;
		--side;
	}
	if (c0 < camC)
		c0 = camC;
	if (r0 < camR)
		r0 = camR;
	if (c1 > cmax)
		c1 = cmax;
	if (r1 > rmax)
		r1 = rmax;
	{
		register short cx = (c0 + c1) >> 1;
		short cy = (r0 + r1) >> 1;
		register short dx = col > cx ? col - c0 : c1 - col;
		register short dy = row > cy ? row - r0 : r1 - row;

		dx <<= 1;
		dy <<= 1;
		rings = MAX(dx, dy);
	}
	do {
		long step = 1;

		++ring;
		if (ring & 1)
			step = -step;
		side = 2;
		while (side) {
			register long n = ring;

			while (n) {
				if (side & 2)
					col += (short)step;
				else
					row += (short)step;
				if (col >= c0 && col <= c1 && row >= r0 && row <= r1) {
					register short type;

					cell = CELL(col, row);
					type = (*cell & 0x3800) >> 11;
					if (type && (hook = g_TileHooks[type]) && more[type])
						more[type] = (char)(*hook)(col, row, self, -1, -1);
				}
				--n;
			}
			--side;
		}
	} while (ring < rings);
}
