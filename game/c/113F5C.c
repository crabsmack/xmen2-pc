/* XNpc_HurtReact - how an XNPC reacts to a hit it survives (XNpc_OnHurt): save the state it was
 * in, play its hurt sound, then by hit type: 9 (from the right) knock it left, 10 knock it right,
 * 11 and 12 stun it; the new state runs on the next update.
 *
 * self:     the object.
 * strength: the hit's damage: the knockback distance (XNpc_KnockbackDist).
 * type:     hit type.
 * The knockback stops at the object's move limit that way (class slot +$7A, GetMoveLimit):
 * kind 2 (no limit) or far enough: the full distance, a shake and slide
 * (XNpc_StateKnockStart); else only up to the limit, as a plain knockback at a wall (kind 0) or
 * a knockback B (XNpc_StateKnockBStart: off an open edge, then explode) at an edge (kind 1).
 * Knocked left its h-flip goes off, knocked right on. */
#include "xmen2.h"

void XNpc_HurtReact(XNpc *self, char strength, char type)
{
#ifdef XMEN2_MPW
	char kind;
#else
	struct { s8 v; operator char() const { return v; } } kind;	/* a game-memory byte: GetMoveLimit, maybe 68000 code, writes it through &kind */
#endif

	if (!self->hurtState)
		self->hurtState = self->state;
	Snd_Stop((unsigned char)(self->stats->hurtSound == 0 ? 99 : self->stats->hurtSound));
	Snd_Play((unsigned char)(self->stats->hurtSound == 0 ? 99 : self->stats->hurtSound));
	switch (type) {
	case 11:			/* XNpc_HurtReact_Stun */
	case 12:
		self->state = XNpc_StateStunStart;
		break;
	case 9: {
		short dist = XNpc_KnockbackDist(self, strength);
		short to = (self->x >> 8) - dist;
		short limit = (*self->cls->method[29])(self, 0, &kind);	/* +$7A GetMoveLimit, left */
		if (kind == 2 || limit <= to) {
			self->state = XNpc_StateKnockStart;
			self->knock = -dist;
		} else {
			self->knock = limit - (self->x >> 8);
			if (!kind)
				self->state = XNpc_StateKnockStart;
			else
				self->state = XNpc_StateKnockBStart;
		}
		self->spr.flags &= ~0x10;
		break;
	}
	case 10: {
		short dist = XNpc_KnockbackDist(self, strength);
		short to = (self->x >> 8) + dist;
		short limit = (*self->cls->method[29])(self, 1, &kind);	/* +$7A GetMoveLimit, right */
		if (kind == 2 || limit >= to) {
			self->state = XNpc_StateKnockStart;
			self->knock = (s8)dist;
		} else {
			self->knock = limit - (self->x >> 8);
			if (!kind)
				self->state = XNpc_StateKnockStart;
			else
				self->state = XNpc_StateKnockBStart;
		}
		self->spr.flags |= 0x10;
		break;
	}
	}
	/* XNpc_HurtReact_Done */
	self->timer = 0;
}
