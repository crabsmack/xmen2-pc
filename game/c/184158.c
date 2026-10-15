/* Beast_SetFrame - BEAST SetFrame (class slot +$56): show frame number frame of Beast; frames $80
 * and up are in his second sprite bank.
 *
 * self: the player.  frame: the frame ($FF = none).
 * The sprite's definition becomes Art_PlayerSpr2 (frame - $80) or Art_BeastSpr (left as it was
 * for $FF); a new bank means no frame of it is in VRAM yet (flags bit 5 off, uploaded $FF).
 * Bit 5 says whether the frame's tiles are already in VRAM; when they aren't the object is
 * redrawn (flags |= 8), as Actor_SetFrame does. */
#include "xmen2.h"

void Beast_SetFrame(register Beast *self, register short frame)
{
	register SprInst *spr = &self->spr;
	void *oldDef;

	oldDef = spr->def;
	if (frame == 0xFF)
		frame = 0xFF;
	else if (frame >= 0x80) {
		frame = frame - 0x80;
		spr->def = CODEPTR(Art_PlayerSpr2);
	} else
		spr->def = CODEPTR(Art_BeastSpr);
	if (spr->def != oldDef) {
		spr->flags &= ~0x20;
		spr->uploaded = 0xFF;
	}
	spr->frame = (u8)frame;
	if (frame != 0xFF && frame == spr->uploaded)
		spr->flags |= 0x20;
	else {
		spr->flags &= ~0x20;
		self->flags |= 8;
	}
}
