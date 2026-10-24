/* BrainChild_Init - BRAINCHILD Init (class slot +$12): the parent's (BIONPC) Init, then sprite
 * Art_BrainChildSpr (palette 1), the arena x $60-$220 / y $60-$140, the Chamber's shared stats
 * record (g_A4Stats) with health 4 (its health from it), its AI script (Tbl_BrainChildAiScript:
 * BrainChild_ActFacePlayer, BrainChild_ActShoot, BrainChild_ActIdle30), state and default
 * state BrainChild_StateStart, flags6 |= $1083, not moving, its animation
 * (Tbl_BrainChildAnim), its boxes (Obj_UpdateBoxes), sound $45 restarted, and the camera keeps it
 * in view (g_A4CamFocus).
 *
 * self: the object.  x, y: position (8 fraction bits).  pl: its placement entry (passed on). */
#include "xmen2.h"

#define g_A4Stats	(*(PTR(NpcStats) *)g_SceneVar3B14)	/* scene variable: the stats record of the Chamber's enemies */
#define g_A4CamFocus	(*(PTR(Actor) *)&g_SceneVar3B20)	/* scene variable: the enemy the camera also keeps in view (0: none) */

void BrainChild_Init(BrainChild *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_BrainChild_vtable)->parent->method[3])(self, x, y, pl);	/* BIONPC Init */
	self->spr.def = CODEPTR(Art_BrainChildSpr);
	self->spr.flags = self->spr.flags & ~3 | 1;
	self->xMin = 0x60;
	self->xMax = 0x220;
	self->ceilY = 0x60;
	self->floorY = 0x140;
	g_A4Stats->health = 4;
	self->stats = g_A4Stats;
	self->health = g_A4Stats->health;
	AiScript_Set(&self->ai, CODEPTR(Tbl_BrainChildAiScript));
	self->state = self->defaultState = BrainChild_StateStart;
	self->flags6 |= 0x80;
	self->flags6 |= 0x1000;
	self->flags6 |= 2;
	self->flags6 |= 1;
	STOP_MOVING(self);
	self->unread_A4 = 0;
	Obj_SetAnim(self, CODEPTR(Tbl_BrainChildAnim));
	Obj_UpdateBoxes(self);
	Snd_Stop(0x45);
	Snd_Play(0x45);
	g_A4CamFocus = (Actor *)self;
}
