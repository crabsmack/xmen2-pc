/* CursorInput_StatePoll - BOXINPUTACTOR state: read the player's pad for its select cursor. Nothing
 * once the choice is confirmed or while either slot is busy (an input or a fly-in being carried
 * out). Start (pressed): CursorInput_Confirm. While the portrait is hidden the cursor waits out
 * its move delay (counted down here); else Left / Right (latched): CursorInput_MoveLeft /
 * _MoveRight. Taking an input makes the slot busy; the next state runs from the next update.
 *
 * self: the cursor (its slot's player, 1 or 2, picks the pad). Returns self->flags & 8. */
#include "xmen2.h"

#define g_P1Slot	(*(PTR(BigSprite) *)g_SceneVar3B28)	/* scene variable: player 1's select slot */
#define g_P2Slot	(*(PTR(BigSprite) *)g_SceneVar3B2C)	/* scene variable: player 2's select slot */

short CursorInput_StatePoll(BoxInputActor *self)
{
	register char busy = g_P1Slot->busy || g_P2Slot->busy;
	register BigSprite *slot = self->slot;

#if defined(XMEN2_NATIVE)
	if (g_opt.players > 2) {	/* phase 7: players 3-4's slots are busy too; a Start pressed meanwhile waits */
		if (P34_AnyBusy())
			busy = 1;
		if (P34_TakeStart(slot, (P34_SlotX(slot) ? P34_SlotX(slot)->padPressed
			: slot->player == 1 ? g_Pad1Pressed : g_Pad2Pressed) & 0x80, busy)) {
			self->slot->busy = 1;
			SET_STATE(self, CursorInput_Confirm);
			return self->flags & 8;
		}
	}
	if (P34_SlotX(slot)) {	/* phase 7: players 3 and 4, their own pads; a press moves once */
		register PlayerX *x = P34_SlotX(slot);

		if (!slot->confirmed && !busy) {
			if (slot->hidden && slot->moveDelay)	/* as players 1-2's: their portraits fly too */
				slot->moveDelay--;
			else if (x->padLatch & 0xC) {
				register short dir = x->padLatch & 0xC;

				self->slot->busy = 1;
				x->padLatch &= ~0xC;
				if (dir & 4)
					SET_STATE(self, CursorInput_MoveLeft);
				else
					SET_STATE(self, CursorInput_MoveRight);
				return self->flags & 8;
			}
		}
		SET_STATE(self, CursorInput_StatePoll);
		return self->flags & 8;
	}
#endif
	if (!slot->confirmed && !busy) {
		if (slot->player == 1) {
			if (g_Pad1Pressed & 0x80) {
				self->slot->busy = 1;
				SET_STATE(self, CursorInput_Confirm);
				return self->flags & 8;
			}
		} else if (g_Pad2Pressed & 0x80) {
			self->slot->busy = 1;
			SET_STATE(self, CursorInput_Confirm);
			return self->flags & 8;
		}
		if (slot->hidden && slot->moveDelay)
			slot->moveDelay--;
		else if (slot->player == 1) {
			if (g_Pad1Latch & 4) {
				self->slot->busy = 1;
				SET_STATE(self, CursorInput_MoveLeft);
				return self->flags & 8;
			}
			if (g_Pad1Latch & 8) {
				self->slot->busy = 1;
				SET_STATE(self, CursorInput_MoveRight);
				return self->flags & 8;
			}
		} else {
			if (g_Pad2Latch & 4) {
				self->slot->busy = 1;
				SET_STATE(self, CursorInput_MoveLeft);
				return self->flags & 8;
			}
			if (g_Pad2Latch & 8) {
				self->slot->busy = 1;
				SET_STATE(self, CursorInput_MoveRight);
				return self->flags & 8;
			}
		}
	}
	SET_STATE(self, CursorInput_StatePoll);
	return self->flags & 8;
}
