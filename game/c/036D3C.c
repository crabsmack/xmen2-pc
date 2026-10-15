/* MagnetoBoss_StateIntroShake3 - MAGNETOBOSS intro: shake (Tbl_MagnetoBossQuake3) and crack the
 * wall (column 0, rows 1-10) again, until it is broken: then rubble flies from rows 6-8 and it
 * bursts in (MagnetoBoss_StateBurstIn); else wait for the shake and repeat
 * (MagnetoBoss_StateIntroWait3).
 *
 * self: the boss.
 * Returns self->flags & 8. */
#include "xmen2.h"

short MagnetoBoss_StateIntroShake3(register MagnetoBoss *self)
{
	unsigned char mirror = 0;

	g_CamShake = CODEPTR(Tbl_MagnetoBossQuake3);
	if (MagnetoBoss_CrackWall(0, 1, 10, 0)) {
		MagnetoBoss_SpawnRubble(0, 6, 8, 8, mirror, 0x180, 0x11);
		SET_STATE(self, MagnetoBoss_StateBurstIn);
		return self->flags & 8;
	}
	SET_STATE(self, MagnetoBoss_StateIntroWait3);
	return self->flags & 8;
}
