/* Seg_ApplyObjPos - place a walker on its floor segment: x/y from its position along it.
 *
 * obj: the walker (any Actor class), on seg at segPos (8 fraction bits).
 * The segment type's hook (g_SegTypeHooks, message 6) runs first and does it all when it returns
 * non-zero. Else, with pos = segPos >> 8 and rise = slope * pos >> 8, from the endpoints by type
 * (Seg_ApplyObjPos_Cases):
 *   3, $E, $F, $21 floors: (x1 + pos, y1 + rise);   $13-$1C flat: x = x1 + pos, y = y1;
 *   4, $22 reversed: (x2 - pos, y2 - rise);
 *   1, $24 walls: y = pos from the upper end, x = rise from the same end;
 *   2, $23 walls: a vertical one y2 - pos (x = x1); else pos up from the lower end;
 *   5-$D: nothing (returns); others: (x1, y1).
 * Then x/y = those pixels << 8 (Seg_ApplyObjPos_Store). Nothing when it stands on no segment. */
#include "xmen2.h"

void Seg_ApplyObjPos(void *obj)
{
	register Actor *o = (Actor *)obj;
	register Seg *s = o->seg;
	short x, y;
	short pos;
	short rise;

	if (!s)
		return;
	if ((g_SegHookTmp = g_SegTypeHooks[s->type]) ? g_SegHookTmp(6, s, o, 0) : 0)
		return;
	x = s->x1;
	y = s->y1;
	pos = o->segPos >> 8;
	rise = pos * s->slope >> 8;
	switch (s->type) {
	case 5: case 6: case 7: case 8: case 9: case 0xA: case 0xB: case 0xC: case 0xD:
		return;
	case 3: case 0xE: case 0xF: case 0x21:
		/* Seg_ApplyObjPos_Cases */
		y += rise;
		x += pos;
		break;
	case 0x13: case 0x14: case 0x15: case 0x16: case 0x17:
	case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1C:
		x += pos;
		break;
	case 4: case 0x22:
		x = s->x2 - pos;
		y = s->y2 - rise;
		break;
	case 1: case 0x24:
		if (s->y1 > s->y2) {
			y = s->y2 + pos;
			x = s->x2 + rise;
		} else {
			y += pos;
			x += rise;
		}
		break;
	case 2: case 0x23:
		if (s->x1 == s->x2)
			y = s->y2 - pos;
		else if (s->y1 > s->y2) {
			y -= pos;
			x -= rise;
		} else {
			y = s->y2 - pos;
			x = s->x2 - rise;
		}
		break;
	}
	/* Seg_ApplyObjPos_Store */
	o->x = (long)x << 8;
	o->y = (long)y << 8;
}
