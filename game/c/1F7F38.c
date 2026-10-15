/* Obj_SnapToNearestFloor - stand an object on the nearest floor of a type.
 *
 * obj: the object (any Actor class; Blast_Init).  type: the segment type, or 1001 any type, or
 * 1000 the rails ($21-$24).  pos: where along the floor (pixels; negative: from its far end),
 * -1: nearest to the object.
 * Like Obj_SnapToNearestSeg, among the floors only (flat types $13-$1C, or y1 == y2), the distance
 * being |y - y1| plus how far x lies outside x1..x2. Returns the scan's cursor (the end of the
 * segments, not the floor found; Blast_Init ignores it). */
#include "xmen2.h"

#define OBJ	((Actor *)obj)

Seg *Obj_SnapToNearestFloor(void *obj, short type, short pos)
{
	short d;
	short best = 20000;
	short x = OBJ->x >> 8;
	short y = OBJ->y >> 8;
	register Seg *s = g_Segs;
	register Seg *found = 0;
	register long i;
	register long t;

	for (i = 0, s = g_Segs; i <= g_SegHighWater; i++, s++) {
		t = s->type;
		if ((s->flags & 0x80)
		    && (type == t || type == 1001
			|| (type == 1000 && (t == 0x21 || t == 0x22 || t == 0x23 || t == 0x24)))
		    && ((s->type >= 0x13 && s->type <= 0x1C) || s->y1 == s->y2)) {
			d = ABS(y - s->y1) + (x < s->x1 ? s->x1 - x : x < s->x2 ? 0 : x - s->x2);
			if (d < best) {
				best = d;
				found = s;
			}
		}
	}
	if (found) {
		OBJ->flags &= ~0x1000;
		OBJ->seg = found;
		if (pos == -1)
			Seg_ComputeObjPos(found, obj);
		else
			OBJ->segPos = (pos >= 0 ? pos : found->length + pos) << 8;
		Seg_ApplyObjPos(obj);
		Obj_UpdateBoxes(obj);
	}
	return s;
}
