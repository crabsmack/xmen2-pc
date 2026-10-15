/* MagnetoBoss_EnterOutroState - THRONEPHONE pulls MAGNETOBOSS in: the outro
 * (MagnetoBoss_StateOutro) from the next update, and bossFlags bit 7.
 *
 * self: the boss. Called by ThronePhone_StatePullIn. */
#include "xmen2.h"

void MagnetoBoss_EnterOutroState(register MagnetoBoss *self)
{
	self->state = MagnetoBoss_StateOutro;
	self->timer = 0;
	self->bossFlags |= 0x80;
}
