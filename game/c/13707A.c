/* Hornet_Move - HORNET Move (class slot +$4E): run its motion function (+$94), if any. The
 * parent's Move (velocity, floor) isn't called: the motion functions move it themselves.
 *
 * self: the hornet. (The frame step argument is ignored; the motion functions read
 * g_TwoPlayerStep.) */
#include "xmen2.h"

void Hornet_Move(register Hornet *self)
{
	HornetMoveFn move = self->move;

	if (move)
		(*move)(self);
}
