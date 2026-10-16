/* ExodusBlast_StateFlyLeft - EXODUSBLAST state while flying left: left of x $21 (pixels) it
 * starts the psychic flash (Exodus_StartPsiFlash) and dies from the next update
 * (ExodusBlast_StateDie).
 *
 * self: the blast. Returns self->flags & 8 when it switches, no value while flying. */
#include "xmen2.h"

short ExodusBlast_StateFlyLeft(register ExodusBlast *self)
{
	if (self->x < 0x2100) {
		Exodus_StartPsiFlash();
		SET_STATE(self, (StateFn)ExodusBlast_StateDie);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
