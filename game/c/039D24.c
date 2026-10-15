/* BanimazaDoor_PushCallback - the floor-segment hook of the doors' segments (type $1C, installed
 * by BanimazaDoor_ClassInit): on message 3, an object whose body box overlaps the segment's
 * rectangle is pushed 2 px out, to the side of the column's middle it is on.
 *
 * msg: the segment message.  seg: the door's segment (x1/y1 - x2/y2 = the column's rectangle).
 * obj: the object.  c: its contact record (push-out written to pushX/push2X, 0 in y).
 * Returns 0 for other messages, 1 no overlap, 2 pushed. */
#include "xmen2.h"

short BanimazaDoor_PushCallback(short msg, register Seg *seg, Actor *obj, register SegContact *c)
{
	short px;
	short mid;

	if (msg == 3) {
		if (obj->boxLeft <= seg->x2 && obj->boxRight >= seg->x1
		    && obj->boxTop <= seg->y2 && obj->boxBottom >= seg->y1) {
			px = obj->x >> 8;
			mid = (seg->x1 + seg->x2) >> 1;
			if (px < mid) {
				c->pushX = -2;
				c->push2X = -2;
			} else {
				c->pushX = 2;
				c->push2X = 2;
			}
			c->pushY = 0;
			c->push2Y = 0;
			return 2;
		}
		return 1;
	}
	return 0;
}
