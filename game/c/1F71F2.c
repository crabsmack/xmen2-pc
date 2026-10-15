/* Seg_ObjPosUnclamped - set an object's position along a floor segment from its x or y.
 *
 * seg: the segment; obj: the object (any Actor class).
 * Seg_ComputeObjPos without the clamp and without halving vx: the hook (message 5) first, then
 * segPos = (x - x1) << 8 for floors (Seg_ObjPosUnclamped_Horiz: 3, $E, $F, $13-$1C, $21),
 * (x2 - x) << 8 reversed (_Reverse: 4, $22), (y - y1) << 8 for walls 1/$24, (y2 - y) << 8 for
 * walls 2/$23; others unchanged (_Done). */
#include "xmen2.h"

void Seg_ObjPosUnclamped(Seg *seg, void *obj)
{
	register Seg *s = seg;
	register Actor *o = (Actor *)obj;

	if ((g_SegHookTmp = g_SegTypeHooks[s->type]) ? g_SegHookTmp(5, s, o, 0) : 0)
		return;
	switch (s->type) {
	default:
		break;
	case 3: case 0xE: case 0xF: case 0x21:
	case 0x13: case 0x14: case 0x15: case 0x16: case 0x17:
	case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1C:
		/* Seg_ObjPosUnclamped_Horiz */
		o->segPos = ((o->x >> 8) - s->x1) << 8;
		break;
	case 4: case 0x22:
		/* Seg_ObjPosUnclamped_Reverse */
		o->segPos = (s->x2 - (o->x >> 8)) << 8;
		break;
	case 1: case 0x24:
		o->segPos = ((o->y >> 8) - s->y1) << 8;
		break;
	case 2: case 0x23:
		o->segPos = (s->y2 - (o->y >> 8)) << 8;
		break;
	}
}
