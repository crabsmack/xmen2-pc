/* Nightcrawler_Init - NIGHTCRAWLER constructor (class slot +$12): the parent's (XCLIMBER) Init,
 * then Nightcrawler's meter, record, states, speeds and sprite.
 *
 * self: the new player.  x, y: position (8 fraction bits).  place: its placement entry (passed
 * on).
 * playerFlags $2000 off and $10 on (his special fires when A is released); the HUD shows his
 * meter (g_P1HasMeter / g_P2HasMeter); Reset (slot +$6A, empty); Tbl_NightcrawlerCharRecord; the
 * control state (slot +$A6) as state and default state, XPlayer_StateHurt as hurt state; no speed
 * or acceleration x, the record's gravity, maximum speeds x and y its fall speed; no teleport;
 * climbFlags $80. His sprite starts in frame bank Art_EvilNightcrawlerSpr, palette 1, in the VRAM
 * tiles of his player slot. */
#include "xmen2.h"

void Nightcrawler_Init(register Nightcrawler *self, long x, long y, Placement *place)
{
	(*CLASS(Tbl_Nightcrawler_vtable)->parent->method[3])(self, x, y, place);	/* XCLIMBER Init */
	self->playerFlags &= ~0x2000;
	if ((XPlayer *)self == &g_Player1)
		g_P1HasMeter = 1;
	else
		g_P2HasMeter = 1;
	(*self->cls->method[25])(self);		/* Reset */
	self->charRec = (CharRecord *)CODEPTR(Tbl_NightcrawlerCharRecord);
	self->state = (StateFn)self->cls->method[40];		/* StateControl */
	self->defaultState = (StateFn)self->cls->method[40];
	self->hurtState = XPlayer_StateHurt;
	self->vx = 0;
	self->vy = 0;
	self->ax = 0;
	self->ay = self->charRec->gravity;
	self->maxVx = self->charRec->maxFallVy;
	self->maxVy = self->charRec->maxFallVy;
	self->teleportDir = 0;
	self->teleportTime = 0;
	self->climbFlags |= 0x80;
	self->playerFlags |= 0x10;
	Spr_Init(&self->spr, 1, CODEPTR(Art_EvilNightcrawlerSpr), -1, 1,
		(XPlayer *)self == &g_Player1 ? g_P1VramTile : g_P2VramTile, 0);
}
