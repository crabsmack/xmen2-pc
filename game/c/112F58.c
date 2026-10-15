/* Deathbird_StatePeck - DEATHBIRD on the lift switch: the second peck animation, then hit the
 * switch when it ends (Deathbird_StateHitSwitch).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Deathbird_StatePeck(Deathbird *self)
{
	Obj_ChangeAnim(self, CODEPTR(Tbl_DeathbirdAnim7));
	SET_STATE_WAIT(self, (StateFn)Deathbird_StateHitSwitch, 0xFF);
	return self->flags & 8;
}
