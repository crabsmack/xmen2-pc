/* Nightcrawler_SetFrame - NIGHTCRAWLER SetFrame (class slot +$56): show Nightcrawler frame number
 * frame, which may be in any of his three sprite banks.
 *
 * self: the player.  frame: his frame number ($FF = none).
 * Frames 0-$7F are in Art_EvilNightcrawlerSpr, $80-$C7 in Art_PlayerSpr4, 200 and up in
 * Art_PlayerSpr5 (none keeps the definition). A new bank means no frame of it is in VRAM yet
 * (flags bit 5 off, uploaded $FF). Bit 5 says whether the frame's tiles are already in VRAM, and
 * when they aren't the object is redrawn (flags |= 8), as Actor_SetFrame does. */
#include "xmen2.h"

void Nightcrawler_SetFrame(register Nightcrawler *self, register short frame)
{
	register SprInst *spr = &self->spr;
	void *oldDef;

	oldDef = spr->def;
	if (frame == 0xFF)
		frame = 0xFF;
	else if (frame >= 200) {
		frame = frame - 200;
		spr->def = CODEPTR(Art_PlayerSpr5);
	} else if (frame >= 0x80) {
		frame = frame - 0x80;
		spr->def = CODEPTR(Art_PlayerSpr4);
	} else
		spr->def = CODEPTR(Art_EvilNightcrawlerSpr);
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
