/* Seg_MoveAlong - move an object a distance along its chain of floor segments.
 *
 * obj: the walker (any Actor class); dist: the distance (8 fraction bits, signed).
 * segPos moves by dist; past the end of seg it carries on onto the next segment (next), before
 * its start onto the previous one (prev). At an end of the chain ($FF) it stops there and sets
 * flags bit $100. Then seg/segPos are stored and Seg_ApplyObjPos places the object. Nothing
 * when it stands on no segment. */
#include "xmen2.h"

void Seg_MoveAlong(void *obj, short dist)
{
	register Actor *o = (Actor *)obj;
	register Seg *s = o->seg;
	register long pos;
	long len;
	short n;

	if (s) {
		pos = o->segPos;
		len = s->length;
		len <<= 8;
		pos += dist;
		for (;;) {
			if (pos > len) {
				n = s->next;
				if (n != 0xFF) {
					pos -= len;
					s = &g_Segs[n];
					len = s->length;
					len <<= 8;
				} else {
					o->flags |= 0x100;
					pos = len;
					break;
				}
			} else if (pos < 0) {
				n = s->prev;
				if (n != 0xFF) {
					s = &g_Segs[n];
					len = s->length;
					len <<= 8;
					pos += len;
				} else {
					o->flags |= 0x100;
					pos = 0;
					break;
				}
			} else
				break;
		}
		o->seg = s;
		o->segPos = pos;
		Seg_ApplyObjPos(o);
	}
}
