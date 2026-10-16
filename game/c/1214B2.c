/* BioSentinel_TakeDamage - BIOSENTINEL TakeDamage (class slot +$26). Only while vulnerable:
 * psychic hits (types $D-$F) go to the parent's (BASICBIOSENTINEL) TakeDamage; others: sound
 * $63, health -= damage, face away from a type 9 / type 10 hit (9: right, 10: left), 40 frames
 * of hit cooldown and the hurt state (+$60) from the next update.
 *
 * self: the sentinel.  attacker, hx, hy: passed on.  type: the hit type.  damage: health lost. */
#include "xmen2.h"

void BioSentinel_TakeDamage(register BioSentinel *self, Actor *attacker, register char type, register char damage, short hx, short hy)
{
	if (self->vulnerable) {
		switch (type) {
		case 0xD:
		case 0xE:
		case 0xF:
			(*CLASS(Tbl_BioSentinel_vtable)->parent->method[8])(self, attacker, type, damage, hx, hy);	/* BASICBIOSENTINEL TakeDamage */
			break;
		default:
			Snd_Stop(0x63);
			Snd_Play(0x63);
			self->health -= damage;
			switch (type) {
			case 9:
				self->spr.flags &= ~0x10;
				break;
			case 10:
				self->spr.flags |= 0x10;
				break;
			}
			self->hitCooldown = 40;
			self->state = self->hurtState;
			self->timer = 0;
			break;
		}
	}
}
