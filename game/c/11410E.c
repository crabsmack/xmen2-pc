/* XNpc_DeathReact - how an XNPC reacts to the hit that kills it (XNpc_OnDeath): hit types $D-$F
 * just remove it (Kill); else its hurt sound, then by hit type: 9 fling it left, 10 right, 11
 * and 12 blow it up (XNpc_StateExplode); the new state runs on the next update.
 *
 * self:     the object.
 * strength: the hit's damage: the fling goes twice the knockback distance (XNpc_KnockbackDist).
 * type:     hit type.
 * As in XNpc_HurtReact the move limit that way (class slot +$7A) can cut the distance short:
 * at an edge (kind 1) the body is flung high (XNpc_StateDeathFlingHigh, landing 60 px lower),
 * else level (XNpc_StateDeathFling). */
#include "xmen2.h"

void XNpc_DeathReact(XNpc *self, char strength, char type)
{
#ifdef XMEN2_MPW
	char kind;
#else
	struct { s8 v; operator char() const { return v; } } kind;	/* a game-memory byte: GetMoveLimit, maybe 68000 code, writes it through &kind */
#endif

	if (type == 13 || type == 14 || type == 15) {
		(*self->cls->method[19])(self);	/* +$52 Kill */
		return;
	}
	Snd_Stop((unsigned char)(self->stats->hurtSound == 0 ? 99 : self->stats->hurtSound));
	Snd_Play((unsigned char)(self->stats->hurtSound == 0 ? 99 : self->stats->hurtSound));
	switch (type) {
	case 11:			/* XNpc_DeathReact_Explode */
	case 12:
		self->state = (StateFn)XNpc_StateExplode;
		break;
	case 9: {
		short dist = XNpc_KnockbackDist(self, strength) << 1;
		short to = (self->x >> 8) - dist;
		short limit = (*self->cls->method[29])(self, 0, &kind);	/* +$7A GetMoveLimit, left */
		if (kind == 2 || limit <= to) {
			self->state = (StateFn)XNpc_StateDeathFling;
			self->knock = -dist;
		} else {
			self->knock = limit - (self->x >> 8);
			if (!kind)
				self->state = (StateFn)XNpc_StateDeathFling;
			else
				self->state = (StateFn)XNpc_StateDeathFlingHigh;
		}
		self->spr.flags &= ~0x10;
		break;
	}
	case 10: {
		short dist = XNpc_KnockbackDist(self, strength) << 1;
		short to = (self->x >> 8) + dist;
		short limit = (*self->cls->method[29])(self, 1, &kind);	/* +$7A GetMoveLimit, right */
		if (kind == 2 || limit >= to) {
			self->state = (StateFn)XNpc_StateDeathFling;
			self->knock = (s8)dist;
		} else {
			self->knock = limit - (self->x >> 8);
			if (!kind)
				self->state = (StateFn)XNpc_StateDeathFling;
			else
				self->state = (StateFn)XNpc_StateDeathFlingHigh;
		}
		self->spr.flags |= 0x10;
		break;
	}
	}
	/* XNpc_DeathReact_Done */
	self->timer = 0;
}
