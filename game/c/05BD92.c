/* Boulder_Draw - BOULDER Draw (class slot +$1A; replaces ACTOR's, no hit blink): a shattered
 * boulder (hitCooldown > 0) draws its three pieces with its own sprite, frames 3, 4, 5 (quarter
 * pixel positions); a whole one, when its box (40 px around) is on the screen, draws its dust
 * cloud (spr2 at maxVx/maxVy) while that animates, then itself as Actor_DrawSprite does (animDist
 * back along a distance-driven animation's axis).
 *
 * self: the boulder. */
#include "xmen2.h"

/* Is o's body box, grown by m on each side, off the screen? */
#define OFF_SCREEN(o, m) \
	((o)->boxLeft - (m) - g_CameraX > 320 || (o)->boxRight + (m) - g_CameraX < 0 \
	 || (o)->boxTop - (m) - g_CameraY > 224 || (o)->boxBottom + (m) - g_CameraY < 0)

void Boulder_Draw(Boulder *self)
{
	register BoulderPiece *p;
	short frame;
	register short i;

	if (self->hitCooldown > 0) {
		frame = 3;
		p = self->piece;
		for (i = 0; i < 3; i++, p++) {
			register short sx = (p->x >> 2) - g_CameraX;
			register short sy = (p->y >> 2) - g_CameraY;

			if (frame > 6)
				frame = 3;
			self->spr.frame = frame++;
			Spr_Draw(&self->spr, sx, sy);
		}
	} else if (!OFF_SCREEN(self, 40)) {
		if (self->anim2)
			Spr_Draw(&self->spr2, self->maxVx - g_CameraX, self->maxVy - g_CameraY);
		Spr_Draw(&self->spr,
			self->anim && (((AnimScript *)self->anim)->flags & 0x30) == 0x20
				? ((self->x - self->animDist) >> 8) - g_CameraX : (self->x >> 8) - g_CameraX,
			self->anim && (((AnimScript *)self->anim)->flags & 0x30) == 0x30
				? ((self->y - self->animDist) >> 8) - g_CameraY : (self->y >> 8) - g_CameraY);
	}
}
