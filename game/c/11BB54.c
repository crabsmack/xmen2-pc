/* BioFlame_Init - BIOFLAME Init (class slot +$12): the parent's (ACTOR) Init, at its placement
 * position (or x, y), sprite g_BioFlameSprDef in palette 1, flags6 |= $83, its sprite priority
 * bit (2) from the live player (player 1 when neither is), not moving, no owner, no offset, the
 * three flame sprites 0, 10 and 20 px from the nozzle.
 *
 * self: the flame.  x, y: position (8 fraction bits).  pl: its placement entry or 0.
 * (BabyBio_ActAttachFlame makes one, then sets its owner and offset.) */
#include "xmen2.h"

void BioFlame_Init(register BioFlame *self, register long x, register long y, register Placement *pl)
{
	(*CLASS(Tbl_BioFlame_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	if (pl) {
		self->px = pl->x;
		self->py = pl->y;
	} else {
		self->px = (short)(x >> 8);
		self->py = (short)(y >> 8);
	}
	self->x = self->px << 8;
	self->y = self->py << 8;
	Obj_SetSpriteDef((Actor *)self, g_BioFlameSprDef);
	self->spr.flags = self->spr.flags & ~3 | 1;
	self->flags6 |= 0x83;
	if (IS_ALIVE(&g_Player1)) {
		if (g_Player1.spr.flags & 4)
			self->spr.flags |= 4;
		else
			self->spr.flags &= ~4;
	} else if (IS_ALIVE(&g_Player2)) {
		if (g_Player2.spr.flags & 4)
			self->spr.flags |= 4;
		else
			self->spr.flags &= ~4;
	} else {
		if (g_Player1.spr.flags & 4)
			self->spr.flags |= 4;
		else
			self->spr.flags &= ~4;
	}
	STOP_MOVING(self);
	self->offX = 0;
	self->offY = 0;
	self->owner = 0;
	self->flameX[0] = 0;
	self->flameX[1] = 10;
	self->flameX[2] = 20;
}
