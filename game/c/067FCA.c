/* CursorInput_MoveLeft - BOXINPUTACTOR state (Left): a player without a character joins, spending a
 * life (g_P1Lives / g_P2Lives), on the first free character (PlayerSelect_FirstFreeChar); else
 * the cursor asks for the previous free character (PlayerSelect_PrevFreeChar). A new character
 * marks the slot changing (savedChar; its portrait then flies out and back); with no change the
 * slot is no longer busy. Back to CursorInput_StatePoll from the next update.
 *
 * self: the cursor. Returns self->flags & 8. */
#include "xmen2.h"

short CursorInput_MoveLeft(BoxInputActor *self)
{
	register BigSprite *slot = self->slot;
	register short c = slot->charIndex;
	register short n;

	if (c != -1) {
		if ((n = PlayerSelect_PrevFreeChar(c)) != c) {
			slot->changing = 1;
			slot->savedChar = n;
		}
	} else if (slot->player == 1) {
		if (g_P1Lives) {
			if ((n = PlayerSelect_FirstFreeChar()) != -1) {
				LOSE_LIFE(g_P1Lives);
				slot->changing = 1;
				slot->savedChar = n;
			}
		}
	} else if (g_P2Lives) {
		if ((n = PlayerSelect_FirstFreeChar()) != -1) {
			LOSE_LIFE(g_P2Lives);
			slot->changing = 1;
			slot->savedChar = n;
		}
	}
	if (!slot->changing)
		slot->busy = 0;
	SET_STATE(self, CursorInput_StatePoll);
	return self->flags & 8;
}
