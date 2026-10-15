/* Apocalypse_Init - APOCALYPSE Init (class slot +$12): the parent's (ACTOR) Init, then the boss
 * set up hidden above the top-left corner, ready for its entrance script.
 *
 * self: the boss.  x, y: position (8 fraction bits; replaced).  pl: its placement entry.
 * No motion, speed limits $7000, hurt state Apocalypse_StateHurt, sprite g_ApocalypseSprDef with
 * palette 3, attack 1, flags6 |= $1180, layer 0, 60 health, circling angle 180 step 2. Its
 * second sprite (the ClassInit record): palette 3, Art_ApocalypseSpr2 with its own VRAM tiles
 * from g_VramTileCursor (Spr_Init). At (-10, -20) px, no target, bombs at speed $160 (accel 1,
 * timer period 3) from pattern 0, script choice 0, move shifts 5 (approach) and 4 (lunge), the
 * wobble wave (Tbl_ApocalypseWave, shift 7, y 8 steps ahead), hidden (frame $FF), and the
 * entrance script (Tbl_Apocalypse_ScriptALen: Apocalypse_ScriptA) from its first step. */
#include "xmen2.h"

#define g_ApocalypseSpr2	(*(PTR(SprInst) *)g_SceneVar3B1C)	/* scene variable: APOCALYPSE's second sprite */

void Apocalypse_Init(register Apocalypse *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_Apocalypse_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	STOP_MOVING(self);
	self->maxVx = 0x7000;
	self->maxVy = 0x7000;
	self->hurtState = Apocalypse_StateHurt;
	Obj_SetSpriteDef((Actor *)self, g_ApocalypseSprDef);
	self->spr.flags = self->spr.flags & ~3 | 3;
	self->attack = 1;
	self->flags6 |= 0x1180;
	self->layer = 0;
	self->health = 60;
	self->moveFlags = 0;
	self->angle = 180;
	self->angleStep = 2;
	self->spr2 = g_ApocalypseSpr2;
	self->spr2->flags = 3;
	self->spr2->unk_0B = 0;
	self->spr2->frame = 0;
	self->spr2->def = 0;
	self->spr2->uploaded = 0xFF;
	self->spr2->tileBase = -1;
	self->spr2->firstChild = 0xFF;
	self->spr2->unk_0C = 0xFF;
	self->spr2->nextSibling = 0xFF;
	self->spr2->unk_0F = 0xFF;
	self->spr2->def = CODEPTR(Art_ApocalypseSpr2);
	self->spr2->unk_10 = 0;
	g_VramTileCursor = Spr_Init(self->spr2, 1, self->spr2->def, -1, self->spr2->flags & 3,
		g_VramTileCursor, 0);
	self->x = 0xF600;
	self->y = -0x1400;
	self->target = 0;
	self->bombSpeed = 0x160;
	self->bombAccel = 1;
	self->bombPeriod = 3;
	self->patternIndex = 0;
	Apocalypse_SelectBombPattern(self);
	self->choice = 0;
	self->lungeShift = 4;
	self->goShift = 5;
	self->wave = (s16 *)CODEPTR(Tbl_ApocalypseWave);
	self->wavePhase = 0;
	self->waveShift = 7;
	self->waveYPhase = 8;
	(*self->cls->method[20])(self, 0xFF);		/* +$56 SetFrame: hide */
	AiScript_Set(&self->ai, CODEPTR(Tbl_Apocalypse_ScriptALen));
start:	/* (here MPW C pops all the arguments, as the ROM does) */
	AI_FIRST_STEP(self, self->ai, 0);
}
