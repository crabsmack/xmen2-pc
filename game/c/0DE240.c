/* ControlPanel_StateDamaged1 - CONTROLPANEL state: damaged once (frame 2, its switch's flag
 * bit 2, so it comes back damaged); waits ($FF) for a hit.
 *
 * self: the panel. */
#include "xmen2.h"

void ControlPanel_StateDamaged1(register ControlPanel *self)
{
	(*self->cls->method[20])(self, 2);	/* +$56 SetFrame */
	(*self->cls->method[26])(self, 4);	/* +$6E ControlPanel_SetFlags */
	self->timer = 0xFF;
}
