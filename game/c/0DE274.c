/* ControlPanel_StateDamaged2 - CONTROLPANEL state: damaged twice (frame 3, its switch's flag
 * bit 3); waits ($FF) for a hit.
 *
 * self: the panel. */
#include "xmen2.h"

void ControlPanel_StateDamaged2(register ControlPanel *self)
{
	(*self->cls->method[20])(self, 3);	/* +$56 SetFrame */
	(*self->cls->method[26])(self, 8);	/* +$6E ControlPanel_SetFlags */
	self->timer = 0xFF;
}
