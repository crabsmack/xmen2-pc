/* MagneticGlob_Init - MAGNETICGLOB constructor (class slot +$12): the parent's (RTHETAOBJECT)
 * Init, then a globe of the Art_FluxBombSpr sprites, palette 1, no frame yet.
 *
 * self: the globe.  x, y: position (8 fraction bits).
 * pl: placement params (Magneto_SpawnShot): 0 fired by the player (on the players' side,
 * flags6 $10, top speed $2000; else $800), 2 its VRAM tiles (0: keep the old).
 * flags6 |= $8B (no collisions, no hit lists, front). */
#include "xmen2.h"

void MagneticGlob_Init(register MagneticGlob *self, long x, long y, register Placement *pl)
{
	(*CLASS(Tbl_MagneticGlob_vtable)->parent->method[3])(self, x, y, pl);	/* RTHETAOBJECT Init */
	self->flags6 |= 0x8B;
	self->spr.def = CODEPTR(Art_FluxBombSpr);
	self->spr.flags = self->spr.flags & ~3 | 1;
	if (pl->param[0]) {
		self->flags6 |= 0x10;
		self->maxVx = 0x2000;
		self->maxVy = 0x2000;
	} else {
		self->maxVx = 0x800;
		self->maxVy = 0x800;
	}
	if (pl->param[2])
		self->tiles = pl->param[2];
	(*self->cls->method[20])(self, 0xFF);			/* +$56 SetFrame: none */
}
