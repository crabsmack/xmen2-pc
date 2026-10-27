/* PlayerSelect_CalcPortraitPos - prepare a portrait's flight picture: its slot's character frame
 * rendered as the slot's big-sprite picture (PlayerSelect_LoadCharFrame), flipped for player 2
 * (at $F8, facing left: the scaler's horizontal flip, hflip), and the frame's box kept: offX its
 * left edge (its right edge negated when flipped), offY its top, picW/picH the picture's size.
 * No character: the size becomes 0.
 *
 * self: the portrait. */
#include "xmen2.h"

void PlayerSelect_CalcPortraitPos(register ScalingActor *self)
{
	void *def;
	SprFrame *fr;
	register BigSprite *slot = self->slot;
	register short frame;

	frame = PlayerSelect_CharFrameNum(slot);
	def = PlayerSelect_CharSpriteDef(slot);
	fr = SPR_FRAME(def, frame);
	if (!def) {
		self->picW = 0;
		self->picH = 0;
	} else {
		PlayerSelect_LoadCharFrame(slot);
		if (SELECT_FACES_LEFT(self)) {
			slot->params->hflip = 1;
			self->offX = -fr->right;
		} else {
			slot->params->hflip = 0;
			self->offX = fr->left;
		}
		self->offY = fr->top;
		self->picW = slot->params->srcW;
		self->picH = slot->params->srcH;
	}
}
