/* BrainRocket_Update - BRAINROCKET Update (class slot +$4A): the parent's (ACTOR) Update; then,
 * unless that left it gone (flags bit 4), a solid map cell where it was before (Tile_TestAt, mask
 * $4800, the solid map's plane) blows it up: the camera shakes (Tbl_BrainRocketQuake3 on a
 * vertical heading, 90 or 270, else Tbl_BrainRocketQuake2), an explosion at its body box's
 * centre, Kill (slot +$52) and the explosion sound ($38, restarted).
 *
 * self: the rocket. Its d0 on return is whatever was left there. */
#include "xmen2.h"

void BrainRocket_Update(BrainRocket *self)
{
	register short x = self->x >> 8;
	register short y = self->y >> 8;

	(*CLASS(Tbl_BrainRocket_vtable)->parent->method[17])(self);	/* ACTOR Update */
	if (!(self->flags & 0x10)) {
		if ((char)Tile_TestAt(self, x, y, !g_SolidMapIsBg, 0x4800, 1)) {
			if (self->angle == 90 || self->angle == 270)
				g_CamShake = CODEPTR(Tbl_BrainRocketQuake3);
			else
				g_CamShake = CODEPTR(Tbl_BrainRocketQuake2);
			Explosion_Spawn((self->boxRight + self->boxLeft) << 7,
					(self->boxTop + self->boxBottom) << 7, 0);
			(*self->cls->method[19])(self);			/* +$52 Kill */
			Snd_Stop(0x38);
			Snd_Play(0x38);
		}
	}
}
