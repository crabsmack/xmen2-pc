/* Obj_RideRail - move an object along a rail (floor segment types '$', '#', '!', '"') for some
 * steps, going on to the linked segments at the ends.
 *
 * obj: the object (any Actor class), on its rail (seg).  steps: frames to move.
 * Nothing without a segment or without speed. Each step: '$' moves y by vy ('#' against it), '!'
 * moves x by vx ('"' against it), and segPos along the rail by the same amount; the other axis
 * follows the slope (slope * the other speed >> 8). Past the rail's length the object goes onto
 * the next segment at the rail's far end (segPos 0); below 0 onto the previous one at the near
 * end (segPos its length): which endpoint is which depends on the type and, for '$'/'#', on
 * which way the rail runs. At an end with no link it stays where it is.
 * Bug kept: the rail's record is read once, before the loop, so steps after a change of segment
 * still use the first one (only the object's seg changes). Never runs in the corpus. */
#include "xmen2.h"

/* At the rail's end (past its length): on to the next segment, standing at (ex, ey); at its start
   (segPos < 0): back to the previous one at (sx, sy), segPos at its far end. */
#define RAIL_ENDS(o, s, ex, ey, sx, sy) \
	if ((s)->length << 8 < (o)->segPos) { \
		if ((s)->next != 0xFF) { \
			(o)->x = (ex) << 8; \
			(o)->y = (ey) << 8; \
			(o)->seg = g_Segs + (s)->next; \
			(o)->segPos = 0; \
		} \
	} else if ((o)->segPos < 0) { \
		if ((s)->prev != 0xFF) { \
			(o)->x = (sx) << 8; \
			(o)->y = (sy) << 8; \
			(o)->seg = g_Segs + (s)->prev; \
			(o)->segPos = (o)->seg->length << 8; \
		} \
	}

void Obj_RideRail(void *obj, register short steps)
{
	Actor *o = (Actor *)obj;
	register Seg *s = o->seg;

	if (!s || (!o->vx && !o->vy))
		return;
	for (;;) {
		if (steps <= 0)
			break;
		if (s->type == '$') {
			o->y += o->vy;
			o->segPos += o->vy;
			if (s->slope)
				o->x += o->vx * s->slope >> 8;
			if (s->y2 > s->y1) {
				RAIL_ENDS(o, s, s->x2, s->y2, s->x1, s->y1)
			} else {
				RAIL_ENDS(o, s, s->x1, s->y1, s->x2, s->y2)
			}
		} else if (s->type == '#') {
			o->y -= o->vy;
			o->segPos += o->vy;
			if (s->slope)
				o->x -= o->vx * s->slope >> 8;
			if (s->y2 < s->y1) {
				RAIL_ENDS(o, s, s->x2, s->y2, s->x1, s->y1)
			} else {
				RAIL_ENDS(o, s, s->x1, s->y1, s->x2, s->y2)
			}
		} else if (s->type == '!') {
			o->x += o->vx;
			o->segPos += o->vx;
			if (s->slope)
				o->y += o->vy * s->slope >> 8;
			RAIL_ENDS(o, s, s->x2, s->y2, s->x1, s->y1)
		} else if (s->type == '"') {
			o->x -= o->vx;
			o->segPos += o->vx;
			if (s->slope)
				o->y -= o->vy * s->slope >> 8;
			RAIL_ENDS(o, s, s->x1, s->y1, s->x2, s->y2)
		}
		steps--;
	}
}
