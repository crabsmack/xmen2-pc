/* Beast_Init - BEAST constructor (class slot +$12): XCLIMBER's Init, then Beast's record, states,
 * speeds and his two sprites.
 *
 * self: the new player.  x, y: position (8 fraction bits).  pl: its placement entry (passed on).
 * The character record Tbl_BeastCharRecord, the control state (slot +$A6) as state and default
 * state, XPlayer_StateHurt as hurt state, no speed and no acceleration x, normal gravity, maximum
 * speeds x and y the record's fall speed, climbFlags $100. His sprite (Art_BeastSpr, palette 1)
 * in the VRAM tiles of his player slot (g_P1VramTile or g_P2VramTile); the shockwaves' sprite
 * (Art_PlayerSpr) in the last tiles of that slot's g_PlayerTiles. */
#include "xmen2.h"

void Beast_Init(register Beast *self, long x, long y, Placement *pl)
{
	register short tile;
	register short used;
	register long base;

	tile = (XPlayer *)self == &g_Player1 ? g_P1VramTile : g_P2VramTile;
	(*CLASS(Tbl_Beast_vtable)->parent->method[3])(self, x, y, pl);	/* XCLIMBER Init */
	self->charRec = (CharRecord *)CODEPTR(Tbl_BeastCharRecord);
	self->state = (StateFn)self->cls->method[40];		/* StateControl */
	self->defaultState = (StateFn)self->cls->method[40];
	self->hurtState = XPlayer_StateHurt;
	self->vx = 0;
	self->vy = 0;
	self->ax = 0;
	self->ay = self->charRec->gravity;
	self->maxVx = self->charRec->maxFallVy;
	self->maxVy = self->charRec->maxFallVy;
	self->climbFlags |= 0x100;
	Spr_Init(&self->spr, 1, CODEPTR(Art_BeastSpr), -1, 1, tile, 0);
	used = Spr_Init(&self->waveSpr, 1, CODEPTR(Art_PlayerSpr), -1, 1, 0, 0);
	base = tile + g_PlayerTiles - used;
	self->waveSpr.tileBase = (short)base;
}
