/* Barbarian_StateLeap - BARBARIAN state: leap off (sound $AD), the animation stopped; the
 * leap's frames follow from Barbarian_StateLeapRise.
 *
 * self: the object (Barbarian_SetupLeap has set the jump's speeds).
 * Returns self->flags & 8. */
#include "xmen2.h"

short Barbarian_StateLeap(Barbarian *self)
{
	Snd_Stop(0xAD);
	Snd_Play(0xAD);
	self->anim = 0;
	SET_STATE(self, Barbarian_StateLeapRise);
	return self->flags & 8;
}
