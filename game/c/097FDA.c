/* EvilBeam_Construct - EVILBEAM New (class slot +$0A): build a beam at (x, y) in the memory given:
 * cleared (so no animation yet), a Timer running its Update (class slot +$0E) every frame,
 * started, and a draw-list entry for its Draw (class slot +$12, priority, layer $7F).
 *
 * self:   $2A bytes for the beam (nothing is done when 0).
 * x, y:   position, pixels.
 * unused: not read.
 * hflip:  non-zero: the sprite is flipped horizontally.
 * Returns self. Nothing in the game builds a beam (XClone_MakerBufSize only reads its size). */
#include "xmen2.h"

EvilBeam *EvilBeam_Construct(register EvilBeam *self, short x, short y, long unused, char hflip)
{
	if (self) {
		Mem_Set(self, 0, sizeof(EvilBeam));
		self->cls = CLASS(Tbl_EvilBeam_vtable);
		self->x = x;
		self->y = y;
		if (hflip)
			self->spr.flags |= 0x10;
		if (self->timer = Timer_Create((TimerFn)self->cls->method[2], 1, self, 1))	/* +$0E Update */
			Timer_Start(self->timer);
		self->drawNode = DrawList_Add(self->cls->method[3], self, 4, 0x7F, 0, 0);	/* +$12 Draw */
	}
	return self;
}
