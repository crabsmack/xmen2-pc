/* BroodBoss_Init - BROODBOSS Init (class slot +$12): the parent's (BROODBOSSPARTS) Init, then the
 * boss: no motion, attack 1 (hit type 5), 24 health, flags6 |= $5102, states
 * BroodBoss_StateSetup / hurt BroodBoss_StateHurt, sprite Art_BroodBossSpr (palette 2, no
 * priority), layer 0, no claw, leg or wall yet (the arm field is left as it is), arm frame $FF,
 * its AI script; then the arena from the map around the placement: the floor (the segment
 * below, its upper end; none: y + $5A) for both debris pools (the clone debris also bounces at
 * most 3 times), the scroll x the claw swings the arena to (placement param 0, or x - $220),
 * and the camera position the arena pins (CloneA5_CamApply): y = the segment above, its lower
 * end - 8 (none: y - $70), x = the type $14 wall segment to the left, its right end (none:
 * x - $220).
 *
 * self: the boss.  x, y: position (8 fraction bits).  pl: its placement entry. */
#include "xmen2.h"

#define g_BroodArmFrame		(*(s16 *)g_SceneVar3B10)	/* scene variable: frame of the boss arm (BroodClaw_Swing) */
#define g_BroodBossDebris	(*(PTR(DebrisPool) *)g_SceneVar3B14)	/* scene variable: the boss's explosion debris pool */
#define g_CloneDebris		(*(PTR(DebrisPool) *)g_SceneVar3B1C)	/* scene variable: the clone-maker debris pool */
#define g_BroodArenaLeft	(*(s32 *)g_SceneVar3B20)	/* scene variable: scroll x the claw swings the arena to (CloneA5_CamUpdate, BroodClaw_Swing) */
#define g_BroodCamY		(*(s16 *)g_SceneVar3B22)	/* scene variable: camera y in the arena (CloneA5_CamApply pins it) */
#define g_BroodCamX		(*(s16 *)g_SceneVar3B24)	/* scene variable: camera x in the arena (CloneA5_CamApply pins it) */

void BroodBoss_Init(BroodBoss *self, long x, long y, Placement *pl)
{
	register short v;
	register short py;
	Seg *seg;

	(*CLASS(Tbl_BroodBoss_vtable)->parent->method[3])(self, x, y, pl);
	STOP_MOVING(self);
	self->attack = 1;
	self->state = self->defaultState = BroodBoss_StateSetup;
	self->hurtState = BroodBoss_StateHurt;
	self->spr.def = CODEPTR(Art_BroodBossSpr);
	self->spr.flags = self->spr.flags & ~3 | 2;
	self->spr.flags &= ~4;
	self->layer = 0;
	self->flags6 |= 0x5102;
	self->hitType = 5;
	self->health = 24;
	self->claw = 0;
	g_BroodArmFrame = 0xFF;
	self->leg = 0;
	self->wall = 0;
	AiScript_Set(&self->ai, CODEPTR(Tbl_BroodBossScriptLen));	/* 44 steps: BroodBoss_ScrStepClaw, BroodBoss_ScrStepScream */
	v = pl->x;
	py = pl->y;
	seg = Seg_FindBelow(v, py + 0x20, 1000);
	if (seg)
		v = MIN(seg->y1, seg->y2);
	else
		v = py + 0x5A;
	Debris_SetFloorY(g_CloneDebris, v);
	Debris_SetBounceLimit(g_CloneDebris, 3);
	Debris_SetFloorY(g_BroodBossDebris, v);
	g_BroodArenaLeft = (v = pl->param[0]) ? v : pl->x - 0x220;
	seg = Seg_FindAbove(pl->x, pl->y + 0x20, 0);
	if (seg)
		v = MAX(seg->y1, seg->y2) - 8;
	else
		v = pl->y - 0x70;
	g_BroodCamY = v;
	seg = Seg_FindLeft(pl->x, pl->y + 0x20, 0x14);
	if (seg)
		v = MAX(seg->x1, seg->x2);
	else
		v = pl->x - 0x220;
	g_BroodCamX = v;
}
