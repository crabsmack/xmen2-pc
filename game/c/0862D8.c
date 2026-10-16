/* BroodBossDamage_Init - BROODBOSSDAMAGE Init (class slot +$12): the parent's (BROODBOSSPARTS) Init,
 * then the arena's hurting floor: no motion, attack 1 (hit type 5), state and default state
 * BroodBossDamage_StateIdle, flags6 |= $5003; every live (flags bit 7) type-$16 floor segment
 * becomes a DamageRect (from the stack allocator) relative to the part's on-screen origin, and the
 * segment itself is freed (flags 0, no owner, unlinked from its neighbours on both sides).
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry, or null (then a
 * local one with the pixel position of x/y stands in).
 * The origin is the pixel position + (-$20, $5B), minus the boss x offset and the camera: the
 * rects are compared with players' boxes from the part's x/y (BroodBossDamage_Update), which
 * BroodBossParts_Update places the same way. */
#include "xmen2.h"

void BroodBossDamage_Init(BroodBossDamage *self, long x, long y, Placement *pl)
{
	Placement none;
	short size;
	short ox;
	short oy;
	unsigned short i;
	Seg *seg;
	DamageRect *r;

	if (!pl) {
		pl = &none;
		pl->x = (short)(x >> 8);
		pl->y = (short)(y >> 8);
	}
	(*CLASS(Tbl_BroodBossDamage_vtable)->parent->method[3])(self, x, y, pl);	/* BROODBOSSPARTS Init */
	STOP_MOVING(self);
	self->attack = 1;
	self->state = self->defaultState = BroodBossDamage_StateIdle;
	self->flags6 |= 0x5003;
	self->hitType = 5;
	self->rectCount = 0;
	self->rects = 0;
	for (seg = g_Segs, i = 0; i <= g_SegHighWater; i++, seg++) {
		if ((seg->flags & 0x80) && seg->type == 0x16)
			self->rectCount++;
	}
	if (self->rectCount > 0) {
		size = self->rectCount * 8;
		self->rects = (DamageRect *)Mem_StackAlloc(size, STR(Str_Empty_0864E0, ""));
		ox = (short)(x >> 8);
		oy = (short)(y >> 8);
		ox += -0x20;
		oy += 0x5B;
		ox -= g_BroodBossOffsetX >> 8;
		ox -= g_CameraX;
		oy -= g_CameraY;
		r = self->rects;
		for (seg = g_Segs, i = 0; i <= g_SegHighWater; i++, seg++) {
			if ((seg->flags & 0x80) && seg->type == 0x16) {
				r->left = seg->x1 - ox;
				r->top = seg->y1 - oy;
				r->right = seg->x2 - ox;
				r->bottom = seg->y2 - oy;
				r++;
				seg->flags = 0;
				seg->nextOwned = 0xFF;
				if (seg->prev != 0xFF) {
					g_Segs[seg->prev].next = 0xFF;
					seg->prev = 0xFF;
				}
				if (seg->next != 0xFF) {
					g_Segs[seg->next].prev = 0xFF;
					seg->next = 0xFF;
				}
			}
		}
	}
}
