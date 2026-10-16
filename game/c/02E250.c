/* ExodusBlast_StateFlyRight - EXODUSBLAST state while flying right: past x $160 (pixels) it
 * starts the psychic flash (Exodus_StartPsiFlash) and dies from the next update
 * (ExodusBlast_StateDie).
 *
 * self: the blast. Returns self->flags & 8 when it switches, no value while flying. */
#include "xmen2.h"

short ExodusBlast_StateFlyRight(register ExodusBlast *self)
{
	if (self->x > 0x16000) {
		Exodus_StartPsiFlash();
		SET_STATE(self, (StateFn)ExodusBlast_StateDie);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
