/* PortraitActor_StateWaitTurn - SCALINGACTOR state: the portrait waits, hidden, before flying back
 * in. Another change of character restarts the wait (30 frames, cursor move delay 10). When the
 * wait is over and no slot is busy, this one becomes busy, takes its character's sprite and frame
 * (none: $FF), prepares the flight picture (PlayerSelect_CalcPortraitPos) and flies in
 * (PortraitActor_StartFlyIn, from the next update).
 *
 * self: the portrait. Returns self->flags & 8. */
#include "xmen2.h"

#define g_P1Slot	(*(PTR(BigSprite) *)g_SceneVar3B28)	/* scene variable: player 1's select slot */
#define g_P2Slot	(*(PTR(BigSprite) *)g_SceneVar3B2C)	/* scene variable: player 2's select slot */

short PortraitActor_StateWaitTurn(ScalingActor *self)
{
	register BigSprite *slot = self->slot;
	register char busy;

	if (slot->changing) {
		PlayerSelect_RevertCursor(slot);
		slot->busy = 0;
		slot->timer = 30;
		slot->moveDelay = 10;
	}
	if (slot->timer) {
		slot->timer--;
		SET_STATE(self, PortraitActor_StateWaitTurn);
		return self->flags & 8;
	}
	busy = g_P1Slot->busy || g_P2Slot->busy;
	if (!busy) {
		slot->busy = 1;
		if ((self->spr.def = PlayerSelect_CharSpriteDef(self->slot)) != 0)
			self->frame = PlayerSelect_CharFrameNum(self->slot);
		else
			self->frame = 0xFF;
		self->spr.frame = self->frame;
		PlayerSelect_CalcPortraitPos(self);
		SET_STATE(self, PortraitActor_StartFlyIn);
		return self->flags & 8;
	} else {
		SET_STATE(self, PortraitActor_StateWaitTurn);
		return self->flags & 8;
	}
}
