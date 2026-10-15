/* MagnetoBoss_StateIntroShake1 - MAGNETOBOSS intro: the screen shakes (Tbl_MagnetoBossQuake1)
 * and the wall at column 0, rows 1-10, cracks one stage; then wait for the shake to end
 * (MagnetoBoss_StateIntroWait1).
 *
 * self: the boss.
 * Returns self->flags & 8. */
#include "xmen2.h"

short MagnetoBoss_StateIntroShake1(register MagnetoBoss *self)
{
	g_CamShake = CODEPTR(Tbl_MagnetoBossQuake1);
	MagnetoBoss_CrackWall(0, 1, 10, 0);
	SET_STATE(self, MagnetoBoss_StateIntroWait1);
	return self->flags & 8;
}
