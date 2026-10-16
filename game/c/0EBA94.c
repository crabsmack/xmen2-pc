/* CoreShields_TakeDamage - CORESHIELDS TakeDamage (class slot +$26; the parent's isn't called):
 * break the front panel, or if it is already broken the one before it (wrapping 0 to 7); when
 * one broke: hit-flash $B4 frames, sound $D0, an explosion (kind 1) at the centre of its body box.
 *
 * self: the shields.  attacker, type, damage, hx, hy: the hit (unused).
 * No health is taken. */
#include "xmen2.h"

void CoreShields_TakeDamage(register CoreShields *self, Actor *attacker, char type, char damage,
	short hx, short hy)
{
	register char hit = 0;
	register short i;

	if (!self->broken[self->rotation]) {
		self->broken[self->rotation] = 1;
		hit = 1;
	} else {
		i = self->rotation;
		i -= 1;
		if (i < 0)
			i = 7;
		if (!self->broken[i]) {
			self->broken[i] = 1;
			hit = 1;
		}
	}
	if (hit) {
		self->hitCooldown = 0xB4;
		Snd_Stop(0xD0);
		Snd_Play(0xD0);
		Explosion_Spawn((self->boxRight + self->boxLeft) << 7, (self->boxTop + self->boxBottom) << 7, 1);
	}
}
