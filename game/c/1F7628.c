/* Seg_ApplyPushOut - the default floor-contact resolver (g_SegResolveHook): stop the motion
 * into the segment, push the object out, flag the axis that was hit.
 *
 * seg: the segment; obj: the object (any Actor class); c: its contact (pushX / pushY).
 * Types $E and $1D: nothing. Solid blocks $13-$1C (Seg_ApplyPushOut_Block): each speed against
 * its push is zeroed. Others (Seg_ApplyPushOut_Default): on a gentle slope (|slope| < $60) the
 * speed into the surface is zeroed (ceiling 4: vy < 0 pushed down; floors 3/$F: vy > 0 pushed
 * up; left walls 1/$20: vx < 0 pushed right; right walls 2/$1F: vx > 0 pushed left); on a steep
 * one, a speed of 0 on one axis takes the other's along the slope's sign, and with both set
 * (Seg_ApplyPushOut_Walls) the walls and the ceiling zero the speeds into them.
 * Then (Seg_ApplyPushOut_Move) x/y move by the push (<< 8), a grounded object (flags bit $1000
 * clear) gets its position along its segment again (Seg_ComputeObjPos), and a changed vx sets
 * flags bit $100, a changed vy bit $80, each with unk_7B = the contact's segment index. */
#include "xmen2.h"

void Seg_ApplyPushOut(Seg *seg, void *obj, SegContact *c)
{
	register Seg *s = seg;
	register Actor *o = (Actor *)obj;
	short vx = o->vx;
	short vy = o->vy;

	switch (s->type) {
	case 0xE: case 0x1D:
		return;
	case 0x13: case 0x14: case 0x15: case 0x16: case 0x17:
	case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1C:
		/* Seg_ApplyPushOut_Block */
		if (o->vx > 0 && c->pushX < 0 || o->vx < 0 && c->pushX > 0)
			o->vx = 0;
		if (o->vy > 0 && c->pushY < 0 || o->vy < 0 && c->pushY > 0)
			o->vy = 0;
		break;
	default:
		/* Seg_ApplyPushOut_Default */
		if (ABS(s->slope) < 0x60) {
			switch (s->type) {
			case 4:
				if (o->vy < 0 && c->pushY > 0)
					o->vy = 0;
				break;
			case 3: case 0xF:
				if (o->vy > 0 && c->pushY < 0)
					o->vy = 0;
				break;
			case 1: case 0x20:
				if (o->vx < 0 && c->pushX > 0)
					o->vx = 0;
				break;
			case 2: case 0x1F:
				if (o->vx > 0 && c->pushX < 0)
					o->vx = 0;
				break;
			}
		} else if (o->vx == 0) {
			if (s->slope > 0)
				o->vx = o->vy;
			if (s->slope < 0)
				o->vx = -o->vy;
		} else if (o->vy == 0) {
			if (s->slope > 0)
				o->vy = o->vx;
			if (s->slope < 0)
				o->vy = -o->vx;
		} else {
			switch (s->type) {
			case 3: case 0xE: case 0xF:
			case 0x13: case 0x14: case 0x15: case 0x16: case 0x17:
			case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1C:
				break;
			case 4:
				/* Seg_ApplyPushOut_Walls */
				if (s->slope < 0 && o->vx < 0)
					o->vx = 0;
				if (s->slope > 0 && o->vx > 0)
					o->vx = 0;
				if (o->vy < 0)
					o->vy = 0;
				break;
			case 1: case 0x20:
				if (s->slope < 0 && o->vy < 0)
					o->vy = 0;
				if (s->slope > 0 && o->vy > 0)
					o->vy = 0;
				if (o->vx < 0)
					o->vx = 0;
				break;
			case 2: case 0x1F:
				if (s->slope > 0 && o->vy < 0)
					o->vy = 0;
				if (s->slope < 0 && o->vy > 0)
					o->vy = 0;
				if (o->vx > 0)
					o->vx = 0;
				break;
			}
		}
		break;
	}
	/* Seg_ApplyPushOut_Move */
	o->x += (long)c->pushX << 8;
	o->y += (long)c->pushY << 8;
	if ((o->flags & 0x1000) == 0)
		Seg_ComputeObjPos(o->seg, o);
	if (vx != o->vx) {
		o->flags |= 0x100;
		o->unk_7B = c->seg;
	}
	if (vy != o->vy) {
		o->flags |= 0x80;
		o->unk_7B = c->seg;
	}
}
