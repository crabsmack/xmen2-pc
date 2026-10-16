/* Barrier_Close - shut a BARRIER: the closed animation (Tbl_ClosedBarrierAnim), closed = 1, and a
 * solid block floor segment (type $15, layer 0) 32 px wide, centred on its x, from top to bottom
 * (Seg_Create).
 *
 * self: the barrier. The segment is kept in a variable and never used. */
#include "xmen2.h"

void Barrier_Close(register Barrier *self)
{
	register short top, x2, bottom, x1;
	register Seg *seg;

	Obj_SetAnim(self, CODEPTR(Tbl_ClosedBarrierAnim));
	self->closed = 1;
	top = self->top;
	bottom = self->bottom;
	x1 = (self->x >> 8) - 0x10;
	x2 = x1 + 0x20;
	seg = Seg_Create(0x15, 0, x1, top, x2, bottom);
}
