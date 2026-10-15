/* Hologram_FindGateSeg - find the arena gate: the floor segment of type $1C 320 px left of the
 * hologram, at its height; open it (bit $40) and keep it.
 *
 * self: the hologram (Hologram_StateSetup).
 * The segment isn't checked for null. Hologram_LockArena closes the gate, Hologram_OpenExit
 * opens it again. */
#include "xmen2.h"

void Hologram_FindGateSeg(register Hologram *self)
{
	register short x = (self->x >> 8) - 320;
	register short y = self->y >> 8;
	register Seg *seg;

	seg = Seg_FindContaining(x, y, 0x1C);
	seg->flags |= 0x40;
	self->gateSeg = seg;
}
