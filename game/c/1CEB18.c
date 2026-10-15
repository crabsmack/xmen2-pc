/* Magneto_Init - MAGNETO constructor (class slot +$12): the player's (XPLAYER) Init, then
 * Magneto's record, states, speeds and sprite.
 *
 * self: the new player.  x, y: position (8 fraction bits).  pl: its placement entry (passed on).
 * Reset (slot +$6A, empty), the character record Tbl_MagnetoCharRecord, the control state
 * (slot +$A6) as state and default state, XPlayer_StateHurt as hurt state, no speed in x or y
 * and no acceleration x, maximum speeds x and y the record's fall speed. His sprite starts in
 * frame bank Art_MagnetoBossSpr2 (Magneto_FrameBank), palette 1, in the VRAM tiles of his
 * player slot (g_P1VramTile or g_P2VramTile). */
#include "xmen2.h"

void Magneto_Init(register Magneto *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_Magneto_vtable)->parent->method[3])(self, x, y, pl);	/* XPLAYER Init */
	(*self->cls->method[25])(self);		/* Reset */
	self->charRec = (CharRecord *)CODEPTR(Tbl_MagnetoCharRecord);
	self->state = (StateFn)self->cls->method[40];		/* StateControl */
	self->defaultState = (StateFn)self->cls->method[40];
	self->hurtState = XPlayer_StateHurt;
	self->vx = 0;
	self->vy = 0;
	self->ax = 0;
	self->maxVx = self->charRec->maxFallVy;
	self->maxVy = self->charRec->maxFallVy;
	Spr_Init(&self->spr, 1, CODEPTR(Art_MagnetoBossSpr2), -1, 1,
		(XPlayer *)self == &g_Player1 ? g_P1VramTile : g_P2VramTile, 0);
}
