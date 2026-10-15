/* Boulder_Update - BOULDER Update (class slot +$4A): the parent's (ACTOR) Update, then step the
 * dust cloud's animation (spr2, anim2: minVx counts frames, twice as fast in two-player
 * stepping, minVy is the step; a one-shot script (flags bit 1) ends after its last step, anim2
 * 0), showing step's frame on spr2 (its tiles upload again unless that frame is uploaded: flags
 * bit 5 cleared); and kill the boulder once it is more than $144 px below the screen top.
 *
 * self: the boulder.
 * The script is read as bytes: step i's frame at +7 + 2i (AnimScript header, steps from +6). */
#include "xmen2.h"

void Boulder_Update(register Boulder *self)
{
	register long dy;

	(*CLASS(Tbl_Boulder_vtable)->parent->method[17])(self);	/* ACTOR Update */
	if (self->anim2) {
		self->minVx += g_TwoPlayerStep ? 2 : 1;
		if (self->minVx >= ((AnimScript *)self->anim2)->delay) {
			self->minVy++;
			if (((AnimScript *)self->anim2)->count <= self->minVy) {
				self->minVy = 0;
				if (((AnimScript *)self->anim2)->flags & 2)
					self->anim2 = 0;
			}
			if (self->anim2) {
				self->spr2.frame = *(self->anim2 + (self->minVy << 1) + 7);
				if (self->spr2.frame == 0xFF || self->spr2.frame != self->spr2.uploaded)
					self->spr2.flags &= ~0x20;
				self->minVx = 0;
			}
		}
	}
	dy = (self->y >> 8) - g_CameraY;
	if (dy > 0x144)
		(*self->cls->method[19])(self);		/* +$52 Kill */
}
