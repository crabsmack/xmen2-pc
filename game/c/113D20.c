/* XNpc_TakeDamage - XNPC TakeDamage (class slot +$26): hits of types 9-12 (4 counts as 12) hurt
 * it; other types go to the parent's (ACTOR) TakeDamage.
 *
 * self:     the object.
 * attacker: what hit it.  type: hit type.  damage: health to lose (also the knockback
 *           strength OnHurt / OnDeath get).  hx, hy: the hit point.
 * A hurting hit takes the health, sets the hurt flag (flags bit 2), shows the stats record's
 * hurt frame (unless $FF) and plays its hurt sound (0: $63). Health 0 or less: 10000 frames
 * before it can be hit again and OnDeath (slot +$72); else 30 frames and OnHurt (+$6E). */
#include "xmen2.h"

void XNpc_TakeDamage(XNpc *self, Actor *attacker, char type, char damage, short hx, short hy)
{
	switch (type) {
	case 4:				/* XNpc_TakeDamage_Type4 */
		type = 12;
		goto hurt;
	default:			/* XNpc_TakeDamage_Default (also types 5-8) */
		(*CLASS(Tbl_XNpc_vtable)->parent->method[8])(self, attacker, type, damage, hx, hy);	/* ACTOR TakeDamage */
		break;
	case 9:
	case 10:
	case 11:
	case 12:
	hurt:				/* XNpc_TakeDamage_Hurt */
		self->health -= damage;
		self->flags |= 4;
		if (self->stats->hurtFrame != 0xFF)
			(*self->cls->method[20])(self, self->stats->hurtFrame);	/* +$56 SetFrame */
		Snd_Stop((unsigned char)(self->stats->hurtSound == 0 ? 99 : self->stats->hurtSound));
		Snd_Play((unsigned char)(self->stats->hurtSound == 0 ? 99 : self->stats->hurtSound));
		if (self->health <= 0) {
			self->hitCooldown = 10000;
			(*self->cls->method[27])(self, damage, type);	/* +$72 OnDeath */
		} else {
			self->hitCooldown = 30;
			(*self->cls->method[26])(self, damage, type);	/* +$6E OnHurt */
		}
		break;
	}
	/* XNpc_TakeDamage_Done */
}
