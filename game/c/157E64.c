/* MagneticGlob_Deactivate - MAGNETICGLOB Deactivate (class slot +$46; the parent's isn't called):
 * take a live globe out of play: inactive (flags $4000), its frame not uploaded, nothing attached,
 * its timer stopped, out of the draw list (drawNode -1).
 *
 * self: the globe. */
#include "xmen2.h"

void MagneticGlob_Deactivate(register MagneticGlob *self)
{
	register Timer *t;

	if (IS_ALIVE(self)) {
		self->flags |= 0x4000;
		self->spr.flags &= ~0x20;
		self->spr.firstChild = 0xFF;
		self->spr.unk_0C = 0xFF;
		t = self->timerObj;
		if (t)
			Timer_Stop(t);
		if (self->drawNode > -1) {
			DrawList_Remove(self->drawNode);
			self->drawNode = -1;
		}
	}
}
