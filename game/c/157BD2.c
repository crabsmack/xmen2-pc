/* MagneticGlob_SetFrame - MAGNETICGLOB SetFrame (class slot +$56): show a frame of the globe.
 *
 * self: the globe.  frame: the frame ($FF: none).
 * Its flips are rewritten with themselves (no change); sprite flags bit 5 says whether the
 * frame's tiles are already in VRAM (the uploaded frame). */
#include "xmen2.h"

void MagneticGlob_SetFrame(register MagneticGlob *self, register short frame)
{
	register SprInst *spr = &self->spr;

	spr->flags = spr->flags & ~0x18 | self->spr.flags & 0x18;
	spr->frame = (u8)frame;
	if (frame != 0xFF && frame == spr->uploaded)
		spr->flags |= 0x20;
	else
		spr->flags &= ~0x20;
}
