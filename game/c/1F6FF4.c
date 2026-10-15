/* Seg_ComputeObjPos - set an object's position along a floor segment from its x or y, clamped
 * to the segment.
 *
 * seg: the segment; obj: the object (any Actor class) landing on it.
 * The segment type's hook (g_SegTypeHooks, message 5) may do it (non-zero). Else segPos (8
 * fraction bits) by type (Seg_ComputeObjPos_Cases), its pixel x or y clamped to the segment:
 *   3, $E, $F, $13-$1C, $21 floors: x - x1, and vx is halved;   4, $22 reversed: x2 - x;
 *   1, $24 walls: y - y1;   2, $23 walls: y2 - y;   others: unchanged (Seg_ComputeObjPos_Done:
 *   the exit). */
#include "xmen2.h"

/* v clamped to lo..hi, as the original wrote it (tested low end first) */
#define CLAMP(v, lo, hi)	((v) < (lo) ? (lo) : (v) > (hi) ? (hi) : (v))

void Seg_ComputeObjPos(Seg *seg, void *obj)
{
	register Actor *o = (Actor *)obj;
	register Seg *s = seg;

	if ((g_SegHookTmp = g_SegTypeHooks[s->type]) ? g_SegHookTmp(5, s, o, 0) : 0)
		return;
	switch (s->type) {
	default:
		break;
	case 3: case 0xE: case 0xF: case 0x21:
	case 0x13: case 0x14: case 0x15: case 0x16: case 0x17:
	case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1C:
		/* Seg_ComputeObjPos_Cases */
		o->segPos = (CLAMP(o->x >> 8, s->x1, s->x2) - s->x1) << 8;
		o->vx >>= 1;
		break;
	case 4: case 0x22:
		o->segPos = (s->x2 - CLAMP(o->x >> 8, s->x1, s->x2)) << 8;
		break;
	case 1: case 0x24:
		o->segPos = (CLAMP(o->y >> 8, s->y1, s->y2) - s->y1) << 8;
		break;
	case 2: case 0x23:
		o->segPos = (s->y2 - CLAMP(o->y >> 8, s->y1, s->y2)) << 8;
		break;
	}
}
