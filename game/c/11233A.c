/* Deathbird_SetFrame - DEATHBIRD SetFrame (class slot +$56; the parent's isn't called): frames
 * below $80 are the body sprite's (+$2E), frames $80 and up are frame - $80 of the second sprite
 * (spr2), drawn instead of the body (spr2Shown); $FF hides both.
 *
 * self: the boss.  frame: the frame.
 * The second sprite takes the body's flips (flags bits 3-4); its frame is marked uploaded (bit 5)
 * when its tiles are already in VRAM, else the object is flagged for a redraw (flags bit 3).
 * (The $FF test after subtracting $80 never holds.) Without a second sprite only the body is
 * hidden. */
#include "xmen2.h"

void Deathbird_SetFrame(Deathbird *self, short frame)
{
	register SprInst *spr2 = self->spr2;
	SprInst *spr = &self->spr;

	if (!spr2) {
		spr->frame = 0xFF;
		self->spr2Shown = 0;
	} else if (frame == 0xFF) {
		spr2->frame = 0xFF;
		spr->frame = 0xFF;
		self->spr2Shown = 0;
	} else if (frame >= 0x80) {
		frame = frame - 0x80;
		spr2->frame = (u8)frame;
		self->spr2Shown = 1;
		spr2->flags = spr2->flags & ~0x18 | spr->flags & 0x18;
		if (frame != 0xFF && frame == spr2->uploaded)
			spr2->flags |= 0x20;
		else {
			spr2->flags &= ~0x20;
			self->flags |= 8;
		}
		spr->frame = 0xFF;
	} else {
		self->spr2Shown = 0;
		spr->frame = (u8)frame;
		spr2->frame = 0xFF;
	}
}
