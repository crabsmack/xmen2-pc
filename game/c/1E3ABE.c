/* Cyclops_Init - CYCLOPS constructor (class slot +$12): the parent's (XPLAYER) Init, then
 * Cyclops's meter, record, states, speeds, sprite and an empty beam.
 *
 * self: the new player.  x, y: position (8 fraction bits).  place: its placement entry (passed
 * on).
 * playerFlags $2000 off; the HUD shows his meter (g_P1HasMeter / g_P2HasMeter);
 * Tbl_CyclopsCharRecord; the control state (slot +$A6) as state and default state,
 * XPlayer_StateHurt as hurt state; no speed or acceleration x, the record's gravity, maximum
 * speeds x and y its fall speed. His sprite: Art_CyclopsSpr, palette 1, in the VRAM tiles of
 * his player slot. The beam is cleared (its frame count first, then all of it) with no VRAM
 * tiles (-1) and no frame uploaded ($FF). His Reset (slot +$6A) is not called. */
#include "xmen2.h"

void Cyclops_Init(register Cyclops *self, long x, long y, Placement *place)
{
	(*CLASS(Tbl_Cyclops_vtable)->parent->method[3])(self, x, y, place);	/* XPLAYER Init */
	self->playerFlags &= ~0x2000;
	if ((XPlayer *)self == &g_Player1)
		g_P1HasMeter = 1;
	else
		g_P2HasMeter = 1;
	self->charRec = (CharRecord *)CODEPTR(Tbl_CyclopsCharRecord);
	self->state = (StateFn)self->cls->method[40];		/* StateControl */
	self->defaultState = (StateFn)self->cls->method[40];
	self->hurtState = XPlayer_StateHurt;
	self->vx = 0;
	self->vy = 0;
	self->ax = 0;
	self->ay = self->charRec->gravity;
	self->maxVx = self->charRec->maxFallVy;
	self->maxVy = self->charRec->maxFallVy;
	Spr_Init(&self->spr, 1, CODEPTR(Art_CyclopsSpr), -1, 1,
		(XPlayer *)self == &g_Player1 ? g_P1VramTile : g_P2VramTile, 0);
	self->beam.frameCount = 0;
	Mem_Set(&self->beam, 0, sizeof self->beam);
	self->beam.uploaded = 0xFF;
	self->beam.tileBase = -1;
}
