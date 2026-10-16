/* SavageBioBase_SetFrame - SAVAGEBIOBASE SetFrame (class slot +$56): show a frame of one of four
 * sprite definitions, picked by the frame number: $80 and up Art_BasicBioSentinelSpr, $60-$7F
 * Art_BasicBioSentinelSpr3, $40-$5F Art_SavageBioSpr (frame - that base), below $40
 * Art_BasicBioSentinelSpr2; $FF: none (the definition is kept).
 *
 * self: the object.  frame: the frame number ($FF: none).
 * A new definition makes the frame's tiles not uploaded (spr flags bit 5 off, uploaded $FF). A
 * frame whose tiles are already in VRAM keeps them (bit 5 on); else bit 5 goes off and flags 8
 * makes the next Update recompute its boxes. */
#include "xmen2.h"

void SavageBioBase_SetFrame(SavageBioBase *self, short frame)
{
	register SprInst *spr = &self->spr;
	void *old = spr->def;

	if (frame == 0xFF)
		frame = 0xFF;
	else if (frame >= 0x80) {
		frame = frame - 0x80;
		spr->def = CODEPTR(Art_BasicBioSentinelSpr);
	} else if (frame >= 0x60) {
		frame = frame - 0x60;
		spr->def = CODEPTR(Art_BasicBioSentinelSpr3);
	} else if (frame >= 0x40) {
		frame = frame - 0x40;
		spr->def = CODEPTR(Art_SavageBioSpr);
	} else
		spr->def = CODEPTR(Art_BasicBioSentinelSpr2);
	if (spr->def != old) {
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
