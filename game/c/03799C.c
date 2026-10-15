/* MagnetoBoss_StateSpin - MAGNETOBOSS caught by THRONEPHONE (MagnetoBoss_EnterSpinState): each
 * time the spin animation (Tbl_MagnetoBossAnim2_2) ends, flip horizontally and play it again.
 *
 * self: the boss. */
#include "xmen2.h"

void MagnetoBoss_StateSpin(register MagnetoBoss *self)
{
	if (!self->anim) {
		self->spr.flags ^= 0x10;
		Obj_ChangeAnim(self, CODEPTR(Tbl_MagnetoBossAnim2_2));
	}
}
