/* Obj_SnapToNearestSeg - stand an object on the nearest floor segment of a type.
 *
 * obj: the object (any Actor class; Spark_StateStart).  type: the segment type, or 1001 any type,
 * or 1000 the rails ($21-$24).  pos: where along the segment (pixels), -1: nearest to the object.
 * Of the segments in use (g_Segs up to g_SegHighWater, flags bit 7), the first whose midpoint is
 * nearest (|dx| + |dy|, under 20000) to the object's pixel position: the object walks on it (flags
 * $1000 off) at pos (or Seg_ComputeObjPos), placed there with its boxes recomputed. Nothing when
 * there is none. Never runs in the corpus. */
#include "xmen2.h"

#define OBJ	((Actor *)obj)

void Obj_SnapToNearestSeg(void *obj, register short type, short pos)
{
	short i;
	short d;
	short best = 20000;
	register Seg *found = 0;
	register short x = OBJ->x >> 8;
	register short y = OBJ->y >> 8;
	register Seg *s = g_Segs;
	register long t;

	for (i = 0, s = g_Segs; i <= g_SegHighWater; i++, s++) {
		t = s->type;
		if ((s->flags & 0x80)
		    && (type == t || type == 1001
			|| (type == 1000 && (t == 0x21 || t == 0x22 || t == 0x23 || t == 0x24)))) {
			d = ABS(((s->x1 + s->x2) >> 1) - x) + ABS(((s->y1 + s->y2) >> 1) - y);
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
			OBJ->segPos = pos << 8;
		Seg_ApplyObjPos(obj);
		Obj_UpdateBoxes(obj);
	}
}
