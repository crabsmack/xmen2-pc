/* Magneto_SetFrame - MAGNETO SetFrame (class slot +$56): show Magneto frame number frame, which
 * may be in any of his three sprite banks.
 *
 * self: the player.  frame: Magneto's frame number ($FF = none).
 * The sprite's definition becomes the frame's bank (Magneto_FrameBank); a new bank means no frame
 * of it is in VRAM yet (flags bit 5 off, uploaded $FF). The frame shown is the frame inside the
 * bank (Magneto_FrameInBank); bit 5 says whether its tiles are already in VRAM, and when they
 * aren't the object is redrawn (flags |= 8), as Actor_SetFrame does. */
#include "xmen2.h"

void Magneto_SetFrame(register Magneto *self, register short frame)
{
	register SprInst *spr = &self->spr;
	void *oldDef;

	oldDef = spr->def;
	spr->def = Magneto_FrameBank(frame);
	if (spr->def != oldDef) {
		spr->flags &= ~0x20;
		spr->uploaded = 0xFF;
	}
	frame = Magneto_FrameInBank(frame);
	spr->frame = (u8)frame;
	if (frame != 0xFF && frame == spr->uploaded)
		spr->flags |= 0x20;
	else {
		spr->flags &= ~0x20;
		self->flags |= 8;
	}
}
