/* LightBeam_Init - LIGHTBEAM Init (class slot +$12): the parent's (ACTOR) Init, then a row of light
 * beams: LightBeam_StateIdle as state, default and hurt state, flags6 |= $50C3; from the
 * placement its kind (param 0), column spacing (param 1, 0: 4) and column count (param 2, 0: 4);
 * the first column's x offset ($3B for kind 0, else $2E) and the top y: the heights of CloneA1's
 * parallax bands 0..kind added up, - 1 (the beams hang from that band's edge); the beams' sprite
 * definition (g_LightBeamSprDef, palette 2) and frame = kind.
 *
 * self: the beam row.  x, y: position (8 fraction bits).  pl: its placement entry (never null).
 * A kind above 2 leaves the x offset and the top's - 1 unset (whatever the registers held). */
#include "xmen2.h"

#define g_LightBeamSprDef	g_TextWinBottom	/* reused in levels: the beams' sprite-definition slot (LightBeam_ClassInit) */

void LightBeam_Init(register LightBeam *self, long x, long y, Placement *pl)
{
	short top;
	ParallaxDesc *desc;
	register ParallaxBand *band;
	short xOff;
	short adjust;
	short n;

	top = 0;
	desc = (ParallaxDesc *)CODEPTR(Tbl_CloneA1Parallax);
	band = (ParallaxBand *)&desc->type;	/* overwritten at once: the ROM has both (lea (a0) / lea 2(a0)) */
	band = desc->band;
	(*CLASS(Tbl_LightBeam_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->state = (StateFn)LightBeam_StateIdle;
	self->defaultState = (StateFn)LightBeam_StateIdle;
	self->hurtState = (StateFn)LightBeam_StateIdle;
	self->flags6 |= 0x50C3;
	self->kind = pl->param[0] ? pl->param[0] : 0;
	self->spacing = pl->param[1] ? pl->param[1] : 4;
	self->count = pl->param[2] ? pl->param[2] : 4;
	switch (self->kind) {
	case 0:
		xOff = 0x3B;
		adjust = -1;
		break;
	case 1:
		xOff = 0x2E;
		adjust = -1;
		break;
	case 2:
		xOff = 0x2E;
		adjust = -1;
		break;
	}
	for (n = self->kind + 1; n; n--) {
		top += band->height;
		band++;
	}
	self->xOff = xOff;
	self->topY = top + adjust;
	self->unread_9A = -1;
	self->unread_9C = -1;
	self->unread_9E = -1;
	Obj_SetSpriteDef((Actor *)self, g_LightBeamSprDef);
	self->sprDef = g_LightBeamSprDef;
	self->spr.flags = self->spr.flags & ~3 | 2;
	(*self->cls->method[20])(self, self->kind);	/* +$56 SetFrame */
}
