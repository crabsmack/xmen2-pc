/* Hornet_MoveStartSwing - HORNET's first motion function (Hornet_Init): from the next Move on,
 * swing (Hornet_MoveSwing).
 *
 * self: the hornet. */
#include "xmen2.h"

void Hornet_MoveStartSwing(Hornet *self)
{
	self->move = (HornetMoveFn)Hornet_MoveSwing;
}
