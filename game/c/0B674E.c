/* BrainBabyBio_Init - BRAINBABYBIO Init (class slot +$12): the parent's (BABYBIO) Init, then the
 * Chamber's settings: palette 3, the arena x $60-$220 / y $60-$140 as its patrol range, the
 * Chamber's shared stats record (g_A4Stats) with health 4 (its health from it), its AI script
 * (Tbl_BrainBabyBio_ScriptLen: the steps of BrainBabyBio_Script, BabyBio_ActPlayAnimC,
 * BabyBio_ActFacePlayer, BabyBio_ActStartFiring twice, BabyBio_ActStopWait30), state and
 * default state BrainBabyBio_StateStart, flags6 bit
 * $1000, its home at its position, frame 4 (slot +$56), its boxes (Obj_UpdateBoxes), sound $46
 * restarted, and the camera keeps it in view (g_A4CamFocus).
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry (passed on). */
#include "xmen2.h"

#define g_A4Stats	(*(PTR(NpcStats) *)g_SceneVar3B14)	/* scene variable: the stats record of the Chamber's enemies */
#define g_A4CamFocus	(*(PTR(Actor) *)g_SceneVar3B20)	/* scene variable: the enemy the camera also keeps in view (0: none) */

void BrainBabyBio_Init(BrainBabyBio *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_BrainBabyBio_vtable)->parent->method[3])(self, x, y, pl);	/* BABYBIO Init */
	SavageA4_NopStatic();
	self->spr.flags = self->spr.flags & ~3 | 3;
	self->xMin = 0x60;
	self->xMax = 0x220;
	self->ceilY = 0x60;
	self->floorY = 0x140;
	g_A4Stats->health = 4;
	self->stats = g_A4Stats;
	self->health = g_A4Stats->health;
	AiScript_Set(&self->ai, CODEPTR(Tbl_BrainBabyBio_ScriptLen));
	self->state = self->defaultState = (StateFn)BrainBabyBio_StateStart;
	self->flags6 |= 0x1000;
	self->homePx = self->x >> 8;
	self->homePy = self->y >> 8;
	(*self->cls->method[20])(self, 4);	/* +$56 SetFrame */
	Obj_UpdateBoxes(self);
	Snd_Stop(0x46);
	Snd_Play(0x46);
	g_A4CamFocus = (Actor *)self;
}
