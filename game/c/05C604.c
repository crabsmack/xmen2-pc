/* Tusk_StateVulnerableWait - TUSK: when the countdown is out, choose the next attack
 * (Tusk_StateChooseAttack).
 *
 * self: TUSK.
 * Returns 0 when it switches; no value while waiting. */
#include "xmen2.h"

short Tusk_StateVulnerableWait(Tusk *self)
{
	if (self->countdown == 0) {
		self->state = Tusk_StateChooseAttack;
		return 0;
	}
	NO_RETURN_VALUE
}
