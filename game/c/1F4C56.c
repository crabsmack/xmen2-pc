/* Actor_Move - ACTOR method +$4E: move an object by its velocity for one frame.
 *
 * obj: the object (any Actor class).
 * Clears flags bits 7-8 and keeps the velocity in prevVx/prevVy, then moves once, or
 * g_TwoPlayerStep + 1 times. Each step:
 *   - free (flags bit $1000): vx/vy += ax/ay, clamped to maxVx/maxVy, and x/y move by them
 *     unless below minVx/minVy (half as far when g_HalfSpeed is set);
 *   - on a floor segment (seg): the segment type's hook (g_SegTypeHooks, message 1) may give
 *     a negative acceleration, else ax; vx is clamped the same way and moves segPos along the
 *     segment (also below minVx on a segment whose +$04 is set). When segPos changed (or flags
 *     bit $800): with g_SegFlag38C4 it is kept within the segment (flags bit $100 at the
 *     end); past either end it walks to the previous/next segment, or the object leaves the
 *     chain (Seg_AlignObjVelocity, Obj_Detach); still on it, Seg_ApplyObjPos sets x/y.
 * Hand-written 68000. */
#include "xmen2.h"

void Actor_Move(void *obj)
{
	Actor *o = (Actor *)obj;
	signed char steps = (signed char)g_TwoPlayerStep;
	Seg *s;
	SegHookFn hook;
	long before;
	long dv;
	short accel;
	short v;
	unsigned char n;

	o->flags &= ~0x0180;
	o->prevVx = o->vx;
	o->prevVy = o->vy;
	for (;;) {
		if (o->flags & 0x1000) {
			o->vx += o->ax;
			o->vy += o->ay;
			v = o->vx;
			if (v >= 0) {
				if (v > o->maxVx) {
					o->vx = o->maxVx;
					v = o->vx;
				}
				if (v > o->minVx) {
					dv = v;
					if (g_HalfSpeed)
						dv >>= 1;
					o->x += dv;
				}
			} else {
				v = -v;
				if (v > o->maxVx) {
					o->vx = -o->maxVx;
					v = o->maxVx;
				}
				if (v > o->minVx) {
					dv = (short)-v;
					if (g_HalfSpeed)
						dv >>= 1;
					o->x += dv;
				}
			}
			v = o->vy;
			if (v >= 0) {
				if (v > o->maxVy) {
					o->vy = o->maxVy;
					v = o->vy;
				}
				if (v > o->minVy) {
					dv = v;
					if (g_HalfSpeed)
						dv >>= 1;
					o->y += dv;
				}
			} else {
				v = -v;
				if (v > o->maxVy) {
					o->vy = -o->maxVy;
					v = o->maxVy;
				}
				if (v > o->minVy) {
					dv = (short)-v;
					if (g_HalfSpeed)
						dv >>= 1;
					o->y += dv;
				}
			}
		} else {
			before = o->segPos;
			s = o->seg;
			accel = 0;
			hook = g_SegTypeHooks[(unsigned char)s->type];
			if (hook)
				accel = hook(1L, s, o, 0L);
			if (accel >= 0)
				accel = o->ax;
			o->vx += accel;
			v = o->vx;
			if (v >= 0) {
				if (v > o->maxVx) {
					o->vx = o->maxVx;
					v = o->vx;
				}
				if (s->slope != 0 || v > o->minVx) {
					dv = v;
					if (g_HalfSpeed)
						dv >>= 1;
					o->segPos += dv;
				}
			} else {
				v = -v;
				if (v > o->maxVx) {
					o->vx = -o->maxVx;
					v = o->maxVx;
				}
				if (s->slope != 0 || v > o->minVx) {
					dv = (short)-v;
					if (g_HalfSpeed)
						dv >>= 1;
					o->segPos += dv;
				}
			}
			if ((o->flags & 0x800) || o->segPos != before) {
				s = o->seg;
				if (g_SegFlag38C4) {
					if (o->segPos < 0)
						o->segPos = 0;
					if ((long)s->length << 8 < o->segPos) {
						o->segPos = ((long)s->length << 8) - 0x70;
						o->flags |= 0x100;
					}
				}
				while (o->segPos < 0) {
					n = s->prev;
					if (n == 0xFF) {
						Seg_AlignObjVelocity(o);
						Obj_Detach(o);
						break;
					}
					s = &g_Segs[n];
					o->seg = s;
					o->segPos += (long)s->length << 8;
				}
				if (!(o->flags & 0x1000)) {
					while ((long)s->length < o->segPos >> 8) {
						n = s->next;
						if (n == 0xFF) {
							Seg_AlignObjVelocity(o);
							Obj_Detach(o);
							break;
						}
						o->segPos -= (long)s->length << 8;
						s = &g_Segs[n];
						o->seg = s;
					}
					if (!(o->flags & 0x1000))
						Seg_ApplyObjPos(o);
				}
			}
		}
		if (steps-- <= 0)
			return;
	}
}
