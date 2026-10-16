/* LineSegKiller_Deactivate - LINESEGKILLER Deactivate (class slot +$46; the parent's isn't
 * called): only flags |= $4000; it keeps its resources.
 *
 * self: the object. */
#include "xmen2.h"

void LineSegKiller_Deactivate(LineSegKiller *self)
{
	self->flags |= 0x4000;
}
