/* Wolverine_Init - WOLVERINE constructor (class slot +$12): XCLIMBER's Init, his own Reset, then
 * Wolverine's record, states, speeds, healing timer and sprite.
 *
 * self: the new player.  x, y: position (8 fraction bits).  pl: its placement entry (passed on).
 * Reset is WOLVERINE's own slot +$6A (Wolverine_Reset, empty), called through the class, not the
 * object. The character record Tbl_WolverineCharRecord, the control state (slot +$A6) as state
 * and default state, XPlayer_StateHurt as hurt state, no speed and no acceleration x, normal
 * gravity, maximum speeds x and y the record's fall speed, climbFlags $80 (he can hang from
 * ceilings), healTimer 900. His sprite (Art_EvilWolverineSpr, palette 1) in the VRAM tiles of
 * his player slot (g_P1VramTile or g_P2VramTile). */
#include "xmen2.h"

void Wolverine_Init(register Wolverine *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_Wolverine_vtable)->parent->method[3])(self, x, y, pl);	/* XCLIMBER Init */
	(*CLASS(Tbl_Wolverine_vtable)->method[25])(self);			/* Reset */
	self->charRec = (CharRecord *)CODEPTR(Tbl_WolverineCharRecord);
	self->state = (StateFn)self->cls->method[40];		/* StateControl */
	self->defaultState = (StateFn)self->cls->method[40];
	self->hurtState = XPlayer_StateHurt;
	self->vx = 0;
	self->vy = 0;
	self->ax = 0;
	self->ay = self->charRec->gravity;
	self->maxVx = self->charRec->maxFallVy;
	self->maxVy = self->charRec->maxFallVy;
	self->climbFlags |= 0x80;
	self->healTimer = 900;
	Spr_Init(&self->spr, 1, CODEPTR(Art_EvilWolverineSpr), -1, 1,
		(XPlayer *)self == &g_Player1 ? g_P1VramTile : g_P2VramTile, 0);
}
