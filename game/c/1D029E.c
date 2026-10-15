/* Gambit_Init - GAMBIT constructor (class slot +$12): XPLAYER's Init, then Gambit's record,
 * states, speeds, his sprite and his cards.
 *
 * self: the new player.  x, y: position (8 fraction bits).  pl: its placement entry (passed on).
 * playerFlags $2000 cleared; his player slot shows the second HUD meter (g_P1HasMeter or
 * g_P2HasMeter). The character record Tbl_GambitCharRecord, the control state (slot +$A6) as
 * state and default state, XPlayer_StateHurt as hurt state, no speed and no acceleration x,
 * gravity $5C, maximum speeds x and y the record's fall speed. His sprite (Art_EvilGambitSpr,
 * palette 1) in the VRAM tiles of his player slot (g_P1VramTile or g_P2VramTile); the card
 * frames (Art_GambitSpr) preloaded into the last 13 tiles of that slot's g_PlayerTiles (cardTile)
 * and the trail's (Art_GambitSpr2) 12 tiles further. Throw kind 0, his cards g_GambitCards, all
 * free (Gambit_ClearCards). */
#include "xmen2.h"

void Gambit_Init(Gambit *self, long x, long y, Placement *pl)
{
	register GambitCard *cards = g_GambitCards;
	register Gambit *p = self;
	SprInst spr;

	(*CLASS(Tbl_Gambit_vtable)->parent->method[3])(p, x, y, pl);	/* XPLAYER Init */
	p->playerFlags &= ~0x2000;
	if ((XPlayer *)p == &g_Player1)
		g_P1HasMeter = 1;
	else
		g_P2HasMeter = 1;
	p->charRec = (CharRecord *)CODEPTR(Tbl_GambitCharRecord);
	p->state = (StateFn)p->cls->method[40];		/* StateControl */
	p->defaultState = (StateFn)p->cls->method[40];
	p->hurtState = XPlayer_StateHurt;
	p->vx = 0;
	p->vy = 0;
	p->ax = 0;
	p->ay = 0x5C;
	p->maxVx = p->charRec->maxFallVy;
	p->maxVy = p->charRec->maxFallVy;
	Spr_Init(&p->spr, 1, CODEPTR(Art_EvilGambitSpr), -1, 1,
		(XPlayer *)p == &g_Player1 ? g_P1VramTile : g_P2VramTile, 0);
	p->cardTile = ((XPlayer *)p == &g_Player1 ? g_P1VramTile : g_P2VramTile) + g_PlayerTiles - 13;
	Spr_Init(&spr, 1, CODEPTR(Art_GambitSpr), 0, 1, p->cardTile, 0);
	Spr_PreloadToHeap(&spr, p->cardTile);
	Spr_Init(&spr, 1, CODEPTR(Art_GambitSpr2), 0, 1, p->cardTile + 12, 0);
	Spr_PreloadToHeap(&spr, p->cardTile + 12);
	p->throwKind = 0;
	p->cards = cards;
	Gambit_ClearCards(p);
}
