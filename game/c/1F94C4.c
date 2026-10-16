/* Obj_FindHitPoint - where a target's hit points meet an attacker's second box.
 *
 * att: the attacker (its box2, pixels).  tgt: the object hit, whose frame gives the points.
 * pts: tgt's hit-point list for that frame (signed bytes, offsets from tgt's position).
 * ox, oy: tgt's position (pixels).  rx, ry: the point found (both -1 when none).
 * Nothing (0) when the attacker shows no frame. The box is taken relative to (ox, oy), mirrored
 * by tgt's flips. With the frame's hitFlags bit 0 the list is one rectangle (x1, y1, x2, y2, in
 * any order): when it overlaps the box, the point is the overlap's centre (from tgt's own pixel
 * position). Else the list is (x, y) pairs ended by (0, 0); a pair ($7F, $7F) is followed by a
 * condition (c, v): unless byte (c >> 6) & 3 of tgt's spr.partStates equals v, the next (c & $3F)
 * pairs are skipped. The first point inside the box is the result. Returns 1 found, 0 not. */
#include "xmen2.h"

char Obj_FindHitPoint(Actor *att, Actor *tgt, s8 *pts, short ox, short oy, s16 *rx, s16 *ry)
{
	short cx;
	short x1;
	short y1;
	short tmp;
	char hflip;
	char vflip;
	short left;
	short top;
	SprFrame *frame;
#ifndef XMEN2_MPW
	SprDef *def;
#endif
	register short right;
	register short bottom;
	register short y;
	register short x;

	hflip = vflip = 0;
	if (att->spr.frame == 0xFF)
		return 0;
#ifdef XMEN2_MPW
	frame = SPR_FRAME(tgt->spr.def, tgt->spr.frame);
#else
	def = (SprDef *)tgt->spr.def;
	if (!def)	/* no definition: the 68000 reads the vector table */
		def = (SprDef *)Vectors;
	frame = SPR_FRAME(def, tgt->spr.frame);
#endif
	*ry = -1;
	*rx = -1;
	if (tgt->spr.flags & 0x10) {
		hflip = 1;
		left = -(att->box2Right - ox);
		right = -(att->box2Left - ox);
	} else {
		left = att->box2Left - ox;
		right = att->box2Right - ox;
	}
	if (tgt->spr.flags & 8) {
		vflip = 1;
		top = -(att->box2Bottom - oy);
		bottom = -(att->box2Top - oy);
	} else {
		top = att->box2Top - oy;
		bottom = att->box2Bottom - oy;
	}
	if (frame->hitFlags & 1 ? 1 : 0) {
		x1 = *pts++;
		y1 = *pts++;
		x = *pts++;
		y = *pts++;
		if (x < x1) {
			tmp = x;
			x = x1;
			x1 = tmp;
		}
		if (y < y1) {
			tmp = y;
			y = y1;
			y1 = tmp;
		}
		if (right >= x1 && x >= left && bottom >= y1 && y >= top) {
			cx = (MAX(x1, left) + MIN(x, right)) >> 1;
			x = (MAX(y1, top) + MIN(y, bottom)) >> 1;
			*rx = (tgt->x >> 8) + (hflip ? -cx : cx);
			*ry = (tgt->y >> 8) + (vflip ? -x : x);
			return 1;
		}
	} else {
		for (;;) {
			x = *pts++;
			y = *pts++;
			if (x == 0 && y == 0)
				break;
			if (x == 0x7F && y == 0x7F) {
				x = *pts++;
				y = *pts++;
				if (*((s8 *)&tgt->spr.partStates + ((x >> 6) & 3)) == y)
					continue;
				pts += (x & 0x3F) << 1;
				continue;
			}
			if (x < left || x > right || y < top || y > bottom)
				continue;
			*rx = ox + (hflip ? -x : x);
			*ry = oy + (vflip ? -y : y);
			return 1;
		}
	}
	return 0;
}
