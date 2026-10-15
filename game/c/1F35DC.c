/* XPlayer_StateCrouchLook - crouched and looking down: hold the pose while no button changes and
 * the player stays on the ground, else the control state (slot +$A6), now.
 *
 * self: the player.
 * (The pad index is compared as a long: the ROM sign-extends it before the test.) */
#include "xmen2.h"

void XPlayer_StateCrouchLook(XPlayer *self)
{
	if (((long)self->pad == 0 ? (g_Pad1Pressed | g_Pad1Released) & 0x7F
			    : (g_Pad2Pressed | g_Pad2Released) & 0x7F)
	    || self->flags & 0x1000)
		GOTO_STATE(self, (StateFn)self->cls->method[40]);	/* StateControl */
	else
		Obj_ChangeAnim(self, CHAR_PART(self->charRec, lookDownAnim));
}
