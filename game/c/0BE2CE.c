/* Barbarian_StateAttack - BARBARIAN state: attack (sound $AC) with the next of its five attack
 * animations (attackIndex 0-4): Barbarian_StateAttack0 .. Barbarian_StateAttack4.
 *
 * self: the object.
 * Returns self->flags & 8.
 * The cases are the disassembly's fragments Barbarian_StateAttack_Reset (default, then case 0),
 * Barbarian_StateAttack_Case1 (cases 1-3), Barbarian_StateAttack_Case4 and
 * Barbarian_StateAttack_Done (the shared return). */
#include "xmen2.h"

short Barbarian_StateAttack(Barbarian *self)
{
	Snd_Stop(0xAC);
	Snd_Play(0xAC);
	switch (++self->attackIndex) {
	default:			/* Barbarian_StateAttack_Reset */
		self->attackIndex = 0;
	case 0:
		SET_STATE(self, Barbarian_StateAttack0);
		return self->flags & 8;
	case 1:				/* Barbarian_StateAttack_Case1 */
		SET_STATE(self, Barbarian_StateAttack1);
		return self->flags & 8;
	case 2:
		SET_STATE(self, Barbarian_StateAttack2);
		return self->flags & 8;
	case 3:
		SET_STATE(self, Barbarian_StateAttack3);
		return self->flags & 8;
	case 4:				/* Barbarian_StateAttack_Case4 */
		SET_STATE(self, Barbarian_StateAttack4);
		return self->flags & 8;
	}
}
