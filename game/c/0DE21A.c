/* ControlPanel_StateIntact - CONTROLPANEL state: intact (frame 1); waits ($FF) for a hit.
 *
 * self: the panel. */
#include "xmen2.h"

void ControlPanel_StateIntact(register ControlPanel *self)
{
	(*self->cls->method[20])(self, 1);	/* +$56 SetFrame */
	self->timer = 0xFF;
}
