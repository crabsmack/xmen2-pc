/* Apocalypse_SetFrame - APOCALYPSE SetFrame (class slot +$56; the parent's isn't called): show a
 * frame of the body or of the second sprite.
 *
 * self: the boss.  frame: $FF hides both; $80 + n shows frame n of the second sprite and hides
 * the body (the second sprite has high priority, except frames $81-$83 unless moveFlags bit 5);
 * below $80 shows body frame n (high priority) with second-sprite frame 0. The second sprite's
 * flags bit 5 says its frame's tiles are already in VRAM; when they aren't, flags |= 8 (redraw).
 * (Frame $80 + $7F, $FF after the subtraction, never counts as uploaded.) */
#include "xmen2.h"

void Apocalypse_SetFrame(Apocalypse *self, short frame)
{
	register SprInst *spr2 = self->spr2;
	register SprInst *spr = &self->spr;

	if (frame == 0xFF) {
		spr2->frame = 0xFF;
		spr->frame = 0xFF;
	} else if (frame >= 0x80) {
		if (!(self->moveFlags & 0x20) && (frame == 0x81 || frame == 0x82 || frame == 0x83))
			spr2->flags &= ~4;
		else
			spr2->flags |= 4;
		frame = frame - 0x80;
		spr2->frame = frame;
		if (frame != 0xFF && frame == spr2->uploaded)
			spr2->flags |= 0x20;
		else {
			spr2->flags &= ~0x20;
			self->flags |= 8;
		}
		spr->frame = 0xFF;
	} else {
		spr2->flags |= 4;
		spr2->frame = 0;
		if (spr2->uploaded == 0)
			spr2->flags |= 0x20;
		else
			spr2->flags &= ~0x20;
		spr->frame = frame;
		spr->flags |= 4;
	}
}
