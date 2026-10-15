/* XNpc_StateKnockSlide - XNPC state: the knockback slide: move by the slide speed (prevVx),
 * slowing by prevVy each frame, until a wall (solid map tiles $4000 at the box edge it moves
 * toward, half way down), the speed turning round or the counter (16 frames) running out; then
 * the hurt is over (EndHurt).
 *
 * self: the object.
 * Twice a frame in two-player step mode. Returns flags bit 3 while sliding; nothing in
 * particular after EndHurt. */
#include "xmen2.h"

short XNpc_StateKnockSlide(XNpc *self)
{
	if (self->shake > 0) {
		if (self->prevVx > 0 && (char)Tile_TestAt(0, self->boxRight,
			(self->boxTop + self->boxBottom) >> 1, !g_SolidMapIsBg, 0x4000, 1)) {
			(*self->cls->method[28])(self);	/* +$76 EndHurt */
			RETURN_NO_VALUE;
		}
		if (self->prevVx < 0 && (char)Tile_TestAt(0, self->boxLeft,
			(self->boxTop + self->boxBottom) >> 1, !g_SolidMapIsBg, 0x4000, 1)) {
			(*self->cls->method[28])(self);	/* +$76 EndHurt */
			RETURN_NO_VALUE;
		}
		self->shake -= self->shake > 1 ? (g_TwoPlayerStep ? 2 : 1) : (self->shake == 1 ? 1 : 0);
		self->x += self->prevVx;
		self->prevVx += self->prevVy;
		if (g_TwoPlayerStep) {
			self->x += self->prevVx;
			self->prevVx += self->prevVy;
		}
		if (self->prevVx > 0 && self->prevVy > 0 || self->prevVx < 0 && self->prevVy < 0) {
			(*self->cls->method[28])(self);	/* +$76 EndHurt */
			RETURN_NO_VALUE;
		}
		SET_STATE(self, XNpc_StateKnockSlide);
		return self->flags & 8;
	}
	(*self->cls->method[28])(self);	/* +$76 EndHurt */
	NO_RETURN_VALUE
}
