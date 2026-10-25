/* HornetBullet_Init - HORNETBULLET Init (class slot +$12): the parent's (ACTOR) Init, not moving
 * (whoever fires it sets the velocity), sprite set g_HornetBulletSprDef with palette 1, damage 1,
 * flags6 |= $4082, priority bit (2) from the live player (player 1 when neither is).
 *
 * self: the bullet.  x, y, pl: passed to the parent's Init. Never runs in the corpus. */
#include "xmen2.h"

void HornetBullet_Init(HornetBullet *self, long x, long y, Placement *pl)
{
	(*CLASS(Tbl_HornetBullet_vtable)->parent->method[3])(self, x, y, pl);	/* ACTOR Init */
	STOP_MOVING(self);
	Obj_SetSpriteDef((Actor *)self, g_HornetBulletSprDef);
	self->spr.flags = self->spr.flags & ~3 | 1;
	self->attack = 1;
	self->flags6 |= 0x4082;
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
	} else
#if defined(XMEN2_NATIVE)
	if (!P34_CopyLiveLayer(&self->spr, 1))	/* players 1-2 out: a live player 3-4's (R11) */
#endif
	{
		if (g_Player1.spr.flags & 4)
			self->spr.flags |= 4;
		else
			self->spr.flags &= ~4;
	}
}
