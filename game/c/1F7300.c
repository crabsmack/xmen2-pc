/* Seg_CollideBlock - collide an object's box with a solid block segment (types $13-$1C).
 *
 * obj: the object (any Actor class); seg: the block (its endpoints are the box corners);
 * c: the contact record to fill (pushX / pushY, flags).
 * Returns 1 (no contact: the boxes don't overlap, or a block carried by an object moving up),
 * 3 (the object is above the block: its box bottom at or above the block's top) or 2 (contact).
 * Vertical push, when the object's centre x is over the block: down out of the block's bottom
 * (pushY > 0), or up onto its top (pushY < 0, flags bit 3: landing) when that is the smaller one;
 * horizontal push, unless the box bottom is within 12 px of the top (0 for an object in the air,
 * flags bit $1000): the smaller of the two sides. Pushes are clipped to +-127. A block next to
 * the segment the object stands on doesn't push sideways. With both pushes within the object's
 * width + 3, a contact that already pushed in x (flags bit 1) or y (bit 6) drops that push
 * (unless both); with both still set, the larger one is dropped. cy is computed, never read
 * (the original's). */
#include "xmen2.h"

short Seg_CollideBlock(void *obj, Seg *seg, SegContact *c)
{
	register Seg *s = seg;
	register SegContact *ct = c;
	short margin;
	short top;
	register short bottom;
	register short cx = (((Actor *)obj)->boxLeft + ((Actor *)obj)->boxRight) >> 1;
	short cy = (((Actor *)obj)->boxTop + ((Actor *)obj)->boxBottom) >> 1;
	short width = ((Actor *)obj)->boxRight - ((Actor *)obj)->boxLeft;
	long a, b;

	if (s->y1 >= s->y2) {
		top = s->y2;
		bottom = s->y1;
	} else {
		top = s->y1;
		bottom = s->y2;
	}
	if (bottom <= ((Actor *)obj)->boxTop)
		return 1;
	if (((Actor *)obj)->boxLeft >= s->x2)
		return 1;
	if (((Actor *)obj)->boxRight < s->x1)
		return 1;
	if (top >= ((Actor *)obj)->boxBottom)
		return 3;
	if (cx < s->x2 && cx >= s->x1) {
		a = s->y2 - ((Actor *)obj)->boxTop;
		b = ((Actor *)obj)->boxBottom - s->y1;
		if (a > 127)
			a = 127;
		if (a < -127)
			a = -127;
		if (b > 127)
			b = 127;
		if (b < -127)
			b = -127;
		if (a > 0) {
			if (b != 0 && a < b) {
				if (s->owner != -2 && (ct->flags & 0x40)) {
#ifdef XMEN2_MPW
					if ((s->owner == 0xFF ? 0 : g_ObjTable[s->owner])->vy < 0)
						return 1;
#else
					/* an empty object slot: the 68000 reads the vector table */
					Actor *carrier = (Actor *)g_ObjTable[s->owner];

					if (!carrier)
						carrier = (Actor *)Vectors;
					if (carrier->vy < 0)
						return 1;
#endif
				}
				ct->pushY = a;
			} else {
				ct->flags |= 8;
				ct->pushY = -b;
			}
		}
	}
	margin = 12;
	if ((((Actor *)obj)->flags & 0x1000) != 0)
		margin = 0;
	if (((Actor *)obj)->boxTop < s->y2 && ((Actor *)obj)->boxBottom - margin >= s->y1) {
		a = s->x2 - ((Actor *)obj)->boxLeft;
		b = ((Actor *)obj)->boxRight - s->x1;
		if (a > 127)
			a = 127;
		if (a < -127)
			a = -127;
		if (b > 127)
			b = 127;
		if (b < -127)
			b = -127;
		if (a > 0 && b > 0) {
			if (a < b)
				ct->pushX = a;
			else
				ct->pushX = -b;
		}
	}
	a = ABS(ct->pushX);
	b = ABS(ct->pushY);
	if ((s->prev != 0xFF && &g_Segs[s->prev] == ((Actor *)obj)->seg)
	    || (s->next != 0xFF && &g_Segs[s->next] == ((Actor *)obj)->seg)) {
		a = 0;
		ct->pushX = 0;
	}
	if (a != 0 && a <= width + 3 && b != 0 && b <= width + 3 && (ct->flags & 0x42) != 0x42) {
		if ((ct->flags & 2) == 2) {
			a = 0;
			ct->pushX = 0;
		}
		if ((ct->flags & 0x40) == 0x40) {
			b = 0;
			ct->pushY = 0;
			ct->flags &= ~8;
		}
	}
	if (a != 0 && b != 0) {
		if (a > b) {
			a = 0;
			ct->pushX = 0;
		} else {
			b = 0;
			ct->pushY = 0;
			ct->flags &= ~8;
		}
	}
	if (ct->pushX | ct->pushY)
		return 2;
	return 1;
}
