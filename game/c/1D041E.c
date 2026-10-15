/* Gambit_SetFrame - GAMBIT SetFrame (class slot +$56): show frame number frame of Gambit; frames
 * 100-149 are in his second sprite bank, 150 and up in his third.
 *
 * self: the player.  frame: the frame ($FF = none).
 * The sprite's definition becomes Art_PlayerSpr9 (frame - 150), Art_PlayerSpr8 (frame - 100) or
 * Art_EvilGambitSpr (left as it was for $FF); a new bank means no frame of it is in VRAM yet
 * (flags bit 5 off, uploaded $FF). Bit 5 says whether the frame's tiles are already in VRAM;
 * when they aren't the object is redrawn (flags |= 8), as Actor_SetFrame does. */
#include "xmen2.h"

void Gambit_SetFrame(register Gambit *self, register short frame)
{
	register SprInst *spr = &self->spr;
	void *oldDef;

	oldDef = spr->def;
	if (frame == 0xFF)
		frame = 0xFF;
	else if (frame >= 150) {
		frame = frame - 150;
		spr->def = CODEPTR(Art_PlayerSpr9);
	} else if (frame >= 100) {
		frame = frame - 100;
		spr->def = CODEPTR(Art_PlayerSpr8);
	} else
		spr->def = CODEPTR(Art_EvilGambitSpr);
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
