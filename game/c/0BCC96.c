/* SavageMediumBio_Init - SAVAGEMEDIUMBIO Init (class slot +$12): the parent's (SAVAGEBIOBASE)
 * Init, then the Chamber's settings: flags6 |= $83 (not hittable until it is ready), the
 * Chamber's shared stats record (g_A4Stats) with health 4 (its health from it), its AI script
 * (Tbl_SavageMediumBioAiScript), state and default state SavageMediumBio_StateAppear, sprite
 * Art_BasicBioSentinelSpr3 with palette 3, its home at its position, frame $60 (slot +$56) and
 * its boxes (Obj_UpdateBoxes).
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry (passed on). */
#include "xmen2.h"

#define g_A4Stats	(*(PTR(NpcStats) *)g_SceneVar3B14)	/* scene variable: the stats record of the Chamber's enemies */

void SavageMediumBio_Init(SavageMediumBio *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_SavageMediumBio_vtable)->parent->method[3])(self, x, y, pl);	/* SAVAGEBIOBASE Init */
	SavageBio_NopStatic();
	self->flags6 |= 0x83;
	g_A4Stats->health = 4;
	self->stats = g_A4Stats;
	self->health = g_A4Stats->health;
	AiScript_Set(&self->ai, CODEPTR(Tbl_SavageMediumBioAiScript));
	self->state = SavageMediumBio_StateAppear;
	self->defaultState = SavageMediumBio_StateAppear;
	self->spr.def = CODEPTR(Art_BasicBioSentinelSpr3);
	self->spr.flags = self->spr.flags & ~3 | 3;
	self->homeX = self->x >> 8;
	self->homeY = self->y >> 8;
	(*self->cls->method[20])(self, 0x60);		/* +$56 SetFrame */
	Obj_UpdateBoxes(self);
}
