/* FlyBrood_StateFlyOffKill - FLYBROOD state after a dive or fly-by: once it is past the screen
 * edge it is heading for (30 px beyond the left edge facing left, $15E px right of the left
 * edge facing right), kill it (class slot +$52).
 *
 * self: the object. */
#include "xmen2.h"

void FlyBrood_StateFlyOffKill(FlyBrood *self)
{
	char off = 0;

	if (self->spr.flags & 0x10) {
		if ((self->x >> 8) < g_CameraX - 0x1E)
			off = 1;
	} else {
		if ((self->x >> 8) > g_CameraX + 0x15E)
			off = 1;
	}
	if (off)
		(*self->cls->method[19])(self);		/* +$52 Kill */
}
