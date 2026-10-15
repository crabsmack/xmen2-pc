/* Hornet_Init - HORNET Init (class slot +$12): placed at its placement entry's position, the
 * parent's (XNPC) Init, then its stats, sprite, motion and attack parameters.
 *
 * self: the object.  x, y: position (8 fraction bits), replaced by the placement's when pl is
 * given.  pl: its placement entry (or Hornet_SpawnForDeathbird's record); the parameters are read
 * even when it is 0.
 * Not moving, Tbl_HornetStats (health from it), state Hornet_StateAttackDelay, sprite set
 * g_HornetSprDef with palette 3, damage 1, flags6 |= $80, motion Hornet_MoveStartSwing. Placement
 * params 1-5 (0: the default): swing radius (64), swing step (2), attack delay (60), shots per
 * volley (1), shot delay (10). The swing starts at -180 degrees going forward (0 going back)
 * around a centre one radius to the side; circle path at shift 4, lifetime 480. Priority bit
 * (2) from the live player (player 1 when neither is); idle animation. */
#include "xmen2.h"

void Hornet_Init(Hornet *self, long x, long y, Placement *pl)
{
	if (pl) {
		short px = pl->x;
		short py = pl->y;

		x = self->x = px << 8;
		y = self->y = py << 8;
	}
	(*CLASS(Tbl_Hornet_vtable)->parent->method[3])(self, x, y, pl);	/* XNPC Init */
	STOP_MOVING(self);
	self->stats = (NpcStats *)CODEPTR(Tbl_HornetStats);
	self->health = self->stats->health;
	self->state = (StateFn)Hornet_StateAttackDelay;
	Obj_SetSpriteDef((Actor *)self, g_HornetSprDef);
	self->spr.flags = self->spr.flags & ~3 | 3;
	self->attack = 1;
	self->flags6 |= 0x80;
	self->move = (HornetMoveFn)Hornet_MoveStartSwing;
	self->moveFlags = 0;
	self->shotsLeft = 0;
	self->swingRadius = pl->param[0] ? pl->param[0] : 0x40;
	self->swingStep = pl->param[1] ? pl->param[1] : 2;
	self->attackDelay = pl->param[2] ? pl->param[2] : 60;
	self->volleyShots = pl->param[3] ? pl->param[3] : 1;
	self->shotDelay = pl->param[4] ? pl->param[4] : 10;
	self->swingAngle = self->swingStep >= 0 ? -180 : 0;
	self->swingCentre = self->swingStep >= 0 ? self->swingRadius : -self->swingRadius;
	self->unk_B2 = 8;
	self->unk_B4 = 9;
	self->path = (FlyPathStep *)CODEPTR(Tbl_HornetCirclePath);
	self->pathShift = 4;
	self->lifetime = 480;
	if (IS_ALIVE(&g_Player1)) {
		if (g_Player1.spr.flags & 4)
			self->spr.flags |= 4;
		else
			self->spr.flags &= ~4;
	} else if (IS_ALIVE(&g_Player2)) {
		if (g_Player2.spr.flags & 4)
			self->spr.flags |= 4;
		else
			self->spr.flags &= ~4;
	} else {
		if (g_Player1.spr.flags & 4)
			self->spr.flags |= 4;
		else
			self->spr.flags &= ~4;
	}
	Obj_ChangeAnim(self, CODEPTR(Tbl_HornetAnimIdle));
}
