/* Apocalypse_TakeDamage - APOCALYPSE TakeDamage (class slot +$26; the parent's isn't called).
 * While circling it only flinches: frame 7, sound $EB, 60 frames of hit cooldown, no health
 * lost. Otherwise its hurt state (+$60, if any) runs from the next update, 10 frames of hit
 * cooldown and health -= damage.
 *
 * self: the boss.  damage: health lost.  attacker, type, hx, hy (the hit point): unused. */
#include "xmen2.h"

void Apocalypse_TakeDamage(register Apocalypse *self, Actor *attacker, char type, char damage, short hx, short hy)
{
	if (self->moveFlags & 1) {
		(*self->cls->method[20])(self, 7);		/* +$56 SetFrame */
		Snd_Stop(0xEB);
		Snd_Play(0xEB);
		self->hitCooldown = 60;
	} else {
		if (self->hurtState) {
			self->state = self->hurtState;
			self->timer = 0;
		}
		self->hitCooldown = 10;
		self->health -= damage;
	}
}
