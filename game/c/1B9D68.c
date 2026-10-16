/* Psylocke_Init - PSYLOCKE constructor (class slot +$12): XCLIMBER's Init, then Psylocke's record,
 * states, speeds and her two sprites.
 *
 * self: the new player.  x, y: position (8 fraction bits).  pl: its placement entry (passed on).
 * The character record Tbl_PsylockeCharRecord, the control state (slot +$A6) as state and default
 * state, XPlayer_StateHurt as hurt state, no speed and no acceleration x, normal gravity, maximum
 * speeds x and y the record's fall speed, climbFlags $100. Her sprite (Art_EvilPsylockeSpr,
 * palette 1) in the VRAM tiles of her player slot (g_P1VramTile or g_P2VramTile); the psi
 * effect's (Art_BlastSpr) in the last 16 tiles of that slot's g_PlayerTiles, its frames loaded
 * there now (Spr_PreloadToHeap), and g_PsylockeFxSpr pointing at it. */
#include "xmen2.h"

void Psylocke_Init(register Psylocke *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_Psylocke_vtable)->parent->method[3])(self, x, y, pl);	/* XCLIMBER Init */
	self->charRec = (CharRecord *)CODEPTR(Tbl_PsylockeCharRecord);
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
	Spr_Init(&self->spr, 1, CODEPTR(Art_EvilPsylockeSpr), -1, 1,
		(XPlayer *)self == &g_Player1 ? g_P1VramTile : g_P2VramTile, 0);
	{
		register short fxTile;

		fxTile = (long)(((XPlayer *)self == &g_Player1 ? g_P1VramTile : g_P2VramTile)
			+ g_PlayerTiles) - 0x10;	/* the sum in 32 bits, as the original */
		Spr_Init(&self->fxSpr, 1, CODEPTR(Art_BlastSpr), 0, 1, fxTile, 0);
		Spr_PreloadToHeap(&self->fxSpr, fxTile);
	}
	g_PsylockeFxSpr = &self->fxSpr;
end:	;
}
