/* MagnetoBoss_Init - MAGNETOBOSS constructor (class slot +$12): the parent's (ACTOR) Init, then
 * the boss's own VRAM, sprite, wobble and shot settings.
 *
 * self: the new object.  x, y: position (8 fraction bits).  pl: its placement entry, or 0.
 * No vertical speed or gravity, health 50, maximum speed x $600, flags6 |= $1083. VRAM: room
 * for the largest frame of its three sprite sets (Spr_Init into a scratch SprInst:
 * Art_MagnetoBossSpr2, Art_MagnetoBossSpr3, Art_EvilMagnetoSpr) plus 7 tiles for its globes
 * (shotTiles) is taken from the tile cursor; its sprite is Art_EvilMagnetoSpr at the old cursor
 * (tileBase), palette 1. No target, bossFlags $10 (face the target), shotDir $FF, the wobble
 * table Tbl_MagnetoBossOrbitWave (phase 0, shift 9, y phase 8). Placement params (a missing
 * placement is filled in: position, $D2, 0, 5): 0 shotPeriod (0: $D2), also the first
 * cooldown, 2 shotDelay (0: 5). No frame shown, plane B follows it (g_BgLockObj), first state
 * MagnetoBoss_StateIntroStart (also the default), no moveMode, x range -100..8000. */
#include "xmen2.h"

void MagnetoBoss_Init(register MagnetoBoss *self, long x, long y, register Placement *pl)
{
	SprInst tmp;
	Placement def;
	short max = 0;
	short base = g_VramTileCursor;
	short n;
	short v;

	(*CLASS(Tbl_MagnetoBoss_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	self->vy = 0;
	self->ay = 0;
	self->health = 50;
	self->maxVx = 0x600;
	self->flags6 |= 0x1083;
	n = Spr_Init(&tmp, 1, CODEPTR(Art_MagnetoBossSpr2), -1, 1, 0, 0);
	max = MAX(max, n);
	n = Spr_Init(&tmp, 1, CODEPTR(Art_MagnetoBossSpr3), -1, 1, 0, 0);
	max = MAX(max, n);
	n = Spr_Init(&tmp, 1, CODEPTR(Art_EvilMagnetoSpr), -1, 1, 0, 0);
	max = MAX(max, n);
	g_VramTileCursor += max + 7;
	self->shotTiles = g_VramTileCursor - 7;
	self->tileBase = base;
	Spr_Init(&self->spr, 1, CODEPTR(Art_EvilMagnetoSpr), -1, 1, base, 0);
	self->target = 0;
	self->bossFlags = 0x10;
	self->shotDir = 0xFF;
	self->wave = (s16 *)CODEPTR(Tbl_MagnetoBossOrbitWave);
	self->wavePhase = 0;
	self->waveShift = 9;
	self->waveYPhase = 8;
	if (!pl) {
		pl = &def;
		pl->x = x >> 8;
		pl->y = y >> 8;
		pl->param[0] = 0xD2;
		pl->param[1] = 0;
		pl->param[2] = 5;
		pl->param[3] = 0;
		pl->param[4] = 0;
	}
	if (v = pl->param[0])
		self->shotPeriod = v;
	else
		self->shotPeriod = 0xD2;
	if (v = pl->param[2])
		self->shotDelay = v;
	else
		self->shotDelay = 5;
	self->shotCooldown = self->shotPeriod;
	(*self->cls->method[20])(self, 0xFF);		/* SetFrame: none */
	g_BgLockObj = self;
	self->state = MagnetoBoss_StateIntroStart;
	self->defaultState = MagnetoBoss_StateIntroStart;
	self->moveMode = 0;
	self->bossFlags &= ~0xE;
	self->minX = -100;
	self->maxX = 8000;
}
