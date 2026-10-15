/* Cyclops_AirAttackDown - CYCLOPS Down+B in the air (class slot +$BE): the same as B in the air
 * (his AirAttack, slot +$C2: Cyclops_AirAttack), now.
 *
 * self: the player. */
#include "xmen2.h"

void Cyclops_AirAttackDown(Cyclops *self)
{
	GOTO_STATE(self, (StateFn)self->cls->method[47]);	/* AirAttack */
}
