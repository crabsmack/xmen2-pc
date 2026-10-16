/* Teleporter_StateWaitFade - TELEPORTER state: once the fade in is over (g_PalFadeActive clear), the
 * CloneA2 palette effects run again, Start pauses again (Pause_Check), flags6 bit 12 is cleared
 * and the pad waits for the next use (Teleporter_StateWait).
 *
 * self: the pad.
 * Returns flags & 8 when done; nothing while the fade runs (state functions' results are not
 * read there). */
#include "xmen2.h"

#define g_CloneA2PalOff	(*(s16 *)g_SceneVar3B04)	/* scene variable: non-zero: no flicker, no background palette swaps */

short Teleporter_StateWaitFade(Teleporter *self)
{
	if (!g_PalFadeActive) {
		g_CloneA2PalOff = 0;
		g_PauseHandler = FN_DATA(Pause_Check);
		self->flags6 &= ~0x1000;
		SET_STATE(self, Teleporter_StateWait);
		return self->flags & 8;
	}
	NO_RETURN_VALUE
}
