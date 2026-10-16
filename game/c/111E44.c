/* Deathbird_Init - DEATHBIRD Init (class slot +$12): clear the object (keeping its object-table
 * index), the parent's (ACTOR) Init, Reset, then the boss: off the top left of the screen
 * (100 px out), waiting off the left side (moveKind 1), 12 health, its burst and second sprite.
 *
 * self: the boss.  x, y: position (8 fraction bits), passed to the parent and kept as the wobble
 * anchor.  pl: its placement entry (passed to the parent).
 * States Deathbird_StateStart (also the default) and hurt Deathbird_StateHurt; flags6 |= $1180;
 * body sprite g_DeathbirdSprDef with palette 1 and priority; maxSpeed $1000, circling heading,
 * turn rate and turn acceleration 0; the burst timer (Deathbird_UpdateBurst, every frame,
 * stopped) and record (g_SpaceA1Buf); the second sprite's record (g_SceneVar3B18: palette 1 with
 * priority, Art_DeathbirdSpr2, $3D VRAM tiles from g_VramTileCursor, nothing attached); the
 * attack list and hornet schedule at their starts, the first hornet in 120 frames, the flap
 * sound in 13; finally both sprites hidden (SetFrame $FF). */
#include "xmen2.h"

#define g_DeathbirdSpr2		(*(PTR(SprInst) *)g_SceneVar3B18)	/* scene variable: DEATHBIRD's second sprite */
#define g_DeathbirdAttack	(*(PTR(s8) *)g_SceneVar3B14)	/* scene variable: next entry of the attack list */
#define g_DeathbirdHornets	(*(PTR(HornetWave) *)g_SceneVar3B1C)	/* scene variable: next entry of the hornet schedule */
#define g_DeathbirdWingTimer	(*(s16 *)g_SceneVar3B02)	/* scene variable: frames before the flap sound may play again */
#define g_DeathbirdHeading	(*(s32 *)g_SceneVar3B20)	/* scene variable: circling heading, degrees, 8 fraction bits */
#define g_DeathbirdTurnRate	(*(s32 *)g_SceneVar3B24)	/* scene variable: added to the heading each step */
#define g_DeathbirdTurnAccel	(*(s32 *)g_SceneVar3B28)	/* scene variable: added to the turn rate each step */

void Deathbird_Init(register Deathbird *self, long x, long y, Placement *pl)
{
	register short size;
	register char *body;
	char index;

	size = (*self->cls->method[0])();			/* GetSize */
	body = (char *)self;
	body += sizeof self->cls;
	size -= 4;
	index = self->index;
	Mem_Set(body, 0, size);
	self->index = index;
	(*CLASS(Tbl_Deathbird_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	(*self->cls->method[25])(self);				/* Reset */
	self->state = (StateFn)Deathbird_StateStart;
	self->defaultState = (StateFn)Deathbird_StateStart;
	self->hurtState = (StateFn)Deathbird_StateHurt;
	self->flags6 |= 0x80;
	self->flags6 |= 0x1000;
	self->flags6 |= 0x100;
	Obj_SetSpriteDef((Actor *)self, g_DeathbirdSprDef);
	self->spr.flags = self->spr.flags & ~3 | 1;
	self->spr.flags |= 4;
	self->x = (g_CameraX - 100) << 8;
	self->y = (g_CameraY - 100) << 8;
	g_DeathbirdAttack = (s8 *)CODEPTR(Tbl_DeathbirdAttackOrder);
	self->health = 12;
	self->ay = 0;
	self->speed = 0;
	self->accel = 0;
	self->maxSpeed = 0x1000;
	g_DeathbirdHeading = 0;
	g_DeathbirdTurnRate = 0;
	g_DeathbirdTurnAccel = 0;
	self->moveKind = 1;
	self->anchorX = x;
	self->anchorY = y;
	self->wobbleX = 0;
	self->wobbleY = 0;
	self->wobbleVx = 0;
	self->wobbleVy = 0;
	self->unread_AA = 0;
	self->unread_AC = 0;
	self->burstTimer = Timer_Create((TimerFn)Deathbird_UpdateBurst, 1, self, 1);
	self->burst = (DeathbirdBurst *)g_SpaceA1Buf;
	self->spr2 = g_DeathbirdSpr2;
	self->spr2->flags = 5;
	self->spr2->unread_0B = 0;
	self->spr2->frame = 0;
	self->spr2->uploaded = 0xFF;
	self->spr2->firstChild = 0xFF;
	self->spr2->parent = 0xFF;
	self->spr2->nextSibling = 0xFF;
	self->spr2->prevSibling = 0xFF;
	self->spr2->def = CODEPTR(Art_DeathbirdSpr2);
	self->spr2->partStates = 0;
	self->spr2->tileBase = g_VramTileCursor;
	g_VramTileCursor += 0x3D;
	self->spr2Shown = 0;
	g_HornetWaveTimer = 120;
	g_DeathbirdWingTimer = 13;
	g_DeathbirdHornets = (HornetWave *)CODEPTR(Tbl_DeathbirdHornetSchedule);
	(*self->cls->method[20])(self, 0xFF);			/* SetFrame: hide */
}
