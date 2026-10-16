/* CapedAcolyte_StateShoot - CAPEDACOLYTE: fire one gauss bullet of the burst, the next one after
 * shotDelay frames; after the last, the next script step in 26 frames.
 *
 * self: the object.
 * The muzzle depends on the pose: Tbl_CapedAcolyteAnim2 30 px ahead and 50 up,
 * Tbl_CapedAcolyteAnim3 18 ahead and 34 up (bullet flag 1); any other pose leaves the offsets
 * unset. The bullet starts at the unmirrored offset even when the acolyte faces left (only the
 * offset passed on is mirrored, with flag $800). Flags: $8000 sprite priority, 2 always.
 * shotDelay -1 fires the next shot at once (a nested call, whose result it returns).
 * Returns flags bit 3 while the burst goes on; no value after the last shot. */
#include "xmen2.h"

short CapedAcolyte_StateShoot(register CapedAcolyte *self)
{
	long bx;
	register long by;
	register long flags = self->spr.flags & 4 ? 0x8000 : 0;
	register short dx;
	register short dy;

	if (self->anim == CODEPTR(Tbl_CapedAcolyteAnim2)) {
		dx = 30;
		dy = -50;
	} else if (self->anim == CODEPTR(Tbl_CapedAcolyteAnim3)) {
		dx = 18;
		dy = -34;
		flags |= 1;
	}
	bx = self->x + ((long)dx << 8);
	by = self->y + ((long)dy << 8);
	if (self->spr.flags & 0x10) {
		dx = -dx;
		flags |= 0x800;
	}
	flags |= 2;
	GaussBullet_Fire(bx, by, self, dx, dy, flags);
	if (self->shotsLeft > 0)
		self->shotsLeft--;
	if (self->shotsLeft > 0) {
		if (self->shotDelay == -1) {
			self->state = CapedAcolyte_StateShoot;
			return CapedAcolyte_StateShoot(self);
		} else {
			SET_STATE_WAIT(self, CapedAcolyte_StateShoot, self->shotDelay);
			return self->flags & 8;
		}
	} else
		AI_NEXT_STEP(self, self->ai, 26);
	NO_RETURN_VALUE
}
