/* CapedAcolyte_StateHoverNearPlayer - CAPEDACOLYTE: while countdown runs (CapedAcolyte_ActHover),
 * fly towards a point beside and above the nearest player; then stop and go on with the script.
 *
 * self: the object.
 * The target (pixels) is 100 px from the player on the acolyte's side of it (beside the player
 * the other way when that side leaves its x range xMin..xMax and the other doesn't), 60 px
 * above it. The velocity covers 1/32 of the distance from home to the target per frame. Turns
 * to face the player while more than 16 px from the target in x. Returns flags bit 3 while
 * hovering; no value once the time is up (next script step on the next update). */
#include "xmen2.h"

short CapedAcolyte_StateHoverNearPlayer(register CapedAcolyte *self)
{
	short dx, dy;			/* distance to the target in pixels (dy unused) */
	register XPlayer *p;
	short hy;		/* home y in pixels */
	register short hx;		/* home x in pixels */
	short px;		/* player x */
	short ty;		/* target y */
	register short tx;		/* target x */

	hx = self->homeX >> 8;
	hy = self->homeY >> 8;
	if (self->countdown <= 0) {
		STOP_MOVING(self);
		AI_NEXT_STEP(self, self->ai, 0);
	} else {
		p = Player_Nearest((Actor *)self);
		px = p->x >> 8;
		if (hx < px) {
			tx = px - 100;
			if (tx < self->xMin && px + 100 <= self->xMax)
				tx = px + 100;
		} else {
			tx = px + 100;
			if (tx > self->xMax && px - 100 >= self->xMin)
				tx = px - 100;
		}
		ty = (p->y >> 8) - 60;
		dx = ABS(tx - hx);
		dy = ABS(ty - hy);
		if (dx > 16) {
			if (p->x < self->x)
				self->spr.flags |= 0x10;
			else
				self->spr.flags &= ~0x10;
		}
		self->vx = (tx - hx << 8) >> 5;
		self->vy = (ty - hy << 8) >> 5;
		SET_STATE(self, CapedAcolyte_StateHoverNearPlayer);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
