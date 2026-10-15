/* Seg_GatherNearObj - list the floor segments near an object: the contact list Obj_CollideSegs
 * resolves.
 *
 * obj: the object (any Actor class); c: its contact list (Obj_CollideSegs' 30 records).
 * First the segments of the grid cell of its box centre (Seg_GridCell's clamping, written out),
 * then every segment created after the level's own (g_SegLevelCount .. g_SegHighWater: those
 * objects own). A segment is listed (a cleared record with its index) when it is in use and
 * not skipped (flags bits 7-6 = 10), and its box widened by 64 px overlaps the object's box; a
 * cell segment the object stands on is left out. An owned segment is marked skip (contact flags
 * 4, segment flags bit 6) when its owner (not $FE) is the object or one of the objects it is
 * attached to (SprInst.unk_0C chain), or for a player (index < 2) while its passFrames is
 * positive; a player counts passFrames down then. The list ends with a record of flags 1.
 * The two scans differ at the left edge (< against <=), the original's.
 * Hand-written 68000 in an MPW frame (a2, exg, dbra, 16-bit sums): it can't byte-match. */
#include "xmen2.h"

void Seg_GatherNearObj(void *obj, SegContact *c)
{
	Actor *o = (Actor *)obj;
	Actor *a;
	Seg *s;
	u8 *p;
	short cx, cy;
	short lo, hi, t;
	short n, count;
	unsigned char i;

	cx = (short)(o->boxLeft + o->boxRight);
	if (cx < 0)
		cx = 0;
	else {
		cx >>= g_SegGridShiftX + 1;
		if (cx >= g_SegGridW)
			cx = g_SegGridW - 1;
	}
	cy = (short)(o->boxTop + o->boxBottom);
	if (cy < 0)
		cy = 0;
	else {
		cy >>= g_SegGridShiftY + 1;
		if (cy >= g_SegGridH)
			cy = g_SegGridH - 1;
	}
	p = &g_SegGridPool[g_SegGrid[(unsigned short)cy * (unsigned short)g_SegGridW + cx]];

	/* the cell's segments */
	while ((i = *p++) != 0xFF) {
		s = &g_Segs[i];
		lo = s->y1;
		hi = s->y2;
		if (hi < lo) {
			t = lo;
			lo = hi;
			hi = t;
		}
		if ((short)(s->x2 + 64) < o->boxLeft || (short)(s->x1 - 64) >= o->boxRight
		    || (short)(lo - 64) >= o->boxBottom || (short)(hi + 64) < o->boxTop)
			continue;
		if ((s->flags & 0xC0) != 0x80 || s == o->seg)
			continue;
		c->flags = 0;
		c->seg = i;
		c->pushX = 0;
		c->pushY = 0;
		c->push2X = 0;
		c->push2Y = 0;
		c->event = 0;
		c++;
	}

	/* the segments objects own */
	n = g_SegLevelCount;
	s = &g_Segs[n];
	for (count = g_SegHighWater - g_SegLevelCount; count >= 0; count--, n++, s++) {
		lo = s->y1;
		hi = s->y2;
		if (hi < lo) {
			t = lo;
			lo = hi;
			hi = t;
		}
		if ((short)(s->x2 + 64) <= o->boxLeft || (short)(s->x1 - 64) >= o->boxRight
		    || (short)(lo - 64) >= o->boxBottom || (short)(hi + 64) < o->boxTop)
			continue;
		if ((s->flags & 0xC0) != 0x80)
			continue;
		c->flags = 0;
		c->seg = (u8)n;
		c->pushX = 0;
		c->pushY = 0;
		c->push2X = 0;
		c->push2Y = 0;
		c->event = 0;
		c++;
		if (s->owner == -2)
			continue;
		if ((u8)s->owner != o->index && !((s8)o->index < 2 && s->passFrames > 0)) {
			a = (u8)s->owner == 0xFF ? 0 : (Actor *)g_ObjTable[(u8)s->owner];
			for (;;) {
				if (!a)
					goto next;
				if (a->index == o->index)
					break;
				a = a->spr.unk_0C == 0xFF ? 0 : (Actor *)g_ObjTable[a->spr.unk_0C];
			}
		}
		c[-1].flags = 4;
		s->flags |= 0x40;
		if ((s8)o->index < 2 && s->passFrames != 0)
			s->passFrames--;
	next:	;
	}

	c->flags = 1;
	c->seg = 0;
	c->pushX = 0;
}
