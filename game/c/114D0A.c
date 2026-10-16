/* BioNpc_TakeDamage - BIONPC's TakeDamage (class slot +$26): 30 frames of hit cooldown. A psychic
 * hit (types $D-$F) stuns it: 30 stun frames, its timer runs BioNpc_UpdateStunned and its
 * draw-list node BioNpc_DrawStunned, it loses the damage, its palette is saved (stun bits 14-15),
 * sound $36 and its stats' hurt frame (slot +$56 SetFrame; none for $FF). Other hits go to the
 * parent's (XNPC) TakeDamage, unless it is stunned (then they are ignored).
 *
 * self: the object.  attacker: who hit it.  type: hit type.  damage: health lost.  hx, hy: the
 * hit point (passed on). */
#include "xmen2.h"

void BioNpc_TakeDamage(register BioNpc *self, Actor *attacker, register char type, register char damage,
	short hx, short hy)
{
	register DrawNode *node;

	self->hitCooldown = 30;
	switch (type) {
	case 0xD:
		self->stun = self->stun & 0xC000 | 30;
		break;
	case 0xE:
		self->stun = self->stun & 0xC000 | 30;
		break;
	case 0xF:
		self->stun = self->stun & 0xC000 | 30;
		break;
	default:
		if (!(self->stun & 0x3FFF))
			(*CLASS(Tbl_BioNpc_vtable)->parent->method[8])(self, attacker, type, damage, hx, hy);	/* XNPC TakeDamage */
		return;
	}
	((Timer *)self->timerObj)->func = (TimerFn)BioNpc_UpdateStunned;
	if (self->drawNode >= 0) {
		node = &g_DrawNodes[self->drawNode];
		node->draw = (DrawFn)BioNpc_DrawStunned;
	}
	self->health -= damage;
	self->stun = (self->spr.flags & 3) << 14 | self->stun & 0x3FFF;
	Snd_Stop(0x36);
	Snd_Play(0x36);
	if (self->stats->hurtFrame != 0xFF)
		(*self->cls->method[20])(self, self->stats->hurtFrame);	/* +$56 SetFrame */
}
