/* Deathbird_StateHitSwitch - DEATHBIRD on the lift switch: hit it (the switch tile (7, 11) as if
 * it touched it: SpaceA1_SwitchTileHit reverses the lift), then fly off (SetMoveMode 4,
 * Deathbird_StateFlyOff).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Deathbird_StateHitSwitch(Deathbird *self)
{
	SpaceA1_SwitchTileHit(7, 11, self, -1, -1);
	(*self->cls->method[27])(self, 4);	/* SetMoveMode */
	SET_STATE(self, (StateFn)Deathbird_StateFlyOff);
	return self->flags & 8;
}
