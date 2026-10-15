/* Deathbird_StateLandOnSwitch - DEATHBIRD at the lift switch: stand on the floor below (240, 368)
 * (the higher end of that segment), stop (SetMoveMode 0), beside the switch on the side it came
 * from (x 280 facing left, else 200 facing right), and peck (Tbl_DeathbirdAnim6, then
 * Deathbird_StatePeck when it ends).
 *
 * self: the boss. Returns self->flags & 8. */
#include "xmen2.h"

short Deathbird_StateLandOnSwitch(Deathbird *self)
{
	Seg *seg = Seg_FindBelow(0xF0, 0x170, 0x3E8);

	if (seg) {
		short y = MIN(seg->y1, seg->y2);

		self->y = y << 8;
	}
	(*self->cls->method[27])(self, 0);	/* SetMoveMode */
	if ((self->x >> 8) > 0xF0) {
		self->x = 0x118 << 8;
		self->spr.flags |= 0x10;
	} else {
		self->x = 0xC8 << 8;
		self->spr.flags &= ~0x10;
	}
	Obj_ChangeAnim(self, CODEPTR(Tbl_DeathbirdAnim6));
	SET_STATE_WAIT(self, (StateFn)Deathbird_StatePeck, 0xFF);
	return self->flags & 8;
}
