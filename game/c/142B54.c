/* TechWallGun_Init - TECHWALLGUN Init (class slot +$12): the parent's (ACTOR) Init, then a gun
 * mounted by placement param 0: 0 on a left wall (facing right, frame 2), 1 on the ceiling
 * (frame 8, its own first state), 2 on a right wall (h-flipped, frame 2).
 *
 * self: the gun.  x, y: position (8 fraction bits).  pl: its placement entry.
 * Not moving; states TechWallGun_StartWall (StartCeiling on the ceiling), hurt state
 * TechWallGun_StateHurt; flags6 |= $4081; sprite Art_Spr_1432FE, palette 0 in g_ScnSpaceB1,
 * else 3; priority bit (2) set. Range 250 px; params 1-3 (0: the default): pause after a burst
 * (120), shots per burst (1), frames between shots (10); first burst level (direction 2,
 * animation Tbl_Anim_1432AC). */
#include "xmen2.h"

void TechWallGun_Init(register TechWallGun *self, long x, long y, register Placement *pl)
{
	(*CLASS(Tbl_TechWallGun_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	STOP_MOVING(self);
	self->state = TechWallGun_StartWall;
	self->defaultState = TechWallGun_StartWall;
	self->hurtState = TechWallGun_StateHurt;
	self->flags6 |= 0x80;
	self->flags6 |= 0x4000;
	self->flags6 |= 1;
	self->spr.def = CODEPTR(Art_Spr_1432FE);
	if (g_ScnSpaceB1 == g_SceneIndex)
		self->spr.flags = self->spr.flags & ~3 | 0;
	else
		self->spr.flags = self->spr.flags & ~3 | 3;
	if ((self->mount = pl->param[0]) == 0) {
		self->spr.flags &= ~0x10;
		(*self->cls->method[20])(self, 2);	/* +$56 SetFrame */
	} else if (self->mount == 1) {
		self->spr.flags &= ~0x10;
		(*self->cls->method[20])(self, 8);	/* +$56 SetFrame */
		self->state = TechWallGun_StartCeiling;
		self->defaultState = TechWallGun_StartCeiling;
		self->hurtState = TechWallGun_StateHurt;
	} else if (self->mount == 2) {
		self->spr.flags |= 0x10;
		(*self->cls->method[20])(self, 2);	/* +$56 SetFrame */
	}
	self->spr.flags |= 4;
	self->unread_8C = 0;
	self->unread_9C = 40;
	self->range = 250;
	self->aimState = 0;
	self->burstPause = pl->param[1] ? pl->param[1] : 120;
	self->burstShots = pl->param[2] ? pl->param[2] : 1;
	self->shotDelay = pl->param[3] ? pl->param[3] : 10;
	self->shotsLeft = 0;
	self->shotDir = 2;
	self->fireAnim = CODEPTR(Tbl_Anim_1432AC);
}
