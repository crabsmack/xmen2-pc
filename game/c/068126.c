/* CursorInput_Confirm - BOXINPUTACTOR state (Start): a player without a character joins, spending a
 * life (g_P1Lives / g_P2Lives), on the first free character (the slot then changing, its
 * portrait flies in); a player with one confirms it: "SELECTED" printed under the player's side
 * (row $16; player 1 from column 5, player 2 right-aligned from column $18) in the full-screen
 * text window (3-$16, 5-$22), the previous window put back after. With no change the slot is no
 * longer busy. Back to CursorInput_StatePoll from the next update.
 *
 * self: the cursor. Returns self->flags & 8. */
#include "xmen2.h"

#define g_TextCol	(*(s16 *)g_SceneVar3B04)	/* scene variable: Cerebro text cursor column */
#define g_TextRow	(*(s16 *)g_SceneVar3B06)	/* scene variable: Cerebro text cursor row */
#define g_P1Slot	(*(PTR(BigSprite) *)g_SceneVar3B28)	/* scene variable: player 1's select slot */
#define g_P2Slot	(*(PTR(BigSprite) *)g_SceneVar3B2C)	/* scene variable: player 2's select slot */

short CursorInput_Confirm(BoxInputActor *self)
{
	register BigSprite *slot = self->slot;
	short top;
	register short left, bottom, right;
	register short n;

	top = g_TextWinTop;
	left = g_TextWinLeft;
	bottom = g_TextWinBottom;
	right = g_TextWinRight;
	g_TextWinTop = 3;
	g_TextWinLeft = 5;
	g_TextWinBottom = 0x16;
	g_TextWinRight = 0x22;
#if defined(XMEN2_NATIVE)
	if (g_opt.players > 2) {	/* phase 7: "READY" in the player's column; players 3 and 4 their own lives */
		if (slot->charIndex != -1) {
			slot->confirmed = 1;
			P34_SelectText(slot->player, 1);
		} else {
			register s16 *lives = P34_SlotX(slot) ? &P34_SlotX(slot)->lives
				: slot->player == 1 ? &g_P1Lives : &g_P2Lives;

			if (*lives && (n = PlayerSelect_FirstFreeChar()) != -1) {
				LOSE_LIFE(*lives);
				slot->changing = 1;
				slot->savedChar = n;
			}
		}
	} else
#endif
	if (slot->player == 1) {
		if (g_P1Slot->charIndex != -1) {
			slot->confirmed = 1;
			g_TextCol = 5;
			g_TextRow = 0x16;
			Cerebro_PrintText(STR(Str_Selected_06824E, "SELECTED   "), 0);
		} else if (g_P1Lives) {
			if ((n = PlayerSelect_FirstFreeChar()) != -1) {
				LOSE_LIFE(g_P1Lives);
				slot->changing = 1;
				slot->savedChar = n;
			}
		}
	} else if (g_P2Slot->charIndex != -1) {
		slot->confirmed = 1;
		g_TextCol = 0x18;
		g_TextRow = 0x16;
		Cerebro_PrintText(STR(Str_Selected, "   SELECTED"), 1);
	} else if (g_P2Lives) {
		if ((n = PlayerSelect_FirstFreeChar()) != -1) {
			LOSE_LIFE(g_P2Lives);
			slot->changing = 1;
			slot->savedChar = n;
		}
	}
	if (!slot->changing)
		slot->busy = 0;
	g_TextWinTop = top;
	g_TextWinLeft = left;
	g_TextWinBottom = bottom;
	g_TextWinRight = right;
	SET_STATE(self, CursorInput_StatePoll);
	return self->flags & 8;
}
