/* ControlBox_StateWaitSwitch - CONTROLBOX state: wait for its switch (flag word bit 0: its control
 * panel destroyed), then ControlBox_StateActivate from the next update.
 *
 * self: the box. Returns self->flags & 8 once set; while waiting, no value (MPW's d0). */
#include "xmen2.h"

short ControlBox_StateWaitSwitch(ControlBox *self)
{
	register s16 *flags = self->switchFlags;

	if (*flags & 1) {
		SET_STATE(self, ControlBox_StateActivate);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
