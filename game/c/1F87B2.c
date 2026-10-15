/* Seg_CollideObj - test an object's box against one floor segment and fill its contact record.
 *
 * seg: the segment; obj: the object (any Actor class); c: its contact record.
 * The segment type's hook (g_SegTypeHooks, message 3) may decide: a result whose low byte is
 * non-zero is returned as it is (Seg_CollideObj_Ret). Else pushX/pushY are cleared and, by type
 * (Seg_CollideObj_Cases; the surface position along a slope is the slope times the distance of
 * the box centre from the first endpoint):
 *   1, $20 left walls: the box overlapping the wall's height and its left edge up to 64 px into
 *     the wall: pushX > 0 ($20 also raises event 4, flags bit 15);
 *   2, $12, $1F right walls: the same from the right, pushX < 0 ($1F event 4; $12 ends the scene,
 *     Scene_EndAfter(1), and frees itself: no contact);
 *   3, $E, $F floors (_CeilFloor): with the box centre over it and the box bottom up to 64 px
 *     into it, pushY < 0 and flags bit 3 (landing); a box above it gives 3. Not while standing
 *     on a type-3 floor. $E: players only, raising event 2 instead; $F: others only, with event 3;
 *   4 ceiling: the box top up to 64 px into it, pushY > 0;
 *   $13-$1C solid blocks: Seg_CollideBlock;   $1D trigger box: a player inside raises event 2.
 * Returns 1 no contact (Seg_CollideObj_RetNone), 2 contact (Seg_CollideObj_RetHit), 3 above a
 * floor.
 * Hand-written 68000 in an MPW frame (a2, exg, a jump table of its own): it can't byte-match. */
#include "xmen2.h"

/* where the surface is at distance d along the segment: slope * d >> 8, with slopes of
   exactly +-1 (+-$100) and 0 done without the multiply */
#define SLOPE_OFS(s, d) \
	((s)->slope == 0 ? 0L : (s)->slope == 0x100 ? (long)(short)(d) \
	 : (s)->slope == -0x100 ? (long)(short)-(d) : (long)(s)->slope * (short)(d) >> 8)

short Seg_CollideObj(Seg *seg, void *obj, SegContact *c)
{
	Seg *s = seg;
	Actor *o = (Actor *)obj;
	SegHookFn hook;
	short r;
	short cx, cy, lo, hi, t;
	long push;

	hook = g_SegTypeHooks[(u8)s->type];
	r = 0;
	if (hook)
		r = hook(3L, s, o, c);
	if ((char)r)
		return r;	/* Seg_CollideObj_Ret */
	cx = (short)(o->boxLeft + o->boxRight) >> 1;
	cy = (short)(o->boxTop + o->boxBottom) >> 1;
	lo = s->y1;
	hi = s->y2;
	if (hi < lo) {
		t = lo;
		lo = hi;
		hi = t;
	}
	c->pushX = 0;
	c->pushY = 0;
	switch ((u8)s->type) {
	case 1: case 0x20:
		if (hi <= o->boxTop || lo >= o->boxBottom)
			return 1;
		push = SLOPE_OFS(s, cy - s->y1) + (short)(s->x1 - o->boxLeft);
		if (push <= 0 || push > 64)
			return 1;
		c->pushX = (s8)push;
		if (s->type == 0x20) {
			c->event = 4;
			c->flags |= 0x8000;
		}
		return 2;
	case 2: case 0x12: case 0x1F:
		if (hi <= o->boxTop || lo >= o->boxBottom)
			return 1;
		push = SLOPE_OFS(s, cy - s->y1) + (short)(s->x1 - o->boxRight);
		if (push >= 0 || push < -64)
			return 1;
		c->pushX = (s8)push;
		if (s->type == 0x1F) {
			c->event = 4;
			c->flags |= 0x8000;
			return 2;
		}
		if (s->type == 0x12) {
			Scene_EndAfter(1);
			s->flags = 0;
			return 1;
		}
		return 2;
	case 0x13:
	case 0x14: case 0x15: case 0x16: case 0x17: case 0x18:
	case 0x19: case 0x1A: case 0x1B: case 0x1C:
		return Seg_CollideBlock(o, s, c);
	case 0x1D:
		/* Seg_CollideObj_Trigger */
		if ((s8)o->index >= 2 || o->boxTop >= s->y2 || o->boxLeft >= s->x2
		    || o->boxRight < s->x1 || o->boxBottom <= s->y1)
			return 1;
		c->event = 2;
		c->flags |= 0x8000;
		return 2;
	case 4:
		/* Seg_CollideObj_CeilFloor: the ceiling */
		if (o->boxLeft >= s->x2 || o->boxRight < s->x1 || cx >= s->x2 || cx < s->x1)
			return 1;
		push = SLOPE_OFS(s, cx - s->x1) + (short)(s->y1 - o->boxTop);
		if (push <= 0 || push > 64)
			return 1;
		c->pushY = (s8)push;
		return 2;
	case 3: case 0xE: case 0xF:
		if (o->seg && o->seg->type == 3)
			return 1;
		if (cx >= s->x2 || cx < s->x1)
			return 1;
		push = SLOPE_OFS(s, cx - s->x1) + (short)(s->y1 - o->boxBottom);
		if (push >= 0)
			return 3;
		if (push < -64)
			return 1;
		if (s->type == 0xE) {
			if ((s8)o->index >= 2)
				return 1;
			c->event = 2;
			c->flags |= 0x8000;
			return 2;
		}
		if (s->type == 0xF) {
			if ((s8)o->index < 2)
				return 1;
			c->event = 3;
			c->flags |= 0x8000;
		}
		c->pushY = (s8)push;
		c->flags |= 8;
		return 2;
	}
	return 1;	/* Seg_CollideObj_RetNone */
}
