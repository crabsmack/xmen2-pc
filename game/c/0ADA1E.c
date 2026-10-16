/* TrapDoor_Init - TRAPDOOR Init (class slot +$12): the parent's (XNPC) Init, then find its floor
 * segment and pick its map blocks.
 *
 * self: the object.  x, y: position (8 fraction bits), passed on.
 * pl:   its placement entry (x, y in pixels; it must be there: no stack copy as other Inits make).
 * Not moving, flags6 |= $4083, every state (state, default and hurt) TrapDoor_StateStart. The
 * door is the first in-use type-$13 segment of g_Segs (up to g_SegHighWater) whose rectangle
 * (x1, y1 the low corner) holds the placement point; 0 when there is none. Its four map blocks
 * are $7F-$82 in the scene g_ScnSavageA2, $82-$85 elsewhere. */
#include "xmen2.h"

void TrapDoor_Init(TrapDoor *self, long x, long y, Placement *pl)
{
	short px, py;
	long i;
	Seg *seg;
	short closed, sag, open, wide;

	(*CLASS(Tbl_TrapDoor_vtable)->parent->method[3])(self, x, y, pl);	/* XNPC Init */
	STOP_MOVING(self);
	self->flags6 |= 0x4083;
	self->state = TrapDoor_StateStart;
	self->defaultState = TrapDoor_StateStart;
	self->hurtState = TrapDoor_StateStart;
	self->door = 0;
	px = pl->x;
	py = pl->y;
	for (seg = g_Segs, i = 0; i <= g_SegHighWater; i++, seg++) {
		if (seg->flags & 0x80 && seg->type == 0x13 && px >= seg->x1 && px <= seg->x2
		    && py >= seg->y1 && py <= seg->y2) {
			self->door = seg;
			break;
		}
	}
	if (g_ScnSavageA2 == g_SceneIndex) {
		closed = 0x7F;
		sag = closed + 1;
		open = sag + 1;
		wide = open + 1;
	} else {
		closed = 0x82;
		sag = closed + 1;
		open = sag + 1;
		wide = open + 1;
	}
	self->tileClosed = closed;
	self->tileSag = sag;
	self->tileOpen = open;
	self->tileWide = wide;
}
