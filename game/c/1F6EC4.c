/* Seg_AlignObjVelocity - turn a walker's velocity into motion along its floor segment.
 *
 * obj: the walker (any Actor class), on seg.
 * The segment type's hook (g_SegTypeHooks, message 2) may do it all (non-zero): then only
 * Seg_ApplyObjPos. Else the new velocity by type:
 *   3, $E, $F, $21 floors (Seg_AlignObjVelocity_Slope): vx kept, vy = slope * vx >> 8;
 *   $22 reversed (_Reverse): both negated;   $13-$1C flat (_Flat): vx kept, vy 0;
 *   1 / 2 walls: vy = vx / -vx, vx = slope * vy >> 8 (the old vy);
 *   5-$D: nothing at all (returns); others ($23, $24 too): 0, 0.
 * Then the velocity is stored and Seg_ApplyObjPos places the object (_Store). Nothing when it
 * stands on no segment. */
#include "xmen2.h"

void Seg_AlignObjVelocity(void *obj)
{
	register Actor *o = (Actor *)obj;
	register Seg *s = o->seg;
	short vx, vy;

	if (!s)
		return;
	if ((g_SegHookTmp = g_SegTypeHooks[s->type]) ? g_SegHookTmp(2, s, o, 0) : 0) {
		Seg_ApplyObjPos(o);
		return;
	}
	vx = 0;
	vy = 0;
	switch (s->type) {
	case 3: case 0xE: case 0xF: case 0x21:
		/* Seg_AlignObjVelocity_Slope */
		vy = o->vx * s->slope >> 8;
		vx = o->vx;
		break;
	case 0x22:
		/* Seg_AlignObjVelocity_Reverse */
		vy = -(o->vx * s->slope) >> 8;
		vx = -o->vx;
		break;
	case 2:
		vy = -o->vx;
		vx = o->vy * s->slope >> 8;
		break;
	case 1:
		vy = o->vx;
		vx = o->vy * s->slope >> 8;
		break;
	case 0x13: case 0x14: case 0x15: case 0x16: case 0x17:
	case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1C:
		/* Seg_AlignObjVelocity_Flat */
		vx = o->vx;
		break;
	case 5: case 6: case 7: case 8: case 9: case 0xA: case 0xB: case 0xC: case 0xD:
		return;
	}
	/* Seg_AlignObjVelocity_Store */
	o->vx = vx;
	o->vy = vy;
	Seg_ApplyObjPos(o);
}
