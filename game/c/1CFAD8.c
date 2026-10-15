/* Gambit_GroundThrow - GAMBIT state: the ground throw: throw the cards (Gambit_ThrowCards), then
 * recover when the animation ends (timer $FF): crouched after the crouched throw (kind 4,
 * XPlayer_CrouchAttackRecover), else XPlayer_AttackRecover.
 *
 * self: the player. */
#include "xmen2.h"

void Gambit_GroundThrow(register Gambit *self)
{
	Gambit_ThrowCards(self);
	self->timer = 0xFF;
	if (self->throwKind == 4)
		self->state = (StateFn)XPlayer_CrouchAttackRecover;
	else
		self->state = (StateFn)XPlayer_AttackRecover;
}
